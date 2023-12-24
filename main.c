#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define EXAMPLE_FILE "programing_languages.example"
#define EXAMPLE_FILE_SIZE strlen(EXAMPLE_FILE)+1


int read_file(char *buffer, char *file_name)
{
    FILE *file;

    file = fopen(file_name, "r");

    if (file == NULL)
    {
        printf("Warning!!!");
        return 1;
    }

    fread(buffer, sizeof(char), BUFFER_SIZE, file);

    fclose(file);

    return 0;
}


int main(int argc, char *argv[])
{   
    int opt;
    int file_name_customized = 0;
    char file_name[100];
    char buffer[BUFFER_SIZE] = {"\0"};

    while ((opt = getopt(argc, argv, "f:")) != -1) {
        switch (opt) {
            case 'f':
                snprintf(file_name, strlen(argv[2])+1, "%s", argv[2]);
                file_name_customized = 1;
                break;
            default:
                snprintf(file_name, EXAMPLE_FILE_SIZE, EXAMPLE_FILE);
        }
    }

    if (!file_name_customized)
    {
        snprintf(file_name, EXAMPLE_FILE_SIZE, EXAMPLE_FILE);
    }

    char buffer[BUFFER_SIZE] = {"\0"};

    if (read_file(buffer, EXAMPLE_FILE) == 1)
    {
        printf("Got Error To Open %s File!\n", file_name);
        return errno;
    }

    printf("%s\n", buffer);

}
