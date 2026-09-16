#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// #define RED "\033[31m"
// #define BLUE "\033[36m"

int  compare_numbers(const void* data, const void* next_data);
int  compare_lines(const void* data, const void* next_data);
void sorting(void* numbers_array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* next_data));
void print_array(int* numbers_array, size_t number_elements);
void swap(char* data, char* next_data);

int main(void)
{
    int numbers_array[] = {5, 4, 3, 2, 1, 1, 2, 3, 4, 5, 0};
    size_t number_elements = sizeof(numbers_array) / sizeof(numbers_array[0]);

    const char* lines_array[] =
    {
        "a",
        "c",
        "l",
        "h",
        "b"
    };
    size_t line_elements = sizeof(lines_array) / sizeof(lines_array[0]);

    sorting(numbers_array, number_elements, sizeof(int),   &compare_numbers);
    sorting(lines_array,   line_elements,   sizeof(char*), &compare_lines);

    print_array(numbers_array, number_elements);

    return 0;
}

void sorting(void* numbers_array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* next_data))
{
    if (number_elements <= 1) return;

    size_t right_pointer = number_elements - 1;
    size_t left_pointer = 0;

    void* reference_value = (void*)calloc(1, type_size);
    memcpy(reference_value, (unsigned char*)numbers_array + type_size * number_elements / 2, type_size);

    while (left_pointer <= right_pointer)
    {
        while (compare_function((unsigned char*)numbers_array + type_size * left_pointer, reference_value) < 0)
        {
            left_pointer++;
            assert(left_pointer < number_elements);
        }

        while (compare_function((unsigned char*)numbers_array + type_size * right_pointer, reference_value) > 0)
        {
            right_pointer--;
            //assert(right_pointer >= 0);
        }

        //assert(left_pointer < right_pointer);

        if (left_pointer <= right_pointer)
        {
            swap((char*)numbers_array + type_size * left_pointer, (char*)numbers_array + type_size * right_pointer);

            if (left_pointer != number_elements - 1)
                left_pointer++;

            if (right_pointer != 0)
                right_pointer--;

            assert(left_pointer != number_elements);
            //assert(right_pointer >= 0);
        }
    }

    if (right_pointer > 0)
        sorting(numbers_array, right_pointer + 1, type_size, compare_function);
    if ((number_elements - right_pointer - 1) > 1)
        sorting((unsigned char*)numbers_array + right_pointer + 1, number_elements - right_pointer - 1, type_size, compare_function);
}

void print_array(int* numbers_array, size_t number_elements)
{
    for (int i = 0; i < number_elements; i++)
    {
        printf("%d ", *(numbers_array + i));
    }
}

void swap(char* data, char* next_data)
{
    char temp = *data;
    *data = *next_data;
    *next_data = temp;
}

int compare_numbers(const void* data, const void* next_data)
{
    return (*(const int*)data - *(const int*)next_data);
}

int compare_lines(const void* data, const void* next_data)
{
    return strcmp(*(const char**)data, *(const char**)next_data);
}

