#include <iostream>
#include <fstream>
#include <string.h>
#include "../includes/extract.hpp"
#include "../includes/decrypt.hpp"
#include "../includes/general.hpp"


/*
This is how an extraction work :
    - there is hidden parts of the file hidden in the embedding file that we need to extract, this where the header come in to give us more infos about where these parts are allocated
    *file A* : 
     _______________
    |magic bytes    |
    |  file header  | <---------- data we need for extraction
    |_______________|
    |               |
    |   file data   |
    |_______________|
    |Hidden enc data|
    |---------------|
    |More file data |
    |---------------|
    |Hidden enc data|
    |---------------|
    |   file data   |
    |_______________|
*/

struct s_header{
    int s_offset;       // where the first part of the file start
    int s_sizec;        // the size of every chunk of hidden data
    int s_sizef;        // size of the embeded file
    int s_nparts;       // number of hidden parts
    int s_shift;        // the size of the data between every hidden data
    int s_lsize;        // the size of the last part of hidden data
    int s_hash_offset;  // the offset of the hash 
    char s_signature[14];    // used for verification
    char s_hash[33];    // embededfile md5 hash
};
void extract(char* stegfile, char* passphrase, char* outputfile, bool encryption){
    // checking if the output is null
    if(outputfile == NULL){
        outputfile = "output";
    }
    // starting to read the header and extracting values from it
    int storage[7] ;                // an array that will store all the file header
    char tempstorage[15];
    int r_size = 4 ;                // how many byte to read
    int r_offset = 16 ;             // where to start (this will change as long as the loop loops)
    std::ifstream steg;
    int i = 0 ; 
    steg.open(stegfile);
    while(i < 7){
        steg.seekg(r_offset);
        steg.read(tempstorage, r_size) ;
        removeChar(tempstorage, 'x');
        storage[i] = atoi(tempstorage);
        r_offset += 4 ;
        i++ ;
    }
    // assgning the values readed to the struct
    struct s_header header ;
    header.s_offset = storage[0];
    header.s_sizec = storage[1] ; 
    header.s_sizef = storage[2] ;
    header.s_nparts = storage[3] ; 
    header.s_shift = storage[4] ; 
    header.s_lsize = storage[5] ;
    header.s_hash_offset = storage[6];
    // reading the hash part
    steg.seekg(header.s_hash_offset);
    steg.read(header.s_hash, 32) ;       // reading it directly into the header var from the struct some junk bytes to apear 
    header.s_hash[32] = '\0';
    std::cout << header.s_hash << std::endl ;

    //closing the file 
    steg.close();
}
