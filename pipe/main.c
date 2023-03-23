#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Use %s <message\n", argv[0]);
        return 0;
    }
    pid_t pid;
    int fd[2];
    // https://linux.die.net/man/2/pipe
    if(pipe(fd) < 0){
        perror("perror()");
        return -1;
    }

    // https://linux.die.net/man/2/fork
    pid = fork();
    if(pid == 0) {
        // children
        #define PIPE_SIZE 4096
        char msg [PIPE_SIZE];
        int nr;
        bzero(msg, PIPE_SIZE);
        nr = read(fd[0], msg, PIPE_SIZE);

        if(nr > 0) {
            printf("children (%d) read pipe fd[0] (%d) total of %d bytes - message: %d.\n",
                (int) getpid(),
                fd[1],
                nr,
                msg
            );
        }
        return 0;
    } else if(pid > 0) {
        // father
        char *msg_pipe_chield = argv[1];
        int nw;
        nw = write(fd[1], msg_pipe_chield, strlen(msg_pipe_chield));
        if(nw > 0) {
            printf("the father write on pipe fd[1](%d) to child (%d) total of %d bytes - message with %d characters: %d. \n",
                (int) getpid(),
                fd[1],
                (int) pid,
                strlen(msg_pipe_chield),
                msg_pipe_chield
            );
        }
        // https://linux.die.net/man/2/waitpid
        waitpid(-1, NULL, 0);
    } else {
        //error
        perror("fork()");
        return -1;
    }
    return 0;
}
