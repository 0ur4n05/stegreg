#include <iostream>
#include <unistd.h>
#include "../includes/embed.hpp"
#include "../includes/extract.hpp"
#include "../includes/check.hpp"

// check a signature already putted in the file 
void help();

int main(int argc,char** argv){
    // getting arguments
    int c ;
    bool encode = 1, decode = 1, encryption = 0;
    // initializing the variables to store the arguments values
    char *coverfile; char* embedingfile ;char* stegofile;char *passphrase;char* outputfile;
    while((c = getopt(argc,argv,"edvhf:b:c:s:p:no:")) != -1){
        switch (c){
            case 'e':
                encode = 0 ;
                break;
            case 'd':
                decode = 0 ;     
                break;
            case 'v':
                std::cout << "Stegreg 1.0\n\n\nMade by 0UR4N05\nhttps://github.com/0UR4N05/stegreg" << std::endl;
                return 0;
                break;
            case 'h' :
                help();
                return 0;
                break ; 
            case 'f' : 
                check(optarg);
                break;
            // embeding parameters
            case 'b' : 
                embedingfile = optarg ; 
                break ; 
            case 'c' : 
                coverfile = optarg ;
                break;
            // extracting params
            case 's' : 
                stegofile = optarg ;
                break;            
            // optional params
            case 'p' : 
                passphrase = optarg ;
                break;
            case 'n' : 
                encryption = 1 ; 
                break;
            case 'o' : 
                outputfile = optarg ;
                break;
            default:
                abort();    
        }
    }
    // doing some checks
    if(decode == 0 && encode == 0){
        std::cout << "only include one argument : -e or -d" << std::endl ; 
    }else if(encode == 0){
        embed(coverfile,embedingfile,passphrase,outputfile,encryption);
    }else if(decode == 0){
        extract(stegofile,passphrase,outputfile,encryption);
    }else{
        std::cout << "No mode selected use -h to see the help menu" << std::endl ;
        exit(0);
    }
}


void help(){
    std::cout << "Stegreg 1.0\n\na tool to hide data in files or extract data from files\n1. Mode selection\n -e                    encode file\n -d                    decode file\n -f <name_file>        check if data is embeded in the file\n\n1.2 Embeding files\n*Make sure that the cover file is bigger than the embeding file*\n -c <cover_file>       overfile path\n -b <embeding_file>    embeding file path\n\n1.2 Extracting data\n -s <stego_file>       stego file path\n\n2. Optional\n -o <output>           output file name when extracted (default:output)\n -h                    print this message and exit\n -p <passphrase>       set a paraphrase for the encryption of the data\n -n                    embeding the file without encryption\n -v                    display stegreg version and exit\n\n" << std::endl ;
    exit(0);
}