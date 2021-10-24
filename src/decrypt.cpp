#include <iostream>
#include "../lib/Blowfish/blowfish.h"
#include <string.h>

unsigned char *decrypt(char* encrypted_text, char *passphrase){ 
    if(passphrase == NULL){
        passphrase = "mypassphrase";
    }else{  
        if(strlen(passphrase) >= 1024){
            std::cout << "passphrase need to be smaller than 1024 byte" << std::endl ;
            exit(0);
        }
    }

    unsigned char unsigned_content[strlen(encrypted_text) + 5] = {0} ; 
    int i = 0 ;
    strncpy((char *)unsigned_content, encrypted_text, strlen(encrypted_text));
// converting to unsigned keys
    i = 0;
    unsigned char unsigned_key[strlen(passphrase) + 5] = {0} ; 
    while(i < strlen(passphrase)){
        unsigned_key[i] = passphrase[i] ;
        i++ ;
    }
    Blowfish blowfish;
    blowfish.SetKey(unsigned_key,sizeof(unsigned_key));
    blowfish.Decrypt(unsigned_content,unsigned_content,sizeof(unsigned_content));
    unsigned char* unsig_content = unsigned_content ;
    return unsig_content ;
}