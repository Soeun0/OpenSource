#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void my_assert(int condition, const char *message) {
    if (!condition) {
        fprintf(stderr, "Assertion failed: %s\n", message);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    FILE *f;
    my_assert(argc >= 2, "Usage: perror_use file_name");

    if ((f = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        exit(1);
    }

    printf("Opened file \"%s\".\n", argv[1]);
    fclose(f);
}

