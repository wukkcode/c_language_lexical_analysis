#define _CRT_SECURE_NO_WARNINGS
#include "scanner.h"
#include <stdio.h>
#include <stdlib.h>

static void run(const char *source)
{
    // ��ʼ���ʷ�������
    initScanner(source);

    int line = -1;
    // ��ӡToken, ����TOKEN_EOFΪֹ
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
    // ���û��������û�ÿ����һ�д��룬����һ�д��룬����������
    // repl��"read evaluate print loop"����д
    // ����run()
    char source_code[1024];
    printf(">");
    fgets(source_code, 1023, stdin);
    run(source_code);
    return;
}

static char *readFile(const char *path)
{
    // �û������ļ������������ļ������ݶ����ڴ棬����ĩβ���'\0'
    // ע��: ����Ӧ��ʹ�ö�̬�ڴ���䣬���Ӧ������ȷ���ļ��Ĵ�С��
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
    // ����'.c'�ļ�:�û������ļ��������������ļ�������������
    // ����run
    char *source_code = readFile(path);
    run(source_code);
    free(source_code);
    return;
}

int main(int argc, const char *argv[])
{
    if (argc == 1)
    {
        // ./scanner û�в���,����뽻��ʽ����
        repl();
    }
    else if (argc == 2)
    {
        // ./scanner file �����һ������,����������ļ�
        runFile(argv[1]);
    }
    else
    {
        fprintf(stderr, "Usage: scanner [path]\n");
        exit(1);
    }

    return 0;
}