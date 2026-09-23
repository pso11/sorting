/**
 * @file onegin_rap_battle.h
 * @brief Header file of onegin_rap_battle.c
 * @author Pshanichnaya Sofia
 * @date 2026-09-20
 */

/**
 * @brief   Macros for error checking
 */
#define CHECK_ERROR_CODE()                                               \
    if (error_code != OK)                                                \
    {                                                                    \
        printf("Error code %d -> %s", error_code, strerror(error_code)); \
        return 0;                                                        \
    }                                                                    \

/**
 * @brief   Enum for errors code
 */
enum status
{
    OK = -1,
    NOT_OK = -2
};

/**
 * @brief   Structure
 * @details A structure containing a pointer to the start of a string and its length.
 */
struct line
{
    char*  pointer;
    size_t length;
};

/**
 * @brief   Structure for text from file
 * @details A structure containing name of source file, name of file with all sortings, the file descriptor of an open file, pointer to a buffer containing the entire text from the file, lines number in text, array of structures.
 */
struct text
{
    const char*  source_file;
    const char*  destination_file;
    int    file_descriptor;
    char*  text_buffer;
    size_t lines_number;
    struct line* array;
};


/**
 * @brief     Сomparator for lines from structure.
 * @param[in] data Value being compared.
 * @param[in] referenc_data the value against which the comparison is made.
 * @return    Returns 1 if data > referenc_data, 0 if data = referenc_data, -1 if data < referenc_data.
 * @note      We don't compare punctual signs, compares in alphabetic order from left to right
 */
int  alphabetic_compare_lines(const void* data, const void* reference_data);

/**
 * @brief     Сomparator for lines from structure.
 * @param[in] data Value being compared.
 * @param[in] referenc_data the value against which the comparison is made.
 * @return    Returns 1 if data > referenc_data, 0 if data = referenc_data, -1 if data < referenc_data.
 * @note      We don't compare punctual signs, compares in alphabetic order from right to left
 */
int  rhymed_compare_lines(const void* data, const void* reference_data);

/**
 * @brief     Сomparator for lines from structure.
 * @param[in] data Value being compared.
 * @param[in] referenc_data the value against which the comparison is made.
 * @return    Returns 1 if data > referenc_data, 0 if data = referenc_data, -1 if data < referenc_data.
 * @note      We compare pointers in upper order< to recreate beginning structures array
 */
int  compare_start(const void* data, const void* reference_data);

/**
 * @brief     Quick sort function.
 * @param[in] array Array to sort.
 * @param[in] number_elements Number of elements in array.
 * @param[in] type_size Size of elements type in array.
 * @param[in] compare_function Pointer on comparator function.
 * @note      Hoar method
 */
void my_qsort(void* array, const size_t number_elements, const size_t type_size, int (*compare_function)(const void* data, const void* reference_data));

/**
 * @brief      Partition for quick sort.
 * @param[in]  array Array to sort.
 * @param[in]  number_elements Number of elements in array.
 * @param[out] right_idx Right index of array.
 * @param[out] left_idx Left index of array.
 * @param[in]  compare_function Pointer on comparator function.
 * @param[in]  type_size Size of elements type in array.
 * @param[in]  reference_data Reference data to compare with.
 */
void partition(void* array, const size_t number_elements, size_t* right_idx, size_t* left_idx, int (*compare_function)(const void* data, const void* reference_data), const size_t type_size, const void* reference_data);

/**
 * @brief     Debug function to print lines in array.
 * @param[in] lines_array Pointer on printed lines array.
 * @param[in] line_elements Number of elements in lines array.
 */
void print_lines_array(const char** lines_array, const size_t line_elements);

/**
 * @brief     Function to change value.
 * @param[in] data Value one to be changed.
 * @param[in] next_data Value two to be changed.
 * @param[in] type_size Size of values type.
 * @note      byte-by-byte replacement
 */
void swap(void* data, void* next_data, const size_t type_size);

/**
 * @brief     Function to copy reference point.
 * @param[in] array Array of data we need to sort.
 * @param[in] number_elements Number of elements in this array.
 * @param[in] type_size Size of values type.
 * @note      Memory creates by calloc
 */
void* create_reference_point(const void* array, const size_t number_elements, const size_t type_size);

/**
 * @brief     Function to clear memory taken for reference point.
 * @param[in] reference_pointer Pointer on memory block with reference point
 */
void free_reference_point(void* reference_pointer);

/**
 * @brief            Function to fill the text structure with data from file.
 * @param[in] onegin Pointer on structure with all information from text
 * @return           Returns error code or -1 if OK
 */
int file_work(struct text* onegin);

/**
 * @brief     Function that reads file and fill buffer with all data from file.
 * @param[in] windows_number_elements Numbers of elements in file returned by function fstat.
 * @param[in] file_descriptor File descriptor.
 * @return    Returns pointer on data buffer.
 * @note      On windows windows_number_elements includes \r befor \n as ending of line.
 */
char* create_text_buffer(const size_t windows_number_elements, const int file_descriptor);

/**
 * @brief     Function that define numbers of elements in file.
 * @param[in] file_descriptor File descriptor.
 * @return    Returns number of elements in file.
 * @note      On windows number of elements includes \r befor \n as ending od line.
 */
size_t define_file_size(const int file_descriptor);

/**
 * @brief     Writes smth in file.
 * @param[in] array Pointer on text structure.
 * @param[in] lines_number Numbers of lines in file.
 * @param[in] destination_file Name of file in wich we write smth.
 * @return Error code, -1 if OK
 */
int fwrite_sorted(const struct line* array, const size_t lines_number, const char* destination_file);

/**
 * @brief     Fills line structure with pointers on line and length line.
 * @param[in] text_bufferr Pointer on buffer with all data from file.
 * @param[in] array Pointer on structure array.
 * @note      We change \n on \0.
 */
void fill_struct_array(char* text_bufferr, struct line* array);

/**
 * @brief     Counts number of lines in file.
 * @param[in] text_bufferr Pointer on buffer with all data from file.
 * @param[in] element_code Code of element accepted as end of line
 */
size_t count_lines(const char* text_buffer, size_t element_code);

/**
 * @brief     Cleans everything made by calloc in structure.
 * @param[in] onegin Pointer on needed to clean text structure.
 */
void free_struct(struct text* onegin);

/**
 * @brief     Delets everything from file.
 * @param[in] destination_file Name of file we need to clean.
 */
int clean_destination_file(const char* destination_file);

/**
 * @brief     Writes buffer with given number of elements(lines) in file.
 * @param[in] destination_file Name of file we need to write smth in.
 * @param[in] text_buffer Array of lines.
 * @param[in] lines_number Number of lines in this array.
 */
int fwrite_buffer(const char* destination_file, char* text_buffer, const size_t lines_number);

