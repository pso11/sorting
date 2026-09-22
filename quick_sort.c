#include <TXLib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define DARK_BLUE "\033[34m"
#define PINK "\033[35m"
#define LIGHT_BLUE "\033[36m"
#define WHITE "\033[37m"
#define RESET "\033[0m"

void sorting(int* numbers_array, size_t numbers);
void print_array(int* numbers_array, size_t numbers, size_t right_pointer, size_t left_pointer, size_t reference_point);
void swap(int* numbers_array, int left_pointer, int right_pointer);

int main(void)
{
    int numbers_array[] = {3, 2, 1, 10, 9, -5, 4, -10, 0, 0, 0, 0, 0};
    size_t numbers = sizeof(numbers_array) / sizeof(numbers_array[0]);

    sorting(numbers_array, numbers);
    for (size_t i = 0; i < numbers; i++)
    {
        printf("%10d   ", *(numbers_array + i));
        printf(WHITE);
    }


    return 0;
}

void sorting(int* numbers_array, size_t numbers)
{
    size_t right_pointer = numbers - 1;
    size_t left_pointer = 0;
    int reference_point = numbers_array[numbers / 2];

    if (numbers <= 1)
        return;

    while (left_pointer <= right_pointer)
    {
        print_array(numbers_array, numbers, right_pointer, left_pointer, reference_point);
        getchar();

        while (numbers_array[left_pointer] < reference_point)
        {
            left_pointer++;
            assert(left_pointer < numbers);
        }

        while (numbers_array[right_pointer] >  reference_point)
            right_pointer--;

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
    {
        printf(LIGHT_BLUE"Partition array\n");
        sorting(numbers_array, right_pointer + 1);
    }
    if ((numbers - right_pointer - 1) > 1)
    {
        printf(LIGHT_BLUE"Partition array\n");
        sorting(numbers_array + right_pointer + 1, numbers - right_pointer - 1);
    }

}

void print_array(int* numbers_array, size_t numbers, size_t right_pointer, size_t left_pointer, size_t reference_point)
{
    printf(WHITE);
    printf("Array:   ");
    for (size_t i = 0; i < numbers; i++)
    {
        if (numbers_array[i] == numbers_array[right_pointer])
            printf(GREEN);
        if (numbers_array[i] == numbers_array[left_pointer])
            printf(PINK);
        if (numbers_array[i] == numbers_array[reference_point])
        {
            printf(YELLOW);
            if (numbers_array[i] == numbers_array[right_pointer] || numbers_array[i] == numbers_array[left_pointer])
                printf(RED);
        }
        printf("%10d   ", *(numbers_array + i));
        printf(WHITE);
    }
    putchar('\n');
    printf("Index:   ");
    for (size_t i = 0; i < numbers; i++)
    {
        printf(DARK_BLUE"%10d   ", i);
        printf(WHITE);
    }
    putchar('\n');
    printf("Pointer: ");
    for (size_t i = 0; i < numbers; i++)
    {
        printf("%10p   ", numbers_array + i);
        printf(WHITE);
    }
    putchar('\n');
    printf(WHITE"\nNumber of elemetnts = <%d>\nright pointer = <%d> left pointer = <%d> reference_point = <%d>\n" WHITE, numbers, numbers_array[right_pointer], numbers_array[left_pointer], numbers_array[reference_point]);
}

void swap(int* numbers_array, int left_pointer, int right_pointer)
{
    int temp = *(numbers_array + left_pointer);
    *(numbers_array + left_pointer) = *(numbers_array + right_pointer);
    *(numbers_array + right_pointer) = temp;
}
