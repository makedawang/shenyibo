#ifndef MD5_H
#define MD5_H

#include <iostream>
#include <string>
#include <cstring>
using namespace std;

typedef unsigned int bit32;

void MD5Hash(string input, bit32 *state);
void md5_serial(const char* input, unsigned char* output);
void md5_neon(const char* inputs[4], unsigned char outputs[4][16]);

#endif