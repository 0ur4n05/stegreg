#include <iostream>
#include <string.h>
#include <fstream>
// this file contain general fucntion used in this program to prevent repetition 

int filesize(char* filename){
    FILE *file;
    if((file = fopen(filename,"r"))){
        fseek(file,0,SEEK_END);
    }else{
        printf("This file doesn't exist\n");
        exit(0);
    }
    // getting the size of the file
    int filesize = ftell(file);
    return filesize ;
}
// get the encrypted_text a
std::string spliter(unsigned char *encrypted, int offset, int bytes){
    char list_cutting[bytes+1] = {0} ;              // this where the encryted text will get stored 
    int i  = 0;                                         // simple iterator 
    while(i < bytes){
        list_cutting[i] = encrypted[offset];
        i++;
        offset++;
    }
    char *p = list_cutting;
    std::string r(p) ;
    return r;
}
void removeChar(char *str, char garbage){
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}

bool is_png(char *filename){
    // opening the file
    char png_magic_bytes[9] = "\x89\x50\x4E\x47\x0D\x0A\x1A\x0A";
    char file_magic_bytes[9] ;
    std::ifstream file;
    file.open(filename);
    // reading the bytes
    file.seekg(0);
    file.read(file_magic_bytes,8);
    // comparing 
    if(memcmp(png_magic_bytes,file_magic_bytes,8)){
        return 0 ;          // false
    }else{
        return 1 ;          // true
    }
    return 1;
}
// checks if the file given exists
void exist(char *filename){
    FILE *file;
    file = fopen(filename ,"r");
    if(!file){
        std::cout << "File doesn't exist"<< std::endl;
        exit(0);
    }
}