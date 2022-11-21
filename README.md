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


