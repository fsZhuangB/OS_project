#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int pid = fork();
    if (pid < 0)
        {
            fprintf(stderr, "Forked Failed");
            return 1;
        }
    else if (pid == 0)
    {
        printf("I am child!\n");
    }
    else
    {
        /** parent will wait for the child to complete */
        wait(NULL);
        printf("Child complete");
    }
    return 0;
}