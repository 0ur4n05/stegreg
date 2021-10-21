# Stegreg
![STEGREG_LOGO](https://i.ibb.co/FXzykfH/stegreglogo.png)
## Introduction 
Stegreg is a steganography tool made in C++ that encrypt and hide your data inside an image.

## Installation
```
git clone https://github.com/0UR4N05/stegreg.git
cd stegreg/
make
make install
```

## Usage 
Hide a message inside the image 
```
    ./stegreg -e -c <coverfile> -b <file to hide>
```
Read a message inside an image
```
    ./stegreg -d -s <stegfile>
```

More help? 
```
    ./stegreg -h
```

## File formats? 
Stegreg now is supporting these file formats :

File type     | Short description                                                            | 
------------- | ---------------------------------------------------------------------------- |
Image(jpg)    | Data are randomely hidden between the bytes of the image                     |
Image(png)    | png images are really sensitive, data will be hidden in the end ot the file  |


## Change log 


## Issues
Bug reports are welcome! Please report all bugs on the issue tracker or send them to devalfo@protonmail.com. 

Thank you.