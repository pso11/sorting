#include <TXLib.h>
#include <stdio.h>
#include <string.h>

#define LINES 6
#define MAX_LENGTH_LINE 50

void sort_lines(char* string_array);
void print_lines(char* string_array);
char* create_string_array(void);
bool first_line_upper(char* line, char* next_line);
void swap(char* line, char* next_line);

int main(void)
{
    char* string_array = create_string_array();

    sort_lines(string_array);

    print_lines(string_array);

    return 0;
}

void sort_lines(char* string_array)
{
    for (int i = LINES - 1; i > 0 ; i--)
    {
        for (int j = 0; j < i; j++)
        {
            char* line = string_array + j * MAX_LENGTH_LINE;
            char* next_line = line + MAX_LENGTH_LINE;

            if (first_line_upper(line, next_line))
                swap(line, next_line);
        }
    }
}

void print_lines(char* string_array)
{
    for (size_t i = 0; i < LINES; i++)
    {
        printf("%s\n", string_array + i * MAX_LENGTH_LINE);
    }
}

char* create_string_array(void)
{
    static char string_array[LINES][MAX_LENGTH_LINE] =
    {
        "pshanichnaya arina",
        "pshanichnaya sofia",
        "my mum",
        "my granny",
        "my dad",
        "rostics"
    };
    return (char*)string_array;
}

bool first_line_upper(char* line, char* next_line)
{
    return strcmp(line, next_line) > 0;
}

void swap(char* line, char* next_line)
{
    for (int i = 0; i < MAX_LENGTH_LINE; i++)
    {
        char temp = *(line + i);
        line[i] = next_line[i];
        next_line[i] = temp;
    }
}
