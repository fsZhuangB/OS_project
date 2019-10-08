#ifndef BASIC_H
#define BASIC_H
#include "basic_IO.h"


 /** 
  * this function initlize for shell
  * */
void lsh_loop()
{
    char *line;
    char **args;
    int status = 1;
    int position = 0;

    while (status)
    {
        printf(">osh ");
        fflush(stdout);
        line = read_line();
        args = split_line(line);

        add_history(position, args);

        if (strcmp(args[0], "history") == 0)
        {
            history(hist);
        }
        else
        {
            status = lsh_execute(args);
        }
        position++;

        free(line);
        free(args);
    }
}

char *read_line(void)
{
    int bufferSize = BUFFER_SIZE;
    int c, position = 0;
    char *buffer = malloc(sizeof(char) * bufferSize);

    /** 如果分配失败 */
    if (!buffer)
    {    
        fprintf(stderr, "osh: allocation error!\n");
        exit(EXIT_FAILURE);
    }

    /** 采取暂时分配一定长度的空间， 如果分配不够， 再重新分配
     *  动态增长的策略
     */
    while (1)
    {
        c = getchar();
        if (c == '\n' || c == EOF)
        {
            buffer[position] = '\0'; // 如果读取到末尾，则将其替换为'\0'并返回
            return buffer;
        }
        else
        {
            buffer[position] = c;
        }
        position++;

        /** 暂时不处理buffer溢出的情况 */
    }
}

/** 处理token */
char **split_line(char *line)
{
    int bufferSize = BUFFER_SIZE, position = 0;

    /** 指向字符数组的指针tokens */
    char **tokens = malloc(bufferSize * sizeof(char*)); // char * 的大小乘 bufferSize的空间
    char *token;

    if (!tokens)
    {
        fprintf(stderr, "osh: allocation error...\n");
        exit(EXIT_FAILURE);
    }

    /** 使用标点划分命令 */
    token = strtok(line, TOKEN_DELIM);
    while (token != NULL)
    {
        /**
         * 在这里将划分好的命令存入tokens数组
        */
        tokens[position] = token;
        // hist[position] = token;
        position++;

        /** 这里要记得进行buffersize的检查 
         *  大于buffersize要重新分配空间
        */
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    // hist[position] = NULL;
    return tokens;
}

int launch(char **args)
{

    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // printf("This is the command %s", args[0]);
        // 这是子进程
        if (execvp(args[0], args) == -1)
        {
            perror("osh");
        }

        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        // 出错状态
        perror("osh");
    }
    else
    {
        do 
        {
            /**
             *  pid用来指定想等待的进程id
             *  WUNTRACED: 为了实现某种操作，由pid指定的任意一进程已经被暂停，
             *  且其状态自暂停以来还未被报告过，则返回其状态
             * */
            wpid = waitpid(pid, &status, WUNTRACED);

        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

int lsh_num_builtins(void)
{
    return sizeof(builtin_str) / sizeof(char *);
}

/**
 * 内置函数的实现
*/
int lsh_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    }
    else
    {
        if (chdir(args[1]) != 0)
            perror("osh");
    }
    return 1;
}

int lsh_help(char **args)
{
  int i;
  printf("Fletcher's OSH\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for (i = 0; i < lsh_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Use the man command for information on other programs.\n");
  return 1;
}

int lsh_exit(char **args)
{
    return 0;
}

int lsh_execute(char **args)
{
    int i;
    if (args[0] == NULL)
        return 1;

    for (i = 0; i < lsh_num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }


    if (strcmp(args[0], "!") == 0)
    {
        return launch(hist);
    }

    return launch(args);
}

int history(char **args)
{
    int i = 0;
    char *commands = hist[i];

    for ( ; i < count; i++) {
        printf("%d %s\n", i, commands);
    }

    return 1;
}

int main(void)
{
    lsh_loop();
  
	return EXIT_SUCCESS;

}
#endif