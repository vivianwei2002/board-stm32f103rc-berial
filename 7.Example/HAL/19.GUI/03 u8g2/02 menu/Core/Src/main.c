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
#include "oled/oled.h"
#include <stdbool.h>
#include "math.h"
#include "matrixkey/matrixkey.h"
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
u8g2_t u8g2;

#include "u8g2/mui_u8g2.h"
#include "u8g2/mui.h"
#include "u8g2/u8g2.h"
#include "u8g2/u8x8.h"

uint8_t _x, _y;

void u8g2_SetCursor(u8g2_t* u8g2, uint8_t x, uint8_t y)
{
    _x = x;
    _y = y;
}

void u8g2_Print(u8g2_t* u8g2, const char* str)
{
    u8g2_DrawStr(u8g2, _x, _y, str);
}

void u8g2_Print2(u8g2_t* u8g2, int num)
{
    static char buff[16];
    sprintf(buff, "%d", num);
    u8g2_DrawStr(u8g2, _x, _y, buff);
}

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// #include <Arduino.h>

#define SPEED      4  // 16的因�??
#define ICON_SPEED 12
#define ICON_SPACE 48  // 图标间隔，speed倍数

void chart_draw_frame();

static const uint8_t icon_pic[][200] = {
    {
        0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x3E,
        0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00,
        0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x00,
        0x00, 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00, 0x00,
        0x7F, 0x00, 0x00, 0x00, 0x80, 0x7F, 0x00, 0x00,
        0x00, 0x80, 0x7F, 0x00, 0x00, 0x00, 0xC0, 0x7F,
        0x00, 0x00, 0x00, 0xE0, 0x7F, 0x00, 0x00, 0x00,
        0xF8, 0x7F, 0x00, 0x00, 0xF0, 0xF8, 0xFF, 0xFF,
        0x01, 0xFC, 0xF8, 0xFF, 0xFF, 0x07, 0xFC, 0xF8,
        0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF, 0x07,
        0xFE, 0xF8, 0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF,
        0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF, 0x07, 0xFE,
        0xF8, 0xFF, 0xFF, 0x07, 0xFE, 0xF8, 0xFF, 0xFF,
        0x03, 0xFE, 0xF8, 0xFF, 0xFF, 0x03, 0xFE, 0xF8,
        0xFF, 0xFF, 0x03, 0xFE, 0xF8, 0xFF, 0xFF, 0x03,
        0xFE, 0xF8, 0xFF, 0xFF, 0x01, 0xFE, 0xF8, 0xFF,
        0xFF, 0x01, 0xFE, 0xF8, 0xFF, 0xFF, 0x01, 0xFE,
        0xF8, 0xFF, 0xFF, 0x01, 0xFE, 0xF8, 0xFF, 0xFF,
        0x00, 0xFE, 0xF8, 0xFF, 0xFF, 0x00, 0xFC, 0xF8,
        0xFF, 0x7F, 0x00, 0xFC, 0xF8, 0xFF, 0x3F, 0x00,
        0xF8, 0xF8, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*"C:\Users\ROG\Desktop\三连\点赞.bmp",0*/
                                                 /* (36 X 35 )*/
    },
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00,
        0x00, 0x1F, 0x00, 0x00, 0x00, 0x80, 0x1F, 0x00,
        0x00, 0x00, 0x80, 0x3F, 0x00, 0x00, 0x00, 0xC0,
        0x3F, 0x00, 0x00, 0x00, 0xC0, 0x3F, 0x00, 0x00,
        0x00, 0xC0, 0x7F, 0x00, 0x00, 0x00, 0xE0, 0xFF,
        0x00, 0x00, 0x00, 0xF0, 0xFF, 0x01, 0x00, 0x00,
        0xFC, 0xFF, 0x03, 0x00, 0xE0, 0xFF, 0xFF, 0xFF,
        0x00, 0xFC, 0xFF, 0xFF, 0xFF, 0x07, 0xFE, 0xFF,
        0xFF, 0xFF, 0x07, 0xFC, 0xFF, 0xFF, 0xFF, 0x07,
        0xFC, 0xFF, 0xFF, 0xFF, 0x03, 0xF8, 0xFF, 0xFF,
        0xFF, 0x01, 0xF0, 0xFF, 0xFF, 0xFF, 0x00, 0xE0,
        0xFF, 0xFF, 0x7F, 0x00, 0xC0, 0xFF, 0xFF, 0x3F,
        0x00, 0x80, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0xFF,
        0xFF, 0x1F, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00,
        0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0xFF,
        0x0F, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00, 0x00,
        0xFF, 0xFF, 0x0F, 0x00, 0x00, 0xFF, 0xFF, 0x0F,
        0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0xFF,
        0xFF, 0x1F, 0x00, 0x80, 0xFF, 0xF0, 0x1F, 0x00,
        0x80, 0x3F, 0xC0, 0x1F, 0x00, 0x80, 0x1F, 0x00,
        0x1F, 0x00, 0x00, 0x07, 0x00, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00 /*"C:\Users\ROG\Desktop\三连\收藏.bmp",0*/
             /* (36 X 37 )*/
    },
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x1F,
        0x00, 0x00, 0x00, 0xF0, 0xFF, 0x01, 0x00, 0x00,
        0xFC, 0xFF, 0x07, 0x00, 0x00, 0xFF, 0xFF, 0x0F,
        0x00, 0x80, 0xFF, 0xFF, 0x1F, 0x00, 0xC0, 0xFF,
        0xFF, 0x7F, 0x00, 0xE0, 0x07, 0x00, 0x7C, 0x00,
        0xF0, 0x03, 0x00, 0xFC, 0x00, 0xF0, 0x03, 0x00,
        0xFC, 0x01, 0xF8, 0xFF, 0xF1, 0xFF, 0x01, 0xF8,
        0xFF, 0xF1, 0xFF, 0x03, 0xF8, 0x7F, 0xC0, 0xFF,
        0x03, 0xFC, 0x1F, 0x00, 0xFF, 0x03, 0xFC, 0x07,
        0x00, 0xFE, 0x07, 0xFC, 0x07, 0x01, 0xFC, 0x07,
        0xFC, 0xC3, 0x31, 0xF8, 0x07, 0xFC, 0xE1, 0xF1,
        0xF8, 0x07, 0xFC, 0xF1, 0xF1, 0xF0, 0x07, 0xFC,
        0xF1, 0xF1, 0xF0, 0x07, 0xFC, 0xF1, 0xF1, 0xF1,
        0x07, 0xFC, 0xF1, 0xF1, 0xF1, 0x07, 0xFC, 0xF1,
        0xF1, 0xF1, 0x03, 0xF8, 0xF1, 0xF1, 0xF1, 0x03,
        0xF8, 0xFF, 0xF1, 0xFF, 0x03, 0xF8, 0xFF, 0xF1,
        0xFF, 0x01, 0xF0, 0xFF, 0xF1, 0xFF, 0x01, 0xF0,
        0xFF, 0xF1, 0xFF, 0x00, 0xE0, 0xFF, 0xF1, 0x7F,
        0x00, 0xC0, 0xFF, 0xFF, 0x7F, 0x00, 0x80, 0xFF,
        0xFF, 0x3F, 0x00, 0x00, 0xFF, 0xFF, 0x0F, 0x00,
        0x00, 0xFC, 0xFF, 0x07, 0x00, 0x00, 0xF0, 0xFF,
        0x01, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00 /*"C:\Users\ROG\Desktop\三连\投币.bmp",0*/
                               /* (36 X 36 )*/
    },
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00,
        0x00, 0x00, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00,
        0xFC, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x01, 0x00,
        0x00, 0x00, 0xFC, 0x03, 0x00, 0x00, 0x00, 0xFC,
        0x07, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0x00, 0x00,
        0x00, 0xFE, 0x1F, 0x00, 0x00, 0xF8, 0xFF, 0x3F,
        0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x00, 0xC0, 0xFF,
        0xFF, 0xFF, 0x01, 0xE0, 0xFF, 0xFF, 0xFF, 0x03,
        0xF0, 0xFF, 0xFF, 0xFF, 0x07, 0xF0, 0xFF, 0xFF,
        0xFF, 0x0F, 0xF8, 0xFF, 0xFF, 0xFF, 0x0F, 0xFC,
        0xFF, 0xFF, 0xFF, 0x07, 0xFC, 0xFF, 0xFF, 0xFF,
        0x03, 0xFE, 0xFF, 0xFF, 0xFF, 0x01, 0xFE, 0xFF,
        0xFF, 0xFF, 0x00, 0xFF, 0x03, 0xFE, 0x3F, 0x00,
        0xFF, 0x00, 0xFC, 0x1F, 0x00, 0x3F, 0x00, 0xFC,
        0x0F, 0x00, 0x1F, 0x00, 0xFC, 0x07, 0x00, 0x07,
        0x00, 0xFC, 0x03, 0x00, 0x03, 0x00, 0xFC, 0x01,
        0x00, 0x01, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00,
        0x3C, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00,
        0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, /*"C:\Users\13944\Desktop\fenxiang.bmp",0*/
    },
};

