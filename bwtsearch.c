#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define RAM_COPY_SIZE 8388608
#define RAM_COPY_SIZE_OUTPUT 11782912
#define HASHTABLE_SIZE 16384

//#define MEM_OCC_MAX_SIZE 1048576
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

struct Dict_entry {
    int* key;
    int key_count;
    struct Result_entry** result_entry;
};
struct Result_entry {
    char* partial;
    unsigned int start_pos;
};
struct Result {
    int line;
    char* partial;
    unsigned int start_pos;
};
unsigned int hashCode( unsigned int bwt_length,unsigned int key){
    if(bwt_length < HASHTABLE_SIZE){
        return key;
    } 

    return key/(bwt_length/(HASHTABLE_SIZE-1));
}
int findInDict(struct Dict_entry *buckets[],unsigned int bwt_length, unsigned int key){
    
    int bucket = hashCode(bwt_length,key);
    struct Dict_entry* node =  buckets[bucket];

    if(node == NULL)    {
        return -1;
    }
    int i = 0;
    for(i = 0;i< (*node).key_count;i++){
        int val = ((*node).key)[i];
        if(val  == key){
            return 1 ;
        }
        if(val  > key){
            return -1; 
        }
    }
    return -1;
}
int addToDict(struct Dict_entry *buckets[],unsigned int bwt_length, unsigned int newkey, struct Result_entry * result_entry){

    int i;
    int bucket = hashCode(bwt_length,newkey);
    
    struct Dict_entry* node;
    
    if (buckets[bucket] == NULL){
      
        node =   (struct Dict_entry*) calloc (1,sizeof (struct Dict_entry));
        buckets[bucket] = node;
        
        int * key = (unsigned int *) calloc (1, sizeof(int));
        (*node).key= key;
        ((*node).key)[0] = newkey;
        (*node).key_count = 1;
        //printf("h0 %d %d\n",(*node).key_count,bucket);
        if (result_entry != NULL){
           (*node).result_entry  = (struct Result_entry **) malloc(1*sizeof(struct Result_entry *));
            struct Result_entry ** res = (*node).result_entry;

            res[0] = result_entry;
        }
    } else {

        node = buckets[bucket];
        int valToInsert = newkey;
        struct Result_entry *reToInsert = result_entry;

        for(i = 0;i< (*node).key_count;i++){
            int key = ((*node).key)[i];
            if(key == newkey){
                return 0;
            } else if(valToInsert  < key){
                ((*node).key)[i] = valToInsert; 
                valToInsert = key;
                if (result_entry != NULL){
                    struct Result_entry * tmp = ((*node).result_entry)[i];
                    ((*node).result_entry)[i] = reToInsert;
                    reToInsert = tmp;
                }
            }
        }
        (*node).key_count = (*node).key_count + 1;
        (*node).key = (int *) realloc( (*node).key , (*node).key_count * sizeof(int) );
        
        ((*node).key)[(*node).key_count-1] = valToInsert;
        if (result_entry != NULL){  
            int count = (*node).key_count;
            //printf ("%p %d\n",reToInsert, count * sizeof(struct Result_entry *));
            (*node).result_entry  = (struct Result_entry **) realloc((*node).result_entry,  count * sizeof(struct Result_entry *)  );
            struct Result_entry ** res = (*node).result_entry;
            res[((*node).key_count)-1] = reToInsert;
            //printf("aaa%paaa",((*node).result_entry)[(*node).key_count-1] );
        }
    }
    

    //(*node).key = newkey;
    //(*node).value = newvalue;   
    //(*node).next = buckets[bucket];
    
    return 0;
}
int generate_index ( const char* bwt_filename, const char* idx_filename);
//inline static unsigned char char_to_index(unsigned char in_char);
//inline static unsigned char index_to_char(unsigned char index);
unsigned char index_to_char[96] = {10,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51
,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82
,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109
,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126};
unsigned char char_to_index[127] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,
14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44
,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74
,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95};
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
int int_compare (const void * a, const void * b) {
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}
//unsigned int collisions = 0;
int main( int argc, const char* argv[] ) {


    // Read Arguments
    if( argc <= 3 ) {
        printf("Not enough arguments");
        return(0);
    }

    unsigned short i,j;
    const char* opt_value = NULL;
    const char* bwt_filename = NULL;
    const char* idx_filename = NULL;
    const char* opt_filename = NULL;
    const char* query_string = NULL;
    
    for (i = 1; i < argc; i++){
        if (strcmp("-m",argv[i]) == 0 || strcmp("-n",argv[i]) == 0 || strcmp("-o",argv[i]) == 0){
            opt_value = argv[i];
        //} else if (opt_value != NULL && strcmp("-o",opt_value) == 0 && opt_filename == NULL) {
         
        } else if (bwt_filename == NULL) {
            bwt_filename = argv[i];
        } else if (idx_filename == NULL) {
            idx_filename = argv[i];
        } else if (query_string == NULL) {
            query_string = argv[i];
            opt_filename = argv[i];
        }
    }
    
    FILE *idx_fp;
    idx_fp = fopen(idx_filename, "rb");

    // check index exists
    if( idx_fp ==  NULL ) {
        // generate one if it doesn't
        generate_index(bwt_filename,idx_filename);
        idx_fp = fopen(idx_filename, "rb");
    } 
    setvbuf ( idx_fp  , NULL , _IONBF , 0 );
    //setvbuf ( idx_fp  , NULL , _IOFBF , 262144 );
    FILE *bwt_fp;
    bwt_fp = fopen(bwt_filename, "rb"); // for random reads
    //printf("hi\n");
    
    struct Index_header ih = {0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    }};
    // Read index header
    fread(&ih, sizeof(struct Index_header), 1, idx_fp);

    // Copy as much of original bwt file into memory as allowable
    struct Memory_copy mc;
    mc.start=0;
    if (opt_value != NULL && strcmp("-o",opt_value) == 0 && opt_filename != NULL) {
        mc.length= ih.bwt_length < RAM_COPY_SIZE_OUTPUT ? ih.bwt_length : RAM_COPY_SIZE_OUTPUT;
        
    }else{
        mc.length= ih.bwt_length < RAM_COPY_SIZE ? ih.bwt_length : RAM_COPY_SIZE;
    }
    mc.end = mc.start+mc.length;
    
    mc.c = malloc(mc.length * sizeof(char));
    fread(mc.c, 1, mc.length, bwt_fp);

    // Sampling frequency of Occ
    unsigned int interval = ih.bwt_length/ (((  (ih.bwt_length/2)+2000-sizeof(struct Index_header)   ) / 192));
    interval = interval + 1;
    
    
    // -o option decoding 
    if (opt_value != NULL && strcmp("-o",opt_value) == 0 && opt_filename != NULL) {
        //printf("hihi\n");
        FILE *opt_fp;
        opt_fp = fopen(opt_filename, "wb");
        setvbuf(opt_fp,NULL,_IOFBF,131072);

        
        //setvbuf(opt_fp,NULL,_IONBF,65536);
        unsigned char c;
        //unsigned int pos = 0;
        unsigned int first_newline = ih.bwt_length;
        unsigned int line_count =0;


        unsigned int output_length = 0;
        unsigned int next_mem_cpy = RAM_COPY_SIZE_OUTPUT;
        //unsigned int partitions = ;

        // create occ table in output file
        unsigned int occ_entries = generate_infile_occ ( opt_fp, bwt_fp,  ih);
        fclose(bwt_fp);
        
        
        bwt_fp = fopen(bwt_filename, "rb"); // for random reads
        //setvbuf ( bwt_fp  , NULL , _IONBF , 0 );
        char bwt_buf[interval*2];
        setvbuf ( bwt_fp  , bwt_buf , _IOFBF , interval*2 );

        //int bwt_fp2 = open(bwt_filename, O_RDONLY);
        //posix_fadvise(bwt_fp2,0,0,POSIX_FADV_RANDOM);
        FILE *opt_random_fp;
        opt_random_fp = fopen(opt_filename, "rb");
        setvbuf ( opt_random_fp  , NULL , _IONBF , 0 );
         
        //setvbuf ( opt_random_fp , NULL , _IONBF ,0 );

        //printf("part %d of %d size %d \n",0,mem_occ_partitions, partitionsize);
        //for(line_count = 1;line_count<5;line_count++){
        for(line_count = 1;line_count<(ih).c_table[1];line_count++){
            
            output_length = output_length +
             fprint_decoded_line(mc,opt_fp,idx_fp,bwt_fp,opt_random_fp,ih,line_count,interval,output_length,occ_entries);

            
            // simple optimisation to shift memory copy based on statistics, to block with least access

            if(output_length > next_mem_cpy ){
                if (next_mem_cpy+RAM_COPY_SIZE_OUTPUT < ih.bwt_length){
                    fseek(bwt_fp, next_mem_cpy, SEEK_SET);
                    mc.start=next_mem_cpy;
                    mc.end = mc.start+mc.length;
                    fread(mc.c, 1, mc.length, bwt_fp);
                    //int k=0;
                    //printf("%d %d %d \n",mc.start, mc.length, ih.bwt_length);
                }
                next_mem_cpy = next_mem_cpy + RAM_COPY_SIZE_OUTPUT;                    
            }

        }
        //printf("line %d\n",output_length);
        fprint_decoded_line(mc,opt_fp,idx_fp,bwt_fp,opt_random_fp,ih,0,interval,output_length,occ_entries);

        fclose(opt_fp);
    } else  { 

        // Standard BWT Search
        i = strlen(query_string)-1;

        unsigned char symbol_index=char_to_index[query_string[i]];
        int first=ih.c_table[symbol_index];
        int last=symbol_index != 95 ? ih.c_table[symbol_index+1] -1 : ih.bwt_length - 1;

        while (first <= last && i > 0) {
            symbol_index=char_to_index[query_string[i-1]];
            first=ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,first,interval  );
            
            if (last != ih.bwt_length - 1) { 
                last= ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,last+1,interval  )-1;
            } else {
                last= ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,last,interval)+ (char_to_index[get_char (mc,bwt_fp,last)]==symbol_index?1:0) - 1;
            }
            i=i-1;
        }
        
        if  (opt_value != NULL && strcmp("-m",opt_value) == 0) {
            printf("%d\n",last-first+1);
        }   else if   (opt_value != NULL && strcmp("-n",opt_value) == 0) {
            //printf("%d\n",last-first+1);

            if (last-first+1 == 0 || last-first+1 == 1){
                printf("%d\n",last-first+1);
            } else {
                struct Dict_entry **buckets = (struct Dict_entry **) calloc(HASHTABLE_SIZE, sizeof(struct Dict_entry *));
                
                for (i=0; i < HASHTABLE_SIZE; i++){
                    buckets[i] = NULL;
                    //printf("%s\n",dictionary[i]); 
                }
                //printf("%d %d\n",first, last);
                //int *line_nums = (int *) malloc((last-first+1) *sizeof(int) );
                int line;
                unsigned int matches = 0;
                unsigned matches_deduped = 0;
                for(line = first; line <= last; line ++){
                    int l = get_line_num(mc,idx_fp,bwt_fp,ih,line,interval);
                    //printf("line %d\n",l);
                    if(findInDict(buckets,ih.bwt_length,l) == -1){
                        matches_deduped = matches_deduped+1;
                        addToDict(buckets,ih.bwt_length,l,NULL);
                    } else {

                    }
                    
                }

                /*qsort(line_nums, last-first+1, sizeof(int), int_compare );
                matches = 0;
                
                for(line = first; line <= last; line ++){
                    //printf("sorted line %d\n",line_nums[matches]);
                    matches = matches + 1;
                }
                int prevLine =-1;
                unsigned matches_deduped = 1;
                matches = 0;
                for(line = first; line < last; line ++){
                    if (line_nums[matches] != line_nums[matches+1]){
                        printf("cmp %d %d\n",line_nums[matches] , line_nums[matches+1]);
                        matches_deduped = matches_deduped + 1;
                    }
                    matches = matches + 1;
                }*/
                printf("%d\n",matches_deduped);
                // free dictionary and contents
                for (i=0; i < HASHTABLE_SIZE; i++){
                    struct Dict_entry* node =  buckets[i];
                    if(node != NULL){
                        free( (*node).key );
                        free(node);
                    }
                    
                }
                free(buckets);
                //free(line_nums);
            }
        } else {
            // output all lines
            struct Dict_entry **buckets = (struct Dict_entry **) calloc(HASHTABLE_SIZE, sizeof(struct Dict_entry *));
            
            for (i=0; i < HASHTABLE_SIZE; i++){
                buckets[i] = NULL;
                //printf("%s\n",dictionary[i]); 
            }
            int line;
            unsigned int matches = 0;
            unsigned matches_deduped = 0;
            for(line = first; line <= last; line ++){
                struct Result * r = get_line_num_with_result(mc,idx_fp,bwt_fp,ih,line,interval);
                //printf("line %d\n",(*r).line);
                if(findInDict(buckets,ih.bwt_length,(*r).line) == -1){
                    struct  Result_entry * re = (struct Result_entry *) malloc (sizeof (struct Result_entry));
                    //(*re).line = (*r).line;
                    (*re).start_pos = (*r).start_pos;
                    //printf("startpos %d\n",(*r).start_pos);
                    (*re).partial = (*r).partial;
                    //printf("%s",(*re).partial);
                    matches_deduped = matches_deduped+1;
                    addToDict(buckets,ih.bwt_length,(*r).line,re);
                } else {
                    free((*r).partial);
                }
                free(r);     
            }
            for (i=0; i < HASHTABLE_SIZE; i++){
                struct Dict_entry* node =  buckets[i];
                if(node != NULL){
                    int k =0;
                    //printf("nodepointer=%p\n",node);
                    
                    struct  Result_entry ** res = (*node).result_entry;
                    //printf("respointer=%p\n",res);
                    
                    for (k=0; k < (*node).key_count; k++){
                        struct  Result_entry *renew = res[k]; 
                        //printf("k=%d pointer=%d\n",k,renew);
                        //printf("k=%d startpos=%d pointer=%p\n",k,,renew);

                        //printf("\n %d %d\n",k, (*node).key_count);
                        printf("%s",(*renew).partial);
                        //printf("startpos %d", ((*(*node).result_entry)[k]).start_pos);
                        if(ih.c_table[1] == ((*node).key)[k]+1){
                            print_decoded_line_until(mc,idx_fp, bwt_fp, ih, 0, (*renew).start_pos, interval);
                        } else {
                            print_decoded_line_until(mc,idx_fp, bwt_fp, ih, ((*node).key)[k]+1, (*renew).start_pos, interval);
                        }
                    }
                    //free( (*node).key );
                    //free(node);
                }
                
            }
            for (i=0; i < HASHTABLE_SIZE; i++){
                struct Dict_entry* node =  buckets[i];
                if(node != NULL){
                    int k =0;
                    struct  Result_entry ** res = (*node).result_entry;
                    for (k=0; k < (*node).key_count; k++){
                        struct  Result_entry *renew = res[k]; 
                        free( (*renew).partial);
                        free(renew);
                    }
                    free(res);
                    free( (*node).key );
                    free(node);
                }
                
            }
            free(buckets);
            //free(buckets);
        }

    }
  
    
    //print_array(ih.c_table,96);
    /*for (i =0;i<96;i++){
        printf("%c|%d ",index_to_char(i),occ(idx_fp, bwt_fp, ih,i,  1, interval));
        
    }
    printf("\n");*/

    

    fclose(idx_fp);
    fclose(bwt_fp);
    

    free (mc.c);


    return(0);
   
}
/*inline static unsigned char char_to_index(unsigned char in_char){
    if(in_char == 10){
        return 0;
    } else {
        return in_char - 31; // only 96 ascii characters in use
    }
}*/
/*inline static unsigned char index_to_char(unsigned char index){
    if(index == 0){
        return 10;
    }   else {
        return index + 31;
    }
}*/
inline static unsigned char get_char (struct Memory_copy  mc,FILE *bwt_fp,unsigned int pos) {
    unsigned char symbol ;
    if(pos >= (mc).start &&  pos < (mc).end ){
        char *c = (mc).c;
        return c[pos-(mc).start];
    } else { 
        fseek(bwt_fp, pos, SEEK_SET);
        fread(&symbol, sizeof(char), 1, bwt_fp);
   }
    return symbol;
}

