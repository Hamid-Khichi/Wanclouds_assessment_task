#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <your_name>\n", argv[0]);
        return 1;
    }
    char *name = argv[1];
    printf("Running process with name: %s\n", name);
    // Simulate some activity by calling a system call in a loop
    for (int i = 0; i < 5; i++) {
        printf("Process %s is doing some work... (%d)\n", name, i + 1);
        sleep(10); // Adjusted the sleep for 10 seconds instead of the initial 1
    }
    printf("Process %s completed its work.\n", name);
    return 0;
}
