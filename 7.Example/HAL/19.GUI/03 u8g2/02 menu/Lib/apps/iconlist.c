#include "iconlist.h"

#define FRAMECOUNT 12

iconlist_t iconlist_initialize(item_icon_t items[], uint8_t size, cbk_t painter, cbk_t handler)
{
    if (size == 0) {
        iconlist_t m = {0};
        return m;
    }

    uint8_t width  = CONFIG_SCREEN_WIDTH;
    uint8_t height = CONFIG_SCREEN_HEIGHT;

    iconlist_t m = {
        .repaint = true,

        .painter = painter == 0 ? iconlist_callback_default_painter : painter,
        .handler = handler == 0 ? iconlist_callback_default_handler : handler,

        .width  = width,
        .height = height,

        .items = items,
        .size  = size,

        .h_title = height / 4,

        .index_selected = 0,

        .icon_space = 48,

        .x_icon         = easing_create(EASING_TIMES_SINGLE | EASING_DIR_FORWARD, _easing_calc_OutCubic, 0, FRAMECOUNT, EASING_INTERVAL_NONE),
        .y_title        = easing_create(EASING_TIMES_SINGLE | EASING_DIR_FORWARD, _easing_calc_Linear, 0, FRAMECOUNT, EASING_INTERVAL_NONE),
        .y_padding_icon = easing_create(EASING_TIMES_SINGLE | EASING_DIR_BACKANDFORTH, _easing_calc_OutExpo, 0, FRAMECOUNT, EASING_INTERVAL_NONE),
    };

    return m;
}

void iconlist_callback_default_handler(iconlist_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: iconlist_callback_handler_switch_prev(p); break;
        case KEY_ID_NEXT: iconlist_callback_handler_switch_next(p); break;
        default: {
            println("%s", p->items[p->index_selected].title);
            break;
        }
    }
}

void iconlist_callback_handler_switch_prev(iconlist_t* p)
{
    if (p->index_selected > 0) {
        // decrease selected index
        --p->index_selected;
        // decrease offset
        easing_start_relative(&p->x_icon, -p->icon_space, 1);
        easing_start_relative(&p->y_title, -p->h_title, 1);
        // repaint
        p->repaint = true;
    }
}

void iconlist_callback_handler_switch_next(iconlist_t* p)
{
    if (p->index_selected < p->size - 1) {
        // increase selected index
        ++p->index_selected;
        // increase offset
        easing_start_relative(&p->x_icon, p->icon_space, 1);
        easing_start_relative(&p->y_title, p->h_title, 1);
        // repaint
        p->repaint = true;
    }
}

void iconlist_callback_default_painter(iconlist_t* p)
{
    // easing

    easing_update(&p->y_title);
    easing_update(&p->x_icon);
    easing_update(&p->y_padding_icon);

    p->repaint =
        EASING_IS_RUNNING(p->y_title) ||
        EASING_IS_RUNNING(p->x_icon) ||
        EASING_IS_RUNNING(p->y_padding_icon);

    // new frame

    _clear();

    // draw

    uint8_t index = p->x_icon.nCurrent / p->icon_space;
    if (index > 0) --index;
    int8_t offset = index * p->icon_space - p->x_icon.nCurrent;
    while (index < p->size && offset < p->width) {
        // icon
        u8g2_DrawXBMP(&u8g2, 46 + offset, ((index == p->index_selected) ? p->y_padding_icon.nCurrent : 0) + (p->height - p->items[index].height) / 2, p->items[index].height, p->items[index].width, p->items[index].buff);
        // title
        u8g2_SetClipWindow(&u8g2, 0, 48, p->width, p->height);
        _draw_str((p->width - _str_w_(p->items[index].title)) / 2, 62 - p->y_title.nCurrent + index * 16, p->items[index].title);
        u8g2_SetMaxClipWindow(&u8g2);

        ++index, offset += p->icon_space;
    }

    // update screen

    _update();
}