inline static unsigned int fprint_decoded_line(struct Memory_copy  mc,FILE *opt_fp, FILE *idx_fp, FILE *bwt_fp, FILE* opt_random_fp, struct Index_header ih, unsigned int line_br_pos,unsigned int interval, unsigned int file_last_position, unsigned int file_occ_entries) {
    //unsigned char *reversed = (char *) malloc ( sizeof(char) * 512 ); // max line length is 512
    unsigned char reversed[512];    
    register unsigned int i = 0;
    register unsigned int current_pos = line_br_pos;
    reversed[i] = '\n';
    //statistics [current_pos/RAM_COPY_SIZE_OUTPUT]++;
    // get next char
    //i = i + 1;
    
    do{
        //printf("char %c\n",reversed[i-1]);
        //printf("current_pos %d %d\n", i,current_pos); 
        i = i +1;
        /*if(i >= 512) {
           //printf("realloc\n"); 
            reversed = (char *) realloc(reversed, (i+1) * sizeof(char));
        }*/

        if(current_pos >= (mc).start &&  current_pos < (mc).end ){
            //char *c = (mc).c;
            reversed[i] = ((mc).c)[current_pos-(mc).start];
        } else { 
            
            fseek(bwt_fp, current_pos, SEEK_SET);
            //unsigned char symbol;
            //fread(&symbol, 1, 1, bwt_fp);
            //lseek (bwt_fp2, current_pos, SEEK_SET);
            //read (bwt_fp2, &symbol, 1);
            reversed[i] = getc(bwt_fp);
       }

        unsigned char const symbol_index = char_to_index[reversed[i]];
        
        if ( current_pos < file_occ_entries && current_pos*3 > file_last_position  ){
            
            //lseek(opt_random_fp, current_pos*3, SEEK_SET);
            //read( opt_random_fp,&occ,  3);
            fseek(opt_random_fp, current_pos*3, SEEK_SET);
            unsigned int occ = 0;
            fread(&occ, 3, 1, opt_random_fp);
            current_pos = ih.c_table[symbol_index]+occ;
        } else {
            current_pos = ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,current_pos,interval);
        }
        //statistics [current_pos/RAM_COPY_SIZE_OUTPUT]++;
       
    } while(reversed[i] != '\n' );
    unsigned int const len = i;
    for(;i>0;i--){
        //printf("%c",reversed[i-1]);
        //fwrite (reversed+(i-1), sizeof(char), 1, opt_fp);
        putc( *(reversed+(i-1)),opt_fp);
    }
    //free(reversed);
    return len;
}
inline static unsigned int get_line_num(struct Memory_copy  mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih, unsigned int any_pos,unsigned int interval) {

    unsigned short i = 0;
    unsigned char nextc; // max line length is 512
    
    unsigned int current_pos = any_pos;
    

    // get next char
    //i = i + 1;
    unsigned char symbol_index; //= char_to_index(get_char(mc,bwt_fp,current_pos));
    do{
        //printf("char %c\n",reversed[i-1]);
        //printf("current_pos %d %d\n", i,current_pos); 
        i = i +1;
        nextc = get_char(mc,bwt_fp,current_pos);
        symbol_index = char_to_index[nextc];
        current_pos = ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,current_pos,interval  );
        
    } while(nextc != '\n' && i<5120);

    return current_pos;
}
inline static struct Result * get_line_num_with_result(struct Memory_copy  mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih, unsigned int any_pos,unsigned int interval) {

    unsigned int i = 0;
    unsigned char *reversed = (char *) malloc ( sizeof(char) * 512 ); // max line length is 512
    struct Result *r =  (struct Result*) malloc (sizeof (struct Result));
    unsigned int current_pos = any_pos;
    
    (*r).start_pos = any_pos;
    
    reversed[i] = get_char(mc,bwt_fp,current_pos);
    unsigned char symbol_index = char_to_index[reversed[i]];
    while(reversed[i] != '\n' && i<5120){
        //printf("char %c\n",reversed[i-1]);
        //printf("current_pos %d %d\n", i,current_pos); 
        i = i +1;
        if(i >= 512) {
            //printf("realloc\n"); 
            reversed = (char *) realloc(reversed, (i+1) * sizeof(char));
        }
        current_pos = ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,current_pos,interval  );
        reversed[i] = get_char(mc,bwt_fp,current_pos);
        symbol_index = char_to_index[reversed[i]];
        
        
    } 

    unsigned int const len = i;
    (*r).line = ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,current_pos,interval  );
    (*r).partial = (char *) malloc ( sizeof(char) * (len+1) );
    ((*r).partial)[len] = '\0';
    for(i=0;i<len;i++){
        //printf("%c",reversed[i-1]);
        ((*r).partial)[i]  = reversed[len-i-1];
    }
    //((*r).partial)[len] = '\0';
    free(reversed);
    return r;
}
inline static unsigned int print_decoded_line_until(struct Memory_copy  mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih, unsigned int line_br_pos,unsigned int until_pos,unsigned int interval) {
    unsigned char *reversed = (char *) malloc ( sizeof(char) * 512 ); // max line length is 512
    int i = 0;
    //printf("\nline until %d %d\n",line_br_pos,until_pos);
    
    unsigned int current_pos = line_br_pos;
    reversed[i] = '\n';

    // get next char
    //i = i + 1;
    unsigned char symbol_index;// = char_to_index(get_char(mc,bwt_fp,current_pos));
    do{
        //printf("char %c\n",reversed[i-1]);
        //printf("current_pos %d %d\n", i,current_pos); 
        i = i +1;
        if(i >= 512) {
            //printf("realloc\n"); 
            reversed = (char *) realloc(reversed, (i+1) * sizeof(char));
        }
        reversed[i] = get_char(mc,bwt_fp,current_pos);
        //printf("pos %d char %d\n",current_pos,reversed[i]);
        symbol_index = char_to_index[reversed[i]];
        current_pos = ih.c_table[symbol_index]+occ( mc, idx_fp,bwt_fp,ih,symbol_index,current_pos,interval  );
        
    } while(reversed[i] != '\n' && i<5120 && current_pos != until_pos );
    //printf("#");
    for(i=i;i>=0;i--){
        printf("%c",reversed[i]);
        //fwrite (reversed+(i-1), sizeof(char), 1, opt_fp);
    }
    //printf("@");
    free(reversed);
    return 0;
}
inline static unsigned int occ(struct Memory_copy mc,FILE *idx_fp, FILE *bwt_fp, struct Index_header ih,unsigned char symbol_index, unsigned int pos,unsigned int interval){
    
    //unsigned char symbol = index_to_char(symbol_index);
    
    unsigned int const biginterval = interval*1.5;
    unsigned int closest_indexed_pos;
    if (pos < ih.occ_4_byte){
        closest_indexed_pos = pos + interval/2; 
        closest_indexed_pos = closest_indexed_pos - closest_indexed_pos % interval;
        //if (closest_indexed_pos >= ih.occ_4_byte) {
        //    closest_indexed_pos = ih.occ_4_byte;
        //}    
        //printf("symbol_index %d pos %d  closest_indexed_pos %d interval %d biginterval %d length %d skip %d\n",symbol_index,pos,closest_indexed_pos,interval,biginterval,ih.bwt_length,ih.occ_4_byte);
        if (closest_indexed_pos >= ih.bwt_length) {
                closest_indexed_pos = closest_indexed_pos - interval;

        }     
    } else  {
        
        closest_indexed_pos = (pos - ih.occ_4_byte) + biginterval/2;
        closest_indexed_pos = (closest_indexed_pos - closest_indexed_pos % biginterval) + ih.occ_4_byte;
        //closest_indexed_pos = closest_indexed_pos + ih.occ_4_byte;
        if (closest_indexed_pos >= ih.bwt_length) {
                closest_indexed_pos = closest_indexed_pos - biginterval;
                //exit(0);
        }        
        //printf("symbol_index %d pos %d  closest_indexed_pos %d interval %d biginterval %d length %d skip %d\n",symbol_index,pos,closest_indexed_pos,interval,biginterval,ih.bwt_length,ih.occ_4_byte); 
    }


    
    unsigned int occ_val_4_bytes;
    //printf("|%d %d|\n",closest_indexed_pos,pos);
    if (closest_indexed_pos < ih.occ_4_byte ){
        if (closest_indexed_pos == 0){
                    occ_val_4_bytes = 0;
            } else {
        //printf("ho\n");
                fseek(idx_fp, sizeof(struct Index_header) + ((closest_indexed_pos/interval)-1)*192 + symbol_index*2, SEEK_SET);
                unsigned short occ_val;
                fread(&occ_val, sizeof(short), 1, idx_fp);
                occ_val_4_bytes = occ_val;
            }
    } else {
        unsigned int const pre_4_byte_offset = ((ih.occ_4_byte/interval)-1)*192;
        unsigned int const post_4_byte_offset = (((closest_indexed_pos-ih.occ_4_byte)/(biginterval))  )*96*3;
        //printf("offsets %d %d %d %d %d\n",pre_4_byte_offset,post_4_byte_offset,closest_indexed_pos,ih.occ_4_byte,biginterval);
        fseek(idx_fp, sizeof(struct Index_header) + pre_4_byte_offset + post_4_byte_offset + symbol_index*3, SEEK_SET);
        occ_val_4_bytes=0;
        
        //char * pointer = (char*) &occ_val_4_bytes;
        //*pointer = 0x00;
        //pointer++;
        fread(&occ_val_4_bytes, sizeof(char), 3, idx_fp);
        //printf("result %d\n ", occ_val_4_bytes);
    }

    
   
    
    
    if(closest_indexed_pos < pos ){
        unsigned char const symbol = index_to_char[symbol_index];
        register unsigned int  diff_length = pos-closest_indexed_pos;
        //register unsigned int i;
        if(pos >= (mc).start &&  pos < (mc).end && closest_indexed_pos >= (mc).start &&  closest_indexed_pos < (mc).end ){   
                         
            register unsigned char *diff  = (mc).c + closest_indexed_pos-  (mc).start;
            while(diff_length-- > 0){
                //if(*(diff++)==symbol){ ++occ_val_4_bytes;  }
                occ_val_4_bytes = occ_val_4_bytes + ((*(diff++)==symbol) ? 1 : 0);
            } 
        } else {  
            unsigned char diff2 [diff_length];
            register unsigned char *diff = diff2;
            fseek(bwt_fp, closest_indexed_pos, SEEK_SET);
            fread(diff, diff_length,sizeof(char) , bwt_fp);
            while(diff_length-- > 0){
                //if(*(diff++)==symbol){ ++occ_val_4_bytes;  }
                occ_val_4_bytes = occ_val_4_bytes + ((*(diff++)==symbol) ? 1 : 0);
            } 
            //free(diff);
        }
    } else if(closest_indexed_pos > pos ){
        unsigned char const symbol = index_to_char[symbol_index];
        register unsigned int diff_length  = closest_indexed_pos - pos;
        //register unsigned int i;
        if(pos >= (mc).start &&  pos < (mc).end && closest_indexed_pos >= (mc).start &&  closest_indexed_pos < (mc).end ){ 
            register unsigned char *diff  = (mc).c + pos - (mc).start;            
            while(diff_length-- > 0){
                //if(*(diff++)==symbol){ --occ_val_4_bytes;  }
                occ_val_4_bytes = occ_val_4_bytes - ((*(diff++)==symbol) ? 1 : 0);
            } 
            
        } else {
            unsigned char diff2 [diff_length];
            register unsigned char *diff = diff2;
            //diff = (char *) malloc ( sizeof(char) * diff_length );
            fseek(bwt_fp, pos, SEEK_SET);
            fread(diff, diff_length, sizeof(char), bwt_fp);
            while(diff_length-- > 0){
                //if(*(diff++)==symbol){ --occ_val_4_bytes;  }
                occ_val_4_bytes = occ_val_4_bytes - ((*(diff++)==symbol) ? 1 : 0);
            }
            //printf("%d %d %d %d\n",pos,closest_indexed_pos,mc.start,(mc.start)+mc.length);
            //free(diff);
        }
    } 



    return occ_val_4_bytes;

}


