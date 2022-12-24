/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "spi.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

#include "MPU9250/mpu9250_spi.h"

uint16_t      Classification(void);  //声明
extern double exp(double __x);

int32_t  V = 0, Feng = 0, S = 0;
uint16_t Result;

uint16_t send_data[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint16_t Sample[9];
double   Output[6];

int     Number_Sample = 200;
int16_t ACCEL_X[200];
int16_t ACCEL_Y[200];
int16_t ACCEL_Z[200];
int16_t GYRO_X[200];
int16_t GYRO_Y[200];
int16_t GYRO_Z[200];
int16_t MAG_X[200];
int16_t MAG_Y[200];
int16_t MAG_Z[200];

int16_t GetFeng(int16_t accel[]);
int16_t Getsig(int16_t accel[]);
int16_t GetSMA(int16_t accel_X[], int16_t accel_Y[], int16_t accel_Z[]);

int16_t GetFeng(int16_t accel[]) {
    int     i;
    int16_t MAX = 0, MIN = 0, FENG = 0;
    MAX = accel[0];
    MIN = accel[0];
    for (i = 1; i < Number_Sample; i++) {
        if (accel[i] > MAX) { MAX = accel[i]; }
        if (accel[i] < MIN) { MIN = accel[i]; }
    }
    FENG = MAX - MIN;
    return FENG;
}

int16_t Getsig(int16_t accel[])  //方差
{
    int     i;
    int32_t sig = 0, S = 0, M = 0;
    for (i = 1; i < Number_Sample; i++) {
        S = S + accel[i];
    }
    S = S / Number_Sample;
    for (i = 1; i < Number_Sample; i++) {
        M = M + (accel[i] - S) * (accel[i] - S) / Number_Sample;
    }
    sig = M / 10;
    return sig;
}

int16_t GetSMA(int16_t accel_X[], int16_t accel_Y[], int16_t accel_Z[])  //幅值面积
{
    int     i;
    int32_t SMA = 0, S = 0;
    for (i = 1; i < Number_Sample; i++) {
        S = S + abs(accel_X[i]) + abs(accel_Y[i]) + abs(accel_Z[i]);
    }
    SMA = S / Number_Sample;

    return SMA;
}

uint16_t Classification(void) {
    int      i, j;
    uint16_t result;
    //	double A,S;
    //	double Inputweight[10][6]=
    //	{
    //   {-0.8163,    0.1016,   -0.1901,   -0.9304,    0.6338,    0.5209},
    //   {-0.1958,    0.7418,   -0.6529,   -0.4143,   -0.6211,    0.7107},
    //   {-0.4096,   -0.9155,    0.1504,    0.6029,   -0.7526,   -0.2343},
    //   {-0.3870,    0.8094,    0.2124,   -0.3070,    0.6420,   -0.8307},
    //   {-0.7889,   -0.7381,   -0.5711,   -0.8334,    0.2758,    0.4677},
    //   { 0.1877,    0.6675,    0.0399,    0.0222,   -0.9678,   -0.3360},
    //   {-0.4345,    0.6009,    0.9784,   -0.2663,    0.7919,    0.6795},
    //   {-0.6896,    0.8358,   -0.0202,    0.4790,    0.0308,   -0.2566},
    //   {-0.9987,   -0.7254,    0.3897,    0.0495,    0.0890,    0.6564},
    //   {-0.4328,    0.0095,   -0.1772,    0.6090,    0.2129,   -0.6470},
    //	};
    //	double Bi[10]=
    //		{
    //    0.1295,
    //    0.8799,
    //    0.0441,
    //    0.6867,
    //    0.7338,
    //    0.4372,
    //    0.3798,
    //    0.9797,
    //    0.3990,
    //    0.4402,
    //		};
    //	double tempH[10];
    //	double temp_H[10];
    //	double H[10];
    //	double  Outputweight[10][3]=
    //	{
    //	 {-0.0073,   -0.0268,    0.0341},
    //   { 0.0000,   -0.0000,    0.0000},
    //   { 0.0000,    0.5856,   -0.5856},
    //   { 0.0279,   -0.1513,    0.1233},
    //   {-0.0000,    0.0000,   -0.0000},
    //   { 0.0228,   -0.4290,    0.4063},
    //   {-1.0013,    0.9742,   -0.9729},
    //   {-0.0190,   -1.5538,    1.5728},
    //   { 1.9548,   -1.5708,   -0.3840},
    //   {-0.0003,   -0.2424,    0.2427},
    //	};

    //  A=0;	S=0;
    //	for(i=0;i<10;i++)
    //	{
    //		for(j=0;j<6;j++)
    //		{
    //			A=Inputweight[i][j]*Sample[j];
    //			S=A+S;
    //		}
    //		temp_H[i] = S;
    //	 }
    //	  for(i=0;i<10;i++)
    //			{
    //				tempH[i] = temp_H[i] + Bi[i];
    //			  H[i] = 1./(1+exp(-tempH[i]));
    //			}
    //  A=0;	 S=0;
    //		for(i=0;i<3;i++)
    //	 {
    //		for(j=0;j<10;j++)
    //		{
    //			A=H[j] * Outputweight[j][i];
    //			S=A+S;
    //		}
    //		Output[i] = S;
    //	 }
    //
    //		if (Output[0]>Output[1]&&Output[0]>Output[2])
    //		{			result=82;		}
    //				if (Output[1]>Output[0]&&Output[1]>Output[2])
    //		{			result=83;		}
    //				if (Output[2]>Output[1]&&Output[2]>Output[0])
    //		{			result=84;		}
    if (Sample[0] < 45) { result = 82; }
    if (Sample[0] > 55 && Sample[0] < 140) { result = 83; }
    if (Sample[0] > 140) { result = 84; }
    return result;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
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
    MX_USART1_UART_Init();
    MX_SPI3_Init();
    /* USER CODE BEGIN 2 */

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    Init_MPU9250();
    delay_ms(10);
    while (1) {
        READ_MPU9250_ACCEL();
        READ_MPU9250_GYRO();
        READ_MPU9250_MAG();
        send_data[0] = mpu_value.Accel[0];
        send_data[1] = mpu_value.Accel[1];
        send_data[2] = mpu_value.Accel[2];
        send_data[3] = mpu_value.Gyro[0];
        send_data[4] = mpu_value.Gyro[1];
        send_data[5] = mpu_value.Gyro[2];
        send_data[6] = mpu_value.Mag[0];
        send_data[7] = mpu_value.Mag[1];
        send_data[8] = mpu_value.Mag[2];
        printf("%d,%d,%d\r\n", send_data[0], send_data[1], send_data[2]);
        // printf("%d,%d,%d\r\n", send_data[6], send_data[7], send_data[8]);
        // printf("%d,%d,%d\r\n", send_data[6], send_data[7], send_data[8]);
        delay_ms(100);  //调节采样速率
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }
    /** Initializes the CPU, AHB and APB busses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
