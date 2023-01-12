
#include "easing.h"

#define PI 3.1415926

static const float DH  = 1. / 22.;
static const float D1  = 1. / 11.;
static const float D2  = 2. / 11.;
static const float D3  = 3. / 11.;
static const float D4  = 4. / 11.;
static const float D5  = 5. / 11.;
static const float D7  = 7. / 11.;
static const float IH  = 1. / (1. / 22.);
static const float I1  = 1. / (1. / 11.);
static const float I2  = 1. / (2. / 11.);
static const float I4D = 1. / (4. / 11.) / (4. / 11.);
// static const float IH  = 1. / DH;
// static const float I1  = 1. / D1;
// static const float I2  = 1. / D2;
// static const float I4D = 1. / D4 / D4;

float _easing_calc_InBounce(const float t)
{
    float s;
    if (t < D1) {
        s = t - DH;
        s = DH - s * s * IH;
    } else if (t < D3) {
        s = t - D2;
        s = D1 - s * s * I1;
    } else if (t < D7) {
        s = t - D5;
        s = D2 - s * s * I2;
    } else {
        s = t - 1;
        s = 1 - s * s * I4D;
    }
    return s;
}

float _easing_calc_OutBounce(const float t)
{
    return 1. - _easing_calc_InBounce(1. - t);
}

