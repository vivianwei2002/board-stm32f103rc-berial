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
#include "adc.h"
#include "i2c.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "math.h"
#include "apps/menulist.h"
#include "apps/iconlist.h"

#define CONFIG_PADDING_X 4

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

#include "res/images.h"

typedef enum {
    PAGE_ID_NONE = -1,
    //
    PAGE_ID_FIRST = 0,

    PAGE_ID_MAIN_MENULIST = PAGE_ID_FIRST,
    PAGE_ID_PID_MENULIST,
    PAGE_ID_ICON_ICONLIST,

    PAGE_ID_LOGO,
    PAGE_ID_CHART,
    PAGE_ID_TEXTEDIT,
    PAGE_ID_ABOUT,

    PAGE_ID_LAST,
    //
    FUNC_ID_RETURN = PAGE_ID_LAST + 1,
    FUNC_ID_ANIMATION,
} page_id_t;

page_t*   pages[PAGE_ID_LAST];
page_id_t page_selected;  // index of rendering

#define PAGE_SWITCH(ID)    pages[page_selected = ID]->repaint = true
#define PAGE_SET(ID, page) pages[ID] = (page_t*)&page
#define PAGE_RENDER()                                        \
    pages[page_selected]->handler(pages[page_selected]);     \
    if (pages[page_selected]->repaint) {                     \
        println("repaint");                                  \
        pages[page_selected]->repaint = 0;                   \
        pages[page_selected]->painter(pages[page_selected]); \
    }

///

const item_menu_t menu_main[] = {
    {PAGE_ID_LOGO, "MainUI"},
    {PAGE_ID_PID_MENULIST, "+PID Editor"},
    {PAGE_ID_ICON_ICONLIST, "-Icon Test"},
    {PAGE_ID_CHART, "-Chart Test"},
    {PAGE_ID_TEXTEDIT, "-Text Edit"},
    {PAGE_ID_NONE, "-Play Video"},
    {PAGE_ID_ABOUT, "{ About }"},
};

void _painter_logo(page_t* p)
{
    _clear();
    _set_color(0);
    _draw_str(CONFIG_PADDING_X, 12, "Hello World");
    _set_color(1);
    _draw_str(CONFIG_PADDING_X, 24, "Hello World");
    _set_color(2);
    _draw_str(CONFIG_PADDING_X, 36, "Hello World");
    _update();
}

void _handler_logo(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default:
            effect_disappear();
            PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            break;
    }
}

void _painter_chart(page_t* p)
{
    _clear();
    _update();
}
void _handler_chart(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default:
            effect_disappear();
            PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            break;
    }
}

void _painter_textedit(page_t* p)
{
    _clear();
    _update();
}
void _handler_textedit(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default:
            println("hello");
            effect_disappear();
            PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            break;
    }
}

void _painter_about(page_t* p)
{
    _clear();
    _set_color(1);
    _draw_str(2, 12, "MCU : STM32");
    _draw_str(2, 28, "FLASH : 512KB");
    _draw_str(2, 44, "SRAM : 256KB");
    _draw_str(2, 60, "RTC SRAM : 16KB");
    _update();
}

void _handler_about(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE:
            break;
        default:
            effect_disappear();
            PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
            break;
    }
}

void _hander_main(menulist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
        case KEY_ID_OK:

            switch (p->items[p->index_selected].ID) {
                case PAGE_ID_LOGO:
                case PAGE_ID_CHART:
                case PAGE_ID_TEXTEDIT:
                case PAGE_ID_ABOUT:
                case PAGE_ID_PID_MENULIST:
                case PAGE_ID_ICON_ICONLIST: {
                    effect_disappear();
                    PAGE_SWITCH(p->items[p->index_selected].ID);
                    break;
                }

                default:
                    break;
            }

            p->repaint = true;

            break;
    }
}

const item_menu_t menu_pid[] = {
    {0, "-Proportion"},
    {0, "-Integral"},
    {0, "-Derivative"},
    {FUNC_ID_RETURN, "Return"},
};

void _hander_pid(menulist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
        case KEY_ID_OK:

            switch (p->items[p->index_selected].ID) {
                case FUNC_ID_RETURN: {
                    effect_disappear();
                    PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
                    break;
                }
            }
    }
}

const item_icon_t menu_icon[] = {
    {FUNC_ID_ANIMATION, "Likes", 36, 35, res_likes},
    {FUNC_ID_RETURN, "Collection", 36, 37, res_collect},
    {FUNC_ID_ANIMATION, "Slot", 36, 36, res_coin},
    {FUNC_ID_RETURN, "Share", 36, 36, res_share},
};

void _hander_icon(iconlist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: iconlist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: iconlist_callback_handler_switch_next(p); break;
        case KEY_ID_OK:
            switch (p->items[p->index_selected].ID) {
                case FUNC_ID_ANIMATION:
                    easing_start_relative(&p->y_padding_icon, -p->icon_space / 8, 1);
                    p->repaint = true;
                    break;
                case FUNC_ID_RETURN: {
                    effect_disappear();
                    PAGE_SWITCH(PAGE_ID_MAIN_MENULIST);
                    break;
                }
                default: break;
            }
    }
}

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
    HAL_Delay(500);
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    MX_I2C2_Init();
    MX_USART2_UART_Init();
    // MX_ADC1_Init();
    /* USER CODE BEGIN 2 */
    /* USER CODE END 2 */
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    // HAL_ADCEx_Calibration_Start(&hadc1);

    key_init();
    u8g2_init();

    page_t page_logo     = page_new(_painter_logo, _handler_logo);
    page_t page_chart    = page_new(_painter_chart, _handler_chart);
    page_t page_textedit = page_new(_painter_textedit, _handler_textedit);
    page_t page_about    = page_new(_painter_about, _handler_about);

    PAGE_SET(PAGE_ID_LOGO, page_logo);
    PAGE_SET(PAGE_ID_CHART, page_chart);
    PAGE_SET(PAGE_ID_TEXTEDIT, page_textedit);
    PAGE_SET(PAGE_ID_ABOUT, page_about);

    menulist_t menulist_main = menulist_initialize(menu_main, ARRAY_SIZE(menu_main), 4, 0, _hander_main);
    menulist_t menulist_pid  = menulist_initialize(menu_pid, ARRAY_SIZE(menu_pid), 4, 0, _hander_pid);

    PAGE_SET(PAGE_ID_MAIN_MENULIST, menulist_main);
    PAGE_SET(PAGE_ID_PID_MENULIST, menulist_pid);

    iconlist_t iconlist_icon = iconlist_initialize(menu_icon, ARRAY_SIZE(menu_icon), 0, _hander_icon);

    PAGE_SET(PAGE_ID_ICON_ICONLIST, iconlist_icon);

    PAGE_SWITCH(PAGE_ID_FIRST);

    while (1) {
        PAGE_RENDER();

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
    RCC_OscInitTypeDef       RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef       RCC_ClkInitStruct = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit     = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
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

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection    = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
