#ifndef GENERAL_HPP  
#define GENERAL_HPP

int filesize(char* filename);
std::string spliter(unsigned char *encrypted, int offset, int bytes);
void removeChar(char *str, char garbage);
bool is_png(char *filename);
void exist(char *filename);

#endif