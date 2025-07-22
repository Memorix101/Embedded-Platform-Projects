#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"

// forward declaration
void SystemClock_Config(void);

UART_HandleTypeDef huart2;
uint32_t last_toggle_time = 0;
bool led_state = false;
char buffer[32]; // Buffer for LCD display
u_int8_t counter = 0;

int main()
{
  // HAL initialization
  HAL_Init();
  SystemClock_Config();
  UART_Init();

  __HAL_RCC_GPIOA_CLK_ENABLE(); // Enable GPIOA clock
  __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable GPIOB clock
  __HAL_RCC_GPIOC_CLK_ENABLE(); // Enable GPIOC clock

  printf("STM32 Application started\r\n");

  // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = RS_Pin | EN_Pin | D4_Pin | D5_Pin | D6_Pin | D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  LCD_Init();
  LCD_Clear();

  LCD_WriteString("123 Hello World!");
  HAL_Delay(300);

  LCD_SetCursor(2, 1);
  LCD_WriteString("Line two");
  HAL_Delay(300);
  LCD_Clear();
  while (1)
  {
    //LCD_Clear();
    sprintf(buffer, "Counter: %d", counter++);
    LCD_SetCursor(1, 1);
    LCD_WriteStringLine(buffer, LCD_16); // Clear and write string

    LCD_SetCursor(2, 1);
    LCD_WriteString("Line two");

   // HAL_Delay(100); // Update every second
  }
}

void EXTI15_10_IRQHandler(void)
{
  // HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13); // HAL_GPIO_EXTI_Callback

  // Clear the EXTI pending bit
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_13);

  // PC13 (onboard) Interrupt
  if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == RESET)
  {
    // Toggle LED on PA5
    // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    led_state = !led_state; // Toggle state
  }
}

void EXTI1_IRQHandler(void)
{
  // HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13); // HAL_GPIO_EXTI_Callback

  // Clear the EXTI pending bit
  __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_1);

  // PC13 (onboard) Interrupt
  if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == RESET)
  {
    // Toggle LED on PA5
    // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
    led_state = !led_state; // Toggle state
  }
}

/*void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}*/

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Oscillator (HSI) config
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  // Set clock configuration
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI; // 16 MHz HSI
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

void UART_Init(void)
{
  // Enable UART2 and GPIO clocks
  __HAL_RCC_USART2_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Configure GPIO pins for UART2
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // PA2 = TX, PA3 = RX for UART2 on most Nucleo boards
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2; // AF7 for UART2
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Configure UART
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;

  HAL_UART_Init(&huart2);
}

// Implement _write for printf redirection (used by libc)
int _write(int file, char *ptr, int len)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)ptr, len, HAL_MAX_DELAY);
  return len;
}