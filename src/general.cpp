#include <iostream>
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
char* spliter(unsigned char *encrypted, int offset, int bytes){
    char list_cutting[bytes+1] = {0} ;              // this where the encryted text will get stored 
    int i  = 0;                                         // simple iterator 
    while(i < bytes){
        list_cutting[i] = encrypted[offset];
        i++;
        offset++;
    }
    char *result = list_cutting ;
    return result;
}
void removeChar(char *str, char garbage){
    char *src, *dst;
    for (src = dst = str; *src != '\0'; src++) {
        *dst = *src;
        if (*dst != garbage) dst++;
    }
    *dst = '\0';
}
