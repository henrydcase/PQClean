#ifndef PQCLEAN_MCELIECE460896_SSE_UTIL_H
#define PQCLEAN_MCELIECE460896_SSE_UTIL_H
/*
  This file is for loading/storing data in a little-endian fashion
*/


#include "vec128.h"

#include <stdint.h>

void PQCLEAN_MCELIECE460896_SSE_store_i(unsigned char *out, uint64_t in, int i);
void PQCLEAN_MCELIECE460896_SSE_store2(unsigned char *dest, uint16_t a);
uint16_t PQCLEAN_MCELIECE460896_SSE_load2(const unsigned char *src);
uint32_t PQCLEAN_MCELIECE460896_SSE_load4(const unsigned char *src);
void PQCLEAN_MCELIECE460896_SSE_irr_load(vec128 *out, const unsigned char *in);
void PQCLEAN_MCELIECE460896_SSE_store8(unsigned char *out, uint64_t in);
uint64_t PQCLEAN_MCELIECE460896_SSE_load8(const unsigned char *in);
vec128 PQCLEAN_MCELIECE460896_SSE_load16(const unsigned char *in);

void PQCLEAN_MCELIECE460896_SSE_store16(unsigned char *out, vec128 in);

#endif
