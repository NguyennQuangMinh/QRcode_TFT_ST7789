#ifndef __FONT__H__
#define __FONT__H__
#include <stdint.h>
typedef struct
{
	const uint8_t *font;
	const uint16_t *font_map;
}Font;
extern const Font Font1;
extern const Font Font2;
extern const Font Font3;
extern const Font Font_size8;
//extern const Font tiengviet;

unsigned char read_font(int16_t x, int16_t y,  uint16_t txt,const Font *my_font);
uint16_t UTF8_GetAddr(unsigned char *utf8_char,unsigned char *char_offset);
#endif //__FONT__H__



