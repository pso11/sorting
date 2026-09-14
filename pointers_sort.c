#include <TXLib.h>
#include <stdio.h>
#include <string.h>

const int LINES = 7;

#define MAX_LENGTH_LINE 50

void   sort_lines(char** pointers_array);
void   print_lines(char** pointers_array);
char*  create_string_array(void);
char** create_pointers_array(char* string_array);
bool   first_string_upper(char** pointers_array, int j);
void   transposition(char** pointers_array, int j);

int main(void)
{
    char* string_array = create_string_array();

    char** pointers_array = create_pointers_array(string_array);

    sort_lines(pointers_array);

    print_lines(pointers_array);

    return 0;
}

void sort_lines(char** pointers_array)
{
    for (int i = LINES - 1; i > 0 ; i--)
    {
        int counter_changes = 0;

        for (int j = 0; j < i; j++)
        {
            if (first_string_upper(pointers_array, j))
            {
                transposition(pointers_array, j);

                counter_changes++;
            }
        }

        if (counter_changes == 0)
            break;
    }
}

void print_lines(char** pointers_array)
{
    for (size_t i = 0; i < LINES; i++)
    {
        printf("%s\n", *(pointers_array + i));
    }
}

char* create_string_array(void)
{
    static char string_array[LINES][MAX_LENGTH_LINE] =
    {
        "a",
        "c",
        "e",
        "g",
        "f",
        "d",
        "b"
    };

    return (char*)string_array;
}

char** create_pointers_array(char* string_array)
{
    static char* pointers_array[LINES] = {};
    for (int i = 0; i < LINES; i++)
        pointers_array[i] = string_array + i * MAX_LENGTH_LINE;

    return pointers_array;
}

bool first_string_upper(char** pointers_array, int j)
{
    return strcmp(pointers_array[j], pointers_array[j + 1]) > 0;
}

void transposition(char** pointers_array, int j)
{
    char* temp = pointers_array[j];
    pointers_array[j] = pointers_array[j + 1];
    pointers_array[j + 1] = temp;
}
