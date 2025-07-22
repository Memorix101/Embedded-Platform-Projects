
#include <stdint.h>

#define RS_Pin GPIO_PIN_15
#define RS_GPIO_Port GPIOB
#define EN_Pin GPIO_PIN_14
#define EN_GPIO_Port GPIOB
#define D4_Pin GPIO_PIN_13
#define D4_GPIO_Port GPIOB
#define D5_Pin GPIO_PIN_10
#define D5_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_4
#define D6_GPIO_Port GPIOB
#define D7_Pin GPIO_PIN_5
#define D7_GPIO_Port GPIOB

typedef enum
{
    LCD_DATA,
    LCD_COMMAND
} LCD_RS_t; // RS - Register Select

typedef enum
{
    LCD_8 = 8,
    LCD_16 = 16,
    LCD_40 = 40,
    LCD_80 = 80,
} LCD_LINE_LENGTH_t;

void LCD_Send(uint8_t data, LCD_RS_t arg);
void LCD_Clear(void);
void LCD_WriteString(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_GotoXY(uint8_t x, uint8_t y);
void LCD_Home(void);
void LCD_WriteChar(char c);
void LCD_WriteStringLine(char *str, LCD_LINE_LENGTH_t lineCharacters);
void LCD_WriteStringAtPosition(uint8_t row, uint8_t col, char *str, uint8_t fieldWidth);