#ifndef __ITEM_H__
#define __ITEM_H__

#include "apps/base.h"

template <typename T = easing_type_t>
class vec2 {
private:
    T m_x, m_y;

public:
    vec2(void) : m_x(0), m_y(0) {}
    vec2(T v) : m_x(v), m_y(v) {}
    vec2(T x, T y) : m_x(x), m_y(y) {}
    ~vec2() {}

    // setter
    T x(const T v) { m_x = v; }
    T y(const T v) { m_y = v; }
    // getter
    T x(void) const { return m_x; }
    T y(void) const { return m_y; }
    T w(void) const { return m_x; }
    T h(void) const { return m_y; }
};

///////

enum {
    PADDING_X = 4,
    PADDING_Y = 4,
    FONTSIZE  = 8,
};

template <typename T = easing_type_t>
class item {
public:
    // callback
    typedef void (*cbk_t)(item<T>* item, void* arg1, void* arg2);

    // style
    enum style {
        DEFAULT           = 0 << 0,
        UNSELECTABLE      = 1 << 0,  // Unselectable
        CAPTURE_KEY_FIRST = 1 << 1,  // Capture keys first 优先捕获按键
    };

protected:
    vec2<T> m_offset;  // x,y
    vec2<T> m_size;    // w,h

    const char* m_title;

    cbk_t m_cbk;
    void* m_arg1;
    void* m_arg2;

    style m_style : 2;

public:
    item(const char* title,
         style       style = style::DEFAULT,
         cbk_t       cbk   = nullptr,
         void*       arg1  = nullptr,
         void*       arg2  = nullptr)
        : m_title(title), m_style(style), m_cbk(cbk), m_arg1(arg1), m_arg2(arg2) {}
    ~item() {}

    void render(const vec2<T>& pos)
    {
        uint8_t h = PADDING_Y + FONTSIZE + pos.y();
        if (m_style & UNSELECTABLE)
            _draw_vline(pos.x(), pos.y(), h);
        _draw_str(PADDING_X + pos.x(), h, m_title);
    }

    bool event(key_id_t key)
    {
        switch (key) {
            case KEY_ID_OK: {
                if (m_cbk) m_cbk(this, m_arg1, m_arg2);
                println("%s", m_title);
                break;
            }
            default: break;
        }
        return true;
    }

    const char* title(void) const { return m_title; }

    item& set(cbk_t cbk, void* arg1, void* arg2)
    {
        m_cbk  = cbk;
        m_arg1 = arg1;
        m_arg2 = arg2;
    }

    style style(void) const { return m_style; }
    item& set(style style)
    {
        m_style = style;
        return *this;
    }
    // void addStyle(t style) { m_style |= style; }
    // void delStyle(t style) { m_style &= ~style; }

    vec2<T> offset(void) const { return m_offset; }
    vec2<T> size(void) const { return m_size; }

    vec2<T> maskpos(void) const { return vec2<T>(0, 0); }
    vec2<T> masksize(void) const { return vec2<T>(PADDING_X * 2 + _str_w_(m_title), PADDING_Y * 2 + FONTSIZE); }
};

template <typename T>
class item_checkbox : public item<T> {
private:
public:
    item_checkbox() {}
    ~item_checkbox() {}
};

//////

template <typename easing_type_t>
class item;

// template <uint8_t capacity = 1>
class list {
private:
    // items
    item<easing_type_t>* m_pItems;
    int8_t               m_nLength;

    // index of selected item
    int8_t m_nMaskedIndex;
    int8_t m_nSelectedIndex;

    int8_t m_nDisplayedCount;

    // easing
    // easing_t m_MaskY;

    easing_type_t m_ScrollX;
    easing_fast   m_SliderY;
    easing_type_t m_SliderH;

    easing_type_t m_LineH;

    easing_core m_EasingCore;
    easing_pos  m_EasingPos[6];
    easing_pos& m_ItemX = m_EasingPos[0];
    easing_pos& m_ItemY = m_EasingPos[1];
    easing_pos& m_MaskX = m_EasingPos[2];
    easing_pos& m_MaskY = m_EasingPos[3];
    easing_pos& m_MaskW = m_EasingPos[4];
    easing_pos& m_MaskH = m_EasingPos[5];

