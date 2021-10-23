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
    int s_sizef;        // size of the embeded file
    int s_sizec;        // the size of every chunk of hidden data
    int s_nparts;       // number of hidden parts
    int s_shift;        // the size of the data between every hidden data
    int s_lsize;        // the size of the last part of hidden data
    int s_hash_offset;  // the offset of the hash 
    char s_signature[14];    // used for verification
    char s_hash[33];    // embededfile md5 hash
};

void embed(char* coverfile, char* embedingfile, char* passphrase, char* outputfile, bool encryption){
    srand(time(0));      // seed for random generator
// some checks before the embeding 
    // getting the size of the coverfile0 file
    int cosize = filesize(coverfile);
    // checking if the files exists
    exist(coverfile);
    exist(embedingfile);
// checking if the user gave an output file
    if(outputfile == NULL){
        outputfile = "output" ;
    }
/* magic bytes checking for png files :
        png files are sensitive when appending data randomely inside them, so i used different way to embed data inside them
*/
    bool is = is_png(coverfile);            // checking if the file is png using magic bytes "https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwi1yKy8-d3zAhXx7OAKHX20DRwQFnoECAQQAw&url=https%3A%2F%2Fmedium.com%2F%40d.harish008%2Fwhat-is-a-magic-byte-and-how-to-exploit-1e286da1c198&usg=AOvVaw2gBXWj8B3lAQjhU-xUT2fF"
    if(is == 1){
        pngembed(coverfile,embedingfile,passphrase,outputfile,encryption);
    }
    // encrypting the data using blowfish algorithm "ecb"
    unsigned char *encrypted_text = encrypt(embedingfile, passphrase);
    // assigning and calculating header parts
    int random_number  =( rand() % 20 ) ;       // random number for header s_sizec
    if(random_number < 10 ){    // random number must not be lower than 10
        random_number += 10 ;
    }
    struct s_header header ; 
    header.s_offset = 200 ;         // where the first encrypted bytes will be
    header.s_sizef = strlen((char *) encrypted_text) ;       // the size of encrypted text 
        // How the size of every chunk of data is calculated : 
            // (header.sizef * random number between 10 --> 20) / 100 
    header.s_sizec =(header.s_sizef * random_number) / 100;
    header.s_nparts = header.s_sizef / header.s_sizec ; 
    header.s_shift = header.s_sizec + 30 ;      // the data between encrypted hidden data
    header.s_lsize = header.s_sizef % header.s_sizec ;
    header.s_hash_offset = 46 ;
    strcpy(header.s_signature , "\x70\x6c\x65\x61\x73\x65\x20\x6d\x79\x73\x65\x6c\x66");
    if(header.s_sizef > cosize){
        printf("Coverfile is small for the embeding,chose a bigger file\n");
        exit(0);
    } 
    // writing the header 
    // opening the coverfile and storing his content on a variable 
    auto cover_content = std::ostringstream{};
    std::ifstream input_file(coverfile);
    cover_content << input_file.rdbuf();
    std::string string_cover_content = cover_content.str();
    // writing the signature to avoid the junk data that would be after 
    string_cover_content.insert(cosize , header.s_signature);
    // putting the header values inside an array to be easy to write at a loop
    int s_array[7] = {header.s_offset,header.s_sizef, header.s_sizec, header.s_nparts,header.s_shift, header.s_lsize,header.s_hash_offset};
    int i = 0, f = 16 ;
    // when reading the header using extract.cpp the x added here will be removed and the strings will be converted to ints to get the right values
    // reading these header maybe will crash when using bigest files "more than 9999" so this is temporarely until i change it
    while(i < 7){
        if(s_array[i] < 9){
            string_cover_content.insert(f , std::to_string(s_array[i]) + "xxx");
        }else if(s_array[i] < 99){
            string_cover_content.insert(f , std::to_string(s_array[i]) + "xx");     // the x appended to the file are used to make every int take 4 bytes, i thought adding null bytes would be better but null bytes are used as string terminators, x's are used temporarily until i find a better solution 
        }else if(s_array[i] < 999){
            string_cover_content.insert(f , std::to_string(s_array[i]) + "x");
        }else if(s_array[i] < 9999){
            string_cover_content.insert(f , std::to_string(s_array[i]));
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
        // inserting the hash in the variable
    string_cover_content.insert(header.s_hash_offset, md5filehash);
    // writing the first part aftert the header 
    string_cover_content.insert(header.s_offset , spliter(encrypted_text,0,header.s_sizec)); 
    int data_written = header.s_sizec ; 
    header.s_nparts-- ;
    // just for testing, please future me delete this
    /*std::cout << "encrypted text lenght : " << header.s_sizef << std::endl 
              << "size of everychunk : " << header.s_sizec << std::endl 
              << "parts " << header.s_nparts << std::endl 
              << "shift " << header.s_shift << std::endl 
              << " last chunk size "  << header.s_lsize << std::endl ;
*/
    // injecting the encrytped data inside the string_cover_content
    while(header.s_nparts > 0){
        // tests to monitor the writing, please delete that later 
 /*       std::cout << "data written to the file " << data_written << std::endl 
                  << "offset to write " << header.s_offset << std::endl 
                  << "part " << header.s_nparts << std::endl ;*/
        header.s_offset += header.s_shift + header.s_sizec ;
        // storing the splitted part in a string 
        std::string splitted = spliter(encrypted_text,data_written,header.s_sizec);
        // converting from a string to char pointer 
        char splitted_container[splitted.length() + 10];
        strcpy(splitted_container,splitted.c_str());
        string_cover_content.insert(header.s_offset , splitted_container); 
        data_written += header.s_sizec  ;
        header.s_nparts-- ;
    }
    // writing the last part of the encrypted text
    std::string splitted = spliter(encrypted_text,data_written,header.s_lsize);
    char split_cont[splitted.length() + 10];
    strcpy(split_cont,splitted.c_str());
    // establishing the shift
    header.s_offset += header.s_shift + header.s_sizec ;
    // writing to the variable 
    string_cover_content.insert(header.s_offset , split_cont);
    // writing to the output file and closing the other files
    std::ofstream output(outputfile);
    output << string_cover_content ;
    output.close();
    emfileh.close();
    std::cout << "Embedding done successfully" << std::endl ;
}