int generate_infile_occ ( FILE * opt_fp, FILE * bwt_fp, struct Index_header ih){


   rewind(bwt_fp);
    //unsigned int start = 2*(ih.bwt_length/3); // start 2 thirds of the way approximately
    //fseek(bwt_fp, start, SEEK_SET);
    unsigned char c = 0;

    unsigned int entries = 0;
    unsigned char c_table_pos = 0;



    unsigned int occ_4_bytes[96]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   

    do {
        c = fgetc(bwt_fp);

        if( feof(bwt_fp) ) {
           break;
        }
        unsigned char index = char_to_index[c];
        

        char * p = (char*) &occ_4_bytes;
        //memcpy (data,  p + index*4, 3*sizeof(char)); 
        fwrite (p+index*4, sizeof(char), 3, opt_fp); 
        
        entries = entries + 1;

        occ_4_bytes[index] = occ_4_bytes[index] + 1;
        
        if( entries * 3 > ih.bwt_length-3) {
            break;
        }


        
    } while(1);


    rewind(opt_fp);
    return entries;
}
int generate_index ( const char* bwt_filename, const char* idx_filename){

    FILE *idx_fp;
    FILE *bwt_fp;
    bwt_fp = fopen(bwt_filename, "rb");

    idx_fp = fopen(idx_filename, "wb");
    

    struct Index_header ih = {0,0,{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    }};
    // Save index header at start of index file
    fwrite (&ih, sizeof(struct Index_header), 1, idx_fp);

    unsigned char c;
    
    // Calculate file size
    fseek(bwt_fp, 0L, SEEK_END);
    ih.bwt_length = ftell(bwt_fp);
    ih.occ_4_byte = ih.bwt_length;
    rewind(bwt_fp);
    
    unsigned int interval = ih.bwt_length/ (((  (ih.bwt_length/2)+2000-sizeof(struct Index_header)   ) / 192));
    interval = interval + 1;
    unsigned int biginterval = interval*1.5;
    //printf("length %d \n",ih.bwt_length);
   unsigned short occ[96]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    unsigned int occ_4_bytes[96]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    ,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   
    //printf("count %d \n",(((  (ih.bwt_length/2)+2000-sizeof(struct Index_header)   ) / 192)+1));
    //printf("interval %d \n",interval);

    unsigned int i = 0;

    do {
        c = fgetc(bwt_fp);

        if( feof(bwt_fp) ) {
           break;
        }
        unsigned char index = char_to_index[c];

        // switch to 4 byte occurance table
        if(occ[index] == USHRT_MAX && ih.occ_4_byte == ih.bwt_length){
            if( i % interval == 0) {
                ih.occ_4_byte = i;
                //printf("option1");
            } else {
                ih.occ_4_byte = i + interval - (i % interval);

                //printf("option2");
            }
        }

        
        if ( i % interval == 0 && ih.occ_4_byte == ih.bwt_length  && i > 0){
            fwrite (occ, sizeof(occ), 1, idx_fp);
            //printf("i %d\n",i);
            //print_array_s(occ,96);
        }
        
     
        if (  (i-ih.occ_4_byte) % (biginterval) == 0 && i >= ih.occ_4_byte && ih.occ_4_byte != ih.bwt_length  && i > 0){
            
            char j;
            char * p = (char*) &occ_4_bytes;
            //if(i==ih.occ_4_byte){
            //    printf("firstnum %d %d %d %d\n",occ_4_bytes[66],biginterval,i,ih.occ_4_byte);
            //}
            for (j=0; j < 96;j++){
                
                fwrite (p, sizeof(char), 3, idx_fp); // optimisation: just save 3 bytes
                p++;p++;p++;p++;
            }
            //fwrite (occ_4_bytes, sizeof(occ_4_bytes), 1, idx_fp);
            //
        }

        occ[index] = occ[index] + 1;
        occ_4_bytes[index] = occ_4_bytes[index] + 1;
        if (index != 95 ){
            ih.c_table[index+1] = ih.c_table[index+1] + 1;
        }
        i = i + 1;
        

    } while(1);
    
    //printf("gen index bwt_length %d occ_4_byte %d interval %d\n",ih.bwt_length,ih.occ_4_byte,interval);
    for(i=1;i<96;i++){
        ih.c_table[i] = ih.c_table[i] + ih.c_table[i-1];  
    }
    fflush(idx_fp);
    rewind(idx_fp);
    fwrite (&ih, sizeof(struct Index_header), 1, idx_fp);
    //printf("headersize %d\n",sizeof(struct Index_header));
    fflush(idx_fp);
    fclose(idx_fp);
    fclose(bwt_fp);
  

    return(0);
}


