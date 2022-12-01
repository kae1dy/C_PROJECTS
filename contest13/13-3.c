#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>

enum { CHMOD = 0611 };

int
runprog(char *prog, int fdr, int fdw){

    if (fdr < 0 || fdw < 0) return EXIT_FAILURE;

    int pid = fork();
    if (pid < 0) return EXIT_FAILURE;
    
    if (pid == 0) {
        if (fdr != 0) {
            if (dup2(fdr, 0) < 0) return EXIT_FAILURE;
            close(fdr);
        }
        if (fdw != 0) {
            if (dup2(fdw, 1) < 0) return EXIT_FAILURE;
            close(fdw);
        }
        execlp(prog, prog, NULL);
        exit(EXIT_FAILURE);
    }
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) return WEXITSTATUS(status);
    else return EXIT_FAILURE;
}

int
main(int argc, char *argv[]){
    int pid, fd[2];
    int fdr, fdw, status;

    if (pipe(fd) < 0 || (pid = fork()) < 0) exit(EXIT_FAILURE);
    
    if (pid == 0) {
        fdr = open(argv[4], O_RDONLY);
        close(fd[0]);

        status = !runprog(argv[1], fdr, fd[1]) && !runprog(argv[2], 0, fd[1]);
        close(fd[1]), close(fdr);

        exit(!status);
    }
    fdw = open(argv[5], O_WRONLY | O_APPEND | O_CREAT, CHMOD);

    close(fd[1]);
    runprog(argv[3], fd[0], fdw);
    close(fd[0]), close(fdw);
    
    while (wait(NULL) > 0) {}
    exit(EXIT_SUCCESS);
}