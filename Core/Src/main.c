/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "LEDConfig.h"
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
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
uint8_t g_u8LEDCallGreenData = 0;
uint8_t g_u8LEDRespGreenData = 0;
uint8_t g_u8LEDCallRedData = 0;
uint8_t g_u8LEDRespRedData = 0;

uint8_t g_u8AllLEDCallGreenData = 0;
uint8_t g_u8AllLEDRespGreenData = 0;
uint8_t g_u8AllLEDCallRedData = 0;
uint8_t g_u8AllLEDRespRedData = 0;

uint8_t g_u8ActiveRaw = 0;
uint8_t g_u8ActiveRawColor = 1;
uint8_t g_u8StepNumber = 0;
uint8_t g_u8AllLinesUnicolor = 4; /*поменять на ноль если нужен режим мигание и потом все горят сразу и снова мигание*/
uint8_t g_u8ChangeColor = 1;
uint8_t g_u8DisplayAllLinesUnicolor = 0;
uint8_t g_u8BinaryGreen = 0b00000000;
uint8_t g_u8BinaryRed = 0b00000000;

int g_u32TimePeriod = 0;

uint8_t g_u8NeedToDisplayLEDData = 1;
uint8_t g_u8NeedToDefineLEDGreenData = 0;
uint8_t g_u8NeedToDefineLEDRedData = 0;
uint8_t g_u8NeedToDefineLEDNothingData = 0;

RCC_ClkInitTypeDef sClokConfig;
uint32_t pFLatency;
uint32_t u32Prescaler;
uint32_t g_frequency;

uint8_t g_au8GreenCalls[m_NUMBER_OF_LINES] = {
		m_CALL_GREEN_LED_8_POS,
		m_CALL_GREEN_LED_7_POS,
		m_CALL_GREEN_LED_6_POS,
		m_CALL_GREEN_LED_5_POS,
		m_CALL_GREEN_LED_4_POS,
		m_CALL_GREEN_LED_3_POS,
		m_CALL_GREEN_LED_2_POS,
		m_CALL_GREEN_LED_1_POS};

uint8_t g_au8GreenResponses[m_NUMBER_OF_LINES] = {
		m_RESP_GREEN_LED_8_POS,
		m_RESP_GREEN_LED_7_POS,
		m_RESP_GREEN_LED_6_POS,
		m_RESP_GREEN_LED_5_POS,
		m_RESP_GREEN_LED_4_POS,
		m_RESP_GREEN_LED_3_POS,
		m_RESP_GREEN_LED_2_POS,
		m_RESP_GREEN_LED_1_POS};

uint8_t g_au8RedCalls[m_NUMBER_OF_LINES] = {
		m_CALL_RED_LED_8_POS,
		m_CALL_RED_LED_7_POS,
		m_CALL_RED_LED_6_POS,
		m_CALL_RED_LED_5_POS,
		m_CALL_RED_LED_4_POS,
		m_CALL_RED_LED_3_POS,
		m_CALL_RED_LED_2_POS,
		m_CALL_RED_LED_1_POS};

