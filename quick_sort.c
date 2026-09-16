#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// #define RED "\033[31m"
// #define BLUE "\033[36m"

int  compare_numbers(const void* data, const void* reference_data);
int  compare_lines(const void* data, const void* reference_data);
void sorting(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data));
void print_numbers_array(int* numbers_array, size_t number_elements);
void print_lines_array(const char** lines_array, size_t line_elements);
void swap(void* data, void* next_data, size_t type_size);
void* creat_reference_point(void* array, size_t number_elements, size_t type_size);
void free_reference_point(void* reference_pointer);

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

    sorting(numbers_array, number_elements, sizeof(int),   &compare_numbers);
    sorting(lines_array,   line_elements,   sizeof(char*), &compare_lines);

    print_numbers_array(numbers_array, number_elements);
    print_lines_array(lines_array, line_elements);

    return 0;
}

void sorting(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data))
{
    if (number_elements <= 1) return;

    size_t right_pointer = number_elements - 1;
    size_t left_pointer = 0;

    void* reference_pointer = creat_reference_point(array, number_elements, type_size);

    while (left_pointer <= right_pointer)
    {
        while (compare_function((unsigned char*)array + type_size * left_pointer, reference_pointer) < 0)
        {
            left_pointer++;
            assert(left_pointer < number_elements);
        }

        while (compare_function((unsigned char*)array + type_size * right_pointer, reference_pointer) > 0)
        {
            right_pointer--;
            //assert(right_pointer >= 0);
        }

        //assert(left_pointer < right_pointer);

        if (left_pointer <= right_pointer)
        {
            swap((unsigned char*)array + type_size * left_pointer, (unsigned char*)array + type_size * right_pointer, type_size);

            if (left_pointer != number_elements - 1)
                left_pointer++;

            if (right_pointer != 0)
                right_pointer--;

            assert(left_pointer != number_elements);
            //assert(right_pointer >= 0);
        }
    }

    free_reference_point(reference_pointer);

    if (right_pointer > 0)
        sorting((unsigned char*)array, right_pointer + 1, type_size, compare_function);

    if ((number_elements - right_pointer - 1) > 1)
        sorting((unsigned char*)array + type_size * (1 + right_pointer), number_elements - right_pointer - 1, type_size, compare_function);

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
        printf("%s\n", *(lines_array + i));
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
