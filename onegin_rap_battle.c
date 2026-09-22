#include <TXLib.h>
#include <stdio.h>

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <errno.h>

#include "onegin_rap_battle.h"

int main(int argc, char* argv[])
{
    struct text onegin = {0};
    file_work(argc, argv, &onegin);
    assert(onegin.text_buffer != NULL);
    //for (int i = 0; i < 734; i++)
    //    printf("%d\n", onegin.array[i].length);
    my_qsort(onegin.array, onegin.lines_number, sizeof(struct line), &rhymed_compare_lines);
    fwrite_sorted(onegin.array, onegin.lines_number, onegin.destination_file);

    my_qsort(onegin.array, onegin.lines_number, sizeof(struct line), &alphabetic_compare_lines);
    fwrite_sorted(onegin.array, onegin.lines_number, onegin.destination_file);

    my_qsort(onegin.array, onegin.lines_number, sizeof(struct line), &compare_start);
    fwrite_sorted(onegin.array, onegin.lines_number, onegin.destination_file);

    fwrite_buffer(onegin.destination_file, onegin.text_buffer, onegin.lines_number);

    free_struct(&onegin);

    return 0;
}

void my_qsort(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data))
{
    if (number_elements <= 1) return;

    size_t right_idx = number_elements - 1, left_idx = 0;

    void* reference_data = create_reference_point(array, number_elements, type_size);

    partition(array, number_elements, &right_idx, &left_idx, compare_function, type_size, reference_data);

    free_reference_point(reference_data);

    if (right_idx > 0)
        my_qsort((unsigned char*)array, right_idx + 1, type_size, compare_function);

    if ((number_elements - right_idx - 1) > 1)
        my_qsort((unsigned char*)array + type_size * (1 + right_idx), number_elements - right_idx - 1, type_size, compare_function);

}

void swap(void* data, void* next_data, size_t type_size)
{
    assert(data != NULL);
    assert(next_data != NULL);

    for (size_t i = 0; i < type_size; i++)
    {
        char temp = *((char*)data + i);
        *((char*)data + i) = *((char*)next_data + i);
        *((char*)next_data + i) = temp;
    }
}

