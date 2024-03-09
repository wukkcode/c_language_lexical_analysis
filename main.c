#define _CRT_SECURE_NO_WARNINGS
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

static void run(const char *source)
{
    // 初始化词法分析器
    initScanner(source);

    int line = -1;
    // 打印Token, 遇到TOKEN_EOF为止
    for (;;)
    {
        Token token = scanToken();

        if (token.line != line)
        {
            printf("%4d ", token.line);
            line = token.line;
        }
        else
        {
            printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start);

        if (token.type == TOKEN_EOF)
            break;
    }
}

static void repl()
{
    // 与用户交互，用户每输入一行代码，分析一行代码，并将结果输出
    // repl是"read evaluate print loop"的缩写
    // 调用run()
    char source_code[1024];
    printf(">");
    fgets(source_code, 1023, stdin);
    run(source_code);
    return;
}

static char *readFile(const char *path)
{
    // 用户输入文件名，将整个文件的内容读入内存，并在末尾添加'\0'
    // 注意: 这里应该使用动态内存分配，因此应该事先确定文件的大小。
    FILE *source_file = fopen(path, "r");
    if (source_file == NULL)
    {
        printf("Error: open file %s failed\n", path);
        fclose(source_file);
        exit(0);
    }
    fseek(source_file, 0L, SEEK_END);
    long int file_length = ftell(source_file);
    rewind(source_file);
    char *buffer = (char *)malloc(sizeof(char) * (file_length + 1));
    if (buffer == NULL)
    {
        printf("Error: malloc failed in readFile() function!\n");
        fclose(source_file);
        exit(1);
    }
    fread(buffer, sizeof(char), file_length, source_file);
    buffer[file_length] = '\0';
    fclose(source_file);
    return buffer;
}

static void runFile(const char *path)
{
    // 处理'.c'文件:用户输入文件名，分析整个文件，并将结果输出
    // 调用run
    char *source_code = readFile(path);
    run(source_code);
    free(source_code);
    return;
}

int main(int argc, const char *argv[])
{
    if (argc == 1)
    {
        // ./scanner 没有参数,则进入交互式界面
        repl();
    }
    else if (argc == 2)
    {
        // ./scanner file 后面跟一个参数,则分析整个文件
        runFile(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}