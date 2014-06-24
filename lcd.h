#ifndef LCD_H_INCLUDED
#define LCD_H_INCLUDED

//############################
//####                    ####
//####  Port declaration  ####
//####                    ####
//############################

//TODO

//############################

void lcd_init();
void lcd_clrscr();
void lcd_gotoxy(int x, int y);
void lcd_puts(char *s);
void lcd_putc(char c);

#endif