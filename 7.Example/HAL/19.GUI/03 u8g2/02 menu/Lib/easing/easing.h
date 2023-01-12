
// https://blog.csdn.net/z2014z/article/details/120691794

#ifndef __EASING_H__
#define __EASING_H__

#ifdef __cpluscplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include <math.h>

// #define easing_mills() HAL_GetTick()

////////////////

typedef float (*easing_calc_t)(const float t);

typedef float easing_pos_t;  // data type

typedef enum {
    EASING_TIMES_SINGLE     = 0 << 0,  // 单次(default)
    EASING_TIMES_MANYTIMES  = 1 << 0,  // 多次
    EASING_TIMES_FOREVER    = 1 << 1,  // 循环
    EASING_DIR_FORWARD      = 0 << 0,  // 正向(default)
    EASING_DIR_REVERSE      = 1 << 2,  // 反向
    EASING_DIR_BACKANDFORTH = 1 << 3,  // 来回
} easing_mode_t;

typedef enum {
    EASING_INTERVAL_NONE   = 0,
    EASING_INTERVAL_5MS    = 5,
    EASING_INTERVAL_10MS   = 10,
    EASING_INTERVAL_20MS   = 20,
    EASING_INTERVAL_50MS   = 50,
    EASING_INTERVAL_100MS  = 100,
    EASING_INTERVAL_200MS  = 200,
    EASING_INTERVAL_500MS  = 500,
    EASING_INTERVAL_1000MS = 1000,
} easing_interval_t;

typedef enum {
    EASING_STATE_PAUSE,
} easing_state_t;

typedef struct easing {
    easing_mode_t dwMode;

    // type
    easing_calc_t lpfnCalc;  // _easing_calc_xxx

    // position
    easing_pos_t nStart;
    easing_pos_t nStop;
    easing_pos_t nDelta;
    easing_pos_t nCurrent;  // range: [nStart, nStop]

    // progress
    uint8_t nFrameCount;
    uint8_t nFrameIndex;  // current frame
    float   fProgress;    // current progress, range: [0,1]

    int16_t nTimes;
    int8_t  nDirection;

    uint32_t nMills;
    uint16_t nInterval;  // time interval per frame (ms)
} easing_t;

//////////////// _easing_calc_xxx

float _easing_calc_Linear(const float t);  // linear t

float _easing_calc_InQuad(const float t);  // quadratic t^2
float _easing_calc_OutQuad(const float t);
float _easing_calc_InOutQuad(const float t);

float _easing_calc_InCubic(const float t);  // cubic t^3
float _easing_calc_OutCubic(const float t);
float _easing_calc_InOutCubic(const float t);

float _easing_calc_InQuart(const float t);  // quartic t^4
float _easing_calc_OutQuart(const float t);
float _easing_calc_InOutQuart(const float t);

float _easing_calc_InQuint(const float t);  // quintic t^5
float _easing_calc_OutQuint(const float t);
float _easing_calc_InOutQuint(const float t);

float _easing_calc_InSine(const float t);  // sinusoidal 正弦 sin(t)
float _easing_calc_OutSine(const float t);
float _easing_calc_InOutSine(const float t);

float _easing_calc_InExpo(const float t);  // exponential 指数 2^t
float _easing_calc_OutExpo(const float t);
float _easing_calc_InOutExpo(const float t);

float _easing_calc_InCirc(const float t);  // circular 圆形
float _easing_calc_OutCirc(const float t);
float _easing_calc_InOutCirc(const float t);

float _easing_calc_InBack(const float t);  // elastic 衰减三次幂 (s+1)t^3 - st^2
float _easing_calc_OutBack(const float t);
float _easing_calc_InOutBack(const float t);

float _easing_calc_InElastic(const float t);  // elastic 衰减正弦
float _easing_calc_OutElastic(const float t);
float _easing_calc_InOutElastic(const float t);

float _easing_calc_InBounce(const float t);  // back 衰减反弹
float _easing_calc_OutBounce(const float t);
float _easing_calc_InOutBounce(const float t);

////////////////

#define EASING_IS_RUNNING(easing) (easing.nTimes != 0)

easing_t easing_create(
    easing_mode_t dwMode,  // default: EASING_TIMES_SINGLE | EASING_DIR_FORWARD
    easing_calc_t lpfnCalc,
    easing_pos_t  nCurrent,
    uint8_t       nFrameCount,
    uint16_t      nInterval);

void easing_start_absolute(
    easing_t*    pEasing,
    easing_pos_t nStart,
    easing_pos_t nStop,
    uint8_t      nTimes);

void easing_start_relative(
    easing_t*    pEasing,
    easing_pos_t nDistance,
    uint8_t      nTimes);

// update easing position
void easing_update(easing_t* easing);

#ifdef __cpluscplus
}
#endif

#endif