uint8_t icon_width[] = {35, 37, 36, 36};

const float PID_MAX = 10.00;  // PID�??大允许�??
// PID变量
float Kpid[3] = {9.97, 0.2, 0.01};  // Kp,Ki,Kd
// float Kp=8.96;
// float Ki=0.2;
// float Kd=0.01;

uint8_t disappear_step = 1;

float angle, angle_last;
// 实时坐标
uint8_t chart_x;
bool    frame_is_drawed = false;

// 指向buf首地�??的指�??
uint8_t* buf_ptr;
uint16_t buf_len;

// 选择界面变量
uint8_t x;
int16_t y, y_trg;                  // 目标和当�??
uint8_t line_y, line_y_trg;        // 线的位置
uint8_t box_width, box_width_trg;  // 框的宽度
int16_t box_y, box_y_trg;          // 框的当前值和目标�??
int8_t  ui_select;                 // 当前选中那一�??

// pid界面变量
// int8_t pid_y,pid_y_trg;
uint8_t pid_line_y, pid_line_y_trg;        // 线的位置
uint8_t pid_box_width, pid_box_width_trg;  // 框的宽度
int16_t pid_box_y, pid_box_y_trg;          // 框的当前值和目标�??
int8_t  pid_select;                        // 当前选中那一�??

