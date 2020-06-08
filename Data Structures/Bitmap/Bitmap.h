#define BIT_SET(set, index) ((set)[(index)>>5] |= (1 << ((index)&31)))
#define BIT_IS_SET(set, index) ((set)[(index)>>5] & (1 << ((index)&31)))
#define BIT_OFF(set, index) ((set)[(index)>>5] &= ~(1 << ((index)&31)))
#define BIT_ZERO(set) \
	do {for(int i = 0; i < 4; i++) (set)[i] = 0;} while(0)
#define DIV_ROUND_UP(n, d) (((n) + (d) -1) / (d))
#define BITSET_DECLARE(set, bits) unsigned int set[DIV_ROUND_UP(bits, 32)]
