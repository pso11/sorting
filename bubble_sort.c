#include <TXLib.h>
#include <stdio.h>
#include <string.h>

#define LINES 6
#define MAX_LENGTH_LINE 50

void sort_lines(char* string_array);
void print_lines(char* string_array);
char* create_string_array(void);

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
            if (strcmp(string_array + j * MAX_LENGTH_LINE, string_array + (j + 1) * MAX_LENGTH_LINE) > 0)
            {
                char temp[MAX_LENGTH_LINE] = "";
                strncpy(temp, string_array + j * MAX_LENGTH_LINE, strlen(string_array + j * MAX_LENGTH_LINE) + 1);
                strncpy(string_array + j * MAX_LENGTH_LINE, string_array + (j + 1) * MAX_LENGTH_LINE, 
                        strlen(string_array + (j + 1) * MAX_LENGTH_LINE) + 1);
                strncpy(string_array + (j + 1) * MAX_LENGTH_LINE, temp, strlen(temp) + 1);
            }
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
