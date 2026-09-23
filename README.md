# Several types of sorting algorithms

## Bubble sort
```C
void sort_lines(char* string_array)
{
    for (int i = LINES - 1; i > 0 ; i--)
    {
        for (int j = 0; j < i; j++)
        {
            char* line = string_array + j * MAX_LENGTH_LINE;
            char* next_line = line + MAX_LENGTH_LINE;

            if (first_line_upper(line, next_line))
                swap(line, next_line);
        }
    }
}
```
Swap uses symbol-by-symbol exchange and **SWAP ONLY FOR CHAR, DON'T USE TO SORT NUMBERS ARRAY**

**PROS**
- Easy to understand

**CONS**
- We must go through array again and again to sort -> too long
- Byte-by-byte coping -> too long

In **bubble_sort.c** you should:

1. create lines array in your code
2. be **CAREFUL** it uses static array
3. count lines by yourself and write it in `#define`
4. wtite your oun comparator if you want

>## Buffer sort

```C
void sort_lines(char* string_array)
{
    for (int i = 0; i < LINES - 1; i++)
    {
        for (int j = 0; j < LINES - i - 1; j++)
        {
            char* line = string_array + j * MAX_LENGTH_LINE;
            char* next_line = line + MAX_LENGTH_LINE;

            if (first_line_upper(line, next_line))
            {
                size_t removal = 0;

                for (CONDITION_0)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned long long)

                if (CONDITION_1)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned int)

                if (CONDITION_2)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned short)

                if (CONDITION_3)
                    EXCHANGE_PARTS(line, next_line, removal, unsigned char)
            }
        }
    }
}
```
Still only for char **DON'T USE NUMBERS TO SORT ARRAY**. We force compiler to think that part of **CHAR** array has **INTEGER TYPE** to exchange lines. Has some connection with bubble sort, but diffent *function* swap

**PROS**
- Exchange by exchange in batches of several characters -> much more quicker then **BUBBLE_SORT**

**CONS**
- Still nedda go through array again and again to sort
- A bit difficult to understand

In **buffer_sort.c** you **should**:
1. create lines array in your code
2. be **CAREFUL** it uses static array
3. count lines by yourself and write it in `#define`
4. wtite your oun comparator if you want

**WARNING!!!!** It uses special macros -> open source file if you're curious

## Pointers sort

```C
void sort_lines(char** pointers_array)
{
    for (int i = LINES - 1; i > 0 ; i--)
    {
        int counter_changes = 0;

        for (int j = 0; j < i; j++)
        {
            if (first_string_upper(pointers_array, j))
            {
                transposition(pointers_array, j);

                counter_changes++;
            }
        }

        if (counter_changes == 0)
            break;
    }
}
```
We exchange pointers in pointers array. Still for lines

**PROS**
- Easy to understand
- Quick
- Uses break if there's no changes to do
- You can write your own comparator

**CONS**
- Only for pointers on lines
- You should change comparator in your code

In **pointers_sort.c** you should:

1. create lines array in your code
2. be **CAREFUL** it uses static array
3. count lines by yourself and write it in `#define`
4. wtite your oun comparator if you want


## Quick sort for void*

```C
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
```
**NOW** it's for all types of array. Uses byte-by-byte exchange. Uses **HOAR method**

**PROS**
- Quick
- You can write your own comparator

**CONS**
- Creates memory for reference_point by calloc

In **pointers_sort.c** you should:

# Onegin_rap_battle

![alt text](image.png)

This programm sortes original text and puts in destination file text in alphabetic way from left to right, in rhymed way from right to left, and writes original text at the end by to methods.
### To run the program in the terminal, in addition to the executable file, you need to specify the name of the file containing the source text and the file for the sorted output. **WARNING!!!** We can read information only from ONE file
**Example:**
```C
.\onegin_rap_battle.exe onegin_english.txt mc_onegin.txt
```
### Main information: Structure text
```C
struct text
{
    const char*  source_file;
    const char*  destination_file;
    int    file_descriptor;
    char*  text_buffer;
    size_t lines_number;
    struct line* array;
};
```
It contains all information about our programm data:
1. source_file - name of file with original text, thar we nedda read
2. destination_file - name of file in what we nedda write sorted in 3 ways text
3. file_descriptor - file descriptor of opened source file
4. text_buffer - array with all elements from source file, made by calloc
4. lines_number - number of lines in our file(includes all empty lines)
5. line* array - pointer on structures array with all information about line in buffer

### Main information: Structure line
```C
struct line
{
    char*  pointer;
    size_t length;
};
```
It contains all information about our lines in ext_buffer:
1. pointer - pointer on beginning of line in text_buffer
2. length - length of line under the pointer

### Main information: Tactics

1. We read all elements from inputted in terminal file in text_buffer
2. We take into consideration in counting file size that in Windows line ends with \r\n
3. Put \0 at the end of our text buffer to create one big line and to understand where the enf of this buffer is
4. Then count lines by going throught this buffer and count \n and change \n on \0
5. Create structures array and fill it with all information about lines in buffer
Look at this:
```C
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
```
If you want to look at all this functions go to source file with code

### Main information: Sorting

We use quick sort that was described upper
```C
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
```

### Main information: Comparators
In my_qsort we use our own comparators

1. To compare in alphabetic way -> from left to right and don't compare punctial signs
```C
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
```
2. To compare in rhymed way -> from right to left and don't compare punctial signs
```C
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
```
3. To restore original text. We use the fact that original pointers on lines lie in upper way in lines array
```C
int compare_start(const void* data, const void* reference_data)
{
    size_t pointer_1 = (size_t)((const struct line*)data) -> pointer;
    size_t pointer_2 = (size_t)((const struct line*)reference_data) -> pointer;

    return (pointer_1 - pointer_2);
}
```
### Main information: Writing in file

We write rhymed text, alphabetic text and original text.
**Ways to write original text at the end of destination file**
1. Use my_qsort with comparator number 3 and use simple function **fwrite_sorted**
```C
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
```
2. Print text_buffer with function **fwrite_buffer**
```C
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
```
### Main information: Error codes
If you wanna change code and there's an error you have two options:
1. asserts writed in code and documented asserts
2. error codes, that you can change in main
There're some of them that you can meet
<cerrno.h>

E2BIG         | Список аргументов слишком длинный

EACCES        | Доступ запрещён

EAGAIN        | Ресурс недоступен, попробуйте ещё раз

EBADF         | Неверный файловый дескриптор

EBADMSG       | Неверное сообщение

EBUSY         | Устройство или ресурс занят

EEXIST        | Файл существует

EFAULT        | Неверный адрес

EFBIG         | Файл слишком большой

EIO           | Ошибка Ввода/Вывода

EMFILE        | Слишком большое значение дескриптора файла

ENAMETOOLONG  | Имя файла слишком длинное

[GitHub link](https://github.com)