// icon界面变量
int16_t icon_x, icon_x_trg;
int16_t app_y, app_y_trg;

int8_t icon_select;

uint8_t ui_index, ui_state;

enum  // ui_index
{
    M_LOGO,       // �??始界�??
    M_SELECT,     // 选择界面
    M_PID,        // PID界面
    M_PID_EDIT,   // pid编辑
    M_ICON,       // icon界面
    M_CHART,      // 图表
    M_TEXT_EDIT,  // 文字编辑
    M_VIDEO,      // 视频显示
    M_ABOUT,      // 关于本机
};

enum  // ui_state
{
    S_NONE,
    S_DISAPPEAR,
    S_SWITCH,
    S_MENU_TO_MENU,
    S_MENU_TO_PIC,
    S_PIC_TO_MENU,
};

// const char* text="This is a text Hello world ! follow up one two three four may jun july";

// 菜单结构�??
typedef struct
{
    char* select;
} SELECT_LIST;

static const SELECT_LIST pid[] = {
    {"-Proportion"},
    {"-Integral"},
    {"-Derivative"},
    {"Return"},
};

uint8_t pid_num = sizeof(pid) / sizeof(SELECT_LIST);  // PID选项数量

static const SELECT_LIST list[] = {
    {"MainUI"},
    {"+PID Editor"},
    {"-Icon Test"},
    {"-Chart Test"},
    {"-Text Edit"},
    {"-Play Video"},
    {"{ About }"},
};

const uint8_t list_num           = sizeof(list) / sizeof(SELECT_LIST);  // 选择界面数量
const uint8_t single_line_length = 63 / list_num;
const uint8_t total_line_length  = single_line_length * list_num + 1;

const SELECT_LIST icon[] = {
    {"Likes"},
    {"Collection"},
    {"Slot"},
    {"Share"},
};

// 设备名称
char name[] = "Hello World ";
// 允许名字的最大长�??
const uint8_t name_len   = 12;  // 0-11for name  12 for return
uint8_t       edit_index = 0;
bool          edit_flag  = false;  // 默认不在编辑
uint8_t       blink_flag;          // 默认高亮
const uint8_t BLINK_SPEED = 16;    // 2的�?�数

uint8_t icon_num = sizeof(icon) / sizeof(SELECT_LIST);

// 按键变量
typedef struct {
    bool val;
    bool last_val;
} KEY;

KEY key[3] = {false};

// 按键信息
typedef struct {
    uint8_t id;
    bool    pressed;
} KEY_MSG;

KEY_MSG key_msg = {0};

bool get_key_val(uint8_t ch)
{
    switch (ch) {
        case 0: return HAL_GPIO_ReadPin(KEY_C1_GPIO_Port, KEY_C1_Pin);
        case 1: return HAL_GPIO_ReadPin(KEY_C3_GPIO_Port, KEY_C3_Pin);
        case 2: return HAL_GPIO_ReadPin(KEY_C2_GPIO_Port, KEY_C2_Pin);
        default: break;
    }
}

