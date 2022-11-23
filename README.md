# BWT Search
## Background
Burrows–Wheeler transform is a method of encoding text to increase occurrence of repeating sequential characters.

For example, original string:
`$BANANA`
In BWT encoded form becomes:
`ANNB$AA`
This is a useful property as consecutive characters can compressed.

For example, in run-length-encoding
`WWWWWWWWWWWWBWWWWWWWWWWWWBBBWWWWWWWWWWWWWWWWWWWWWWWWBWWWWWWWWWWWWWW`
can be encoded to 
`WW12BWW12BB3WW24BWW14`

While this is a simplified example, this idea is applied in various compression algorithms.


## Usage
Compile
```
make
```
Number of substrings 
```
bwtsearch -m test.bwt test.idx "ana"
```
Number of lines with substring
```
bwtsearch -n test.bwt test.idx "ana"
```
Decode lines with matching substring
```
bwtsearch -o test.bwt test.idx "ana"
```
Full decoding
```
bwtsearch test.bwt test.idx
```

## Performance
This implementation maximises speed within a limitation that is uses maximum of 16MB of memory and an index file of up to half the size of the encoded file.

The index file is only calculated once, once calculated it is reused to improve performance. The index file stores some metadata. Ideally, it would store a full 'occurance' table, being the number of times a character has appeared at a certain position within the file. However, this would make the index file very large or use a lot of memory, so we only store a sample, and then conduct a count from each sampled location. 

Secondly, for a full decoding, there are many random accesses required. I observed that the random accesses are biased towards the start of the file at the beginning, and the end of the file towards the end. So, I load, to the fullest extent possible, the portion of file into RAM that matches this requirement.

Lastly, in order to dedupe the partial results, I use a custom dictionary structure, which is extremely memory optimesd. 
