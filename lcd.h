#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

//############################
//####                    ####
//####  Port declaration  ####
//####                    ####
//############################

#define LCD_PORT GPIOE
#define LCD_RS_BIT XY
#define LCD_RW_BIT XY
#define LCD_E BIT XY
#define LCD_DATA_BITS (X | Y | Z)

//############################

void lcd_init();
void lcd_clrscr();
void lcd_gotoxy(int x, int y);
void lcd_puts(char *s);
void lcd_putc(char c);
void lcd_write_byte(unsigned char b);

#endif