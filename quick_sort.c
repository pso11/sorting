#include <stdio.h>
#include <assert.h>
#include <limits.h>

// #define RED "\033[31m"
// #define BLUE "\033[36m"

void sorting(void* numbers_array, int numbers);
void print_array(int* numbers_array, int numbers);
void swap(int* numbers_array, int left_pointer, int right_pointer);

int main(void)
{
    int numbers_array[] = {1, 1};
    int numbers = sizeof(numbers_array) / sizeof(numbers_array[0]);

    sorting(numbers_array, numbers);

    print_array(numbers_array, numbers);

    return 0;
}

void sorting(int* numbers_array, int numbers)
{
    size_t right_pointer = numbers - 1;
    size_t left_pointer = 0;
    size_t reference_point = numbers_array[numbers / 2];

    if (numbers <= 1)
        return;

    while (left_pointer <= right_pointer)
    {
        while (numbers_array[left_pointer] < reference_point)
        {
            left_pointer++;
            assert(left_pointer < numbers);
        }

        while (numbers_array[right_pointer] >  reference_point)
        {
            right_pointer--;
            assert(right_pointer >= 0);
        }

        //assert(left_pointer < right_pointer);

        if (left_pointer <= right_pointer)
        {
            swap(numbers_array, left_pointer, right_pointer);

            if (left_pointer != numbers - 1)
                left_pointer++;

            if (right_pointer != 0)
                right_pointer--;

            assert(left_pointer != numbers);
        }
    }

    if (right_pointer > 0)
        sorting(numbers_array, right_pointer + 1);
    if ((numbers - right_pointer - 1) > 1)
        sorting(numbers_array + right_pointer + 1, numbers - right_pointer - 1);
}

void print_array(int* numbers_array, int numbers)
{
    for (int i = 0; i < numbers; i++)
    {
        printf("%d ", *(numbers_array + i));
    }
}

void swap(int* numbers_array, int left_pointer, int right_pointer)
{
    int temp = *(numbers_array + left_pointer);
    *(numbers_array + left_pointer) = *(numbers_array + right_pointer);
    *(numbers_array + right_pointer) = temp;
}
