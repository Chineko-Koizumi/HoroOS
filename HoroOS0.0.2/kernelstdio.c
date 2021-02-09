
#include "kernelstdio.h"

const size_t VGA_WIDTH = 80;
const size_t VGA_HEIGHT = 25;

char terminal_color_itterator = 0;

size_t 		terminal_row;
size_t 		terminal_column;
uint8_t 	terminal_color;
uint16_t* 	terminal_buffer;

inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) 
{
	return fg | bg << 4;
}
 
inline uint16_t vga_entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void strrev(char* str, uint32_t length)
{
	uint32_t start = 0;
	uint32_t end = length - 1;
	
	while(start<end)
	{
		char temp= *(str+end);
		*(str+end) = *(str+start);
		*(str+start) = temp;
		start++;
		end--;
	}
}
 
void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
	printf("Terminal ready.\n");
}

void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}
 
void terminal_clear()
{
	 for (size_t y = 0; y < VGA_HEIGHT; y++) 
	 {
		for (size_t x = 0; x < VGA_WIDTH; x++) 
		{
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}
 
void terminal_scroll_up()
{
	for(size_t i = VGA_WIDTH; i < VGA_WIDTH*VGA_HEIGHT; ++i)
	{
		terminal_buffer[i-VGA_WIDTH]= terminal_buffer[i];
	}	
	 
	for(size_t i = VGA_WIDTH*VGA_HEIGHT - VGA_WIDTH; i < VGA_WIDTH*VGA_HEIGHT; ++i)
	{
		terminal_buffer[i] = vga_entry(' ', terminal_color);
	}
}
 
void terminal_putchar(char c) 
{
	switch(c)
	{
		case '\n':
		{
			terminal_putentryat('\0', terminal_color, terminal_column, terminal_row);
			terminal_column = 0;
			
			if (++terminal_row == VGA_HEIGHT)
			{
				terminal_row = VGA_HEIGHT-1;
				terminal_scroll_up();
			}
			
		}return;
		
		default:
		{
			terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
			if (++terminal_column == VGA_WIDTH) 
			{
				terminal_column = 0;
				if (++terminal_row == VGA_HEIGHT)
				{
					terminal_row = VGA_HEIGHT-1;
					terminal_scroll_up();
				}
			}
		}return;
	}
	
}
 
void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}
 
void printf(const char* data) 
{
	terminal_write(data, strlen(data));
}

char* itoa(int32_t num, char* str, uint8_t base) 
{ 
    int32_t i = 0;  
	bool isNegative = false;
	 
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
	
	if (num < 0 && base == 10)
	{
		isNegative = true;
		num = -num;
	}
	
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    // Process individual digits 
    while (num != 0) 
    { 
        int32_t rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
    
    if(isNegative) str[i++] = '-';
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    strrev(str, i); 
  
    return str; 
}  
