#include <TXLib.h>
#include <stdio.h>
#include <assert.h>

#define RED "\033[31m"
#define BLUE "\033[36m"
void sorting(int* numbers_array, int numbers);
void print_array(int* numbers_array, int numbers);

int main(void)
{
    int numbers_array[] = {35, 10, 25, 23, 1, 15, 9};

    int numbers = sizeof(numbers_array) / sizeof(numbers_array[0]);

    sorting(numbers_array, numbers);

    print_array(numbers_array, numbers);

    return 0;
}

void sorting(int* numbers_array, int numbers)
{   
    int right_pointer = numbers - 1;
    int left_pointer = 0;

    while (left_pointer < right_pointer)
    {
        while (*(numbers_array + left_pointer) < *(numbers_array + numbers / 2))
            left_pointer++;

        while (*(numbers_array + right_pointer) > *(numbers_array + numbers / 2))
            right_pointer--;
        
        int temp = *(numbers_array + left_pointer);
        *(numbers_array + left_pointer) = *(numbers_array + right_pointer);
        *(numbers_array + right_pointer) = temp;
    } 

    print_array(numbers_array, numbers);
    printf("\n");

    if (left_pointer > 1)
    {
        printf("Left part sorting: \n");
        sorting(numbers_array, left_pointer + 1);
    }
    if (right_pointer > 1)
    {
        printf("Right part sorting: \n");
        sorting(numbers_array + left_pointer + 1, numbers - left_pointer - 1);
    }
}

void print_array(int* numbers_array, int numbers)
{
    for (int i = 0; i < numbers; i++)
    {
        printf("%d ", *(numbers_array + i));
    }
}