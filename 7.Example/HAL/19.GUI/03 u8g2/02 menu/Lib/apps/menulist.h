#ifndef __MENULIST_H__
#define __MENULIST_H__

// 图标菜单

#include "base.h"

typedef struct {
    uint8_t     ID;
    const char* title;
    void*       arg;
} item_menu_t;

typedef struct {
    bool repaint;

    // callback
    cbk_t painter;
    cbk_t handler;

    // size
    uint16_t width;
    uint16_t height;

    // menus
    item_menu_t* items;
    uint8_t      size;

    // config
    uint8_t      count_display;  // number of items can be displayed per page
    easing_pos_t h_line;         // height of item / line ( page.height / count_display )
    easing_pos_t h_slider;       // height of scroll ( page.height / size)

    // index
    uint8_t index_masked;    // [0, min(count_display,size)), index of the masked item in page
    uint8_t index_selected;  // [0, size), current selected item index in all

    // pos & easing
    uint8_t      x_padding;
    easing_pos_t y_padding;
    easing_t     w_mask;
    easing_t     y_mask;
    uint8_t      x_slider;
    easing_t     y_slider;
    easing_t     y_title;

} menulist_t;

menulist_t menulist_initialize(

    // items
    item_menu_t items[],
    uint8_t     size,
    // show
    uint8_t count_display,  // default = 1
    // cbk
    cbk_t painter,  // default = menulist_callback_default_painter
    cbk_t handler   // default = menulist_callback_default_handler
);

// painter
void menulist_callback_default_painter(menulist_t* p);

// handler
void menulist_callback_default_handler(menulist_t* p);
void menulist_callback_handler_switch_prev(menulist_t* p);
void menulist_callback_handler_switch_next(menulist_t* p);

#endif