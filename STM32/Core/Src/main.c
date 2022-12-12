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
#include "fonts.h"
#include "ssd1306.h"
#include "customChar.h"
#include "string.h"
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
 I2C_HandleTypeDef hi2c1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_USART1_UART_Init(void);
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /*Se inicia la pantalla OLED*/
  SSD1306_Init();

  /*Declaración a variables a utilizar*/
  char snum[5];
  char dnum[5];
  char cnum[5];
  char mnum[5];

  int seg=0;
  int seg2=1;
  int min1=1;
  int min2=1;

  int toggle=0;
  char pack[50]={" "};
  char command;


  /*Función que realiza una pantalla de bienvenida*/
  logo_init();

  /*Diseño de pantalla inicial*/
  SSD1306_GotoXY (30,30);
  SSD1306_Puts ("00:00", &Font_11x18, 1);
  SSD1306_UpdateScreen();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	 /*Función que realiza la animación del ecualizador y se actualiza la pantalla*/
	 toggle_anim(toggle);
	 SSD1306_UpdateScreen();

	 /*Proceso para el recivir datos a través de la comunicación serial*/
	 HAL_UART_Receive(&huart1, &pack, sizeof(pack),3000);

	 /*Declaración de variable dinámicas*/
	 char *token=strtok(pack,",");
	 int cont=0;

	 /*Ciclo while para separar los datos recibos por serial*/
	 while (token !=NULL){
		 /*Obtención del nombre de la canción*/
		 if (cont==0){
			SSD1306_DrawFilledRectangle(0,0, 124,10, 0);
			SSD1306_UpdateScreen();
			SSD1306_GotoXY (0,0);
			SSD1306_Puts (token, &Font_7x10, 1);
		 }
		 /*Obtención del nombre del artist@ o banda*/
		 else if (cont==1){
			SSD1306_DrawFilledRectangle(0,16, 124,10, 0);
			SSD1306_UpdateScreen();
			SSD1306_GotoXY (0,16);
			SSD1306_Puts (token, &Font_7x10, 1);
		 }
		 /*Obtención del tiempo*/
		 else if (cont==2){
			SSD1306_DrawFilledRectangle(27,29, 124,18, 0);
			SSD1306_UpdateScreen();
			/*conversión de caracter a int*/
			int x=atoi(token);
			/*obtención de dígitos individuales*/
			int unit= x%10;
			int deci=((x%100)-unit)/10;
			int cent=x/100;
			int mil=0;
			/*conversión de int a caracter*/
			itoa(unit, snum,10);
			itoa(deci, dnum,10);
			itoa(cent, cnum,10);
			itoa(mil, mnum,10);
			/*impresión de los datos*/
			SSD1306_GotoXY (27,29);
			SSD1306_Puts (mnum, &Font_11x18, 1);
			SSD1306_Puts (cnum, &Font_11x18, 1);
			SSD1306_GotoXY (67,29);
			SSD1306_Puts (dnum, &Font_11x18, 1);
			SSD1306_Puts (snum, &Font_11x18, 1);
		 }
		 /*argumento de separación ',' y contador*/
		 token= strtok(NULL, ",");
		 cont++;
	 }

	 /*Diseño de botones interactivos*/
	 SSD1306_DrawBitmap(20,46,bitmap_backward,15,15,1);
	 SSD1306_DrawBitmap(50,36,bitmap_play,16,16,1);
	 SSD1306_DrawBitmap(80,46,bitmap_forward,15,15,1);
	 SSD1306_UpdateScreen();

	 /*Declaración de caracter a transmitir con botones interactivos*/
	 char character=0b000;

	 /*Lectura de los botones*/
	 int one=HAL_GPIO_ReadPin(GPIOB, PlayPauseButton_Pin);
	 int two=HAL_GPIO_ReadPin(GPIOB, BackwardButton_Pin);
	 int three=HAL_GPIO_ReadPin(GPIOB, ForwardButton_Pin);

	 /*Condición para la transmisión para el caso playPause*/
	 if (one){
		 command=0x1;
		 char dataT[2]={command,'\n'};
		 HAL_UART_Transmit(&huart1, &dataT, sizeof(dataT),150);
		 SSD1306_DrawBitmap(50,36,bitmap_play,16,16,0);
		 SSD1306_UpdateScreen();
		 SSD1306_DrawBitmap(50,36,bitmap_pause,15,16,1);
		 SSD1306_UpdateScreen();
		 HAL_Delay(100);
		 SSD1306_DrawBitmap(50,36,bitmap_pause,15,16,0);
		 SSD1306_UpdateScreen();
		 HAL_Delay(100);
		 SSD1306_DrawBitmap(50,36,bitmap_play,16,16,1);
		 SSD1306_UpdateScreen();
	 }
	 /*Condición para la transmisión para el caso Backward*/
	 if (two){
		 command=0x2;
		 SSD1306_DrawRectangle(20,46,15,15,1);
		 SSD1306_UpdateScreen();
		 char dataT[2]={command,'\n'};
		 HAL_UART_Transmit(&huart1, &dataT, sizeof(dataT),150);

	 }
	 /*Condición para la transmisión para el caso NOT Backward*/
	 else if (!two){
		 SSD1306_DrawRectangle(20,46,15,15,0);
		 SSD1306_UpdateScreen();
	 }
	 /*Condición para la transmisión para el caso Forward*/
	 if (three){
		 command=0x3;
		 SSD1306_DrawRectangle(80,46,15,15,1);
		 SSD1306_UpdateScreen();
		 char dataT[2]={command,'\n'};
		 HAL_UART_Transmit(&huart1, &dataT, sizeof(dataT),150);

	 }
	 /*Condición para la transmisión para el NOT Forward*/
	 else if (!three){
		 SSD1306_DrawRectangle(80,46,15,15,0);
		 SSD1306_UpdateScreen();
	 }

	 /*Función que realiza la animación del ecualizador y se actualiza la pantalla*/
	 toggle_anim(toggle);

	 /*Condición de cambio de la animación del ecualizador*/
	 if (toggle==2){
		 toggle=-1;
	 }
	 toggle++;


	 /*Se actualiza la pantalla*/
	 SSD1306_UpdateScreen();

	 /*Se borra la pantalla de animación del ecualizador*/
	 SSD1306_DrawFilledRectangle(42,52, 35,9, 0);
	 HAL_Delay(2500);

	 /*Se actualiza la pantalla*/
	 SSD1306_UpdateScreen();

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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : BackwardButton_Pin PlayPauseButton_Pin ForwardButton_Pin */
  GPIO_InitStruct.Pin = BackwardButton_Pin|PlayPauseButton_Pin|ForwardButton_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/*Función de impresión de mapas de bit para la animación de bienvenida*/
