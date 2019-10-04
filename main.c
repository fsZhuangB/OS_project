//
// Created by fszhuangb on 2019-09-29.
//

#include "basic_IO.h"

int main(void)
{
    char *line2, **args;
    pid_t pid, wpid;
    int status;

    /** 函数主循环 */
    while (TRUE)
    {
        printf("osh> ");
        fflush(stdout);

        line2 = read_line();

        args = split_line(line2);

        /** 检测是否未输入值，直接结束循环，进行下一次循环 */
        if (args[0] == NULL)
            continue;
        else if (check_history(line2) == -1)
            continue;
        else
            add_history(line2);

        if (strcmp(args[0], "exit") == 0)
            break;
        else if (strcmp(args[0], "history") == 0)
            print_history();
        else if ((pid = fork()) == 0)
        {
            if (execvp(args[0], args) == -1)
            {
                fprintf(stderr, "%s\n", strerror(errno));
                break;
            }
        }
        /** 对于父进程，父进程首先挂起，等待子进程的完成，再继续运行 */
        else if (pid  > 0)
        {
            do{
                wpid =  waitpid(pid, &status, WUNTRACED);
            }while(!WIFEXITED(status) && !WIFSIGNALED(status));
        }
        else
        {
            perror("error forking!");
        }
    }
    free(line2);
    free(args);
    clear_history();
    return 0;
}
