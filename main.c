#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>

#define BUFFER_SIZE 1024
#define EXAMPLE_FILE "programing_languages.example"
#define EXAMPLE_FILE_SIZE strlen(EXAMPLE_FILE)+1
#define REGEX_PATTERN "^[a-zA-Z]+.* \\[]$"
#define True 1
#define False 0


int check_line_format(const char *slice_str)
{   
    regex_t reegex;
    char cp_slice_str[BUFFER_SIZE] = {"\0"};
    int check;

    strncat(cp_slice_str, slice_str, strlen(slice_str));
    cp_slice_str[strlen(cp_slice_str) - 1] = '\0';

    check = regcomp(&reegex, REGEX_PATTERN, REG_EXTENDED);

    if (check)
    {   
        printf("Could not compile regex\n");
        return False;
    }

    check = regexec(&reegex, cp_slice_str, 0, NULL, 0);


    if (check == 0)
    {
        // Regex Match
        return True;
    }
    else if (check == REG_NOMATCH)
    {
        // No match found
        return False;
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


int main(int argc, char *argv[])
{   
    int opt;
    int file_name_customized = 0;
    char file_name[100];
    char buffer[BUFFER_SIZE] = {"\0"};
    int error_line_number = 0;
    char error_line_text[BUFFER_SIZE] = {"\0"};

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


    if (read_file(buffer, EXAMPLE_FILE, error_line_text, &error_line_number) == 0)
    {
        printf("Got Error To Open %s File!\n", file_name);
        printf("error in line %d: %sinvalid text format!\n", error_line_number, error_line_text);
        return 1;
    }

    printf("%s\n", buffer);
    // return True;

}
