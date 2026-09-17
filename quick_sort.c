#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 731
#define MAX_LEN_LINE 100

int  compare_numbers(const void* data, const void* reference_data);
int  alphabetic_compare_lines(const void* data, const void* reference_data);
int  rhymed_compare_lines(const void* data, const void* reference_data);

void my_qsort(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data));
void print_lines_array(const char** lines_array, size_t line_elements);
void swap(void* data, void* next_data, size_t type_size);
void* create_reference_point(void* array, size_t number_elements, size_t type_size);
void free_reference_point(void* reference_pointer);
void write_file(const char** lines_array);
const char** read_file(void);
const char** copy_array(const char** lines_array);
void  free_all_arrays(char** lines_array, char** copied_lines_array);

int main(void)
{
    const char** lines_array = read_file();
    const char** copied_lines_array = copy_array(lines_array);

    my_qsort(lines_array, MAX_LINES, sizeof(char*), &rhymed_compare_lines);
    write_file(lines_array);

    my_qsort(lines_array, MAX_LINES, sizeof(char*), &alphabetic_compare_lines);
    write_file(lines_array);

    write_file(copied_lines_array);

    free_all_arrays((char**)lines_array, (char**)copied_lines_array);

    return 0;
}

void my_qsort(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data))
{
    if (number_elements <= 1) return;

    size_t right_idx = number_elements - 1;
    size_t left_idx = 0;

    void* reference_pointer = create_reference_point(array, number_elements, type_size);

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
        my_qsort((unsigned char*)array, right_idx + 1, type_size, compare_function);

    if ((number_elements - right_idx - 1) > 1)
        my_qsort((unsigned char*)array + type_size * (1 + right_idx), number_elements - right_idx - 1, type_size, compare_function);

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

int alphabetic_compare_lines(const void* data, const void* reference_data)
{
    const char* str1 = *(const char**)data;
    const char* str2 = *(const char**)reference_data;

    int i_1 = 0;
    int i_2 = 0;

    while (str1[i_1] != '\0' || str2[i_2] != '\0')
    {
        int c1 = tolower(str1[i_1]);
        int c2 = tolower(str2[i_2]);

        if (!ispunct(c1) && !ispunct(c2))
        {
            if (c1 == c2){;}
            else if (c1 > c2)
                return 1;
            else if (c1 < c2)
                return -1;
            i_1++;
            i_2++;
        }
        else if (ispunct(c1) && !ispunct(c2))
            i_1++;
        else if (!ispunct(c1) && ispunct(c2))
            i_2++;
        else
        {
            i_1++;
            i_2++;
        }

    }
    return 0;
}

int rhymed_compare_lines(const void* data, const void* reference_data)
{
    const char* str1 = *(const char**)data;
    const char* str2 = *(const char**)reference_data;

    int i_1 = strlen(str1);
    int i_2 = strlen(str2);

    while (i_1 >= 0 && i_2 >= 0)
    {
        int c1 = tolower(str1[i_1]);
        int c2 = tolower(str2[i_2]);

        if (!ispunct(c1) && !ispunct(c2))
        {
            if (c1 == c2){;}
            else if (c1 > c2)
                return 1;
            else if (c1 < c2)
                return -1;
            i_1--;
            i_2--;
        }
        else if (ispunct(c1) && !ispunct(c2))
            i_1--;
        else if (!ispunct(c1) && ispunct(c2))
            i_2--;
        else
        {
            i_1--;
            i_2--;
        }
    }

    return 0;
}

void* create_reference_point(void* array, size_t number_elements, size_t type_size)
{
    void* reference_pointer = (void*)calloc(1, type_size);
    memcpy(reference_pointer, (unsigned char*)array + type_size * (number_elements / 2), type_size);
    return reference_pointer;
}

void free_reference_point(void* reference_pointer)
{
    free(reference_pointer);
}

const char** read_file(void)
{
    char** lines_array = (char**)calloc(MAX_LINES, sizeof(char*));

    FILE* file = fopen("onegin_english.txt", "r");
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

        lines_array[i] = (char*)calloc(strlen(buffer) + 1, sizeof(char));
        strncpy(lines_array[i], buffer, strlen(buffer) + 1);
    }

    fclose(file);

    return (const char**)lines_array;
}

void write_file(const char** lines_array)
{
    FILE* file = fopen("mc_onegin.txt", "a");
    for (size_t i = 0; i < MAX_LINES; i++)
    {
        fprintf(file, "%s", lines_array[i]);
    }
    fprintf(file, "\n");
    fclose(file);
}

const char** copy_array(const char** lines_array)
{
    char** copied_lines_array = (char**)calloc(MAX_LINES, sizeof(char*));

    for (int i = 0; i < MAX_LINES; i++)
    {
        copied_lines_array[i] = (char*)calloc(strlen(lines_array[i]) + 1, sizeof(char*));

        strncpy(copied_lines_array[i], lines_array[i], strlen(lines_array[i]) + 1);
    }

    return (const char**)copied_lines_array;
}

void  free_all_arrays(char** lines_array, char** copied_lines_array)
{
    for (int i = 0; i < MAX_LINES; i++)
        free(lines_array[i]);
    free(lines_array);

    for (int i = 0; i < MAX_LINES; i++)
        free(copied_lines_array[i]);
    free(copied_lines_array);
}
