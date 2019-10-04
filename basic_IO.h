#include <stdio.h>
#include <stdlib.h>    // malloc()
#include <string.h>    // use strtok()
#include <sys/wait.h>  // waitpid()
#include <unistd.h>    // chdir(), fork(), exec(), pid_t
#include <errno.h>     // errno
#include <ctype.h>     // isdigit()


#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1024 
#define HISTORY_SIZE 10 /* buffer size for storing history of commands */
#define TOKEN_DELIM " \t\r\n\a"
#define backward(head, n)	(((head) + HISTORY_SIZE - (n)) % HISTORY_SIZE)
#define forward(head, n)	(((head) + (n)) % HISTORY_SIZE)

static char *history[HISTORY_SIZE]; /** 这是储存历史命令的数组 */
static int count, head;
static int args_count = 0;

/**
 * 内置shell命令说明
 * cd：由于当前进程只能改变它自己的目录，而不能改变子进程的目录，所以只能自己实现
*/
//int my_cd(char **args);
//int my_help(char **args);
//int my_exit(char **args);

/** this function record history of commands */
void add_history(const char *line);

void print_history(void);

void clear_history(void);

/** 检测历史数组，查看是否有内容，并进行打印 */
int check_history(char *line);

//char * builtin_str[] = {
//    "cd",
//    "help",
//    "exit",
//};

/** 函数指针数组, 指向相应的函数入口位置 */
//int (*builtin_func[]) (char**) = {
//    &my_cd,
//    &my_help,
//    &my_exit,
//};


 /** this function read line from user */
char * read_line();

 /** this function split user's input */
char ** split_line(char* line);

int list_num_builtins(void);

int check_background(char **args);

void init(void);