int alphabetic_compare_lines(const void* data, const void* reference_data)
{
    assert(data != NULL);

    const char* str1 = ((const struct line*)data) -> pointer;
    const char* str2 = ((const struct line*)reference_data) -> pointer;

    int i_1 = 0;
    int i_2 = 0;

    while (str1[i_1] != '\0' || str2[i_2] != '\0')
    {
        int c1 = tolower(str1[i_1]);
        int c2 = tolower(str2[i_2]);

        if (!ispunct(c1) && !ispunct(c2))
        {
            if (c1 == c2) {;}
            else if (c1 > c2) return +1;
            else if (c1 < c2) return -1;

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

int compare_start(const void* data, const void* reference_data)
{
    size_t pointer_1 = (size_t)((const struct line*)data) -> pointer;
    size_t pointer_2 = (size_t)((const struct line*)reference_data) -> pointer;

    return (pointer_1 - pointer_2);
}

int rhymed_compare_lines(const void* data, const void* reference_data)
{
    assert(data != NULL);

    const char* str1 = ((const struct line*)data) -> pointer;
    const char* str2 = ((const struct line*)reference_data) -> pointer;

    int i_1 = (int)((const struct line*)data) -> length;
    int i_2 = (int)((const struct line*)reference_data) -> length;

    if (i_1 == 0 || i_2 == 0 )
        return (i_1 - i_2);

    while (i_1 > 0 && i_2 > 0)
    {
        size_t c1 = tolower(str1[i_1]);
        size_t c2 = tolower(str2[i_2]);

        if (!ispunct(c1) && !ispunct(c2))
        {
            if (c1 == c2) {;}
            else if (c1 > c2) return +1;
            else if (c1 < c2) return -1;
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

        assert(i_1 >= 0);
        assert(i_2 >= 0);
    }

    return 0;
}

void* create_reference_point(void* array, size_t number_elements, size_t type_size)
{
    void* reference_data = (void*)calloc(1, type_size);
    memcpy(reference_data, (unsigned char*)array + type_size * (number_elements / 2), type_size);

    assert(reference_data != NULL);

    return reference_data;
}

void free_reference_point(void* reference_data)
{
    free(reference_data);
};


size_t define_file_size(int file_descriptor)
{
    struct stat buff = {};
    fstat(file_descriptor, &buff);

    assert(buff.st_size != 0);

    return buff.st_size;
}

char* create_text_buffer(size_t windows_number_elements, int file)
{
    char* text_buffer = (char*)calloc(windows_number_elements + 1, sizeof(char));
    assert(text_buffer != NULL);

    size_t real_number_elements = read(file, text_buffer, sizeof(char) * windows_number_elements);
    text_buffer[real_number_elements] = '\0';

    return text_buffer;
}

size_t count_lines(const char* text_buffer)
{
    size_t i = 0, lines_number = 0;
    for (; text_buffer[i] != '\0'; i++)
    {
        if (text_buffer[i] == '\n')
            lines_number++;
    }
    if (text_buffer[i - 1] != '\n')
        lines_number++;

    return lines_number;
}

void fwrite_sorted(const struct line* array, size_t lines_number, char* destination_file)
{
    errno = 0;

    FILE* file = fopen(destination_file, "a");
    if (file == NULL)
    {
        printf("Error opening file: %d -> %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < lines_number; i++)
    {
        fprintf(file, "%s", array[i].pointer);
        fputc('\n', file);
    }
    fputc('\n', file);

    if (fclose(file) == EOF)
    {
        printf("Error closing file: %d -> %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void fill_struct_array(char* text_buffer, struct line* array)
{
    array[0].pointer = text_buffer;
    char* temp = text_buffer;
    size_t element = 0, line = 1;

    for(; text_buffer[element] != '\0'; element++)
    {
        if (text_buffer[element] == '\n')
        {
            text_buffer[element] = '\0';
            if (text_buffer[element + 1] != '\0')
                array[line].pointer = text_buffer + element + 1;
            array[line - 1].length = (size_t)(text_buffer + element - temp);
            temp = text_buffer + element + 1;
            line++;
        }
    }

    if (text_buffer[element - 1] != '\0')
        array[line - 1].length = (size_t)(text_buffer + element - temp);
}

void  free_struct(struct text* onegin)
{
    free(onegin -> text_buffer);
    free(onegin -> array);
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

void partition(void* array, size_t number_elements, size_t* right_idx, size_t* left_idx, int (*compare_function)(const void* data, const void* reference_data), size_t type_size, void* reference_data)
{
    while (*left_idx <= *right_idx)
    {
        while (compare_function((unsigned char*)array + type_size * *left_idx, reference_data) < 0)
        {
            (*left_idx)++;
            assert(*left_idx < number_elements);
        }

        while (compare_function((unsigned char*)array + type_size * *right_idx, reference_data) > 0)
        {
            (*right_idx)--;
            //assert(*right_idx >= 0);
        }
        //assert(*left_pointer < right_pointer);
        if (*left_idx <= *right_idx)
        {
            swap((unsigned char*)array + type_size * *left_idx, (unsigned char*)array + type_size * *right_idx, type_size);

            if (*left_idx != number_elements - 1)
                (*left_idx)++;

            if (*right_idx != 0)
                (*right_idx)--;

            assert(*left_idx != number_elements);
        }
    }
}

void file_work(int argc, char* argv[], struct text* onegin)
{
    errno = 0;
    assert(argc == 3);

    onegin -> source_file = argv[1];
    onegin -> destination_file = argv[2];
    free_destination_file(onegin -> destination_file);

    onegin -> file_descriptor = open(onegin -> source_file, O_RDONLY);

    if (onegin -> file_descriptor == -1)
    {
        printf("Error opening file: <%d> -> <%s>", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    size_t windows_number_elements = define_file_size(onegin -> file_descriptor);

    onegin -> text_buffer = create_text_buffer(windows_number_elements, onegin -> file_descriptor);

    close(onegin -> file_descriptor);

    onegin -> lines_number = count_lines((const char*)( onegin -> text_buffer));

    onegin -> array = (struct line*)calloc(onegin -> lines_number, sizeof(line));

    fill_struct_array(onegin -> text_buffer, (struct line*)(onegin -> array));
}

void free_destination_file(char* destination_file)
{
    errno = 0;
    FILE* file = fopen(destination_file, "w");
    if (file == NULL)
    {
        printf("Error opening file: <%d> -> <%s> ", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (fclose(file) == EOF)
    {
        printf("Error closing file: <%d> -> <%s> ", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void fwrite_buffer(char* destination_file, char* text_buffer, size_t lines_number)
{
    FILE* file = fopen(destination_file, "a");
    if (file == NULL)
    {
        printf("Error opening file: <%d> -> <%s> ", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }

    const char* pointer_element_array = text_buffer;
    for (size_t i = 0; i < lines_number; i++)
    {
        fprintf(file, "%s", pointer_element_array);
        fputc('\n', file);
        pointer_element_array = strchr(pointer_element_array, '\0');
        pointer_element_array++;
    }

    if (fclose(file) == EOF)
    {
        printf("Error closing file: <%d> -> <%s> ", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}
