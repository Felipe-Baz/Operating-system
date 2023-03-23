#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Use %s <seconds\n", argv[0]);
        return 0;
    }
    pid_t pid;
    // https://linux.die.net/man/2/fork
    pid = fork();
    if(pid==0) {
        // children
        int s = atoi(argv[1]);
        printf("Children is (%d) sleeping %d s.\n",
               (int) getpid(),
               s
        );
        sleep(s);
        printf("The children wake-up.\n");
        return 0;
    } else if(pid>0) {
        // father
        printf("Father is (%d) waiting.\n",
               (int) getpid(),
               (int) pid
        );
        waitpid(-1, NULL, 0);
        printf("The father get return of children.\n");
    } else {
        //error
        perror("fork()");
        return -1;
    }
    return 0;
}
