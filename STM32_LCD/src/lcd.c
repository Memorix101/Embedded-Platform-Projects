
#include "lcd.h"
#include "stm32f4xx_hal.h"

// LCD 1602
/*
https://www.diyengineers.com/2021/01/07/learn-how-to-use-the-1602-lcd-with-arduino/
https://cdn.sparkfun.com/assets/9/5/f/7/b/HD44780.pdf

1000 0000
High/Left Nibble: Command Bit (Set DDRAM Address) -> Display Line x
Low/Right Nibble: DDRAM Address (0-127) -> Cell Position (Position 0-15)

Code based on https://labprojectsbd.com/2023/03/25/how-to-interface-a-16x2-lcd-with-an-stm32/

Command	                Hex	        Binary	    Function
Clear Display	        0x01	    0000 0001	Clear Display
Return Home	            0x02	    0000 0010	Cursor to (0,0)
Entry Mode	            0x04-0x07	0000 01xx	Cursor Direction
Display Control	        0x08-0x0F	0000 1xxx	Display On/Off, Cursor
Cursor/Display Shift	0x10-0x1F	0001 xxxx	Cursor/Display Shift
Function Set	        0x20-0x3F	001x xxxx	Interface, Lines, Font
Set CGRAM	            0x40-0x7F	01xx xxxx	Custom Character
Set DDRAM	            0x80-0xFF	1xxx xxxx	Cursor-Position

*/

void LCD_Init(void)
{
    LCD_Send(0x02, LCD_COMMAND); // 0000 0010 - Return Home
    LCD_Send(0x28, LCD_COMMAND); // 0010 1000 - Function set: 4-bit, 2 lines, 5x8 dots
    LCD_Send(0x0C, LCD_COMMAND); // 0000 1100 - Display ON, Cursor OFF
    LCD_Send(0x06, LCD_COMMAND); // 0000 0110 - Entry mode set: Increment cursor, no shift
}

void LCD_Send(uint8_t data, LCD_RS_t arg)
{
    switch (arg)
    {
    case LCD_DATA:
        HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
        break;

    case LCD_COMMAND:
        HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
        break;

    default:
        break;
    }

    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (data >> 4) & 1);
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (data >> 5) & 1);
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (data >> 6) & 1);
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (data >> 7) & 1);
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, data & 1);
    HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (data >> 1) & 1);
    HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (data >> 2) & 1);
    HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (data >> 3) & 1);
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t address = 0;

    if (row == 1)
        address = 0x80; // First line at 0x80
    else if (row == 2)
        address = 0xC0; // Second line at 0xC0

    address += (col - 1); // Add column (1-based index)
    LCD_Send(address, LCD_COMMAND);
}

void LCD_GotoXY(uint8_t x, uint8_t y)
{
    uint8_t address = 0;

    switch (y)
    {
    case 0:
        address = 0x80; // First line at 0x80
        break;
    case 1:
        address = 0xC0; // Second line at 0xC0
        break;
    default:
        address = 0x80; // Standard: First line 1
        break;
    }

    address += x;
    LCD_Send(address, LCD_COMMAND);
}

void LCD_WriteChar(char c)
{
    LCD_Send(c, LCD_DATA);
    HAL_Delay(1);
}

void LCD_WriteLine(char *str, LCD_LINE_LENGTH_t lineCharacters)
{
    uint8_t maxLength = lineCharacters;
    uint8_t len = strlen(str);

    LCD_Write(str);

    for (uint8_t i = len; i < maxLength; i++)
    {
        LCD_WriteChar(' ');
    }
}

void LCD_WriteAtPosition(uint8_t row, uint8_t col, char *str, uint8_t fieldWidth)
{
    LCD_SetCursor(row, col);

    uint8_t len = strlen(str);

    LCD_Write(str);

    for (uint8_t i = len; i < fieldWidth; i++)
    {
        LCD_WriteChar(' ');
    }
}

void LCD_Clear(void)
{
    LCD_Send(0x01, LCD_COMMAND);
    HAL_Delay(2);
}

void LCD_Write(char *str)
{
    while (*str)
    {
        LCD_Send(*str++, LCD_DATA);
        HAL_Delay(1);
    }
}