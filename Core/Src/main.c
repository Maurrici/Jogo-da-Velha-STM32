/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OledGrph.h"
#include "OledDriver.h"
#include <stdio.h>
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	// reverses a string 'str' of length 'len'
	void reverse(char *str, int len)
	{
		int i=0, j=len-1, temp;
		while (i<j)
		{
			temp = str[i];
			str[i] = str[j];
			str[j] = temp;
			i++; j--;
		}
	}

	 // Converts a given integer x to string str[].  d is the number
	 // of digits required in output. If d is more than the number
	 // of digits in x, then 0s are added at the beginning.
	int intToStr(int x, char str[], int d)
	{
		int i = 0;
		while (x)
		{
			str[i++] = (x%10) + '0';
			x = x/10;
		}

		// If number of digits required is more, then
		// add 0s at the beginning
		while (i < d)
			str[i++] = '0';

		reverse(str, i);
		str[i] = '\0';
		return i;
	}

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  	float PX = 0;
	float PO = 0;
	int turn = 1; // Turn 1 representa vez do PX = X | Turn 2 representa vez do PO = O

	char board[3][3];

	void renderBoard(){
		for(int i=0; i < 3; i++){
			OledSetCursor(2,i+1);
			for(int j=0; j < 3; j++){
				if(board[i][j] == ' ' && i != 2) OledPutChar('_');
				else if(board[i][j] == ' ' && i == 2) OledPutChar(' ');
				else OledPutChar(board[i][j]);

				if(j != 2) OledPutString("|");
			}
		}
	}

	void renderScoreBoard(){
		// Placar Inicial
		OledSetCursor(8, 0);
		OledPutChar('|');
		OledSetCursor(8, 1);
		OledPutChar('|');
		OledSetCursor(8, 2);
		OledPutChar('|');
		OledSetCursor(8, 3);
		OledPutChar('|');

		OledSetCursor(10, 0);
		char text[5] = {' ','P','X',':'};
		char number[1];

		if(turn == 1){
			text[0] = '>';
		}
		intToStr(PX, number, 1);
		text[4] = number[0];
		OledPutString(text);

		OledSetCursor(10, 2);
		char text2[5] = {' ','P','O',':'};
		char number2[1];
		if(turn == 2){
			text2[0] = '>';
		}
		intToStr(PO, number2, 1);
		text2[4] = number2[0];
		OledPutString(text2);
	}

	void startGame(){
		// Placar
		renderScoreBoard();

		// Tabuleiro inicial
		for(int i=0; i < 3; i++){
			for(int j=0; j < 3; j++){
				board[i][j] = ' ';
			}
		}
		renderBoard();
	}

	void mensagemVencedor(char v) {
		OledClear();

		if(v != ' '){
			char text[11] = {"P  VENCEU!!"};
			OledSetCursor(3, 1);
			text[1] = v;
			OledPutString(text);
			HAL_Delay(3000);
		}else{
			char text[8] = {"EMPATE!!"};
			OledSetCursor(5, 1);
			OledPutString(text);
			HAL_Delay(3000);
		}

		OledClear();
	}

	void verificarVitoria(){
		for(int i=0; i<3; i++){
			if(board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' '){
				if(board[i][0] == 'X'){
					PX++;
				}else{
					PO++;
				}
				mensagemVencedor(board[i][0]);
				startGame();
			}else if(board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' '){
				if(board[0][i] == 'X'){
					PX++;
				}else{
					PO++;
				}
				mensagemVencedor(board[0][i]);
				startGame();
			}else if(board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' '){
				if(board[0][0] == 'X'){
					PX++;
				}else{
					PO++;
				}
				mensagemVencedor(board[0][0]);
				startGame();
			}else if(board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' '){
				if(board[0][2] == 'X'){
					PX++;
				}else{
					PO++;
				}
				mensagemVencedor(board[0][2]);
				startGame();
			}
		}

		int aux = 0;
		for(int i=0; i<3; i++){
			for(int j=0; j<3; j++){
				if(board[i][j] != ' '){
					aux++;
				}
			}
		}

		if(aux == 9){
			mensagemVencedor(' ');
			startGame();
		}
	}
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  OledInit(); // Inicializa o oLed
  OledClear(); //- Apaga todos os dados  do oLed
  startGame();
  while (1)
  {
	  for (int i = 0; i < 3; i++) {
		// Reseta todos os pinos da linha do teclado
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

		// Define a linha que estamos realizando a varredura
		int row = i;
		if (row == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);
		else if (row == 1) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
		else if (row == 2) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

		// Define a coluna pressionada no teclado
		int col = -1;
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) == 1) col = 0;
		else if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10) == 1) col = 1;
		else if (HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_1) == 1) col = 2;

		if(col != -1){
			if(turn == 1 && board[row][col] == ' '){
				board[row][col] = 'X';
				turn = 2;
			}
			else if(board[row][col] == ' '){
				board[row][col] = 'O';
				turn = 1;
			}
			renderBoard();
			renderScoreBoard();
			verificarVitoria();
		}
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

  /*Configure GPIO pin : PF1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