void key_init()
{
    for (uint8_t i = 0; i < (sizeof(key) / sizeof(KEY)); ++i)
        key[i].val = key[i].last_val = get_key_val(i);
}

void key_scan()
{
    for (uint8_t i = 0; i < (sizeof(key) / sizeof(KEY)); ++i) {
        key[i].val = get_key_val(i);        // 获取键�??
        if (key[i].last_val != key[i].val)  // 发生改变
        {
            key[i].last_val = key[i].val;  // 更新状�??
            if (key[i].val == 0) {
                key_msg.id      = i;
                key_msg.pressed = true;
            }
        }
    }
}

/////////////////////////////////////////////

bool move_to_u8(uint8_t* current, uint8_t* target, uint8_t speed)
{
    if (*current < *target) {
        *current += speed;
        if (*current > *target)
            *current = *target;
    } else if (*current > *target) {
        *current -= speed;
        if (*current < *target)
            *current = *target;
    }
    return *current == *target;
}

bool move_to_s16(int16_t* current, int16_t* target, uint8_t speed)
{
    if (*current < *target) {
        *current += speed;
        if (*current > *target)
            *current = *target;
    } else if (*current > *target) {
        *current -= speed;
        if (*current < *target)
            *current = *target;
    }
    return *current == *target;
}

// 移动函数
bool move(int16_t* a, int16_t* a_trg) { return move_to_s16(a, a_trg, SPEED); }

// 移动函数
bool move_icon(int16_t* a, int16_t* a_trg) { return move_to_s16(a, a_trg, ICON_SPEED); }

// 宽度移动函数
bool move_width(uint8_t* a, uint8_t* a_trg, uint8_t select, uint8_t id)
{
    uint8_t step = 16 / SPEED;  // 判断步数
    uint8_t len;
    if (ui_index == M_SELECT) {
        len = abs(u8g2_GetStrWidth(&u8g2, list[select].select) - u8g2_GetStrWidth(&u8g2, list[id == 0 ? select + 1 : select - 1].select));
    } else if (ui_index == M_PID) {
        len = abs(u8g2_GetStrWidth(&u8g2, pid[select].select) - u8g2_GetStrWidth(&u8g2, pid[id == 0 ? select + 1 : select - 1].select));
    }
    uint8_t width_speed = ((len % step) == 0 ? (len / step) : (len / step + 1));  // 计算步长

    return move_to_u8(a, a_trg, width_speed);
}

// 进度条移动函�??
bool move_bar(uint8_t* a, uint8_t* a_trg)
{
    static uint8_t step        = 16 / SPEED;                                                                                          // 判断步数
    static uint8_t width_speed = ((single_line_length % step) == 0 ? (single_line_length / step) : (single_line_length / step + 1));  // 计算步长
    return move_to_u8(a, a_trg, width_speed);
}

// 文字编辑函数
void text_edit(bool dir, uint8_t index)
{
    if (!dir) {
        if (name[index] >= 'A' && name[index] <= 'Z')  // 大写字母
        {
            if (name[index] == 'A') {
                name[index] = 'z';
            } else {
                name[index] -= 1;
            }
        } else if (name[index] >= 'a' && name[index] <= 'z')  // 小写字母
        {
            if (name[index] == 'a') {
                name[index] = ' ';
            } else {
                name[index] -= 1;
            }
        } else {
            name[index] = 'Z';
        }
    } else {
        if (name[index] >= 'A' && name[index] <= 'Z')  // 大写字母
        {
            if (name[index] == 'Z') {
                name[index] = ' ';
            } else {
                name[index] += 1;
            }
        } else if (name[index] >= 'a' && name[index] <= 'z')  // 小写字母
        {
            if (name[index] == 'z') {
                name[index] = 'A';
            } else {
                name[index] += 1;
            }
        } else {
            name[index] = 'a';
        }
    }
}

// 消失函数
void effect_disappear()
{
    static uint8_t step = 0;

    switch (step) {
        case 1:
            for (uint16_t i = 0; i < buf_len; ++i)
                if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x55;
            break;
        case 2:
            for (uint16_t i = 0; i < buf_len; ++i)
                if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0xAA;
            break;
        case 3:
            for (uint16_t i = 0; i < buf_len; ++i)
                if (i % 2 == 0) buf_ptr[i] = buf_ptr[i] & 0x00;
            break;
        case 4:
            for (uint16_t i = 0; i < buf_len; ++i)
                if (i % 2 != 0) buf_ptr[i] = buf_ptr[i] & 0x00;
            break;
        default:
            ui_state = S_NONE;
            step     = 0;
            break;
    }
    ++step;
}