    // repaint flag
    bool m_bRepaint : 1;

private:
    // #define CORRECT(v) ((v - (uint8_t)v) > 0.5f)
    easing_type_t CORRECT(easing_type_t v) { return (v - (uint8_t)v) > 0.5f; }

protected:
    /**
     * @brief update all easing position
     */
    void updateEasing(void)
    {
        m_EasingCore.update();
        m_SliderY.update();
    }

    /**
     * @brief check all easing is ok
     */
    bool checkEasing(void)
    {
        return m_EasingCore.isok() &&
               m_SliderY.isok();
    }

    // render
    void renderItems(void)
    {
        // return;
        easing_type_t yoffset = -m_ItemY.curpos();
        for (uint8_t i = 0; (i < m_nLength) && (yoffset < CONFIG_SCREEN_HEIGHT); ++i, yoffset += m_LineH) {
            if (yoffset < 0) continue;
            u8g2_SetClipWindow(&u8g2, 0, yoffset, CONFIG_SCREEN_WIDTH, yoffset + m_LineH);
            m_pItems[i].render(vec2<easing_type_t>(0, yoffset));
            u8g2_SetMaxClipWindow(&u8g2);
        }
    }

    void renderMask(void)
    {
        _set_color(2);
        _draw_rounded_rect(
            m_MaskX.curpos(),
            m_MaskY.curpos(),
            m_MaskW.curpos(),
            m_MaskH.curpos() + CORRECT(m_MaskY.curpos()),
            1);
        _set_color(1);
    }

    void renderScrollbar(void) {}

public:
    list(item<easing_type_t>* items, uint8_t length) : m_nDisplayedCount(4)
    {
        const uint8_t nFrameCount = 4;

        m_EasingPos = easing_core(easing_pos, ARRAY_SIZE(easing_pos), nFrameCount);

        m_SliderY = easing_fast(0, 0, nFrameCount);

        m_LineH = (easing_type_t)CONFIG_SCREEN_HEIGHT / m_nDisplayedCount;

        setItems(items, length);
    }
    ~list(void) {}

    /**
     * @return item index, -1: fail to add, >0: success
     */
    void setItems(item<easing_type_t>* items, uint8_t length)
    {
        m_nSelectedIndex = m_nMaskedIndex = 0;

        m_ItemX = m_ItemY = m_SliderY = 0.f;

        if ((m_nLength = length) == 0) {
            // null
            m_pItems = nullptr;

            m_MaskX.moveto(0);
            m_MaskY.moveto(0);
            m_MaskW.moveto(0);
            m_MaskH.moveto(0);

            m_SliderH = (easing_type_t)CONFIG_SCREEN_HEIGHT;
        } else {
            // not null
            m_pItems = items;

            vec2<easing_type_t> offset = m_pItems[0].maskpos();
            vec2<easing_type_t> size   = m_pItems[0].masksize();

            m_MaskX.moveto(offset.x());
            m_MaskY.moveto(offset.y());
            m_MaskW.moveto(size.w());
            m_MaskH.moveto(size.h());

            m_SliderH = (easing_type_t)CONFIG_SCREEN_HEIGHT / m_nLength;
        }
        m_SliderY  = 0;
        m_bRepaint = true;
    }

    void render(void)
    {
        if (m_bRepaint) {
            _clear();
            if (m_pItems) {  // no null
                println("repaint");
                m_bRepaint = !checkEasing();
                updateEasing();
                renderItems();
                renderScrollbar();
                renderMask();
            } else {
                m_bRepaint = false;
            }
            _update();
        }
    }

    void switchToPrev(void)
    {
        // seek prev selectable index
        int8_t nPrevIndex = m_nSelectedIndex;
        do {
            if (--nPrevIndex == -1) return;
        } while (m_pItems[nPrevIndex].style() & item<>::style::UNSELECTABLE);
        // delta index
        int8_t nDeltaIndex = m_nSelectedIndex - nPrevIndex;
        // set current index as selected index
        m_nSelectedIndex = nPrevIndex;
        // check if masked index at top
        printv("%d", m_MaskY.nCurrent);

        if (m_nMaskedIndex > 0) {  // below top

            if (nDeltaIndex > m_nMaskedIndex) {
                m_MaskY.move(-m_LineH * m_nMaskedIndex);
                m_ItemY.move(-m_LineH * (nDeltaIndex - m_nMaskedIndex));
                m_nMaskedIndex = 0;
            } else {
                m_MaskY.move(-m_LineH * nDeltaIndex);

                m_nMaskedIndex -= nDeltaIndex;
            }

        } else {
            // at top
            m_ItemY.move(-m_LineH * nDeltaIndex);
        }
        // move scroll ypos
        m_SliderY.move(m_SliderH);
        // move mask offset and size
        vec2<easing_type_t> offset = m_pItems[m_nSelectedIndex].maskpos();
        vec2<easing_type_t> size   = m_pItems[m_nSelectedIndex].masksize();

        m_MaskX.moveto(offset.x());
        m_MaskY.moveto(offset.y());
        m_MaskW.moveto(size.w());
        m_MaskH.moveto(size.h());

        // repaint
        m_bRepaint = true;
    }

