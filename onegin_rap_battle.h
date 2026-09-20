struct line
{
    char* pointer;
    size_t length;
};

struct text
{
    size_t file_descriptor;
    char* text_buffer;
    size_t lines_number;
    struct line* array;
};

int  alphabetic_compare_lines(const void* data, const void* reference_data);
int  rhymed_compare_lines(const void* data, const void* reference_data);
int  compare_start(const void* data, const void* reference_data);

void my_qsort(void* array, size_t number_elements, size_t type_size, int (*compare_function)(const void* data, const void* reference_data));
void partition(void* array, size_t number_elements, size_t* right_idx, size_t* left_idx, int (*compare_function)(const void* data, const void* reference_data), size_t type_size, void* reference_data);
void print_lines_array(const char** lines_array, size_t line_elements);
void swap(void* data, void* next_data, size_t type_size);
void* create_reference_point(void* array, size_t number_elements, size_t type_size);
void free_reference_point(void* reference_pointer);


struct text* file_work(void);
char* create_text_buffer(size_t windows_number_elements, size_t file_descriptor);
size_t define_file_size(size_t file_descriptor);
void write_sorted_file(const struct line* array, size_t lines_number);
void fill_struct_array(char* text_bufferr, struct line* array);
size_t count_lines(const char* text_buffer);
void  free_everything(struct text* onegin);
