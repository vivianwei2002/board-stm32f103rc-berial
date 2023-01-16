#ifndef __EASING_H__
#define __EASING_H__

#include "base.hpp"

class easing_core {
public:
    // mode
    enum mode {
        MODE_BITCNT = 3,
        MODE_MASK   = (1 << MODE_BITCNT) - 1,

        // 次数:
        TIMES_CUSTOM   = 0 << 0,  // 有限
        TIMES_INFINITE = 1 << 0,  // 无限

        TIMES_SET = MODE_BITCNT,

        // 方向:
        DIR_FORWARD      = 0 << 0,  // 正向(default)
        DIR_REVERSE      = 1 << 1,  // 反向
        DIR_BACKANDFORTH = 1 << 2,  // 往返
    };

#define EASING_MODE_NTIMES(times, dir) \
    (easing_core::mode)(easing_core::mode::TIMES_CUSTOM | (times << easing_core::mode::TIMES_SET) | dir)

public:
    easing_core(
        easing_pos    pItems[],
        uint32_t      nLength,
        uint32_t      nFrameCount = 2,  // greater than or equal to 2
        mode          dwMode      = EASING_MODE_NTIMES(1, DIR_FORWARD),
        easing_calc_t lpfnCalc    = _easing_calc_Linear,
        uint32_t      nInterval   = 0)
    {
        assert_param(pItems);
        assert_param(nLength);
        assert_param(lpfnCalc);
        assert_param(nFrameCount >= 2);

        m_dwMode = dwMode;

        m_lpfnCalc = lpfnCalc;

        m_pItems  = pItems;
        m_nLength = nLength;

        m_nFrameIndex = 0;
        m_nFrameCount = nFrameCount;

        m_fProgress    = 0.f;
        m_fCoefficient = 0.f;

        m_nTimes = 0;

        m_bDirection = dwMode & mode::DIR_REVERSE;
#ifdef easing_mills
        m_nInterval = nInterval;
#endif
    }

    ~easing_core() {}

    void start(void);

    // update easing position
    void update(void);

    void stop(void) { m_nTimes = 0; }

    bool isok(void) { return m_nTimes == 0; }

    easing_type_t curpos(uint32_t index) { return m_pItems[index].curpos(); }

    float progress(void) { return m_fProgress; }
    float coefficient(void) { return m_fCoefficient; }

    uint32_t frameIndex(void) { return m_nFrameIndex; }
    uint32_t frameCount(void) { return m_nFrameCount; }

    // operator
    easing_pos& operator[](uint32_t index) { return m_pItems[index]; }

private:
    mode m_dwMode;

    // positions
    easing_pos* m_pItems;
    uint32_t    m_nLength;

    easing_calc_t m_lpfnCalc;
    // calc type: _easing_calc_xxx

    // range: [2,n]
    // nFrameCount = 2 -> nCurrent = nStart or nStop
    // nFrameCount = n -> nCurrent = nStart...nStop
    uint32_t m_nFrameCount;

    // current frame. range: [0, nFrameCount],
    // nFrameIndex = 0 -> nCurrent = nStart/nStop;
    // nFrameIndex = nFrameCount -> nCurrent = nStop/nStart;
    uint32_t m_nFrameIndex;

    // current progress. range: [0,1]
    float m_fProgress;

    // current coefficient = _easing_calc_xxx(progress)
    float m_fCoefficient;

    // <0: infinite times
    // =0: easing ok
    // >0: remaining  times
    int32_t m_nTimes;

    // true: reverse, false：forward
    bool m_bDirection;

#ifdef easing_mills
    // record the start time of the next frame
    uint32_t m_nMills;

    // minimum time interval per frame (ms)
    uint32_t m_nInterval;
#endif
};

void easing_core::start(void)
{
    m_nFrameIndex = 0;  // first frame is nStart
    m_fProgress   = 0.f;

    m_bDirection = m_dwMode & mode::DIR_REVERSE;

    if (m_dwMode & mode::TIMES_INFINITE) {
        m_nTimes = -1;
    } else {
        m_nTimes = m_dwMode >> mode::TIMES_SET;
        if (m_nTimes == 0) m_nTimes = 1;
        if (m_dwMode & mode::DIR_BACKANDFORTH) m_nTimes *= 2;
    }

#ifdef easing_mills
    m_nMills = easing_mills();
#endif
}

void easing_core::update(void)
{
    // isok
    if (isok()) return;

#ifdef easing_mills
    if (m_nInterval > 0) {
        if (easing_mills() < m_nMills) return;
        m_nMills = easing_mills() + m_nInterval;
    }
#endif

    // next frame
    ++m_nFrameIndex;

    if (m_nFrameIndex > m_nFrameCount) {
        if (m_dwMode & mode::DIR_BACKANDFORTH) {
            // reverse direction
            m_bDirection = !m_bDirection;
            // skip once nStart/nStop pos
            m_nFrameIndex = 2;
        } else {
            // at first frame
            m_nFrameIndex = 1;
        }
    }

    easing_pos* src  = m_pItems;
    easing_pos* dest = m_pItems + m_nLength;

    if (m_nFrameIndex == m_nFrameCount) {
        // at last frame
        m_fProgress    = 1.f;
        m_fCoefficient = 1.f;
        while (src < dest) {
            if (src->m_bEnable) src->m_nCurrent = m_bDirection ? src->m_nStart : src->m_nStop;
            ++src;
        }
        // decrease times
        if (!(m_dwMode & mode::TIMES_INFINITE)) --m_nTimes;
    } else {
        // calculate progress
        m_fProgress = (float)(m_nFrameIndex - 1) / (m_nFrameCount - 1);
        // calculate position
        m_fCoefficient = m_lpfnCalc(m_fProgress);
        while (src < dest) {
            if (src->m_bEnable)
                src->m_nCurrent = m_bDirection ? (src->m_nStop - m_fCoefficient * src->m_nDelta) : (src->m_nStart + m_fCoefficient * src->m_nDelta);
            ++src;
        }
    }
}

class easing_fast : public easing_core, public easing_pos {
public:
    easing_fast(easing_type_t nCurrent    = 0,
                easing_type_t nOffset     = 0,
                uint32_t      nFrameCount = 2,  // greater than or equal to 2
                mode          dwMode      = EASING_MODE_NTIMES(1, DIR_FORWARD),
                easing_calc_t lpfnCalc    = _easing_calc_Linear,
                uint32_t      nInterval   = 0)
        : easing_pos(nCurrent, nOffset), easing_core(this, 1, nFrameCount, dwMode, lpfnCalc, nInterval)
    {
    }
    ~easing_fast() {}

    easing_type_t curpos(void) { return easing_pos::curpos(); }
};

#endif