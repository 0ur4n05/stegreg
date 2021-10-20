#include <iostream>
#include "../lib/Blowfish/blowfish.h"
#include <string.h>
#include <fstream>


unsigned char *encrypt(char* embedingfile, char *passphrase){
    // checking if the user provided a passphrase
    if(passphrase == NULL){
        passphrase = "mypassphrase";
    }else{  
        if(strlen(passphrase) >= 1024){
            std::cout << "passphrase need to be smaller than 1024 byte" << std::endl ;
            exit(0);
        }
    }
    // reading the embeding file and putting it into a variable 
    // now getting the content of the file into a variable 
    std::ifstream ifs(embedingfile);
    std::string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
    // converting the string to a unsigned char by a loop 
    unsigned char unsigned_content[content.length() + 5] = {0} ; 
    int i = 0 ;
    while(i < content.length()){
        unsigned_content[i] = content[i];
        i++;
    }
    // same for the key 
    i = 0;
    unsigned char unsigned_key[strlen(passphrase) + 5] = {0} ; 
    while(i < strlen(passphrase)){
        unsigned_key[i] = passphrase[i] ;
        i++ ;
    }
    /* encrypting the unsigned_content "file content"
    algorithm used to encrypt = blowfish
    lib used : https://github.com/ddokkaebi/Blowfish  */
    Blowfish blowfish;
    blowfish.SetKey(unsigned_key,sizeof(unsigned_key));
    blowfish.Encrypt(unsigned_content,unsigned_content,sizeof(unsigned_content));
    unsigned char* unsig_content = unsigned_content ;
    return unsig_content ;
}