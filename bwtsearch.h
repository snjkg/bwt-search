struct Index_header {
//    unsigned int newline_count;
    unsigned int bwt_length;
    unsigned int occ_4_byte;
    unsigned int c_table[96];
};
struct Memory_copy {
//    unsigned int newline_count; 
    unsigned char *c;

    unsigned int start;
    unsigned int length;
    unsigned int end;
};
int print_array(int *arr,int arr_len);
inline static unsigned int occ(struct Memory_copy mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih,unsigned char symbol_index, unsigned int pos,unsigned int interval);
//inline static unsigned int mem_occ(struct Memory_copy *mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih,unsigned char symbol_index, unsigned int pos,unsigned int interval,unsigned int prev_symbol_index,struct Memory_occ *mo, unsigned int * new_start);
inline static unsigned int fprint_decoded_line(struct Memory_copy  mc,FILE *opt_fp, FILE *idx_fp, FILE *bwt_fp, FILE* opt_random_fp, struct Index_header ih, unsigned int line_br_pos,unsigned int interval, unsigned int file_last_position, unsigned int file_occ_entries); 
inline static unsigned int get_line_num(struct Memory_copy   mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih, unsigned int any_pos,unsigned int interval);
inline static struct Result * get_line_num_with_result(struct Memory_copy  mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih, unsigned int any_pos,unsigned int interval);
inline static unsigned char get_char (struct Memory_copy  mc,FILE *bwt_fp,unsigned int pos);
inline static unsigned int print_decoded_line_until(struct Memory_copy   mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih, unsigned int line_br_pos,unsigned int until_pos,unsigned int interval);
int print_array_l(long *arr,int arr_len);
//int generate_mem_occ ( FILE *bwt_fp, struct Memory_occ * memory_occ, struct Index_header ih, unsigned int * new_start, unsigned int partitions);
int generate_infile_occ ( FILE * opt_fp, FILE * bwt_fp, struct Index_header ih);
int generate_index ( const char* bwt_filename, const char* idx_filename);
