#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 7
#define MAX_LEN_LINE 100

int  compare_numbers(const void* data, const void* reference_data);
int  compare_lines(const void* data, const void* reference_data);
void sorting(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data));
void print_numbers_array(int* numbers_array, size_t number_elements);
void print_lines_array(const char** lines_array, size_t line_elements);
void swap(void* data, void* next_data, size_t type_size);
void* creat_reference_point(void* array, size_t number_elements, size_t type_size);
void free_reference_point(void* reference_pointer);
void write_file(const char** text_array);
const char** read_file(void);

int main(void)
{
    int numbers_array[] = {1, 2, 3, 4, 4, 4, 4, 5, 1, 2, 3, 4, 5};
    size_t number_elements = sizeof(numbers_array) / sizeof(numbers_array[0]);

    const char* lines_array[] =
    {
        "mum",
        "dad",
        "low",
        "buba",
        "arinya"
    };
    size_t line_elements = sizeof(lines_array) / sizeof(lines_array[0]);

    const char** text_array = read_file();

    sorting(numbers_array, number_elements, sizeof(int),   &compare_numbers);
    sorting(lines_array,   line_elements,   sizeof(char*), &compare_lines);
    sorting(text_array,    MAX_LINES,       sizeof(char*), &compare_lines);

    print_numbers_array(numbers_array, number_elements);
    print_lines_array  (text_array, MAX_LINES);
    print_lines_array  (lines_array, line_elements);

    write_file(text_array);

    free(text_array);

    return 0;
}

void sorting(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data))
{
    if (number_elements <= 1) return;

    size_t right_idx = number_elements - 1;
    size_t left_idx = 0;

    void* reference_pointer = creat_reference_point(array, number_elements, type_size);

    while (left_idx <= right_idx)
    {
        while (compare_function((unsigned char*)array + type_size * left_idx, reference_pointer) < 0)
        {
            left_idx++;
            assert(left_idx < number_elements);
        }

        while (compare_function((unsigned char*)array + type_size * right_idx, reference_pointer) > 0)
            right_idx--;

        if (left_idx <= right_idx)
        {
            swap((unsigned char*)array + type_size * left_idx, (unsigned char*)array + type_size * right_idx, type_size);

            if (left_idx != number_elements - 1)
                left_idx++;

            if (right_idx != 0)
                right_idx--;

            assert(left_idx != number_elements);
        }
    }

    free_reference_point(reference_pointer);

    if (right_idx > 0)
        sorting((unsigned char*)array, right_idx + 1, type_size, compare_function);

    if ((number_elements - right_idx - 1) > 1)
        sorting((unsigned char*)array + type_size * (1 + right_idx), number_elements - right_idx - 1, type_size, compare_function);

}

void print_numbers_array(int* numbers_array, size_t number_elements)
{
    for (size_t i = 0; i < number_elements; i++)
    {
        printf("%d ", *(numbers_array + i));
    }
    putchar('\n');
}

void print_lines_array(const char** lines_array, size_t line_elements)
{
    for (size_t i = 0; i < line_elements; i++)
    {
        printf("%s", *(lines_array + i));
    }

    putchar('\n');
}


void swap(void* data, void* next_data, size_t type_size)
{
    for (size_t i = 0; i < type_size; i++)
    {
        char temp = *((char*)data + i);
        *((char*)data + i) = *((char*)next_data + i);
        *((char*)next_data + i) = temp;
    }
}

int compare_numbers(const void* data, const void* reference_data)
{
    return (*(const int*)data - *(const int*)reference_data);
}

//int compare_lines(const void* data, const void* reference_data)
//{
//    const char* str1 = *(const char**)data;
//    const char* str2 = *(const char**)reference_data;
//
//    int i_1 = 0;
//    int i_2 = 0;
//
//    while (str1[i_1] != '\0' || str2[i_2] != '\0')
//    {
//        int c1 = tolower(str1[i_1]);
//        int c2 = tolower(str2[i_2]);
//
//        if (!ispunct(c1) && !ispunct(c2))
//        {
//            if (c1 == c2){;}
//            else if (c1 > c2)
//                return 1;
//            else if (c1 < c2)
//                return -1;
//            i_1++;
//            i_2++;
//        }
//        else if (ispunct(c1) && !ispunct(c2))
//            i_1++;
//        else if (!ispunct(c1) && ispunct(c2))
//            i_2++;
//        else
//        {
//            i_1++;
//            i_2++;
//        }
//
//    }
//    return 0;
//}


int compare_lines(const void* data, const void* reference_data)
{
    return strcmp(*(const char**)data, *(const char**)reference_data);
}

void* creat_reference_point(void* numbers_array, size_t number_elements, size_t type_size)
{
    void* reference_pointer = (void*)calloc(1, type_size);
    memcpy(reference_pointer, (unsigned char*)numbers_array + type_size * (number_elements / 2), type_size);
    return reference_pointer;
}

void free_reference_point(void* reference_pointer)
{
    free(reference_pointer);
}

const char** read_file(void)
{
    char** pointers_array = (char**)calloc(MAX_LINES, sizeof(char*));

    FILE* file = fopen("onegin.txt", "r");
    if (file == NULL)
    {
        printf("Can't open file");
        return 0;
    }

    char buffer[MAX_LEN_LINE] = {};

    for (size_t i = 0; i < MAX_LINES; i++)
    {
        int ch = 0;
        size_t y = 0;
        while ((ch = fgetc(file)) != EOF && ch != '\n')
        {
            buffer[y] = (char)ch;
            y++;
        }
        buffer[y] = '\n';
        buffer[y + 1] = '\0';

        //if (fgets(buffer, MAX_LEN_LINE, file) == NULL)
        //    break;

        pointers_array[i] = (char*)calloc(strlen(buffer) + 1, sizeof(char));
        strncpy(pointers_array[i], buffer, strlen(buffer) + 1);
    }

    fclose(file);

    return (const char**)pointers_array;
}

void write_file(const char** text_array)
{
    FILE* file = fopen("mc_onegin.txt", "w");
    for (size_t i = 0; i < MAX_LINES; i++)
    {
        fprintf(file, "%s", text_array[i]);
    }
}
