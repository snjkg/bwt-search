#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "dict.h"

#define HASHTABLE_SIZE 16384

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
