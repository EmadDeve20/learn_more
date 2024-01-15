#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
#define MAX_BUFFER_ARRAY_SIZE 100
#define EXAMPLE_FILE "programing_languages"
#define EXAMPLE_FILE_SIZE strlen(EXAMPLE_FILE)+1
#define NORLAL_REGEX_PATTERN "^[a-zA-Z]+.* \\[]$"
#define PASSED_REGEX_PATTERN "^[a-zA-Z]+.* \\[(x|X)]$"
#define COMMENT_REGEX_PATTERN "^#.*"
#define True 1
#define False 0
#define EXIT_CHARACTER 'Q'
#define DONE_CHARACTER 'D'
#define RELOAD_CHARACTER 'R'


int check_line_format(const char *slice_str)
{   
    regex_t reg_normal, reg_passed, reg_comment;
    char cp_slice_str[BUFFER_SIZE] = {"\0"};
    int check_normal, check_passed, check_comment;

    strncat(cp_slice_str, slice_str, strlen(slice_str));
    cp_slice_str[strlen(cp_slice_str) - 1] = '\0';

    check_normal = regcomp(&reg_normal, NORLAL_REGEX_PATTERN, REG_EXTENDED);
    check_passed = regcomp(&reg_passed, PASSED_REGEX_PATTERN, REG_EXTENDED);
    check_comment = regcomp(&reg_comment, COMMENT_REGEX_PATTERN, REG_EXTENDED);

    if (check_normal || check_passed || check_comment)
    {   
        printf("Could not compile regex\n");
        return False;
    }

    check_normal = regexec(&reg_normal, cp_slice_str, 0, NULL, 0);
    check_passed = regexec(&reg_passed, cp_slice_str, 0, NULL, 0);
    check_comment = regexec(&reg_comment, cp_slice_str, 0, NULL, 0);


    if (check_normal == 0)
    {
        // Regex Normal Match
        return True;
    }
    else if (check_passed == 0 || check_comment == 0)
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


int read_file(char *buffer, char *file_name, char *error_line_text, int *line_number)
{
    FILE *file;
    char line_text[BUFFER_SIZE] = {"\0"};
    int line = 1;

    file = fopen(file_name, "r");

    if (file == NULL)
    {
        return False;
    }

    while (fgets(line_text, sizeof(line_text), file))
    {   
        if (check_line_format(line_text) == 2)
        {
            continue;
        }
        if (!check_line_format(line_text))
        {
            strncat(error_line_text, line_text, strlen(line_text));
            *line_number = line;
            return False;
        }
        else
        {
            strncat(buffer, line_text, strlen(line_text));
        }
        line++;
    }

    // TODO: find a solution to handle this line number better than this shit!
    *line_number = line-2;

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

int generate_random_todo(int *line)
{
    return rand() % *line;
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


void remove_item_in_todo_list(int index, char todo_list[MAX_BUFFER_ARRAY_SIZE][BUFFER_SIZE], int *line_number)
{   
    if (index != *line_number)
    {   
        for (int i = 0; i < *line_number; i++)
        {
            if (i == index)
            {
                memset(todo_list[index], '\0', strlen(todo_list[index]));
                strncat(todo_list[index], todo_list[index+1], strlen(todo_list[index+1]));
                index++;
            }
        }
    }
    else
    {
        memset(todo_list[index], '\0', strlen(todo_list[index]));
    }

    *line_number -= 1;
}

void init_menu(char todo_list[MAX_BUFFER_ARRAY_SIZE][BUFFER_SIZE], int *line_number)
{   
    int random_n, prev_random;
    char select_char;
    
    random_n = generate_random_todo(line_number);

    prev_random = random_n;

    printf("if you want to exit enter (Q or q)\n");
    printf("%s selected! if you have done this item please Enter (d or D) if you want to reload enter (r or R)!\n", todo_list[random_n]);
    
    do
    {
        scanf("%c", &select_char);

        select_char = toupper(select_char);

        switch (select_char)
        {
            case DONE_CHARACTER:
                printf("%s Done!\n", todo_list[random_n]);
                remove_item_in_todo_list(random_n, todo_list, line_number);
                break;
            
            case RELOAD_CHARACTER:
                do {
                    random_n = generate_random_todo(line_number);
                }
                while (random_n == prev_random && *line_number > 1);

                prev_random = random_n;
                printf("%s selected! if you have done this item please Enter (d or D) if you want to reload enter (r or R)!\n", todo_list[random_n]);
                break;

            case EXIT_CHARACTER:
                return;

            default:
                break;
        }

    } while (*line_number >= 0);
    
}

int main(int argc, char *argv[])
{   
    srand(time(0));

    int opt;
    int file_name_customized = 0;
    char file_name[100];
    char buffer[BUFFER_SIZE] = {"\0"};
    int line_number = 0;
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


    if (read_file(buffer, file_name, error_line_text, &line_number) == 0)
    {
        printf("Got Error To Open %s File!\n", file_name);
        printf("error in line %d: %sinvalid text format!\n", line_number, error_line_text);
        return 1;
    }

    get_todo_list(buffer, todo_list);

    init_menu(todo_list, &line_number);

}
