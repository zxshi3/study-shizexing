#include <cstdint>
#include <iostream>

// http://esr.ibiblio.org/?p=5095
// http://stackoverflow.com/questions/6212951/endianness-of-android-ndk
// http://en.wikipedia.org/wiki/C_syntax
// \0 = 0
// \xff = 0xff
//#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)
// "\xff" has two character, the first is 0xff, the second is 0
#define IS_LITTLE_ENDIAN (*(uint16_t*)"\x01" == 0x01)
// if is big endian (*(uint16_t*)"\xff" == 0xff00)

enum {
  ENDIAN_UNKNOWN,
  ENDIAN_BIG,
  ENDIAN_LITTLE,
  ENDIAN_BIG_WORD,   /* Middle-endian, Honeywell 316 style */
  ENDIAN_LITTLE_WORD /* Middle-endian, PDP-11 style */
};

int endianness(void)
{
  uint32_t value;
  uint8_t *buffer = (uint8_t *)&value;

  buffer[0] = 0x00;
  buffer[1] = 0x01;
  buffer[2] = 0x02;
  buffer[3] = 0x03;

  switch (value)
  {
  case UINT32_C(0x00010203): return ENDIAN_BIG;
  case UINT32_C(0x03020100): return ENDIAN_LITTLE;
  case UINT32_C(0x02030001): return ENDIAN_BIG_WORD;
  case UINT32_C(0x01000302): return ENDIAN_LITTLE_WORD;
  default:                   return ENDIAN_UNKNOWN;
  }
}

int main(int argc, char** argv) {
	const char * s = "\xff";
	const char * y = "abc";
	std::cout << (void*)s << std::endl;
	std::cout << (uint16_t)*s << ',' << (uint16_t)*(s+1) << ',' << (uint16_t)*(s+2) << ',' << (uint16_t)*(s+3) << ',' << (uint16_t)*(s+4) << std::endl;
	const uint16_t * p = (uint16_t *)"\xff";
	std::cout << p << std::endl;
	uint16_t n = *(uint16_t *)"\xff";
	std::cout << "n = 0x" << std::hex << n << std::dec << std::endl;
	std::cout << IS_LITTLE_ENDIAN << std::endl;
    int num = 1;
    if(*(char *)&num == 1)
    {
        std::cout << "\nLittle-Endian\n";
    }
    else
    {
        std::cout << "Big-Endian\n";
    }
	return IS_LITTLE_ENDIAN;
}