/**************************界面显示*******************************/

void logo_ui_show()  // 显示logo
{
}

void select_ui_show()  // 选择界面
{
    move_bar(&line_y, &line_y_trg);
    move(&y, &y_trg);
    move(&box_y, &box_y_trg);
    move_width(&box_width, &box_width_trg, ui_select, key_msg.id);
    u8g2_DrawVLine(&u8g2, 126, 0, total_line_length);
    u8g2_DrawPixel(&u8g2, 125, 0);
    u8g2_DrawPixel(&u8g2, 127, 0);
    for (uint8_t i = 0; i < list_num; ++i) {
        u8g2_DrawStr(&u8g2, x, 16 * i + y + 12, list[i].select);  // 第一段输出位�??
        u8g2_DrawPixel(&u8g2, 125, single_line_length * (i + 1));
        u8g2_DrawPixel(&u8g2, 127, single_line_length * (i + 1));
    }
    u8g2_DrawVLine(&u8g2, 125, line_y, single_line_length - 1);
    u8g2_DrawVLine(&u8g2, 127, line_y, single_line_length - 1);
    u8g2_SetDrawColor(&u8g2, 2);
    u8g2_DrawRBox(&u8g2, 0, box_y, box_width, 16, 1);
    u8g2_SetDrawColor(&u8g2, 1);
}

void pid_ui_show()  // PID界面
{
    move_bar(&pid_line_y, &pid_line_y_trg);
    move(&pid_box_y, &pid_box_y_trg);
    move_width(&pid_box_width, &pid_box_width_trg, pid_select, key_msg.id);
    u8g2_DrawVLine(&u8g2, 126, 0, 61);
    u8g2_DrawPixel(&u8g2, 125, 0);
    u8g2_DrawPixel(&u8g2, 127, 0);
    for (uint8_t i = 0; i < pid_num; ++i) {
        u8g2_DrawStr(&u8g2, x, 16 * i + 12, pid[i].select);  // 第一段输出位�??
        u8g2_DrawPixel(&u8g2, 125, 15 * (i + 1));
        u8g2_DrawPixel(&u8g2, 127, 15 * (i + 1));
    }

    u8g2_SetDrawColor(&u8g2, 2);
    u8g2_DrawRBox(&u8g2, 0, pid_box_y, pid_box_width, 16, 1);
    u8g2_SetDrawColor(&u8g2, 1);
    u8g2_DrawVLine(&u8g2, 125, pid_line_y, 14);
    u8g2_DrawVLine(&u8g2, 127, pid_line_y, 14);
}

void pid_edit_ui_show()  // 显示PID编辑
{
    u8g2_DrawBox(&u8g2, 16, 16, 96, 31);
    u8g2_SetDrawColor(&u8g2, 2);
    u8g2_DrawBox(&u8g2, 17, 17, 94, 29);
    u8g2_SetDrawColor(&u8g2, 1);

    // u8g2_DrawBox(&u8g2,17,17,(int)(Kpid[pid_select]/PID_MAX)),30);
    u8g2_DrawFrame(&u8g2, 18, 36, 60, 8);
    u8g2_DrawBox(&u8g2, 20, 38, (uint8_t)(Kpid[pid_select] / PID_MAX * 56), 4);

    u8g2_SetCursor(&u8g2, 22, 30);

    switch (pid_select) {
        case 0: u8g2_Print(&u8g2, "Editing Kp"); break;
        case 1: u8g2_Print(&u8g2, "Editing Ki"); break;
        case 2: u8g2_Print(&u8g2, "Editing Kd"); break;
        default: break;
    }

    u8g2_SetCursor(&u8g2, 81, 44);

    u8g2_Print2(&u8g2, Kpid[pid_select]);
}

void icon_ui_show(void)  // 显示icon
{
    move_icon(&icon_x, &icon_x_trg);
    move(&app_y, &app_y_trg);

    for (uint8_t i = 0; i < icon_num; ++i) {
        u8g2_DrawXBMP(&u8g2, 46 + icon_x + i * ICON_SPACE, 6, 36, icon_width[i], icon_pic[i]);
        u8g2_SetClipWindow(&u8g2, 0, 48, 128, 64);
        u8g2_DrawStr(&u8g2, (128 - u8g2_GetStrWidth(&u8g2, icon[i].select)) / 2, 62 - app_y + i * 16, icon[i].select);
        u8g2_SetMaxClipWindow(&u8g2);
    }
}

