#include <stdint.h>

typedef unsigned char bit;

// clang-format off
struct __attribute__((packed)) FaceBits
{

    /*
    
    The order of this sturcture is the order that the bytes will be written out to the shift registers,
    reordering this structure will re-order the led outputs of the shift registers.
    
    */

    bit _11_4 : 1; bit _11_3 : 1; bit _11_2 : 1; bit _11_1 : 1; bit _11_0 : 1;    
    bit _10_4 : 1; bit _10_3 : 1; bit _10_2 : 1; bit _10_1 : 1; bit _10_0 : 1;    
    bit _9_4 : 1; bit _9_3 : 1; bit _9_2 : 1; bit _9_1 : 1; bit _9_0 : 1;        
    bit _8_4 : 1; bit _8_3 : 1; bit _8_2 : 1; bit _8_1 : 1; bit _8_0 : 1;        
    bit _7_4 : 1; bit _7_3 : 1; bit _7_2 : 1; bit _7_1 : 1; bit _7_0 : 1;        
    bit _6_4 : 1; bit _6_3 : 1; bit _6_2 : 1; bit _6_1 : 1; bit _6_0 : 1;        
    bit _5_4 : 1; bit _5_3 : 1; bit _5_2 : 1; bit _5_1 : 1; bit _5_0 : 1;        
    bit _4_4 : 1; bit _4_3 : 1; bit _4_2 : 1; bit _4_1 : 1; bit _4_0 : 1;        
    bit _3_4 : 1; bit _3_3 : 1; bit _3_2 : 1; bit _3_1 : 1; bit _3_0 : 1;        
    bit _2_4 : 1; bit _2_3 : 1; bit _2_2 : 1; bit _2_1 : 1; bit _2_0 : 1;        
    bit _1_4 : 1; bit _1_3 : 1; bit _1_2 : 1; bit _1_1 : 1; bit _1_0 : 1;        
    bit _12_4 : 1; bit _12_3 : 1; bit _12_2 : 1; bit _12_1 : 1; bit _12_0 : 1;

    /* hr leds are wired first*/
    bit _11 : 1;
    bit _10 : 1;
    bit _9 : 1;
    bit _8 : 1; // 2.1
    bit _7 : 1;
    bit _6 : 1;
    bit _5 : 1;
    bit _4 : 1;
    bit _3 : 1;  // 1.4
    bit _2 : 1;  // 1.3
    bit _1 : 1;  // 1.2
    bit _12 : 1; // 1.1
    
};
// clang-format on

typedef union UFaceBits
{
    struct FaceBits face;
    unsigned char bytes[sizeof(struct FaceBits)];
} UFaceBits;

void test_bitshifting_facebits()
{
    union UFaceBits state = {0};

    state.face._12 = 1;
    state.face._11_4 = 1;

    for (unsigned int b = 0; b < sizeof(struct FaceBits); b++)
        for (unsigned int i = 0; i < 8; i++)
        {
            int bit = (state.bytes[b] >> i) & 1;
            printf("[%3d]:%1d\n", b * 8 + i, bit);
        }
}