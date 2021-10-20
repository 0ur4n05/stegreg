// Copyright by Jackie 2014.10.23
// Descriptin : MD5 Algorithm Implementation
// MD5.cpp
#include "MD5.h"

// Number used while transforming
// in functions FF, GG, HH, II
#define S11 7
#define S12 12
#define S13 17
#define S14 22
#define S21 5
#define S22 9
#define S23 14
#define S24 20
#define S31 4
#define S32 11
#define S33 16
#define S34 23
#define S41 6
#define S42 10
#define S43 15
#define S44 21

// Defualt Constructor, just initalize
MD5::MD5() {
    init();
}

// Constructor with string parameter and transform immediately
MD5::MD5(const std::string& str) {
    init();
    update(str.c_str(), str.length());
    finalize();
}

// MD5 block update operation. Continues an MD5 message-digest
// operation, processing another message block
void MD5::update(const uChar input[], uInt length) {
    // Compute number of bytes mod 64
    uInt index = count[0] / 8 % blockSize;
    // Update number of bits
    if ((count[0] += (length << 3)) < (length << 3)) {
        count[1]++;
    }
    count[1] += (length >> 29);
    // Number of bytes we need to fill in buffer
    uInt firstPart = 64 - index, i;
    // Transform as many times as possible
    if (length >= firstPart) {
        // Fill buffer first, transform
        memcpy(&buffer[index], input, firstPart);
        transform(buffer);
        // Transform chunks of blockSize(64)
        for (i = firstPart; i + blockSize <= length ; i += blockSize) {
            transform(&input[i]);
        }
        index = 0;
    } else {
        i = 0;
    }
    // Buffer remaining input
    memcpy(&buffer[index], &input[i], length - i);
}

 
// MD5 block update operation with unsigned char
void MD5::update(const char input[], uInt length) {
    update((const uChar*)input, length);
}

