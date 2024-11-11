#ifndef SHA256_H
#define SHA256_H

#include "kernel/types.h"
//#include "user/user.h"
#include <stddef.h>

// Type definitions
typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// SHA256 constants and macros
#define ROTRIGHT(word, bits) (((word) >> (bits)) | ((word) << (32 - (bits))))
#define SSIG0(x) (ROTRIGHT(x, 7) ^ ROTRIGHT(x, 18) ^ ((x) >> 3))
#define SSIG1(x) (ROTRIGHT(x, 17) ^ ROTRIGHT(x, 19) ^ ((x) >> 10))
#define CH(x, y, z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x, y, z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x, 2) ^ ROTRIGHT(x, 13) ^ ROTRIGHT(x, 22))
#define EP1(x) (ROTRIGHT(x, 6) ^ ROTRIGHT(x, 11) ^ ROTRIGHT(x, 25))

// SHA256 function declarations
void sha256_transform(uint32_t *state, const uint8_t data[]);
void sha256_init(uint32_t *state);
void sha256_update(uint32_t *state, const uint8_t data[], size_t len, uint8_t *buffer, size_t *buffer_len, uint64_t *bit_len);
void sha256_final(uint32_t *state, uint8_t hash[], uint8_t *buffer, size_t buffer_len, uint64_t bit_len);
uint8_t* sha256(const uint8_t *data, size_t len);

#endif // SHA256_H

