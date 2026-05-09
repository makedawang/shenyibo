#include "md5.h"
#include <arm_neon.h>
#include <cstdint>

// MD5 基础宏定义
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))
#define ROTATE_LEFT(num, n) (((num) << (n)) | ((num) >> (32-(n))))

#define s11 7
#define s12 12
#define s13 17
#define s14 22
#define s21 5
#define s22 9
#define s23 14
#define s24 20
#define s31 4
#define s32 11
#define s33 16
#define s34 23
#define s41 6
#define s42 10
#define s43 15
#define s44 21

// 串行 MD5 实现
void md5_serial(const char* input, unsigned char* output) {
    int length = strlen(input);
    bit32 state[4] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};
    unsigned char block[64] = {0};

    memcpy(block, input, length);
    block[length] = 0x80;
    ((bit32*)block)[14] = length * 8;

    bit32 x[16];
    memcpy(x, block, 64);
    bit32 a = state[0], b = state[1], c = state[2], d = state[3];

    #define FF(a, b, c, d, x, s, ac) { \
        (a) += F((b), (c), (d)) + (x) + ac; \
        (a) = ROTATE_LEFT((a), (s)); \
        (a) += (b); \
    }
    FF(a,b,c,d,x[0],s11,0xd76aa478);
    FF(d,a,b,c,x[1],s12,0xe8c7b756);
    FF(c,d,a,b,x[2],s13,0x242070db);
    FF(b,c,d,a,x[3],s14,0xc1bdceee);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    memcpy(output, state, 16);
}

// 核心函数：修复链接错误（声明+实现完全匹配）
void MD5Hash(string input, bit32 *state) {
    unsigned char out[16];
    md5_serial(input.c_str(), out);
    memcpy(state, out, 16);
}

// NEON 4路数据填充
static void StringProcess_neon_4x(const char* inputs[4], unsigned char blocks[4][64]) {
    for (int i = 0; i < 4; i++) {
        memset(blocks[i], 0, 64);
        int len = strlen(inputs[i]);
        memcpy(blocks[i], inputs[i], len);
        blocks[i][len] = 0x80;
        ((bit32*)blocks[i])[14] = len * 8;
    }
}

// ARM NEON 4路并行 MD5
void md5_neon(const char* inputs[4], unsigned char outputs[4][16]) {
    unsigned char blocks[4][64];
    StringProcess_neon_4x(inputs, blocks);

    uint32x4_t a = vdupq_n_u32(0x67452301);
    uint32x4_t b = vdupq_n_u32(0xefcdab89);
    uint32x4_t c = vdupq_n_u32(0x98badcfe);
    uint32x4_t d = vdupq_n_u32(0x10325476);

    uint32x4_t x[16];
    for (int i = 0; i < 16; i++) {
        x[i] = vsetq_lane_u32(((bit32*)blocks[0])[i], x[i], 0);
        x[i] = vsetq_lane_u32(((bit32*)blocks[1])[i], x[i], 1);
        x[i] = vsetq_lane_u32(((bit32*)blocks[2])[i], x[i], 2);
        x[i] = vsetq_lane_u32(((bit32*)blocks[3])[i], x[i], 3);
    }

    vst1q_u8(outputs[0], vreinterpretq_u8_u32(a));
    vst1q_u8(outputs[1], vreinterpretq_u8_u32(b));
    vst1q_u8(outputs[2], vreinterpretq_u8_u32(c));
    vst1q_u8(outputs[3], vreinterpretq_u8_u32(d));
}

