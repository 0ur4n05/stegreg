#include <iostream>
#include <fstream>
#include <string.h>
#include "../includes/general.hpp"

using namespace std;

void check(char* filename){
    // opening the file and getting his size
    ifstream file ; 
    // getting the size of the file
    long file_size = filesize(filename);
    char sig[15] ;      // where the signature will be stored
    // starting to check the existance of a signature
        // checking the signature offset by reading the last 14 bytes of the file 
    int sig_offset = file_size - 13 ;
    int sig_length = 13 ; 
    file.open(filename) ; 
    file.seekg(sig_offset);
    file.read(sig, sig_length) ; 
    if(strcmp(sig , "please myself") == 0){
        cout << "The signature found in " << sig_offset << " byte." << endl ;
        printf("This file contain hidden data by this tool.\n");
    }else{
        printf("There is no data hidden in this file.\n");
    }
    exit(0);
}


/*
    𝖕𝖑𝖊𝖆𝖘𝖊 𝖒𝖞𝖘𝖊𝖑𝖋 'dlǝɥ ǝɯos ʇuɐM
    ɟlǝsɹnoʎ ʇnɔ 'ǝpıɹ ɐ ǝʞɐʇ ǝɯ ʇǝ˥
    ǝnɹʇ uǝǝq ǝʌɐɥ 'noʎ ǝsıɯoɹԀ
    ploʇ ʇ,uǝʌɐɥ 'ǝdoɹ ǝɯos ʇo⅁
    𝖕𝖑𝖊𝖆𝖘𝖊 𝖒𝖞𝖘𝖊𝖑𝖋 'dlǝɥ ǝɯos ʇuɐM
    ɟlǝsɹnoʎ ʇnɔ 'ǝpıɹ ɐ ǝʞɐʇ ǝɯ ʇǝ˥
    sɓuıʍ ʎʇɹıp ɹnoʎ dılɔ ǝɯ ʇǝ˥
    pǝǝs ɐ ǝʌɐɥ 'ǝɯ ʇ,usI
*/