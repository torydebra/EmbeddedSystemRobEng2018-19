#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.

void writeCharLCD(char word);
void writeStringLCD(char* str);
void moveCursor(int row, int column);
void setupLCD();
void clearLCD(short int row, short int startCol);

#endif	/* LCD_H */

