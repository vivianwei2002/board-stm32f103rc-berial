#ifndef __ICONLIST_H__
#define __ICONLIST_H__

#include "res/images.h"

// 垂直滚动菜单

#include "base.h"

typedef struct {
    uint8_t     ID;
    const char* title;
    uint8_t     width;
    uint8_t     height;
    uint8_t*    buff;
} item_icon_t;

typedef struct {
    bool repaint;

    // callback
    cbk_t painter;
    cbk_t handler;

    // size
    uint16_t width;
    uint16_t height;

    // menus
    item_icon_t* items;
    uint8_t      size;

    // index
    uint8_t index_selected;

    // pos & easing
    easing_t     x_icon;
    easing_t     y_title;
    easing_pos_t h_title;

    uint8_t icon_space;

    easing_t y_padding_icon;

} iconlist_t;

iconlist_t iconlist_initialize(
    // items
    item_icon_t items[],
    uint8_t     size,
    // cbk
    cbk_t painter,  // default = iconlist_callback_default_painter
    cbk_t handler   // default = iconlist_callback_default_handler
);

// painter
void iconlist_callback_default_painter(iconlist_t* p);

// handler
void iconlist_callback_default_handler(iconlist_t* p);
void iconlist_callback_handler_switch_prev(iconlist_t* p);
void iconlist_callback_handler_switch_next(iconlist_t* p);

#endif