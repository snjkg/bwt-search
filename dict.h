struct Dict_entry {
    int* key;
    int key_count;
    struct Result_entry** result_entry;
};
unsigned int hashCode( unsigned int bwt_length,unsigned int key);
int findInDict(struct Dict_entry *buckets[],unsigned int bwt_length, unsigned int key);
int addToDict(struct Dict_entry *buckets[],unsigned int bwt_length, unsigned int newkey, struct Result_entry * result_entry);
