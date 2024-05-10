#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "ascii_logo.c"

#define DEFAULT_STRING_LENGTH 50
#define OS_SIZE_STRUCT 3
#define PHP_SIZE_STRUCT 1
#define PHP_FRAMEWORK_SIZE_STRUCT 2
#define SERVER_SIZE_STRUCT 2
#define DATABASE_SIZE_STRUCT 1
#define ROOT_DIRECTORY_NAME "./docker"
#define DEFAULT_DIRECTORY_PERM 0777
#define PHP_DIRECTORY_NAME  "./docker/php"
#define PHP_DOCKERFILE "./docker/php/Dockerfile"

struct Map {
    int index;
    char key[50];
    char value[50];
} OS[OS_SIZE_STRUCT] = {
    {.index = 1, .key = "Debian 10", .value = "debian:buster-slim"},
    {.index = 2, .key = "Debian 11", .value = "debian:bullseye-slim"},
    {.index = 3, .key = "Debian 12", .value = "debian:bookworm-slim"},
}, PHP[PHP_SIZE_STRUCT] = {
    {.index = 1, .key = "8.3", .value = "php-8.3.7"},
}, SERVER[SERVER_SIZE_STRUCT] = {
    {.index = 1, .key = "nginx", .value = "php-8.3.7"},
    {.index = 2, .key = "nginx unit", .value = "php-8.3.7"},
}, FRAMEWORK[PHP_FRAMEWORK_SIZE_STRUCT] = {
    {.index = 1, .key = "Laravel 11 (require php 8.2 or 8.3)", .value = "laravel/laravel"},
    {.index = 2, .key = "Without framework", .value = "no"},
}, DATABASE[DATABASE_SIZE_STRUCT] = {
    {.index = 1, .key = "MySQL 8.4", .value = "laravel/laravel"},
};

void print_break_line(int length, char divider);
void clear_input_buffer();
char *question(char string[], int length, struct Map *map);
int make_root_directory();
int make_php_directory();
int make_php_dockerfile();
int build_php_dockerfile(char* os, char* php);
void append_os(FILE *file, char* os);
void append_disable_interaction(FILE *file);
int install_packages(FILE *file);
int detach_git_parent_branch(FILE *file);
int make_subdirectories(FILE *file);
int clone_php_sources(FILE *file, char* php);
int install_php(FILE *file);
int install_composer(FILE *file);
int add_user(FILE *file);
int make_composer_directory(FILE *file);
int set_directories_owner(FILE *file);
int change_user(FILE *file);

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
    build_php_dockerfile(os, php);

    return 0;
}

int build_php_dockerfile(char* os, char* php)
{
    FILE* file = fopen(PHP_DOCKERFILE, "a");

    if (file == NULL) {
        printf("PHP Dockerfile cannot open");
        return 1;
    }

    append_os(file, os);
    append_disable_interaction(file);
    install_packages(file);
    detach_git_parent_branch(file);
    make_subdirectories(file);
    clone_php_sources(file, php);
    install_php(file);
    install_composer(file);
    add_user(file);
    make_composer_directory(file);
    set_directories_owner(file);
    change_user(file);

    fclose(file);
}

int change_user(FILE *file)
{
    char buffer[100] = "USER sammy";
    fprintf(file, "%s", buffer);
}

int set_directories_owner(FILE *file)
{
    char buffer[100] = "RUN chown -R sammy:sammy /home/samme && chown -R sammy:sammy /var/www\n\n";
    fprintf(file, "%s", buffer);
}

int make_composer_directory(FILE *file)
{
    char buffer[100] = "RUN mkdir -p /home/sammy/.composer\n";
    fprintf(file, "%s", buffer);
}

int add_user(FILE *file)
{
    char buffer[100] = "RUN useradd -G www-data,root -u 1000 -d /home/sammy sammy\n";
    fprintf(file, "%s", buffer);
}