// MD5 finalization. Ends an MD5 message-digest operation,
// writing the message digest and zeroizing the context
MD5& MD5::finalize() {
    static uChar padding[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    if (!finish) {
        // Save number of bits
        uChar bits[8];
        encode(bits, count, 8);

        // Pad out to 56 mod 64
        uInt index = count[0] / 8 % 64;
        uInt padLength = (index < 56) ? (56 - index) : (120 - index);
        update(padding, padLength);

        // Append length (before padding)
        update(bits, 8);
        // Store state in result
        encode(result, state, 16);

        // Zeroize sensitive information
        memset(buffer, 0, sizeof(buffer));
        memset(count, 0, sizeof(count));
        finish = true;
    }
    return *this;
}

// Return hex representation of digest as string
std::string MD5::hexdigest() {
    if (!finish) {
        return "";
    }
    char temp[33] = {0};
    for (int i = 0; i < 16; i++) {
        sprintf(temp + i * 2, "%02x", result[i]);
    }
    return std::string(temp);
}

// Ouput stream 
std::ostream& operator << (std::ostream& out, MD5 md5) {
    return out << md5.hexdigest();
}

// Initialization
void MD5::init() {
    finish = false;
    count[0] = count[1] = 0;
    // Magic number
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}

// MD5 transform operation.
void MD5::transform(const uChar block[blockSize]) {
    uInt a = state[0], b = state[1], c = state[2], d = state[3], x[16];
    decode (x, block, blockSize);

    // Round One
    FF (a, b, c, d, x[ 0], S11, 0xd76aa478);
    FF (d, a, b, c, x[ 1], S12, 0xe8c7b756);
    FF (c, d, a, b, x[ 2], S13, 0x242070db);
    FF (b, c, d, a, x[ 3], S14, 0xc1bdceee);
    FF (a, b, c, d, x[ 4], S11, 0xf57c0faf);
    FF (d, a, b, c, x[ 5], S12, 0x4787c62a);
    FF (c, d, a, b, x[ 6], S13, 0xa8304613);
    FF (b, c, d, a, x[ 7], S14, 0xfd469501);
    FF (a, b, c, d, x[ 8], S11, 0x698098d8);
    FF (d, a, b, c, x[ 9], S12, 0x8b44f7af);
    FF (c, d, a, b, x[10], S13, 0xffff5bb1);
    FF (b, c, d, a, x[11], S14, 0x895cd7be);
    FF (a, b, c, d, x[12], S11, 0x6b901122);
    FF (d, a, b, c, x[13], S12, 0xfd987193);
    FF (c, d, a, b, x[14], S13, 0xa679438e);
    FF (b, c, d, a, x[15], S14, 0x49b40821);

    // Round Two
    GG (a, b, c, d, x[ 1], S21, 0xf61e2562);
    GG (d, a, b, c, x[ 6], S22, 0xc040b340);
    GG (c, d, a, b, x[11], S23, 0x265e5a51);
    GG (b, c, d, a, x[ 0], S24, 0xe9b6c7aa);
    GG (a, b, c, d, x[ 5], S21, 0xd62f105d);
    GG (d, a, b, c, x[10], S22,  0x2441453);
    GG (c, d, a, b, x[15], S23, 0xd8a1e681);
    GG (b, c, d, a, x[ 4], S24, 0xe7d3fbc8);
    GG (a, b, c, d, x[ 9], S21, 0x21e1cde6);
    GG (d, a, b, c, x[14], S22, 0xc33707d6);
    GG (c, d, a, b, x[ 3], S23, 0xf4d50d87);
    GG (b, c, d, a, x[ 8], S24, 0x455a14ed);
    GG (a, b, c, d, x[13], S21, 0xa9e3e905);
    GG (d, a, b, c, x[ 2], S22, 0xfcefa3f8);
    GG (c, d, a, b, x[ 7], S23, 0x676f02d9);
    GG (b, c, d, a, x[12], S24, 0x8d2a4c8a);

    // Round Three
    HH (a, b, c, d, x[ 5], S31, 0xfffa3942);
    HH (d, a, b, c, x[ 8], S32, 0x8771f681);
    HH (c, d, a, b, x[11], S33, 0x6d9d6122);
    HH (b, c, d, a, x[14], S34, 0xfde5380c);
    HH (a, b, c, d, x[ 1], S31, 0xa4beea44);
    HH (d, a, b, c, x[ 4], S32, 0x4bdecfa9);
    HH (c, d, a, b, x[ 7], S33, 0xf6bb4b60);
    HH (b, c, d, a, x[10], S34, 0xbebfbc70);
    HH (a, b, c, d, x[13], S31, 0x289b7ec6);
    HH (d, a, b, c, x[ 0], S32, 0xeaa127fa);
    HH (c, d, a, b, x[ 3], S33, 0xd4ef3085);
    HH (b, c, d, a, x[ 6], S34,  0x4881d05);
    HH (a, b, c, d, x[ 9], S31, 0xd9d4d039);
    HH (d, a, b, c, x[12], S32, 0xe6db99e5);
    HH (c, d, a, b, x[15], S33, 0x1fa27cf8);
    HH (b, c, d, a, x[ 2], S34, 0xc4ac5665);

    // Round Four
    II (a, b, c, d, x[ 0], S41, 0xf4292244);
    II (d, a, b, c, x[ 7], S42, 0x432aff97);
    II (c, d, a, b, x[14], S43, 0xab9423a7);
    II (b, c, d, a, x[ 5], S44, 0xfc93a039);
    II (a, b, c, d, x[12], S41, 0x655b59c3);
    II (d, a, b, c, x[ 3], S42, 0x8f0ccc92);
    II (c, d, a, b, x[10], S43, 0xffeff47d);
    II (b, c, d, a, x[ 1], S44, 0x85845dd1);
    II (a, b, c, d, x[ 8], S41, 0x6fa87e4f);
    II (d, a, b, c, x[15], S42, 0xfe2ce6e0);
    II (c, d, a, b, x[ 6], S43, 0xa3014314);
    II (b, c, d, a, x[13], S44, 0x4e0811a1);
    II (a, b, c, d, x[ 4], S41, 0xf7537e82);
    II (d, a, b, c, x[11], S42, 0xbd3af235);
    II (c, d, a, b, x[ 2], S43, 0x2ad7d2bb);
    II (b, c, d, a, x[ 9], S44, 0xeb86d391);

    // Add back to the A.B.C.D
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    // Zeroize sensitive information
    memset(x, 0, sizeof x);
}

// Decodes uChar into uInt. Assumes length is a multiple of 4
void MD5::decode(uInt output[], const uChar input[], uInt length) {
    for (uInt i = 0, j = 0; j < length; i++, j += 4) {
        output[i] = (uInt)input[j] | ((uInt)input[j + 1] << 8) |
                    ((uInt)input[j + 2] << 16) | ((uInt)input[j + 3] << 24);
    }
}

// Encodes uInt into uChar. Assumes length is a multiple of 4
void MD5::encode(uChar output[], const uInt input[], uInt length) {
    for (uInt i = 0, j = 0; j < length; i++, j += 4) {
        output[j] = input[i] & 0xff;
        output[j + 1] = (input[i] >> 8) & 0xff;
        output[j + 2] = (input[i] >> 16) & 0xff;
        output[j + 3] = (input[i] >> 24) & 0xff;
    }
}

// F, G， H，I are the basic MD5 functions
inline uInt F(uInt x, uInt y, uInt z) {
    return x & y | ~x & z;
}

inline uInt G(uInt x, uInt y, uInt z) {
    return x & z | y & ~z;
}

inline uInt H(uInt x, uInt y, uInt z) {
    return x ^ y ^ z;
}

inline uInt I(uInt x, uInt y, uInt z) {
    return y ^ (x | ~z);
}

// Rotates x left n bits
inline uInt rotateLeft(uInt x, int n) {
    return (x << n) | (x >> (32 - n));
}

// FF, GG, HH, II transformations for rounds 1, 2, 3, 4.
inline void FF(uInt &a, uInt b, uInt c, uInt d, uInt x, uInt s, uInt ac) {
    a = rotateLeft(a + F(b, c, d) + x + ac, s) + b;
}

inline void GG(uInt &a, uInt b, uInt c, uInt d, uInt x, uInt s, uInt ac) {
    a = rotateLeft(a + G(b, c, d) + x + ac, s) + b;
}

inline void HH(uInt &a, uInt b, uInt c, uInt d, uInt x, uInt s, uInt ac) {
    a = rotateLeft(a + H(b, c, d) + x + ac, s) + b;
}

inline void II(uInt &a, uInt b, uInt c, uInt d, uInt x, uInt s, uInt ac) {
    a = rotateLeft(a + I(b, c, d) + x + ac, s) + b;
}

// md5 function use the parameter to construct a MD5 object
// and return the MD5 value
std::string md5(const std::string str) {
    return MD5(str).hexdigest();
}
