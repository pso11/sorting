#TODO argc in main
#TODO error codes without exit
#TODO README

#include <TXLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <sys\stat.h>
#include <errno.h>

#include "onegin_rap_battle.h"

int main(const int argc, const char* argv[])
{
    int error_code = OK;
    if (argc > 3)
    {
        printf("Our programm can sort only 1 file. LOOk at READ.ME");
        return 0;
    }

    struct text file = {0};
    file.source_file      = argv[1];
    file.destination_file = argv[2];

    error_code = file_work(&file);
    CHECK_ERROR_CODE();

    my_qsort(file.array, file.lines_number, sizeof(struct line), &rhymed_compare_lines);
    error_code = fwrite_sorted(file.array, file.lines_number, file.destination_file);
    CHECK_ERROR_CODE();

    my_qsort(file.array, file.lines_number, sizeof(struct line), &alphabetic_compare_lines);
    error_code = fwrite_sorted(file.array, file.lines_number, file.destination_file);
    CHECK_ERROR_CODE();

    my_qsort(file.array, file.lines_number, sizeof(struct line), &compare_start);
    error_code = fwrite_sorted(file.array, file.lines_number, file.destination_file);
    CHECK_ERROR_CODE();

    error_code = fwrite_buffer(file.destination_file, file.text_buffer, file.lines_number);
    CHECK_ERROR_CODE();

    free_struct(&file);

    return 0;
}

void my_qsort(void* array, const size_t number_elements, const size_t type_size, int (*compare_function)(const void* data, const void* reference_data))
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

void swap(void* data, void* next_data, const size_t type_size)
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

void* create_reference_point(const void* array, const size_t number_elements, const size_t type_size)
{
    void* reference_data = (void*)calloc(1, type_size);
    memcpy(reference_data, (const unsigned char*)array + type_size * (number_elements / 2), type_size);

    assert(reference_data != NULL);

    return reference_data;
}

void free_reference_point(void* reference_data)
{
    free(reference_data);
};


size_t define_file_size(const int file_descriptor)
{
    struct stat buff = {};
    fstat(file_descriptor, &buff);

    assert(buff.st_size != 0);

    return buff.st_size;
}

char* create_text_buffer(const size_t windows_number_elements, const int file)
{
    char* text_buffer = (char*)calloc(windows_number_elements + 1, sizeof(char));
    assert(text_buffer != NULL);

    size_t real_number_elements = read(file, text_buffer, sizeof(char) * windows_number_elements);
    text_buffer[real_number_elements] = '\0';

    return text_buffer;
}

size_t count_lines(const char* text_buffer, size_t element_code)
{
    size_t i = 0, lines_number = 0;
    for (; text_buffer[i] != element_code; i++)
    {
        if (text_buffer[i] == '\n')
            lines_number++;
    }
    if (text_buffer[i - 1] != '\n')
        lines_number++;

    return lines_number;
}

int fwrite_sorted(const struct line* array, size_t lines_number, const char* destination_file)
{
    errno = 0;

    FILE* file = fopen(destination_file, "a");
    if (file == NULL)
        return NOT_OK;

    for (size_t i = 0; i < lines_number; i++)
    {
        fprintf(file, "%s", array[i].pointer);
        fputc('\n', file);
    }
    fputc('\n', file);

    errno = 0;
    if (fclose(file) == EOF)
        return NOT_OK;

    return OK;
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

void  free_struct(struct text* file)
{
    free(file -> text_buffer);
    free(file -> array);
}

void print_lines_array(const char** lines_array, const size_t line_elements)
{
    for (size_t i = 0; i < line_elements; i++)
    {
        printf("<%s>", *(lines_array + i));
        putchar('\n');
    }

    putchar('\n');
}

void partition(void* array, const size_t number_elements, size_t* right_idx, size_t* left_idx, int (*compare_function)(const void* data, const void* reference_data), const size_t type_size, const void* reference_data)
{
    while (*left_idx <= *right_idx)
    {
        while (compare_function((const unsigned char*)array + type_size * *left_idx, reference_data) < 0)
        {
            (*left_idx)++;
            assert(*left_idx < number_elements);
        }

        while (compare_function((const unsigned char*)array + type_size * *right_idx, reference_data) > 0)
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

int file_work(struct text* file)
{
    if (clean_destination_file(file -> destination_file) != OK)
        return NOT_OK;

    errno = 0;
    file -> file_descriptor = open(file -> source_file, O_RDONLY);

    if (file -> file_descriptor == -1)
        return NOT_OK;

    const size_t windows_number_elements = define_file_size(file -> file_descriptor);

    errno = 0;
    file -> text_buffer = create_text_buffer(windows_number_elements, file -> file_descriptor);
    if ((file -> text_buffer) == NULL)
        return NOT_OK;

    errno = 0;
    if (close(file -> file_descriptor) == -1)
        return NOT_OK;

    file -> lines_number = count_lines((const char*)( file -> text_buffer), '\0');

    file -> array = (struct line*)calloc(file -> lines_number, sizeof(line));

    fill_struct_array(file -> text_buffer, (struct line*)(file -> array));

    return OK;
}

int clean_destination_file(const char* destination_file)
{
    errno = 0;
    FILE* file = fopen(destination_file, "w");
    if (file == NULL)
        return NOT_OK;

    errno = 0;
    if (fclose(file) == EOF)
        return NOT_OK;

    return OK;
}

int fwrite_buffer(const char* destination_file, char* text_buffer, size_t lines_number)
{
    errno = 0;
    FILE* file = fopen(destination_file, "a");
    if (file == NULL)
        return NOT_OK;

    const char* pointer_element_array = text_buffer;
    for (size_t i = 0; i < lines_number; i++)
    {
        fprintf(file, "%s", pointer_element_array);
        fputc('\n', file);
        pointer_element_array = strchr(pointer_element_array, '\0');
        pointer_element_array++;
    }

    errno = 0;
    if (fclose(file) == EOF)
        return NOT_OK;

    return OK;
}