void logo_init (void){

	  /*LOGO ANIMATION*/
	  SSD1306_GotoXY (0,0);
	  SSD1306_Puts ("Prehispanic", &Font_11x18, 1);
	  SSD1306_DrawBitmap(0,15,bitmap_prehispanic,78,64,1);

	  SSD1306_GotoXY (88,30);
	  SSD1306_Puts ("MX", &Font_11x18, 1);
	  SSD1306_GotoXY (88,48);
	  SSD1306_Puts ("Auto", &Font_7x10, 1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(3000);
	  SSD1306_Clear();

	  /*MP3 LOGO*/
	  SSD1306_DrawBitmap(0,1,bitmap_mp3,64,32,1);
	  SSD1306_GotoXY (70,40);
	  SSD1306_Puts ("Welcome", &Font_7x10, 1);
	  SSD1306_GotoXY (70,50);
	  SSD1306_Puts ("CarSys", &Font_7x10, 1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(2000);
	  SSD1306_Clear();

	  /*CAR ANIMATION*/
	  SSD1306_DrawBitmap(15,1,bitmap_car1,111,60,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car2,91,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car3,66,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car4,60,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car5,91,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car6,106,56,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car7,105,62,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car8,80,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car9,61,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car10,54,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car11,86,64,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();

	  SSD1306_DrawBitmap(15,1,bitmap_car12,108,61,1);
	  SSD1306_UpdateScreen();
	  HAL_Delay(150);
	  SSD1306_Clear();
}


/*Función de impresión de mapas de bit para la animación del ecualizador*/
void toggle_anim(int toggle){
	if (toggle==0){
		 SSD1306_DrawBitmap(42,52,bitmap_eq1,35,8,1);
		 toggle=1;
	}
	else if (toggle==1){
		 SSD1306_DrawBitmap(42,52,bitmap_eq2,35,7,1);
		 toggle=2;
	}
	else if (toggle==2){
		 SSD1306_DrawBitmap(42,52,bitmap_eq3,35,9,1);
		 toggle=0;
	}
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
