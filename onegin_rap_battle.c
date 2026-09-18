#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <time.h>

int  alphabetic_compare_lines(const void* data, const void* reference_data);
int  rhymed_compare_lines(const void* data, const void* reference_data);

void my_qsort(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data));
void print_lines_array(const char** lines_array, size_t line_elements);
void swap(void* data, void* next_data, size_t type_size);
void* create_reference_point(void* array, size_t number_elements, size_t type_size);
void free_reference_point(void* reference_pointer);

void write_sorted_file(const char** lines_array, size_t lines_number);
void write_poem_file(const char* elements_array, size_t lines_number);
const char* read_file(void);
const char** create_lines_array(char* elements_array, size_t lines_number);
size_t count_lines(const char* elements_array);
void  free_all_arrays(char** lines_array, char* elements_array, size_t lines_number);

int main(void)
{
    const char* elements_array = read_file();
    assert(elements_array != NULL);
    size_t lines_number = count_lines(elements_array);
    const char** lines_array = create_lines_array((char*)elements_array, lines_number);

    my_qsort(lines_array, lines_number, sizeof(char*), &rhymed_compare_lines);
    write_sorted_file(lines_array, lines_number);

    my_qsort(lines_array, lines_number, sizeof(char*), &alphabetic_compare_lines);
    write_sorted_file(lines_array, lines_number);

    write_poem_file(elements_array, lines_number);

    free_all_arrays((char**)lines_array, (char*)elements_array, lines_number);

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
        {
            right_idx--;
            //assert(right_idx >= 0);
        }
        //assert(left_pointer < right_pointer);
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

int alphabetic_compare_lines(const void* data, const void* reference_data)
{
    const char* str1 = *(const char**)data;
    const char* str2 = *(const char**)reference_data;

    int i_1 = 0;
    int i_2 = 0;

    while (str1[i_1] != '\n' || str2[i_2] != '\n')
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
        size_t c1 = tolower(str1[i_1]);
        size_t c2 = tolower(str2[i_2]);

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
};

const char* read_file(void)
{

    FILE* file = fopen("onegin_english.txt", "r");
    if (file == NULL)
    {
        printf("Can't open file");
        return NULL;
    }

    struct stat buff = {};
    fstat(fileno(file), &buff);

    size_t wrong_number_elements = buff.st_size;

    char* elements_array = (char*)calloc(wrong_number_elements, sizeof(char));

    size_t real_number_elements = fread(elements_array, sizeof(char), wrong_number_elements, file);

    elements_array[real_number_elements] = '\0';

    fclose(file);

    return (const char*)elements_array;
}

size_t count_lines(const char* elements_array)
{
    size_t i = 0, lines_number = 0;
    for (; elements_array[i] != '\0'; i++)
    {
        if (elements_array[i] == '\n')
            lines_number++;
    }
    if (elements_array[i - 1] != '\n')
        lines_number++;

    return lines_number;
}

void write_sorted_file(const char** lines_array, size_t lines_number)
{

    FILE* file = fopen("mc_onegin.txt", "a");
    for (size_t i = 0; i < lines_number; i++)
    {
        fprintf(file, "%s", lines_array[i]);
        fputc('\n', file);
    }
    fputc('\n', file);

    fclose(file);
}

const char** create_lines_array(char* elements_array, size_t lines_number)
{
    char** lines_array = (char**)calloc(lines_number, sizeof(char*));
    lines_array[0] = elements_array;
    for(size_t i = 0, j = 1; elements_array[i] != '\0'; i++)
    {
        if (elements_array[i] == '\n')
        {
            elements_array[i] = '\0';
            lines_array[j] = elements_array + i + 1;
            j++;
        }
    }

    return (const char**)lines_array;
}

void free_all_arrays(char** lines_array, char* elements_array, size_t lines_number)
{
    for (size_t i = 0; i < lines_number; i++)
        free(lines_array[i]);
    free(lines_array);

    free(elements_array);
}

void print_lines_array(const char** lines_array, size_t line_elements)
{
    for (size_t i = 0; i < line_elements; i++)
    {
        printf("<%s>", *(lines_array + i));
        putchar('\n');
    }

    putchar('\n');
}

void write_poem_file(const char* elements_array, size_t lines_number)
{
    FILE* file = fopen("mc_onegin.txt", "a");

    const char* ptr = elements_array;
    for (size_t i = 0; i < lines_number + 2; i++)
    {
        fprintf(file, "%s", ptr);
        fputc('\n', file);
        ptr = strchr(ptr, 0);
        ptr++;
    }

    fclose(file);
}
