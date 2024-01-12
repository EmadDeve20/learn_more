#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024
#define MAX_BUFFER_ARRAY_SIZE 100
#define EXAMPLE_FILE "programing_languages"
#define EXAMPLE_FILE_SIZE strlen(EXAMPLE_FILE)+1
#define NORLAL_REGEX_PATTERN "^[a-zA-Z]+.* \\[]$"
#define PASSED_REGEX_PATTERN "^[a-zA-Z]+.* \\[(x|X)]$"
#define True 1
#define False 0


int check_line_format(const char *slice_str)
{   
    regex_t reg_normal, reg_passed;
    char cp_slice_str[BUFFER_SIZE] = {"\0"};
    int check_normal, check_passed;

    strncat(cp_slice_str, slice_str, strlen(slice_str));
    cp_slice_str[strlen(cp_slice_str) - 1] = '\0';

    check_normal = regcomp(&reg_normal, NORLAL_REGEX_PATTERN, REG_EXTENDED);
    check_passed = regcomp(&reg_passed, PASSED_REGEX_PATTERN, REG_EXTENDED);

    if (check_normal || check_passed)
    {   
        printf("Could not compile regex\n");
        return False;
    }

    check_normal = regexec(&reg_normal, cp_slice_str, 0, NULL, 0);
    check_passed = regexec(&reg_passed, cp_slice_str, 0, NULL, 0);


    if (check_normal == 0)
    {
        // Regex Normal Match
        return True;
    }
    else if (check_passed == 0)
    {
        // Regex Passed Match
        return 2;
    }
    else
    {
        // Regex match failed
        return False;
    }
}


int read_file(char *buffer, char *file_name, char *error_line_text, int *error_line_number)
{
    FILE *file;
    char line_text[BUFFER_SIZE] = {"\0"};
    int line_number = 1;

    file = fopen(file_name, "r");

    if (file == NULL)
    {
        return False;
    }

    while (fgets(line_text, sizeof(line_text), file))
    {   
        if (check_line_format(line_text) == 2)
        {
            line_number++;
            continue;
        }

        if (!check_line_format(line_text))
        {
            strncat(error_line_text, line_text, strlen(line_text));
            *error_line_number = line_number;
            return False;
        }
        else
        {
            strncat(buffer, line_text, strlen(line_text));
            line_number++;
        }
    }

    fclose(file);

    return True;
}


void get_todo_list(const char *buffer, char list[MAX_BUFFER_ARRAY_SIZE][BUFFER_SIZE])
{
    char cp_buffer[BUFFER_SIZE] = {0};
    int list_index = 0;

    strncat(cp_buffer, buffer, strlen(buffer));

    char *token = strtok(cp_buffer, "\n");
    char todo[BUFFER_SIZE] = {0};

    while (token != NULL)
    {   
        for (int i = 0; token[i] != ' '; i++)
        {   
            todo[i] = token[i];
        }
        strncpy(list[list_index++], todo, strlen(todo));
        memset(todo, '\0', BUFFER_SIZE);
        token = strtok(NULL, "\n");        
    }
}

// TODO: Complete this Function to edit file
int write_in_file(char *file_name)
{
    FILE *file;

    file = fopen(file_name, "r+");

    fseek(file, 2, SEEK_SET);

    fputs("x", file);

    fclose(file);
}


int main(int argc, char *argv[])
{   
    int opt;
    int file_name_customized = 0;
    char file_name[100];
    char buffer[BUFFER_SIZE] = {"\0"};
    int error_line_number = 0;
    char error_line_text[BUFFER_SIZE] = {"\0"};
    char todo_list[MAX_BUFFER_ARRAY_SIZE][BUFFER_SIZE];

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


    if (read_file(buffer, file_name, error_line_text, &error_line_number) == 0)
    {
        printf("Got Error To Open %s File!\n", file_name);
        printf("error in line %d: %sinvalid text format!\n", error_line_number, error_line_text);
        return 1;
    }

    get_todo_list(buffer, todo_list);

    printf("%s\n", buffer);
    // return True;

}
