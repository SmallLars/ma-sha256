#ifndef __SHA256TOOLS_H__
#define __SHA256TOOLS_H__

#include <stdint.h>

void sha256_padding(uint32_t target[16], const char* input);

#endif //__SHA256TOOLS_H__
