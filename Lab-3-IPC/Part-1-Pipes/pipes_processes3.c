
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int pipefd[2];
    int pipefd2[2];
    int pid, pid2;

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <search_term>\n", argv[0]);
        return 1;
    }

    char *cat_args[] = {"cat", "scores", NULL};
    char *grep_args[] = {"grep", argv[1], NULL};
    char *sort_args[] = {"sort", NULL};

    // Create two pipes
    if (pipe(pipefd) == -1 || pipe(pipefd2) == -1)
    {
        perror("pipe failed");
        return 1;
    }

    pid = fork();

    if (pid == 0)
    {
        // Child creates another child
        pid2 = fork();

        if (pid2 == 0)
        {
            // Child's child executes sort
            dup2(pipefd2[0], 0);

            close(pipefd[0]);
            close(pipefd[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);

            execvp("sort", sort_args);
            perror("sort failed");
            exit(1);
        }
        else if (pid2 > 0)
        {
            // Child executes grep
            dup2(pipefd[0], 0);
            dup2(pipefd2[1], 1);

            close(pipefd[0]);
            close(pipefd[1]);
            close(pipefd2[0]);
            close(pipefd2[1]);

            execvp("grep", grep_args);
            perror("grep failed");
            exit(1);
        }
        else
        {
            perror("fork failed");
            exit(1);
        }
    }
    else if (pid > 0)
    {
        // Parent executes cat scores
        dup2(pipefd[1], 1);

        close(pipefd[0]);
        close(pipefd[1]);
        close(pipefd2[0]);
        close(pipefd2[1]);

        execvp("cat", cat_args);
        perror("cat failed");
        exit(1);
    }
    else
    {
        perror("fork failed");
        exit(1);
    }

    return 0;
}
