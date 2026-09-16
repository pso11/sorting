#include <TXLib.h>
#include <stdio.h>
#include <string.h>

#define LINES 5
#define MAX_LENGTH_LINE 63
#define EXCHANGE_PARTS(line, next_line, removal, type) {                \
                                                                        \
        type buffer = *((type*) (line + removal));                      \
                                                                        \
        *((type*)(line   + removal)) = *((type*)(next_line + removal)); \
        *((type*)(next_line + removal)) = buffer;                       \
                                                                        \
        removal += sizeof(type); }

//Putted in cycle "for"
#define CONDITION_0                                                           \
        size_t k = 0; k < MAX_LENGTH_LINE / sizeof(unsigned long long); k++   \

#define CONDITION_1                                                           \
        ((MAX_LENGTH_LINE - removal) >= 4)                                    \

#define CONDITION_2                                                           \
        ((MAX_LENGTH_LINE - removal) >= 2)                                    \

#define CONDITION_3                                                           \
        ((MAX_LENGTH_LINE - removal) != 0)                                    \


void sort_lines(char* string_array);
void print_lines(char* string_array);
char* create_string_array(void);
bool first_line_upper(char* line, char* next_line);

int main(void)
{
    char* string_array = create_string_array();

    sort_lines(string_array);

    print_lines(string_array);

    return 0;
}

void sort_lines(char* string_array)
{
    for (int i = 0; i < LINES - 1; i++)
    {
        for (int j = 0; j < LINES - i - 1; j++)
        {
            char* line = string_array + j * MAX_LENGTH_LINE;
            char* next_line = line + MAX_LENGTH_LINE;

            if (first_line_upper(line, next_line))
            {
                size_t removal = 0;

                for (CONDITION_0)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned long long)

                if (CONDITION_1)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned int)

                if (CONDITION_2)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned short)

                if (CONDITION_3)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned char)
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
        "a",
        "d",
        "g",
        "b",
        "c"
    };
    return (char*)string_array;
}

bool first_line_upper(char* line, char* next_line)
{
    return strncmp(line, next_line, MAX_LENGTH_LINE) > 0;
}