float _easing_calc_InOutBounce(const float t)
{
    return (t < 0.5) ? _easing_calc_InBounce(t * 2.0) * 0.5 : 1 - _easing_calc_InBounce(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_InCirc(const float t)
{
    return 1.0 - sqrtf(1.0 - t * t);
}

float _easing_calc_OutCirc(const float t)
{
    return 1. - _easing_calc_InCirc(1. - t);
}

float _easing_calc_InOutCirc(const float t)
{
    return (t < 0.5) ? _easing_calc_InCirc(t * 2.0) * 0.5 : 1 - _easing_calc_InCirc(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_InCubic(const float t)
{
    return t * t * t;
}

float _easing_calc_OutCubic(const float t)
{
    return 1. - _easing_calc_InCubic(1. - t);
}

float _easing_calc_InOutCubic(const float t)
{
    return (t < 0.5) ? _easing_calc_InCubic(t * 2.0) * 0.5 : 1 - _easing_calc_InCubic(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_OutElastic(const float t)
{
    float s = 1 - t;
    return 1 - powf(s, 8) + sinf(t * t * 6 * PI) * s * s;
}

float _easing_calc_InElastic(const float t)
{
    return 1.0 - _easing_calc_OutElastic(1.0 - t);
}

float _easing_calc_InOutElastic(const float t)
{
    return (t < 0.5) ? _easing_calc_InElastic(t * 2.0) * 0.5 : 1 - _easing_calc_InElastic(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_InExpo(const float t)
{
    return powf(2, 10 * (t - 1));
}

float _easing_calc_OutExpo(const float t)
{
    return 1.0 - powf(2, -10 * t);
}

float _easing_calc_InOutExpo(const float t)
{
    return (t < 0.5) ? _easing_calc_InExpo(t * 2.0) * 0.5 : 1 - _easing_calc_InExpo(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_Linear(const float t)
{
    return t;
}

float _easing_calc_InQuad(const float t)
{
    return t * t;
}

float _easing_calc_OutQuad(const float t)
{
    return 1. - _easing_calc_InQuad(1. - t);
}

float _easing_calc_InOutQuad(const float t)
{
    return (t < 0.5) ? _easing_calc_InQuad(t * 2.0) * 0.5 : 1 - _easing_calc_InQuad(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_InQuart(const float t)
{
    return t * t * t * t;
}

float _easing_calc_OutQuart(const float t)
{
    return 1. - _easing_calc_InQuart(1. - t);
}

float _easing_calc_InOutQuart(const float t)
{
    return (t < 0.5) ? _easing_calc_InQuart(t * 2.0) * 0.5 : 1 - _easing_calc_InQuart(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_InQuint(const float t)
{
    return t * t * t * t * t;
}

float _easing_calc_OutQuint(const float t)
{
    return 1. - _easing_calc_InQuint(1. - t);
}

float _easing_calc_InOutQuint(const float t)
{
    return (t < 0.5) ? _easing_calc_InQuint(t * 2.0) * 0.5 : 1 - _easing_calc_InQuint(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_InSine(const float t)
{
    return 1.0 - cosf(t * (PI / 2));
}

float _easing_calc_OutSine(const float t)
{
    return 1. - _easing_calc_InSine(1. - t);
}

float _easing_calc_InOutSine(const float t)
{
    return (t < 0.5) ? _easing_calc_InSine(t * 2.0) * 0.5 : 1 - _easing_calc_InSine(2.0 - t * 2.0) * 0.5;
}

float _easing_calc_InBack(const float t)
{
    return 3 * t * t * t - 2 * t * t;
}

float _easing_calc_OutBack(const float t)
{
    return 1. - _easing_calc_InBack(1. - t);
}

float _easing_calc_InOutBack(const float t)
{
    return (t < 0.5) ? _easing_calc_InBack(t * 2.0) * 0.5 : 1 - _easing_calc_InBack(2.0 - t * 2.0) * 0.5;
}

////////////////////////////////////////////////////////////////////////////////////

easing_t easing_create(
    easing_mode_t dwMode,
    easing_calc_t lpfnCalc,
    easing_pos_t  nCurrent,
    uint8_t       nFrameCount,
    uint16_t      nInterval)
{
    if (nFrameCount == 0)
        nFrameCount = 1;

    easing_t easing = {
        .dwMode      = dwMode,
        .lpfnCalc    = lpfnCalc == 0 ? _easing_calc_Linear : lpfnCalc,
        .nStart      = nCurrent,
        .nStop       = nCurrent,
        .nDelta      = 0,
        .nCurrent    = nCurrent,
        .nFrameIndex = nFrameCount,
        .nFrameCount = nFrameCount,
        .fProgress   = 1.0f,
        .nInterval   = nInterval,
        .nMills      = 0,
        .nDirection  = 0,
        .nTimes      = 0,
    };
    return easing;
}

void easing_start_absolute(
    easing_t*    pEasing,
    easing_pos_t nStart,
    easing_pos_t nStop,
    uint8_t      nTimes)
{
    pEasing->nStart = nStart;
    pEasing->nStop  = nStop;
    pEasing->nDelta = nStop - nStart;

    if (pEasing->dwMode & EASING_DIR_REVERSE) {
        pEasing->nDirection  = -1;
        pEasing->fProgress   = 1;
        pEasing->nFrameIndex = pEasing->nFrameCount;
    } else {
        pEasing->nDirection  = +1;
        pEasing->nFrameIndex = 0;
        pEasing->fProgress   = 0;
    }

    if (pEasing->dwMode & EASING_TIMES_FOREVER) {
        pEasing->nTimes = -1;
    } else {
        pEasing->nTimes = ((pEasing->dwMode & EASING_TIMES_MANYTIMES) && (nTimes > 0)) ? nTimes : 1;
        if (pEasing->dwMode & EASING_DIR_BACKANDFORTH) pEasing->nTimes *= 2;
    }

#ifdef easing_mills
    pEasing->nMills = easing_mills();
#endif
}

void easing_start_relative(
    easing_t*    pEasing,
    easing_pos_t nDistance,
    uint8_t      nTimes)
{
    easing_start_absolute(
        pEasing,
#if 1
        pEasing->nCurrent,  // from current pos
#else
        easing->nStop,  // from stop pos
#endif
        pEasing->nStop + nDistance,
        nTimes);
}

void easing_update(easing_t* pEasing)
{
    if (pEasing->nTimes == 0) return;

#ifdef easing_mills
    if (pEasing->nInterval > 0) {
        if (easing_mills() < pEasing->nMills) return;
        pEasing->nMills = easing_mills() + pEasing->nInterval;
    }
#endif

    pEasing->nFrameIndex += pEasing->nDirection;
    if (pEasing->nFrameIndex == 0) {
        pEasing->fProgress = 0.f;
        pEasing->nCurrent  = pEasing->nStart;
    } else if (pEasing->nFrameIndex == pEasing->nFrameCount) {
        pEasing->fProgress = 1.f;
        pEasing->nCurrent  = pEasing->nStop;
    } else {
        pEasing->fProgress = (float)pEasing->nFrameIndex / (pEasing->nFrameCount);
        pEasing->nCurrent  = pEasing->nStart + pEasing->nDelta * pEasing->lpfnCalc(pEasing->fProgress);
        return;
    }

    if (pEasing->nTimes > 0)
        --pEasing->nTimes;
    if (pEasing->dwMode & EASING_DIR_BACKANDFORTH)
        pEasing->nDirection = -pEasing->nDirection;
}
