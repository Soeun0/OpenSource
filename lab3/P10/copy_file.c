/* fileCopy.c */
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

#define SHMSIZE 4096
#define SHMNAME "/my_shm"
#define SEMNAME "/my_sem"

void p(sem_t *semd);
void v(sem_t *semd);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_file_path>\n", argv[0]);
        exit(1);
    }

    FILE *fp, *copy_fp;
    char *path = argv[1];
    char buf[SHMSIZE];
    int status, shmd, count;
    void *shmaddr;
    pid_t pid;
    sem_t *semd;
    int val;

    // Open the semaphore
    if ((semd = sem_open(SEMNAME, O_CREAT, 0600, 1)) == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }

    sem_getvalue(semd, &val);
    memset(buf, 0, SHMSIZE);

    if ((pid = fork()) == 0) {
        // Child process: write to shared memory
        if ((shmd = shm_open(SHMNAME, O_CREAT | O_RDWR, 0666)) == -1) {
            perror("shm_open failed");
            exit(1);
        }

        if (ftruncate(shmd, SHMSIZE) != 0) {
            perror("ftruncate failed");
            exit(1);
        }

        if ((shmaddr = mmap(0, SHMSIZE, PROT_WRITE, MAP_SHARED, shmd, 0)) == MAP_FAILED) {
            perror("mmap failed");
            exit(1);
        }

        if ((fp = fopen(path, "r")) == NULL) {
            perror("fopen failed");
            exit(1);
        }

        p(semd);  // Wait for the semaphore
        printf("Child Process %d: Writing to shared memory\n", getpid());

        while ((count = fread(buf, sizeof(char), SHMSIZE, fp)) > 0) {
            memcpy(shmaddr, buf, count);
        }

        v(semd);  // Release the semaphore
        printf("Child Process %d: Finished writing\n", getpid());

        fclose(fp);
        if (munmap(shmaddr, SHMSIZE) == -1) {
            perror("munmap failed");
            exit(1);
        }

        close(shmd);
        exit(0);

    } else if (pid > 0) {
        // Parent process: read from shared memory
        wait(&status);  // Wait for the child process to finish

        if ((shmd = shm_open(SHMNAME, O_RDWR, 0666)) == -1) {
            perror("shm_open failed");
            exit(1);
        }

        if ((shmaddr = mmap(0, SHMSIZE, PROT_READ, MAP_SHARED, shmd, 0)) == MAP_FAILED) {
            perror("mmap failed");
            exit(1);
        }

        strcat(path, "_2");  // Modify the path to create a copy file

        if ((copy_fp = fopen(path, "w")) == NULL) {
            perror("fopen failed");
            exit(1);
        }

        p(semd);  // Wait for the semaphore
        printf("Parent Process %d: Reading from shared memory\n", getpid());

        fwrite(shmaddr, sizeof(char), SHMSIZE, copy_fp);

        v(semd);  // Release the semaphore
        printf("Parent Process %d: Finished writing to destination\n", getpid());

        fclose(copy_fp);
        if (munmap(shmaddr, SHMSIZE) == -1) {
            perror("munmap failed");
            exit(1);
        }

        close(shmd);

        if (shm_unlink(SHMNAME) == -1) {
            perror("shm_unlink failed");
            exit(1);
        }

        if (sem_close(semd) == -1) {
            perror("sem_close failed");
            exit(1);
        }

    } else {
        perror("fork failed");
        exit(1);
    }

    return 0;
}

void p(sem_t *semd) {
    int ret;
    if ((ret = sem_trywait(semd)) != 0 && errno == EAGAIN) {
        sem_wait(semd);
    } else if (ret != 0) {
        perror("sem_trywait failed");
        exit(1);
    }
}

void v(sem_t *semd) {
    if (sem_post(semd) != 0) {
        perror("sem_post failed");
        exit(1);
    }
}
