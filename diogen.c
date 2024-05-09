#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ascii_logo.c"

#define DEFAULT_STRING_LENGTH 50
#define OS_SIZE_STRUCT 5
#define PHP_SIZE_STRUCT 2
#define PHP_FRAMEWORK_SIZE_STRUCT 4
#define DATABASE_SIZE_STRUCT 4
#define ROOT_DIRECTORY_NAME "./docker"
#define DEFAULT_DIRECTORY_PERM 0777
#define PHP_DIRECTORY_NAME  "./docker/php"

struct Map {
    int index;
    char key[50];
    char value[50];
} OS[OS_SIZE_STRUCT] = {
    {.index = 1, .key = "Debian 10", .value = "debian:buster-slim"},
    {.index = 2, .key = "Debian 11", .value = "debian:bullseye-slim"},
    {.index = 3, .key = "Debian 12", .value = "debian:bookworm-slim"},
    {.index = 4, .key = "Ubuntu 24.04", .value = "ubuntu:noble"},
    {.index = 5, .key = "Alpine 3", .value = "alpine:3"},
}, PHP[PHP_SIZE_STRUCT] = {
    {.index = 1, .key = "8.3", .value = "php-8.3.7"},
    {.index = 2, .key = "8.2", .value = "php-8.2.19"},
}, FRAMEWORK[PHP_FRAMEWORK_SIZE_STRUCT] = {
    {.index = 1, .key = "Laravel 11 (require php 8.2 or 8.3)", .value = "laravel/laravel"},
    {.index = 2, .key = "Yii2", .value = "yiisoft/yii2-app-basic"},
    {.index = 3, .key = "Symfony 7", .value = "symfony/skeleton:7.0.*"},
    {.index = 4, .key = "Without framework", .value = "no"},
}, DATABASE[DATABASE_SIZE_STRUCT] = {
    {.index = 1, .key = "MySQL 8.4", .value = "laravel/laravel"},
    {.index = 2, .key = "PostgreSQL 16.2", .value = "yiisoft/yii2-app-basic"},
    {.index = 3, .key = "Percona MySQL 8.0", .value = "symfony/skeleton:7.0.*"},
    {.index = 4, .key = "MariaDB 11.5", .value = "no"},
};

void print_break_line(int length, char divider);
void clear_input_buffer();
char *question(char string[], int length, struct Map *map);
int make_root_directory();
int make_php_directory();
int make_php_dockerfile();

int main(int argc, char *argv[])
{   
    char os[DEFAULT_STRING_LENGTH];
    char php[DEFAULT_STRING_LENGTH];
    char framework[DEFAULT_STRING_LENGTH];
    char database[DEFAULT_STRING_LENGTH];

    welcome_message();
    print_break_line(50, '=');
    strcpy(os, question("What OS you want to use", OS_SIZE_STRUCT, OS));
    strcpy(php, question("What PHP version you want to use", PHP_SIZE_STRUCT, PHP));
    strcpy(framework, question("What PHP framework you want to install", PHP_FRAMEWORK_SIZE_STRUCT, FRAMEWORK));
    strcpy(database, question("What database you want to install", DATABASE_SIZE_STRUCT, DATABASE));

    make_root_directory();
    make_php_directory();
    make_php_dockerfile();

    return 0;
}

int make_php_dockerfile()
{
    char path[100];
    strcpy(path, PHP_DIRECTORY_NAME);
    strcat(path, "/Dockerfile");
    FILE* ptr = fopen(path, "w");
    return fclose(ptr);
}

int make_php_directory()
{
    return mkdir(PHP_DIRECTORY_NAME, DEFAULT_DIRECTORY_PERM);
}

int make_root_directory()
{
    return mkdir(ROOT_DIRECTORY_NAME, DEFAULT_DIRECTORY_PERM);
}

void print_break_line(int length, char divider)
{
    for(size_t i = 0; i < length; i++) {
        printf("%c", divider);
    }

    printf("\n\n");
}

char *question(char string[], int length, struct Map *map)
{
    unsigned short num;
    char result[DEFAULT_STRING_LENGTH];

    printf("%s\n\n", string);
    
    for(size_t i = 0; i < length; i++) {
        printf("%d - %s\n", map[i].index, map[i].key);
    }

    printf("Enter number: ");
    
    while((scanf("%d", &num) != 1) || (num > 3 || num < 1)) {
        printf("Wrong variant\n");
        clear_input_buffer();
        printf("Enter number: ");
    }
    
    for(size_t i = 0; i < length; i++) {
        if (map[i].index == num) {
            printf("You selected %s\n\n", map[i].key);
            strcpy(result, map[i].value);
        }
    }

    char *ptr = result;

    return ptr;
}

void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}