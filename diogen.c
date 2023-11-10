#include <stdio.h>

void welcome_message();
void debian_version_list();
int question(char *pmessage, char **plist, int list_length);

int main(int argc, char *argv[])
{
    welcome_message();
    debian_version_list();

    return 0;
}

void debian_version_list()
{
    char *list[] = {"1) Latest", "2) 10", "3) 11"};
    question("What is the debian version you want to install?", list, 3);
}

void welcome_message()
{
    printf("================================\n");
    printf("== Welcome to diogen software ==\n");
    printf("================================\n");
    printf("\n");
}

int question(char *pmessage, char **plist, int list_length)
{
    printf("%s\n", pmessage);

    for (int i = 0; i < list_length; ++i) {
        printf("%s\n", *(plist + i));
    }
}