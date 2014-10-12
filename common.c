#include "inc/common.h"
#include "inc/printf.h"

volatile uint8_t x = 0;
volatile uint8_t y = 0;

#define SCREEN_ATTR 0x0F

static void cursor_set(uint8_t int_x, uint8_t int_y)
{
	uint16_t cursorLocation = int_y * 80 + int_x;
	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
        outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
        outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
        outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

void clear_screen()
{

	for(uint32_t point = 0xB8000; point < 0xB8FA0; point = point + 2)
	{
		*((uint8_t volatile *) (point)) = ' ';
		*((uint8_t volatile *) (point + 1)) = SCREEN_ATTR;
	}

	cursor_set(0, 0);
}

void putc ( void* p __attribute__ ((__unused__)), char c)
{

	if(x == 80 || c == '\n')
	{
		y++;
		x = 0;
	} else {

		if(y >= 24)
		{
			// Scrolla, för bövelen, SCROLLA
			memcpy((void *)0xB8000, (void *)0xB80A0, 80 * 23 * 2);
			for(uint8_t tx = 0; tx < 80; tx++)
			{
				(*(volatile uint8_t volatile *) (tx * 2 + 0xB8E60)) = ' ';
				(*(volatile uint8_t volatile *) (tx * 2 + 0xB8E61)) = SCREEN_ATTR;
			}
			y = 23;
			x = 0;
		}

		(*(volatile uint8_t volatile *) ((y * 80 + x) * 2 + 0xB8000)) = c;
		(*(volatile uint8_t volatile *) ((y * 80 + x) * 2 + 0xB8001)) = SCREEN_ATTR;


		x++;
	}
	cursor_set(x, y);
}

void setup_printf()
{
	init_printf(0, putc);
}

int cmp(char *a, char *b, int length)
{
	int i = 0;
        while(i < length)
        {
		if(*(a + i) != *(b + i))
		{
                	return 0;
		}
                i++;
        }
	return 1;
}

void memset(void *p, uint8_t val, uint32_t len)
{
	for(uint32_t i = 0; i < len; i++)
	{
		((uint8_t *)p)[i] = val;
	}
}

void memcpy(void *p1_tmp, void *p2_tmp, uint32_t len)
{
	uint8_t volatile *p1 = p1_tmp;
	uint8_t volatile *p2 = p2_tmp;
	while(len != 0) {
		len--;
		*p1 = *p2;
		++p1; ++p2;
	}
}
