#include <iostream>
#include <string.h>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include "../lib/Md5/MD5.h"
#include "../includes/encrypt.hpp"
#include "../includes/general.hpp"
#include "../includes/png.hpp"



// take a look at extract.cpp to understand this
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

void embed(char* coverfile, char* embedingfile, char* passphrase, char* outputfile, bool encryption){
    srand (time(NULL));
// some checks before the embeding 
    // getting the size of the file
    int cosize = filesize(coverfile);
    // checking the existance of the embeding file
    int emsize = filesize(embedingfile);
    /* checking the size of the files
    we are storing every 5/6 bytes of the embeding file in every 20 byte of the coverfile so now we are checking if the cover file can support it */ 
    if(emsize > cosize){
        printf("Coverfile is small for the embeding,chose a bigger file\n");
        exit(0);
    }
    // checking the output option 
    if(outputfile == NULL){
        outputfile = "output" ;
    }
    // checking the magic bytes of the file
    bool is = is_png(coverfile);
    if(is == 1){
        pngembed(coverfile,embedingfile,passphrase,outputfile,encryption);
    }
    // encrypting the embedding file 
    unsigned char *encrypted_text = encrypt(embedingfile, passphrase);
    // generating the struct values 
    struct s_header file;
    file.s_offset = 60 ;
    file.s_sizec = rand() % 20 + 70;            // the size of the chunks are randomly chosed and putted in the header 
    file.s_sizef = emsize ; 
    file.s_nparts = file.s_sizef / file.s_sizec ; 
    file.s_shift = file.s_sizef / file.s_nparts ; 
    file.s_lsize = file.s_sizef % file.s_sizec ;
    file.s_hash_offset = 46 ;
    strcpy(file.s_signature , "\x70\x6c\x65\x61\x73\x65\x20\x6d\x79\x73\x65\x6c\x66");
    // starting to write the values into the file 
    // opening the embd file 
    auto cover_content = std::ostringstream{};
    std::ifstream input_file(coverfile);
    cover_content << input_file.rdbuf();
    std::string string_cover_content = cover_content.str();
    // writing header values in the variable
    int s_array[7] = {file.s_offset,file.s_sizec, file.s_sizef, file.s_nparts,file.s_shift, file.s_lsize,file.s_hash_offset};
    int i = 0 , f = 16;
    // writing the signature at the end of the file 
    string_cover_content.insert(cosize , file.s_signature);
    // when reading the header using extract.cpp the x added here will be removed and the strings will be converted to ints to get the right values
    // reading these header maybe will crash when using bigest files "more than 65535" so this is temporarely until i change it
    // a loop to write bytes to the file
    while(i < 7){
        if(s_array[i] < 9){
            string_cover_content.insert(f , std::to_string(s_array[i]) + "xxx");
        }else if(s_array[i] < 99){
            string_cover_content.insert(f , std::to_string(s_array[i]) + "xx");     // the x appended to the file are used to make every int take 4 bytes, i thought adding null bytes would be better but null bytes are used as string terminators, x's are used temporarily until i find a better solution 
        }else if(s_array[i] < 999){
            string_cover_content.insert(f , std::to_string(s_array[i]) + "x");
        }else if(s_array[i] < 9999){
            string_cover_content.insert(f , std::to_string(s_array[i]));
        }else{
            std::cout << "File is too big" << std::endl ;
            exit(0);
        }
        i++;
        f = f + 4 ;
    }
    //adding the hash of the file for verification 
        // storing the file content into a variable
    auto hashedf = std::ostringstream{};
    std::ifstream emfileh(embedingfile);
    hashedf << emfileh.rdbuf();
    std::string emfilecontent = hashedf.str();
        // calculating the hash
    std::string md5filehash = md5(emfilecontent);
    string_cover_content.insert(file.s_hash_offset, md5filehash);
    emfileh.close();
    // in this part of the code splitts the "encrypted_text" that contain the encrypted text to small parts that can be fitted and inserted in the cover file

    // opening outputfile
    std::ofstream output(outputfile);
    output << string_cover_content ;
    output.close();
}

