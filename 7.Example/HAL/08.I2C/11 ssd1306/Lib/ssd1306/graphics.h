// crop from u8g2

#ifndef _GRAPHICS_LIB_H_  // gl
#define _GRAPHICS_LIB_H_

#include "ssd1306_i2c.h"

#include <math.h>
#include <stdint.h>
#include <stdbool.h>

////////////////////////////////////////////////////// lib config

#define CONFIG_ENABLE_GRAPHICS 1
#define CONFIG_ENABLE_CLIPRECT 1
#define CONFIG_ENABLE_CIRCLE   1
#define CONFIG_ENABLE_STRING   1

#define SCREEN_WIDTH       SSD1306_HEIGHT
#define SCREEN_HEIGHT      SSD1306_WIDTH
#define SCREEN_ROW_COUNT   SSD1306_ROW_COUNT
#define SCREEN_COL_COUNT   SSD1306_COL_COUNT
#define SCREEN_BUFFER_SZIE SSD1306_BUFFER_SZIE

// #define gl_int  int8_t
// #define gl_uint uint8_t
// #define gl_int  int16_t
// #define gl_uint uint16_t
#define gl_int  int32_t
#define gl_uint uint32_t
#define gl_real float_t
// #define gl_real double_t

#define gl_round roundf
#define gl_cos   cosf
#define gl_sin   sinf
#define gl_atan2 atan2f
#define gl_sqrt  sqrtf

// #define gl_round round
// #define gl_cos   cos
// #define gl_sin   sin
// #define gl_atan2 atan2
// #define gl_sqrt  sqrt

////////////////////////////////////////////////////// define

#define SCREEN_HCENTER (SCREEN_WIDTH / 2)
#define SCREEN_VCENTER (SCREEN_HEIGHT / 2)
#define SCREEN_LEFT    (0)
#define SCREEN_TOP     (0)
#define SCREEN_RIGHT   (SCREEN_WIDTH - 1)
#define SCREEN_BOTTOM  (SCREEN_HEIGHT - 1)

#ifndef PI
#define PI 3.1415926f
#endif

////////////////////////////////////////////////////// types

typedef enum gl_color {
    color_black,
    color_white,
    color_xor,
} gl_color;

typedef enum gl_selection {  // for circle & ellipse
    Selection_None        = 0,
    Selection_LeftTop     = 1 << 1,
    Selection_RightTop    = 1 << 2,
    Selection_RightBottom = 1 << 3,
    Selection_LeftBottom  = 1 << 4,
    Selection_Top         = Selection_LeftTop | Selection_RightTop,
    Selection_Bottom      = Selection_LeftBottom | Selection_RightBottom,
    Selection_Left        = Selection_LeftTop | Selection_LeftBottom,
    Selection_Right       = Selection_RightTop | Selection_RightBottom,
    Selection_ALL         = Selection_Left | Selection_Right,
} gl_selection;

////////////////////////////////////////////////////// variables

static uint8_t screen_buffer[SCREEN_ROW_COUNT][SCREEN_COL_COUNT] = {0};

////////////////////////////////////////////////////// functions

void gl_update(void);  // core interface

#if CONFIG_ENABLE_CLIPRECT

void gl_set_cliprect(gl_int x, gl_int y, gl_uint w, gl_uint h);  // ok
void gl_reset_cliprect(void);                                    // ok

#endif

void gl_draw_pixel(gl_int x, gl_int y, gl_color cr);  // ok

void gl_draw_hline(gl_int x, gl_int y, gl_uint w, gl_color cr);              // ok
void gl_draw_vline(gl_int x, gl_int y, gl_uint h, gl_color cr);              // ok
void gl_draw_line(gl_int x1, gl_int y1, gl_int x2, gl_int y2, gl_color cr);  // ok

void gl_draw_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_color cr);  // ok
void gl_fill_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_color cr);  // ok

#if CONFIG_ENABLE_CIRCLE

void gl_draw_rounded_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_uint r, gl_color cr);  // ok
void gl_fill_rounded_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_uint r, gl_color cr);  // ok

void gl_draw_circle(gl_int xc, gl_int yc, gl_uint r, gl_color cr, gl_selection opt);  // ok
void gl_fill_circle(gl_int xc, gl_int yc, gl_uint r, gl_color cr, gl_selection opt);  // ok

void gl_draw_ellipse(gl_int xc, gl_int yc, gl_uint rx, gl_uint ry, gl_color cr, gl_selection opt);  // ok
void gl_fill_ellipse(gl_int xc, gl_int yc, gl_uint rx, gl_uint ry, gl_color cr, gl_selection opt);  // ok
void gl_draw_ellipse_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_color cr);                   // ok

void gl_draw_arc(gl_int cx, gl_int cy, gl_uint r, gl_real start_angle, gl_real end_angle, gl_color cr);  // ok
void gl_draw_pie(gl_int cx, gl_int cy, gl_uint r, gl_real start_angle, gl_real end_angle, gl_color cr);  // ok
// void gl_fill_pie(INT cx, INT cy, UINT r, float start_angle, float end_angle, gl_color cr);

#endif

#if CONFIG_ENABLE_STRING

// void gl_draw_string();  // at center
// INT  gl_printf(const char* str, ...);

#endif

void gl_draw_triangle(gl_int x0, gl_int y0, gl_int x1, gl_int y1, gl_int x2, gl_int y2, gl_color cr);  // ok
void gl_fill_triangle(gl_int x0, gl_int y0, gl_int x1, gl_int y1, gl_int x2, gl_int y2, gl_color cr);  // ok

void gl_draw_polygon(gl_int x[], gl_int y[], gl_uint len, bool closed, gl_color cr);

void get_rotate_point(gl_int xc, gl_int yc, gl_int x1, gl_int y1, gl_int* x2, gl_int* y2, gl_real angle, bool dir);

//////////////////////////// helper

#if 0  // math

static float _sqrtf(float x)
{
    float    a = x;
    uint32_t i = *(uint32_t*)&x;

    i = (i + 0x3f76cf62) >> 1;
    x = *(float*)&i;
    x = (x + a / x) * 0.5f;

    return x;
}

static inline int _roundf(float n)
{
    if (n - (int)n > 0.5f)
        return (int)n + 1;
    return (int)n;
}

#endif

#if 1

#define _swap_int(a, b) \
    (b) = (a) + (b);    \
    (a) = (b) - (a);    \
    (b) = (b) - (a);

#else

static inline void _swap_int(int* pa, int* pb)
{
    int t = *pa;
    *pa   = *pb;
    *pb   = t;
}

#endif

#if 0

#define _abs(n) ((n > 0) ? n : -n)
#else

static inline int _abs_int(int n)
{
    return (n > 0) ? n : -n;
}

#endif

#endif
