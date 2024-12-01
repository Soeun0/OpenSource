#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void my_system(const char *command) {
    pid_t pid = fork(); 

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        char *args[] = {"sh", "-c", (char *)command, NULL};
        execvp("sh", args); 
        perror("execvp"); 
        exit(1);
    } else {
        waitpid(pid, NULL, 0);
    }
}

int main(int argc, char *argv[]) {
    
    my_system(argv[1]);

    return 0;
}
