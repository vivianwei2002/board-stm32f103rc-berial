#include "menulist.h"

#define FRAMECOUNT 4

#define CORRECT(v) ((v - (uint8_t)v) > 0.5f)

menulist_t menulist_initialize(item_menu_t items[], uint8_t size, uint8_t count_display, cbk_t painter, cbk_t handler)
{
    if (size == 0) {
        menulist_t m = {0};
        return m;
    }

    if (count_display == 0) count_display = 1;

    uint8_t width  = CONFIG_SCREEN_WIDTH;
    uint8_t height = CONFIG_SCREEN_HEIGHT;

    menulist_t m = {
        .repaint = true,

        .painter = painter == 0 ? menulist_callback_default_painter : painter,
        .handler = handler == 0 ? menulist_callback_default_handler : handler,

        .width  = width,
        .height = height,

        .items = items,
        .size  = size,

        .count_display = count_display,

        .h_line   = (easing_pos_t)height / count_display,
        .h_slider = (easing_pos_t)height / size,

        .index_masked   = 0,
        .index_selected = 0,

        .x_padding = 4,
        .y_padding = 8,  // fontsize ?

        .x_slider = width - 2,

        .w_mask = easing_create(EASING_TIMES_SINGLE | EASING_DIR_FORWARD, _easing_calc_OutCubic, _str_w_(items[0].title), FRAMECOUNT, 0),
        .y_mask = easing_create(EASING_TIMES_SINGLE | EASING_DIR_FORWARD, _easing_calc_OutCubic, 0, FRAMECOUNT, 0),

        .y_slider = easing_create(EASING_TIMES_SINGLE | EASING_DIR_FORWARD, _easing_calc_OutCubic, 0, FRAMECOUNT, 0),
        .y_title  = easing_create(EASING_TIMES_SINGLE | EASING_DIR_FORWARD, _easing_calc_Linear, 0, FRAMECOUNT, 0),

    };

    m.y_padding += (m.h_line - m.y_padding) / 2;

    return m;
}

void menulist_callback_default_handler(menulist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: menulist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: menulist_callback_handler_switch_next(p); break;
        default: {
            println("%s", p->items[p->index_selected].title);
            break;
        }
    }
}

void menulist_callback_handler_switch_prev(menulist_t* p)
{
    if (p->index_selected > 0) {
        // decrease selected index
        --p->index_selected;
        // check if masked index at top
        if (p->index_masked > 0) {
            // decrease masked index
            --p->index_masked;
            // decrease mask ypos
            easing_start_relative(&p->y_mask, -p->h_line, 1);
        } else {
            // decrease text yoffset
            easing_start_relative(&p->y_title, -p->h_line, 1);
        }
        // decrease scroll ypos
        easing_start_relative(&p->y_slider, -p->h_slider, 1);
        // change mask width
        easing_start_absolute(&p->w_mask, p->w_mask.nCurrent, _str_w_(p->items[p->index_selected].title), 1);
        // repaint
        p->repaint = true;
    }
}

void menulist_callback_handler_switch_next(menulist_t* p)
{
    if (p->index_selected < p->size - 1) {
        // increase selected index
        ++p->index_selected;
        // check if masked index at bottom
        if (p->index_masked < p->count_display - 1) {
            // increase masked index
            ++p->index_masked;
            // increase mask ypos
            easing_start_relative(&p->y_mask, p->h_line, 1);
        } else {
            // increase text yoffset
            easing_start_relative(&p->y_title, p->h_line, 1);
        }
        // increase scroll ypos
        easing_start_relative(&p->y_slider, p->h_slider, 1);
        // change mask width
        easing_start_absolute(&p->w_mask, p->w_mask.nCurrent, _str_w_(p->items[p->index_selected].title), 1);
        // repaint
        p->repaint = true;
    }
}

INLINE void _draw_2side_line(uint8_t x, easing_pos_t y, uint8_t d, easing_pos_t h)
{
    h += CORRECT(y);
    _draw_vline(x - d, y, h);
    _draw_vline(x + d, y, h);
}

void menulist_callback_default_painter(menulist_t* p)
{
    // easing

    easing_update(&p->w_mask);
    easing_update(&p->y_mask);
    easing_update(&p->y_title);
    easing_update(&p->y_slider);

    p->repaint =
        EASING_IS_RUNNING(p->w_mask) ||
        EASING_IS_RUNNING(p->y_mask) ||
        EASING_IS_RUNNING(p->y_title) ||
        EASING_IS_RUNNING(p->y_slider);

    // new frame

    _clear();

    // title

    uint8_t      index    = p->y_title.nCurrent / p->h_line;
    easing_pos_t y_offset = index * p->h_line - p->y_title.nCurrent;
    while (index < p->size && y_offset < p->height) {
        _draw_str(p->x_padding, y_offset + p->y_padding, p->items[index].title);
        ++index, y_offset += p->h_line;
    }

    // scroll

    _draw_vline(p->x_slider, 0, p->height);  // 轴

    easing_pos_t y = 0;

    for (uint8_t i = 0; i < p->size; ++i) {  // 刻度
        _draw_pixel(p->x_slider - 1, y);
        _draw_pixel(p->x_slider + 1, y);
        y += p->h_slider;
    }
    _draw_pixel(p->x_slider - 1, p->height - 1);
    _draw_pixel(p->x_slider + 1, p->height - 1);

    _draw_2side_line(p->x_slider, p->y_slider.nCurrent, 1, p->h_slider);  // 滑块

    // mask

    _set_color(2);
    _draw_rounded_rect(0, p->y_mask.nCurrent, p->w_mask.nCurrent + p->x_padding * 2, p->h_line + CORRECT(p->y_mask.nCurrent), 1);
    _set_color(1);

    // update screen

    _update();
}