    void switchToNext(void)
    {
        // seek next selectable index
        int8_t nNextIndex = m_nSelectedIndex;
        do {
            if (++nNextIndex == m_nLength) return;
        } while (m_pItems[nNextIndex].style() & item<>::style::UNSELECTABLE);
        // delta index
        int8_t nDeltaIndex = nNextIndex - m_nSelectedIndex;
        // set current index as selected index
        m_nSelectedIndex = nNextIndex;
        // check if masked index at bottom
        int8_t nRemainingCount = m_nDisplayedCount - m_nMaskedIndex - 1;
        if (nRemainingCount > 0) {  // above bottom
            if (nDeltaIndex > nRemainingCount) {
                m_MaskY.move(m_LineH * nRemainingCount);
                m_ItemY.move(m_LineH * (nDeltaIndex - nRemainingCount));
                m_nMaskedIndex = m_nDisplayedCount - 1;
            } else {
                m_MaskY.move(m_LineH * nDeltaIndex);
                m_nMaskedIndex += nDeltaIndex;
            }
        } else {
            // at bottom
            m_ItemY.move(m_LineH * nDeltaIndex);
        }
        // move scroll ypos
        m_SliderY.move(m_SliderH);
        // move mask offset and size
        vec2<easing_type_t> offset = m_pItems[m_nSelectedIndex].maskpos();
        vec2<easing_type_t> size   = m_pItems[m_nSelectedIndex].masksize();
        m_MaskX.moveto(offset.x());
        m_MaskY.moveto(offset.y());
        m_MaskW.moveto(size.w());
        m_MaskH.moveto(size.h());
        // repaint
        m_bRepaint = true;
    }

    void event(key_id_t id)
    {
        switch (id) {
            case KEY_ID_NONE: break;
            case KEY_ID_PREV: {
                println("prev");
                if (m_pItems[m_nSelectedIndex].style() & item<>::style::CAPTURE_KEY_FIRST)
                    if (m_pItems[m_nSelectedIndex].event(KEY_ID_PREV)) break;
                switchToPrev();
                break;
            }
            case KEY_ID_NEXT: {
                println("next");
                if (m_pItems[m_nSelectedIndex].style() & item<>::style::CAPTURE_KEY_FIRST)
                    if (m_pItems[m_nSelectedIndex].event(KEY_ID_NEXT)) break;
                switchToNext();
                break;
            }
            default: {
                m_pItems[m_nSelectedIndex].event(id);
                m_bRepaint = true;
                break;
            }
        }
    }

    item<easing_type_t>& operator[](uint8_t idx)
    {
        if (idx < m_nLength) return m_pItems[idx];
    }
};

void doit(void* arg)
{
    list menulist;

    item<> items[] = {
        item<>("hello0"),
        item<>("hello1#").set(item<>::style::UNSELECTABLE),
        item<>("hello3666666"),
        item<>("hello2#").set(item<>::style::UNSELECTABLE),
        item<>("hello3666666"),
        item<>("hello3666666"),
        item<>("hello4#").set(item<>::style::UNSELECTABLE),
        item<>("hello5"),
        item<>("hello6#").set(item<>::style::UNSELECTABLE),
        item<>("hello7#").set(item<>::style::UNSELECTABLE),
        item<>("hello8#666").set(item<>::style::UNSELECTABLE),
        item<>("hello9#").set(item<>::style::UNSELECTABLE),
        item<>("hello1066666"),
        item<>("hello11"),
        item<>("hello9#").set(item<>::style::UNSELECTABLE),
    };

    menulist.setItems(items, ARRAY_SIZE(items));

    while (1) {
        menulist.event(key_scan());
        menulist.render();
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void app_entry(void* arg)
{
    doit(arg);
}

#ifdef __cplusplus
}
#endif

#endif