uint8_t g_au8RedResponses[m_NUMBER_OF_LINES] = {
		m_RESP_RED_LED_8_POS,
		m_RESP_RED_LED_7_POS,
		m_RESP_RED_LED_6_POS,
		m_RESP_RED_LED_5_POS,
		m_RESP_RED_LED_4_POS,
		m_RESP_RED_LED_3_POS,
		m_RESP_RED_LED_2_POS,
		m_RESP_RED_LED_1_POS};


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void ClearLEDSR(void);
void LoadLEDSR(void);
void ChangeColorLEDSR(void);
void ChangeRawLEDSR(void);

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
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */

  // Установить все линии в первоначальное положение
  HAL_GPIO_WritePin(STP_SR_LED_nCLR_GPIO_Port, STP_SR_LED_nCLR_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(STP_SR_LED_CLK_GPIO_Port, STP_SR_LED_CLK_Pin, GPIO_PIN_RESET);

  ClearLEDSR();

  // Запустить таймер
  if (m_USE_TIMER){

  HAL_TIM_Base_Start_IT(&htim3);

  }

                                                   /* TIME SETTINGS*/

  /*AHBPrescTable * APBPrescTable */
  g_frequency = HAL_RCC_GetPCLK1Freq();

  HAL_RCC_GetClockConfig(&sClokConfig, &pFLatency);
  //u32Prescaler = sClokConfig.APB1CLKDivider;
  u32Prescaler = htim3.Init.Prescaler;
  g_u32TimePeriod = ((g_frequency * m_TIME_TRIGGERING_LED_MS) / ((u32Prescaler + 1) * 1000)) - 1;
  __HAL_TIM_SET_AUTORELOAD(&htim3, g_u32TimePeriod);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

//  NeedToDefineLEDGreenData = 1;

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  //сформировали что отобразить

	  if (g_u8StepNumber == 0){

		  if (g_u8ActiveRawColor == m_GREEN_COLOR) {
			  g_u8LEDCallGreenData = (m_LED_ON << g_au8GreenCalls[g_u8ActiveRaw]);
			  g_u8LEDRespGreenData = (m_LED_ON << g_au8GreenResponses[g_u8ActiveRaw]);
			  g_u8LEDCallRedData = m_LED_OFF;
			  g_u8LEDRespRedData = m_LED_OFF;
		  }
//		  if (g_u8ActiveRawColor == m_RED_COLOR) {
//			  g_u8LEDCallRedData = m_LED_ON << g_au8RedCalls[g_u8ActiveRaw];
//			  g_u8LEDRespRedData = m_LED_ON << g_au8RedResponses[g_u8ActiveRaw];
//			  g_u8LEDCallGreenData = m_LED_OFF;
//			  g_u8LEDRespGreenData = m_LED_OFF;
//		  }
	  }

	  if (g_u8StepNumber != 0) {

		  g_u8BinaryGreen = g_u8BinaryGreen | (m_SR_DATA_bm << g_au8GreenCalls[m_NUMBER_OF_LINES - g_u8StepNumber]); /*для того, чтобы осталось перемигиваться раскомментить*/

	//	  g_u8BinaryRed = g_u8BinaryRed | (m_SR_DATA_bm << g_au8RedCalls[m_NUMBER_OF_LINES - g_u8StepNumber]);

		  if (g_u8ActiveRawColor == m_GREEN_COLOR) {

			  g_u8LEDCallGreenData = (m_LED_ON << g_au8GreenCalls[g_u8ActiveRaw]) | (g_u8BinaryGreen);
			  g_u8LEDRespGreenData = (m_LED_ON << g_au8GreenResponses[g_u8ActiveRaw]) | (g_u8BinaryGreen);
			  g_u8LEDCallRedData = m_LED_OFF | (g_u8BinaryRed);
			  g_u8LEDRespRedData = m_LED_OFF | (g_u8BinaryRed);
		  }
//		  if (g_u8ActiveRawColor == m_RED_COLOR) {
//
//			  g_u8LEDCallRedData = (m_LED_ON << g_au8RedCalls[g_u8ActiveRaw]) | (g_u8BinaryRed);
//			  g_u8LEDRespRedData = (m_LED_ON << g_au8RedResponses[g_u8ActiveRaw]) | (g_u8BinaryRed);
//			  g_u8LEDCallGreenData = m_LED_OFF | (g_u8BinaryGreen);
//			  g_u8LEDRespGreenData = m_LED_OFF | (g_u8BinaryGreen);
//   	      }
      }

	  if (g_u8ActiveRawColor == m_YELLOW_COLOR) {

			 g_u8LEDCallGreenData = m_LED_ON << g_au8GreenCalls[g_u8ActiveRaw];
			 g_u8LEDRespGreenData = m_LED_ON << g_au8GreenResponses[g_u8ActiveRaw];
			 g_u8LEDCallRedData = m_LED_ON << g_au8RedCalls[g_u8ActiveRaw];
			 g_u8LEDRespRedData = m_LED_ON << g_au8RedResponses[g_u8ActiveRaw];
	  }

	  //отобразили

	  if (g_u8NeedToDisplayLEDData) {
		  ClearLEDSR();
		  LoadLEDSR();


		  if (m_USE_TIMER) {
			  g_u8NeedToDisplayLEDData = 0;
		  }

	//	  g_u8ActiveRawColor++;

	//	  if (g_u8ActiveRawColor == m_NUMBER_OF_COLORS) {
	//		  g_u8ActiveRawColor = 1;
			  g_u8ActiveRaw++;
	//	  }
		  if (g_u8ActiveRaw + g_u8StepNumber == m_NUMBER_OF_LINES) {
			  g_u8ActiveRaw = 0;
			  g_u8StepNumber++;
			  if (g_u8StepNumber == m_NUMBER_OF_LINES) {
				  //break;
				  g_u8StepNumber = 0;
				  g_u8BinaryGreen = 0b00000000;
				  g_u8BinaryRed = 0b00000000;
			  }
			//	  g_u8AllLinesUnicolor++;
			  if (g_u8AllLinesUnicolor == m_NUMBER_OF_COLORS) {
				  g_u8AllLinesUnicolor = 0;
			  }
			  //  g_u8DisplayAllLinesUnicolor = 1;
		  }
	  }

	  if (g_u8AllLinesUnicolor == m_GREEN_COLOR) {

		  g_u8AllLEDCallGreenData = m_LED_ON;
		  g_u8AllLEDRespGreenData = m_LED_ON;
		  g_u8AllLEDCallRedData = m_LED_OFF;
		  g_u8AllLEDRespRedData = m_LED_OFF;
	  }

	  if (g_u8AllLinesUnicolor == m_RED_COLOR) {

		  g_u8AllLEDCallGreenData = m_LED_OFF;
		  g_u8AllLEDRespGreenData = m_LED_OFF;
		  g_u8AllLEDCallRedData = m_LED_ON;
		  g_u8AllLEDRespRedData = m_LED_ON;
	  }

	  if (g_u8AllLinesUnicolor == m_YELLOW_COLOR) {

		  g_u8AllLEDCallGreenData = m_LED_ON;            //так горит жёлтый, в зависимости от того, что OFF будет другой цвет - цикл?? условие7??? чтобы по порядку с прерыванием
		  g_u8AllLEDRespGreenData = m_LED_ON;
		  g_u8AllLEDCallRedData = m_LED_ON;
		  g_u8AllLEDRespRedData = m_LED_ON;
	  }


	  if (g_u8DisplayAllLinesUnicolor) {

	      HAL_GPIO_WritePin(LED_CALL_GREEN_SR_DATA_GPIO_Port, LED_CALL_GREEN_SR_DATA_Pin, g_u8AllLEDCallGreenData);
		  HAL_GPIO_WritePin(LED_RESP_GREEN_SR_DATA_GPIO_Port, LED_RESP_GREEN_SR_DATA_Pin, g_u8AllLEDRespGreenData);
		  HAL_GPIO_WritePin(LED_CALL_RED_SR_DATA_GPIO_Port, LED_CALL_RED_SR_DATA_Pin, g_u8AllLEDCallRedData);
		  HAL_GPIO_WritePin(LED_RESP_RED_SR_DATA_GPIO_Port, LED_RESP_RED_SR_DATA_Pin, g_u8AllLEDRespRedData);
		  for (uint8_t n = 0; n < m_NUMBER_OF_LINES; n++) {
			  HAL_GPIO_WritePin(STP_SR_LED_CLK_GPIO_Port, STP_SR_LED_CLK_Pin, GPIO_PIN_SET);
			  HAL_GPIO_WritePin(STP_SR_LED_CLK_GPIO_Port, STP_SR_LED_CLK_Pin, GPIO_PIN_RESET);
		  }
		  g_u8DisplayAllLinesUnicolor = 0;
	  }

	  if (!m_USE_TIMER) {
		  g_u8ActiveRawColor++;
		  if (g_u8ActiveRawColor == m_NUMBER_OF_COLORS) {
			  g_u8ActiveRawColor = 0;
		  }
	  }


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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 15999;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 64999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_RESP_RED_SR_DATA_Pin|LED_RESP_GREEN_SR_DATA_Pin|LED_CALL_RED_SR_DATA_Pin|LED_CALL_GREEN_SR_DATA_Pin
                          |STP_SR_LED_nCLR_Pin|STP_SR_LED_CLK_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_RESP_RED_SR_DATA_Pin LED_RESP_GREEN_SR_DATA_Pin LED_CALL_RED_SR_DATA_Pin LED_CALL_GREEN_SR_DATA_Pin
                           STP_SR_LED_nCLR_Pin STP_SR_LED_CLK_Pin */
  GPIO_InitStruct.Pin = LED_RESP_RED_SR_DATA_Pin|LED_RESP_GREEN_SR_DATA_Pin|LED_CALL_RED_SR_DATA_Pin|LED_CALL_GREEN_SR_DATA_Pin
                          |STP_SR_LED_nCLR_Pin|STP_SR_LED_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void ClearLEDSR(void) {

	HAL_GPIO_WritePin(STP_SR_LED_nCLR_GPIO_Port, STP_SR_LED_nCLR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(STP_SR_LED_nCLR_GPIO_Port, STP_SR_LED_nCLR_Pin, GPIO_PIN_SET);
}

void LoadLEDSR(void) {

	for (uint8_t i = 0; i < m_NUMBER_OF_LINES; i++) {
        HAL_GPIO_WritePin(LED_CALL_GREEN_SR_DATA_GPIO_Port, LED_CALL_GREEN_SR_DATA_Pin, (g_u8LEDCallGreenData & m_SR_DATA_bm));
		HAL_GPIO_WritePin(LED_RESP_GREEN_SR_DATA_GPIO_Port, LED_RESP_GREEN_SR_DATA_Pin, (g_u8LEDRespGreenData & m_SR_DATA_bm));
		HAL_GPIO_WritePin(LED_CALL_RED_SR_DATA_GPIO_Port, LED_CALL_RED_SR_DATA_Pin, (g_u8LEDCallRedData & m_SR_DATA_bm));
		HAL_GPIO_WritePin(LED_RESP_RED_SR_DATA_GPIO_Port, LED_RESP_RED_SR_DATA_Pin, (g_u8LEDRespRedData & m_SR_DATA_bm));

		HAL_GPIO_WritePin(STP_SR_LED_CLK_GPIO_Port, STP_SR_LED_CLK_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(STP_SR_LED_CLK_GPIO_Port, STP_SR_LED_CLK_Pin, GPIO_PIN_RESET);

		g_u8LEDCallGreenData = g_u8LEDCallGreenData >> 1;
		g_u8LEDRespGreenData = g_u8LEDRespGreenData >> 1;
		g_u8LEDCallRedData = g_u8LEDCallRedData >> 1;
		g_u8LEDRespRedData = g_u8LEDRespRedData >> 1;
	}
}

//void ChangeRawLEDSR(void){
//	ActiveRaw++;
//		  if (ActiveRaw == m_NUMBER_OF_LINES) {
//			  ActiveRaw = 0;
//		  }
//}
//
//void ChangeColorLEDSR(void){
//	ActiveRawColor++;
//	if (ActiveRawColor == m_NUMBER_OF_COLORS){
//		ActiveRawColor = 0;
//	}
//}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {

	if (htim == &htim3)
	{
		g_u8NeedToDisplayLEDData = 1;

	}
}

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
