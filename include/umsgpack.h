/*
 * umsgpack.h: MessagePack for MCUs
 * ================================
 *
 *  The MIT License (MIT)
 *
 *  Copyright (c) 2015-2016 Rogier Lodewijks
 *  Copyright (c) 2015-2016 ryochack
 *  Copyright (c) 2015-2016 Takeshi HASEGAWA <hasegaw@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *
 */

#pragma once
#include <stdint.h>

#if defined(__ARM_ARCH_7EM__)  // Covers both Cortex-M7 and M4
/* Common definitions for both cores */
#define UMSGPACK_HW_LITTLE_ENDIAN 1          // Both M7 and M4 are little endian
#define UMSGPACK_FUNC_INT32 1                // Both support 32-bit integers
#define UMSGPACK_INT_WIDTH_32 1              // Native int width is 32-bit
#define UMSGPACK_FUNC_INT16 1                // Enable 16-bit support

// Add compiler intrinsics for byte swapping
#if defined(__GNUC__)
#define _bswap_16(x) __builtin_bswap16(x)
#define _bswap_32(x) __builtin_bswap32(x)
#define _bswap_64(x) __builtin_bswap64(x)
#endif

/* Cortex-M7 specific features */
#if __CORTEX_M == 7
#define UMSGPACK_HW_FLOAT_IEEE754COMPLIANT 1  // FPv5-D16 FPU (double precision)
#define UMSGPACK_FUNC_INT64 1                 // Enable 64-bit int support
#define UMSGPACK_DOUBLE_PRECISION 1           // Enable double precision

/* Cortex-M4 specific features */
#elif __CORTEX_M == 4
#define UMSGPACK_HW_FLOAT_IEEE754COMPLIANT 1  // FPv4-SP FPU (single precision only)
#define UMSGPACK_FUNC_INT64 0                 // Disable 64-bit int support
#endif

/* Memory optimization hints based on i.MX RT1170 architecture */
#define UMSGPACK_OPTIMIZE_TCM 1               // Enable TCM optimization if needed
#define UMSGPACK_CACHE_LINE_SIZE 32           // L1 cache line size

#endif /* __ARM_ARCH_7EM__ */

struct umsgpack_packer_buf {
    unsigned int length;
    unsigned int pos;
    unsigned char data[];
};

#define umsgpack_get_length(buf) buf->pos

#ifdef __cplusplus
extern "C" {
#endif

int umsgpack_pack_array(struct umsgpack_packer_buf *, int);
int umsgpack_pack_uint(struct umsgpack_packer_buf *, unsigned int);
int umsgpack_pack_int(struct umsgpack_packer_buf *, int);

// Always enable INT16 functions since they're needed internally
int umsgpack_pack_uint16(struct umsgpack_packer_buf *, uint16_t);
int umsgpack_pack_int16(struct umsgpack_packer_buf *, int16_t);

#ifdef UMSGPACK_FUNC_INT32
int umsgpack_pack_uint32(struct umsgpack_packer_buf *, uint32_t);
int umsgpack_pack_int32(struct umsgpack_packer_buf *, int32_t);
#endif

#ifdef UMSGPACK_FUNC_INT64
int umsgpack_pack_uint64(struct umsgpack_packer_buf *, uint64_t);
int umsgpack_pack_int64(struct umsgpack_packer_buf *, int64_t);
#endif

int umsgpack_pack_float(struct umsgpack_packer_buf *, float);
int umsgpack_pack_map(struct umsgpack_packer_buf *, uint32_t);
int umsgpack_pack_str(struct umsgpack_packer_buf *, const char *, uint32_t);
int umsgpack_pack_bool(struct umsgpack_packer_buf *, int);
int umsgpack_pack_nil(struct umsgpack_packer_buf *);
void umsgpack_packer_init(struct umsgpack_packer_buf *, size_t);
struct umsgpack_packer_buf *umsgpack_alloc(size_t);
int umsgpack_free(struct umsgpack_packer_buf *);

#ifdef __cplusplus
}
#endif