uint16_t analogRead(uint8_t pin)
{
    HAL_ADC_Start(&hadc1);
    return abs((HAL_ADC_GetValue(&hadc1) - 2048)) * 5;
}

void chart_ui_show()  // chart界面
{
    if (!frame_is_drawed)  // 框架只画�??�??
    {
        u8g2_ClearBuffer(&u8g2);
        chart_draw_frame();
        angle_last      = 20.00 + (float)analogRead(READ) / 100.00;
        frame_is_drawed = true;
    }

    u8g2_DrawBox(&u8g2, 96, 0, 30, 14);

    u8g2_DrawVLine(&u8g2, chart_x + 10, 59, 3);
    if (chart_x == 100) chart_x = 0;

    // u8g2_DrawBox(&u8g2,chart_x+11,24,8,32);

    u8g2_DrawVLine(&u8g2, chart_x + 11, 24, 34);
    u8g2_DrawVLine(&u8g2, chart_x + 12, 24, 34);
    u8g2_DrawVLine(&u8g2, chart_x + 13, 24, 34);
    u8g2_DrawVLine(&u8g2, chart_x + 14, 24, 34);

    // 异或绘制
    u8g2_SetDrawColor(&u8g2, 2);
    angle = 20.00 + (float)analogRead(READ) / 100.00;
    u8g2_DrawLine(&u8g2, chart_x + 11, 58 - (int)angle_last / 2, chart_x + 12, 58 - (int)angle / 2);
    u8g2_DrawVLine(&u8g2, chart_x + 12, 59, 3);
    angle_last = angle;
    chart_x += 2;
    u8g2_DrawBox(&u8g2, 96, 0, 30, 14);
    u8g2_SetDrawColor(&u8g2, 1);

    u8g2_SetCursor(&u8g2, 96, 12);
    u8g2_Print2(&u8g2, angle);
}

void chart_draw_frame()  // chart界面
{
    u8g2_DrawStr(&u8g2, 4, 12, "Real time angle :");
    u8g2_DrawRBox(&u8g2, 4, 18, 120, 46, 8);
    u8g2_SetDrawColor(&u8g2, 2);
    u8g2_DrawHLine(&u8g2, 10, 58, 108);
    u8g2_DrawVLine(&u8g2, 10, 24, 34);
    // 箭头
    u8g2_DrawPixel(&u8g2, 7, 27);
    u8g2_DrawPixel(&u8g2, 8, 26);
    u8g2_DrawPixel(&u8g2, 9, 25);

    u8g2_DrawPixel(&u8g2, 116, 59);
    u8g2_DrawPixel(&u8g2, 115, 60);
    u8g2_DrawPixel(&u8g2, 114, 61);
    u8g2_SetDrawColor(&u8g2, 1);
}

void text_edit_ui_show()
{
    u8g2_DrawRFrame(&u8g2, 4, 6, 120, 52, 8);
    u8g2_DrawStr(&u8g2, (128 - u8g2_GetStrWidth(&u8g2, "--Text Editor--")) / 2, 20, "--Text Editor--");
    u8g2_DrawStr(&u8g2, 10, 38, name);
    u8g2_DrawStr(&u8g2, 80, 50, "-Return");

    uint8_t box_x = 9;

    // 绘制光标
    if (edit_index < name_len) {
        if (blink_flag < BLINK_SPEED / 2) {
            for (uint8_t i = 0; i < edit_index; ++i) {
                char temp[2] = {name[i], '\0'};
                box_x += u8g2_GetStrWidth(&u8g2, temp);
                if (name[i] != ' ') {
                    box_x++;
                }
            }
            char temp[2] = {name[edit_index], '\0'};
            u8g2_SetDrawColor(&u8g2, 2);
            u8g2_DrawBox(&u8g2, box_x, 26, u8g2_GetStrWidth(&u8g2, temp) + 2, 16);
            u8g2_SetDrawColor(&u8g2, 1);
        }
    } else {
        u8g2_SetDrawColor(&u8g2, 2);
        u8g2_DrawRBox(&u8g2, 78, 38, u8g2_GetStrWidth(&u8g2, "-Return") + 4, 16, 1);
        u8g2_SetDrawColor(&u8g2, 1);
    }

    if (edit_flag)  // 处于编辑状�??
    {
        if (blink_flag < BLINK_SPEED) {
            blink_flag += 1;
        } else {
            blink_flag = 0;
        }
    } else {
        blink_flag = 0;
    }
}

