
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

*/

void LCD_Init(void)
{
    LCD_Send(0x02, LCD_COMMAND);
    LCD_Send(0x28, LCD_COMMAND);
    LCD_Send(0x0C, LCD_COMMAND);
    LCD_Send(0x06, LCD_COMMAND);
}

/*void LCD_SendCommand(uint8_t command)
{
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, (command >> 4) & 1);
  HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (command >> 5) & 1);
  HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (command >> 6) & 1);
  HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (command >> 7) & 1);
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, command & 1);
  HAL_GPIO_WritePin(D5_GPIO_Port, D5_Pin, (command >> 1) & 1);
  HAL_GPIO_WritePin(D6_GPIO_Port, D6_Pin, (command >> 2) & 1);
  HAL_GPIO_WritePin(D7_GPIO_Port, D7_Pin, (command >> 3) & 1);
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
  HAL_Delay(1);
  HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_RESET);
  HAL_Delay(1);
}

void LCD_SendData(uint8_t data)
{
  HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);
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
}*/

void LCD_Send(uint8_t data, LCD_ARG_t arg)
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

void LCD_WriteStringClear(char *str)
{
    uint8_t maxLength = 16; // Assuming a 16x2 LCD
    uint8_t len = strlen(str);

    LCD_WriteString(str);

    for (uint8_t i = len; i < maxLength; i++)
    {
        LCD_WriteChar(' ');
    }
}

void LCD_WriteStringAtPosition(uint8_t row, uint8_t col, char *str, uint8_t fieldWidth)
{
    LCD_SetCursor(row, col);

    uint8_t len = strlen(str);

    LCD_WriteString(str);

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

void LCD_WriteString(char *str)
{
    while (*str)
    {
        LCD_Send(*str++, LCD_DATA);
        HAL_Delay(1);
    }
}