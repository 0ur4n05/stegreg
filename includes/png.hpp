#ifndef PNG_HPP  
#define PNG_HPP

void pngembed(char* coverfile, char* embedingfile, char* passphrase, char* outputfile, bool encyrption);
void pngextract(char* stegofile, char* passphrase, char* outputfile, bool encyrption);

#endif