void about_ui_show()  // about界面
{
    u8g2_DrawStr(&u8g2, 2, 12, "MCU : STM32");
    u8g2_DrawStr(&u8g2, 2, 28, "FLASH : 512KB");
    u8g2_DrawStr(&u8g2, 2, 44, "SRAM : 256KB");
    u8g2_DrawStr(&u8g2, 2, 60, "RTC SRAM : 16KB");
}

/**************************界面处理*******************************/

void logo_proc()  // logo界面处理函数
{
    if (key_msg.pressed) {
        key_msg.pressed = false;
        ui_state        = S_DISAPPEAR;
        ui_index        = M_SELECT;
    }
    logo_ui_show();
}

void pid_edit_proc(void)  // pid编辑界面处理函数
{
    if (key_msg.pressed) {
        key_msg.pressed = false;
        switch (key_msg.id) {
            case 0:
                if (Kpid[pid_select] > 0) Kpid[pid_select] -= 0.01;
                break;
            case 1:
                if (Kpid[pid_select] < PID_MAX) Kpid[pid_select] += 0.01;
                break;
            case 2: ui_index = M_PID; break;
            default: break;
        }
    }
    pid_ui_show();
    for (uint16_t i = 0; i < buf_len; ++i) {
        buf_ptr[i] = buf_ptr[i] & (i % 2 == 0 ? 0x55 : 0xAA);
    }
    pid_edit_ui_show();
}

void pid_proc()  // pid界面处理函数
{
    pid_ui_show();
    if (key_msg.pressed) {
        key_msg.pressed = false;
        switch (key_msg.id) {
            case 0:
                if (pid_select != 0) {
                    pid_select -= 1;
                    pid_line_y_trg -= 15;
                    pid_box_y_trg -= 16;
                    break;
                } else {
                    break;
                }
            case 1:
                if (pid_select != 3) {
                    pid_select += 1;
                    pid_line_y_trg += 15;
                    pid_box_y_trg += 16;
                } else {
                    break;
                }
                break;
            case 2:
                if (pid_select == 3) {
                    ui_index   = M_SELECT;
                    ui_state   = S_DISAPPEAR;
                    pid_select = 0;
                    pid_line_y = pid_line_y_trg = 1;
                    pid_box_y = pid_box_y_trg = 0;
                    pid_box_width = pid_box_width_trg = u8g2_GetStrWidth(&u8g2, pid[pid_select].select) + x * 2;
                } else {
                    ui_index = M_PID_EDIT;
                }
                break;
            default:
                break;
        }
        pid_box_width_trg = u8g2_GetStrWidth(&u8g2, pid[pid_select].select) + x * 2;
    }
}

void select_proc(void)  // 选择界面处理
{
    if (key_msg.pressed) {
        key_msg.pressed = false;
        switch (key_msg.id) {
            case 0:
                if (ui_select < 1) break;
                ui_select -= 1;
                line_y_trg -= single_line_length;
                if (ui_select < -(y / 16)) {
                    y_trg += 16;
                } else {
                    box_y_trg -= 16;
                }

                break;
            case 1:
                if ((ui_select + 2) > (sizeof(list) / sizeof(SELECT_LIST))) break;
                ui_select += 1;
                line_y_trg += single_line_length;
                if ((ui_select + 1) > (4 - y / 16)) {
                    y_trg -= 16;
                } else {
                    box_y_trg += 16;
                }

                break;
            case 2:
                switch (ui_select) {
                    case 0:  // return
                        ui_state = S_DISAPPEAR;
                        ui_index = M_LOGO;
                        break;
                    case 1:  // pid
                        ui_state = S_DISAPPEAR;
                        ui_index = M_PID;
                        break;
                    case 2:  // icon
                        ui_state = S_DISAPPEAR;
                        ui_index = M_ICON;
                        break;
                    case 3:  // chart
                        ui_state = S_DISAPPEAR;
                        ui_index = M_CHART;
                        break;
                    case 4:  // textedit
                        ui_state = S_DISAPPEAR;
                        ui_index = M_TEXT_EDIT;
                        break;
                    case 6:  // about
                        ui_state = S_DISAPPEAR;
                        ui_index = M_ABOUT;
                        break;
                    default:
                        break;
                }
            // Serial.println("Btn2");
            default:
                break;
        }
        // Serial.println(ui_select);
        box_width_trg = u8g2_GetStrWidth(&u8g2, list[ui_select].select) + x * 2;
    }
    select_ui_show();
}