int install_composer(FILE *file)
{
    char buffer[100] = "COPY --from=composer:latest /usr/bin/composer /usr/bin/composer\n\n";
    fprintf(file, "%s", buffer);
}

int install_php(FILE *file)
{
    char buffer[1024] = "RUN /tmp/php/buildconf --force \\\n";
    strcat(buffer, "&& /tmp/php/&& ./configure --prefix=/opt/php --enable-embed=shared --disable-short-tags --with-openssl --with-external-pcre --without-sqlite3 --without-pdo-sqlite --with-zlib --enable-bcmath --with-curl --enable-exif --enable-gd --with-jpeg --with-freetype --enable-intl --enable-mbstring --enable-pcntl --with-pdo-pgsql --with-pgsql --with-libedit --with-readline --enable-soap --enable-sockets --with-sodium --with-password-argon2 --enable-sysvmsg --enable-sysvsem --enable-sysvshm --with-zip --with-gnu-ld=yes --enable-zts \\\n");
    strcat(buffer, "&& make && make install && ln -s /opt/php/bin/php /usr/bin\n\n");
    fprintf(file, "%s", buffer);
}

int clone_php_sources(FILE *file, char* php)
{
    char buffer[100] = "RUN git clone --depth=1 --branch=";
    strcat(buffer, php);
    strcat(buffer, " https://github.com/php/php-src.git /tmp/php\n\n");
    fprintf(file, "%s", buffer);
}

int make_subdirectories(FILE *file)
{
    fprintf(file, "%s", "RUN mkdir -p /tmp/php /opt/php /var/www\n");
}

int detach_git_parent_branch(FILE *file)
{
    fprintf(file, "%s", "RUN git config --global advice.detachedHead false\n");
}

int install_packages(FILE *file)
{
    char buffer[1024] = "RUN apt-get update -y && apt-get install -y \\ \n";
    strcat(buffer, "\tbuild-essential \\ \n");
    strcat(buffer, "\tpkg-config \\ \n");
    strcat(buffer, "\tautoconf \\ \n");
    strcat(buffer, "\tbison \\ \n");
    strcat(buffer, "\tre2c \\ \n");
    strcat(buffer, "\tlibxml2-dev \\ \n");
    strcat(buffer, "\tlibsqlite3-dev \\ \n");
    strcat(buffer, "\tcurl \\ \n");
    strcat(buffer, "\tgit \\ \n");
    strcat(buffer, "\tlibssl-dev \\ \n");
    strcat(buffer, "\tlibpcre2-dev \\ \n");
    strcat(buffer, "\tlibpng-dev \\ \n");
    strcat(buffer, "\tlibjpeg-dev \\ \n");
    strcat(buffer, "\tlibfreetype6-dev \\ \n");
    strcat(buffer, "\tlibonig-dev \\ \n");
    strcat(buffer, "\tlibcurl4-gnutls-dev \\ \n");
    strcat(buffer, "\tzlib1g-dev \\ \n");
    strcat(buffer, "\tlibpq-dev \\ \n");
    strcat(buffer, "\tlibedit-dev \\ \n");
    strcat(buffer, "\tlibsodium-dev \\ \n");
    strcat(buffer, "\tlibargon2-dev \\ \n");
    strcat(buffer, "\tlibzip-dev\n\n");

    fprintf(file, "%s", buffer);
}

void append_disable_interaction(FILE *file)
{
    char buffer[256];
    strcpy(buffer, "ENV DEBIAN_FRONTEND noninteractive\n\n");

    fprintf(file, "%s", buffer);
}

void append_os(FILE *file, char* os)
{
    char buffer[256];
    strcpy(buffer, "FROM ");
    strcat(buffer, os);
    strcat(buffer, "\n\n");

    fprintf(file, "%s", buffer);
}

int make_php_dockerfile()
{
    char path[100];
    strcpy(path, PHP_DIRECTORY_NAME);
    strcat(path, "/Dockerfile");
    FILE* file = fopen(path, "w");
    return fclose(file);
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