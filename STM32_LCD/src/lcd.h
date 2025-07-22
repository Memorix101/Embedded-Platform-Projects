
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

void LCD_SendCommand(uint8_t command);
void LCD_SendData(uint8_t data);
void LCD_Clear(void);
void LCD_WriteString(char *str);
void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_GotoXY(uint8_t x, uint8_t y);
void LCD_Home(void);
void LCD_WriteChar(char c);
void LCD_WriteStringClear(char* str);
void LCD_WriteStringAtPosition(uint8_t row, uint8_t col, char* str, uint8_t fieldWidth);