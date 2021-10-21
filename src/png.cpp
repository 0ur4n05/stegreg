#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include "../includes/encrypt.hpp"
#include "../includes/decrypt.hpp"
#include "../includes/general.hpp"

/*
why create new functions for png ?
    - png are really sensitive, that means you can't hide data instide them or modifie their inside, the image cannot be read.
How this works?
    - when adding data inside the png the image cannot be read, but when adding it to the end of the file, that doesn't affect the image, even if it's not obsecure as jpg/jpeg the data is encrypted which that means that a thirth party cannot know what the data means
Header?
    - Most of the header data woudn't be necessairy only the size of the embeded file and the signature would be useful

File 
     -------------------------
    |                         |
    |                         |
    |        Image data       |
    |                         |
    |                         |
    |                         |
    ---------------------------
    |                         |
    |       Hidden data       |
    |                         |
    ---------------------------
    |    Hidden data length   |         <----------- 4 bytes
    ---------------------------
    |         Signature       |         <----------- 14 byte
    ---------------------------

*/
struct c_infos{
    int cf_size;            // coverfile size
    int em_size;            // embeding file size
};

void pngembed(char* coverfile, char* embedingfile, char* passphrase, char* outputfile, bool encyrption){
    // output check
    if(outputfile == NULL){
        outputfile = "output.png" ;
    }
    char signature[14] = "please myself";
    struct c_infos file ;
    std::string files;
    // making space for the filesize
    // encrypting the embeding file 
    unsigned char *encrypted_text = encrypt(embedingfile, passphrase);
    file.em_size = strlen((char *) encrypted_text);
    if(file.em_size < 9){
        files = std::to_string(file.em_size) + "xxx";
    }else if(file.em_size < 99){
        files = std::to_string(file.em_size) + "xx";
    }else if(file.em_size < 999){
        files = std::to_string(file.em_size) + "x";
    }else if(file.em_size < 9999){
        files = std::to_string(file.em_size);
    }else{
        std::cout << "File is too big" << std::endl ;
        exit(0);
    }
    // opening the coverfile and storing it in a variable 
    auto cfcontent = std::ostringstream{};
    std::ifstream cfile(coverfile);
    cfcontent << cfile.rdbuf();
    std::string cfcontentstr = cfcontent.str();
    // encrypting the embeding file 
    std::string enctext((char*) encrypted_text);
    // combining everything 
    cfcontentstr = cfcontentstr + enctext + files;
    // writing to the file
    std::ofstream outp;
    outp.open(outputfile);
    cfcontentstr.insert(cfcontentstr.length(), signature);
    outp << cfcontentstr ;
    outp.close();
    exit(0);
}

void pngextract(char* stegofile, char* passphrase, char* outputfile, bool encyrption){
    // output check
    if(outputfile == NULL){
        outputfile = "extracted_data" ;
    }
    // getting the size of the file 
    int fs = filesize(stegofile);
    int steg_size ;    // stego file size
    char ssstr[5];
    // reading the hidden data lenght
    std::ifstream file;
    file.open(stegofile);
    file.seekg(fs - 17) ;
    file.read(ssstr,4);
    ssstr[4] = '\0';             //terminating the string 
    removeChar(ssstr, 'x');
    steg_size = atoi(ssstr);
    int offset = fs - (steg_size + 17);              // stegofile size - hiddenfile size
    // extracting the data 
    char hidden_encrypted_data[steg_size + 10];
    file.seekg(offset);
    file.read(hidden_encrypted_data, steg_size);
    hidden_encrypted_data[steg_size] = '\0' ;          // terminating the string
    // decrypting the encrypted data 
        // coping it to unsigned char 
    unsigned char* decrypted_data = decrypt(hidden_encrypted_data, passphrase) ;
    std::ofstream ofile(outputfile);
    ofile << (char *) decrypted_data ;
    ofile.close();
    file.close();
    exit(0);
}