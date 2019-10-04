//
// Created by fszhuangb on 2019-09-29.
//
#include "basic_IO.h"


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
        position++;

        /**
         * 这里要记得进行buffersize的检查
         * 大于buffersize要重新分配空间
        */
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[position] = NULL;
    // hist[position] = NULL;
    return tokens;
}

void add_history(const char *line)
{
    if (count < HISTORY_SIZE)
        count++;
    free(history[head]);
    history[head] = strdup(line);
    head = forward(head, 1);
}

void print_history(void)
{
    for (size_t i = count; i; i--)
        printf("%lu %s\n", i, history[backward(head, i)]);
}

void clear_history(void)
{
    for (size_t i = 0; i < HISTORY_SIZE; i++) {
        free(history[i]);
        history[i] = NULL;
    }
}

/**
 * @returns:
 *          error number 0:  It means error but can continue.
 *          error number -1: It means error and there's a command error.
 * */
int check_history(char *line)
{
    size_t index, id;

    if (line[0] != '!')
        return 0;
    if (line[1] == '!')
    {
        if (count == 0)
        {
            fprintf(stderr, "No command in history.\n");
            return -1;
        }
        /** 获取index 1 */
        index = backward(head, 1);
    }
    else if (isdigit(line[1]))
    {
        printf("It's digit!");
        sscanf(&line[1], "%lu", &id);
        if (id == 0 || id > count)
        {
            fprintf(stderr, "No such command in history\n");
            return -1;
        }
        index = backward(head, id);
    }
    else
    {
        fprintf(stderr, "Invalid syntax for history");
        return -1;
    }

    strcpy(line, history[index]);
    printf("%s\n", line);
    return 0;
}