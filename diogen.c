#include <stdio.h>
#include <string.h>
#include "ascii_logo.c"

typedef struct Map {
    int index;
    char key[50];
    char value[50];
} map;

void print_break_line(int length, char divider);
void clear_input_buffer();
char *question(char string[], int length, map *map);

#define DEFAULT_STRING_LENGTH 50
#define OS_SIZE_STRUCT 5
#define PHP_SIZE_STRUCT 2
#define PHP_FRAMEWORK_SIZE_STRUCT 4

int main(int argc, char *argv[])
{   
    char os[DEFAULT_STRING_LENGTH];
    char php[DEFAULT_STRING_LENGTH];
    char framework[DEFAULT_STRING_LENGTH];

    map OS, PHP, FRAMEWORK;

    map OS[OS_SIZE_STRUCT] = {
        {.index = 1, .key = "Debian 10", .value = "debian:buster-slim"},
        {.index = 2, .key = "Debian 11", .value = "debian:bullseye-slim"},
        {.index = 3, .key = "Debian 12", .value = "debian:bookworm-slim"},
        {.index = 4, .key = "Ubuntu 24.04", .value = "ubuntu:noble"},
        {.index = 5, .key = "Alpine 3", .value = "alpine:3"},
    };

    map PHP[PHP_SIZE_STRUCT] = {
        {.index = 1, .key = "8.3", .value = "php-8.3.7"},
        {.index = 2, .key = "8.2", .value = "php-8.2.19"},
    };

    map FRAMEWORK[PHP_FRAMEWORK_SIZE_STRUCT] = {
        {.index = 1, .key = "Laravel 11 (require php 8.2 or 8.3)", .value = "laravel/laravel"},
        {.index = 2, .key = "Yii2", .value = "yiisoft/yii2-app-basic"},
        {.index = 3, .key = "Symfony 7", .value = "symfony/skeleton:7.0.*"},
        {.index = 4, .key = "Without framework", .value = "no"},
    };

    welcome_message();
    print_break_line(50, '=');
    strcpy(os, question("What OS you want to use", 5, OS));
    strcpy(php, question("What PHP version you want to use", 7, PHP));
    strcpy(framework, question("What PHP framework you want to install", 4, FRAMEWORK));

    return 0;
}

void print_break_line(int length, char divider)
{
    for(size_t i = 0; i < length; i++) {
        printf("%c", divider);
    }

    printf("\n\n");
}

char *question(char string[], int length, map *map)
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