void icon_proc(void)  // icon界面处理
{
    icon_ui_show();
    if (key_msg.pressed) {
        key_msg.pressed = false;
        switch (key_msg.id) {
            case 1:
                if (icon_select != (icon_num - 1)) {
                    icon_select += 1;
                    app_y_trg += 16;
                    icon_x_trg -= ICON_SPACE;
                }
                break;
            case 0:
                if (icon_select != 0) {
                    icon_select -= 1;
                    app_y_trg -= 16;
                    icon_x_trg += ICON_SPACE;
                }
                break;
            case 2:
                ui_state    = S_DISAPPEAR;
                ui_index    = M_SELECT;
                icon_select = 0;
                icon_x = icon_x_trg = 0;
                app_y = app_y_trg = 0;
                break;
            default:
                break;
        }
    }
}

void chart_proc()  // chart界面处理函数
{
    chart_ui_show();
    if (key_msg.pressed) {
        key_msg.pressed = false;
        ui_state        = S_DISAPPEAR;
        ui_index        = M_SELECT;
        frame_is_drawed = false;  // �??出后框架为未画状�??
        chart_x         = 0;
    }
}

void text_edit_proc()
{
    text_edit_ui_show();
    if (key_msg.pressed) {
        key_msg.pressed = false;
        switch (key_msg.id) {
            case 0:
                if (edit_flag) {
                    // 编辑
                    text_edit(false, edit_index);
                } else {
                    if (edit_index == 0) {
                        edit_index = name_len;
                    } else {
                        edit_index -= 1;
                    }
                }
                break;
            case 1:
                if (edit_flag) {
                    // 编辑
                    text_edit(true, edit_index);
                } else {
                    if (edit_index == name_len) {
                        edit_index = 0;
                    } else {
                        edit_index += 1;
                    }
                }
                break;
            case 2:
                if (edit_index == name_len) {
                    ui_state   = S_DISAPPEAR;
                    ui_index   = M_SELECT;
                    edit_index = 0;
                } else {
                    edit_flag = !edit_flag;
                }
                break;
            default:
                break;
        }
    }
}

void about_proc()  // about界面处理函数
{
    if (key_msg.pressed) {
        key_msg.pressed = false;
        ui_state        = S_DISAPPEAR;
        ui_index        = M_SELECT;
    }
    about_ui_show();
}
/********************************总的UI显示************************************/

void ui_proc()  // 总的UI进程
{
    switch (ui_state) {
        case S_NONE:
            if (ui_index != M_CHART) u8g2_ClearBuffer(&u8g2);
            switch (ui_index) {
                case M_LOGO: logo_proc(); break;
                case M_SELECT: select_proc(); break;
                case M_PID: pid_proc(); break;
                case M_ICON: icon_proc(); break;
                case M_CHART: chart_proc(); break;
                case M_TEXT_EDIT: text_edit_proc(); break;
                case M_PID_EDIT: pid_edit_proc(); break;
                case M_ABOUT: about_proc(); break;
                default: break;
            }
            break;
        case S_DISAPPEAR: effect_disappear(); break;
        default: break;
    }
    u8g2_SendBuffer(&u8g2);
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
    MX_ADC1_Init();
    /* USER CODE BEGIN 2 */
    HAL_ADCEx_Calibration_Start(&hadc1);
    /* USER CODE END 2 */
    matrix_key_init();
    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    u8g2Init(&u8g2);

    // pinMode(BTN0, INPUT_PULLUP);
    // pinMode(BTN1, INPUT_PULLUP);
    // pinMode(BTN2, INPUT_PULLUP);
    key_init();
    // u8g2_SetBusClock(&u8g2, 800000);

    u8g2_SetFont(&u8g2, u8g2_font_wqy12_t_chinese1);
    // u8g2_SetContrast(&u8g2,10);

    buf_ptr = u8g2_GetBufferPtr(&u8g2);  // 拿到buffer首地�??
    buf_len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);

    x = 4;
    y = y_trg = 0;
    line_y = line_y_trg = 1;
    pid_line_y = pid_line_y_trg = 1;
    ui_select = pid_select = icon_select = 0;
    icon_x = icon_x_trg = 0;
    app_y = app_y_trg = 0;

    box_width = box_width_trg = u8g2_GetStrWidth(&u8g2, list[ui_select].select) + x * 2;          // 两边各多2
    pid_box_width = pid_box_width_trg = u8g2_GetStrWidth(&u8g2, pid[pid_select].select) + x * 2;  // 两边各多2

    ui_index = M_LOGO;
    // ui_index=M_TEXT_EDIT;
    ui_state = S_NONE;

    while (1) {
        key_scan();
        ui_proc();
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
