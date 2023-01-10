
#include "u8g2/mui_u8g2.h"
#include "u8g2/mui.h"
#include "u8g2/u8g2.h"
#include "u8g2/u8x8.h"

#include "stdbool.h"
#include "math.h"
#include "utils.h"
#include "gpio.h"

#define ARRAR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define CONFIG_SCREEN_HEIGHT 64   // height of screen
#define CONFIG_SCREEN_WIDTH  128  // width of screen

uint8_t* buf_ptr;
uint16_t buf_len;

// buf_ptr = u8g2_GetBufferPtr(&u8g2);
// buf_len = 8 * u8g2_GetBufferTileHeight(&u8g2) * u8g2_GetBufferTileWidth(&u8g2);

#define min(a, b) ((a < b) ? (a) : (b))

////////////////////////////////////////////////////////////////////////
#if 0
const uint8_t icon_pic[][200] = {
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
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  // 点赞(36x35)
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
        0x00  // 收藏(36x37)
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
        0x00, 0x00, 0x00, 0x00  // 投币(36x36)
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
        0x00, 0x00, 0x00, 0x00,  // 分享(36x36)
    },
};
#endif
uint8_t icon_width[] = {35, 37, 36, 36};

//////////////////////////////////////////////////////////////////////// easing

typedef struct {
    int16_t current;
    int16_t target;
    int16_t speed;
    bool    running;
} easing_t;

bool do_easing_linear(easing_t* easing)
{
    if (easing->running) {
        if (easing->current < easing->target) {
            easing->current += easing->speed;
            if (easing->current > easing->target)
                easing->current = easing->target;
        } else if (easing->current > easing->target) {
            easing->current -= easing->speed;
            if (easing->current < easing->target)
                easing->current = easing->target;
        }
        return easing->running = (easing->current != easing->target);
    }
    return true;
}

void easing_init(easing_t* easing, int16_t current, int16_t target, int16_t offset, uint8_t step)
{
    int16_t delta   = abs(target - current);
    easing->current = current + offset;
    easing->target  = target + offset;
    easing->speed   = delta / step;
    if (delta % step > 0)
        ++easing->speed;
    easing->running = true;
}

void easing_update(easing_t* easing);

////////////////////////////////////////////////////////////////////////

typedef struct {
    uint8_t id;
    char*   title;
    void (*callback)(void);
} menu_t;

extern struct container;
typedef struct container container_t;

struct container {
    void (*painter)(container_t*);
    // void painter(container_t* container);

    void (*handler)(menu_t*, uint8_t, uint8_t, key_type_t);
    // void handler(menu_t* menu, uint8_t index, uint8_t size, key_type_t key);

    menu_t* menus;
    uint8_t size;  // count of menu

    uint8_t count_display;  // number of menus can be displayed per page

    uint8_t menu_height;    // height of menu ( CONFIG_SCREEN_HEIGHT / count_display )
    uint8_t slider_height;  // height of scroll ( CONFIG_SCREEN_HEIGHT / size)

    // for mask
    easing_t w_mask;
    easing_t y_mask;
    // for scroll
    easing_t y_slider;
    // for text
    easing_t y_text;  // offset of menu title

    uint8_t index_masked;  // [0, min(count_display,size)), index of the masked menu in page

    uint8_t index_selected;  // [0, size), current selected menu index in all

    //
    bool repaint;
};

////////////////////////////////////////////////////////////////////////

typedef enum {
    KEY_ID_NONE = 0,

    KEY_ID_START = 1,

    KEY_ID_PREV = 1,
    KEY_ID_OK   = 2,
    KEY_ID_NEXT = 3,

    KEY_ID_END = 3,
} key_type_t;

typedef struct {
    bool cur;
    bool last;
} key_state_t;

key_state_t keys[KEY_ID_END];

bool key_val(key_type_t index)
{
    switch (index) {
        case KEY_ID_PREV: return HAL_GPIO_ReadPin(KEY_C1_GPIO_Port, KEY_C1_Pin) == GPIO_PIN_RESET;
        case KEY_ID_OK: return HAL_GPIO_ReadPin(KEY_C3_GPIO_Port, KEY_C3_Pin) == GPIO_PIN_RESET;
        case KEY_ID_NEXT: return HAL_GPIO_ReadPin(KEY_C2_GPIO_Port, KEY_C2_Pin) == GPIO_PIN_RESET;
        default: break;
    }
    return false;
}

static key_type_t key_id;

void key_init()
{
    for (uint8_t i = 0; i < ARRAR_SIZE(keys); ++i)
        keys[i].cur = keys[i].last = key_val(i + KEY_ID_START);
}

key_type_t key_scan()
{
    for (key_type_t i = 0; i <= ARRAR_SIZE(keys); ++i) {
        keys[i].cur = key_val(i + KEY_ID_START);
        if (keys[i].last != keys[i].cur) {
            keys[i].last = keys[i].cur;
            printv("%d", keys[i].cur);
            if (keys[i].cur == 1)
                return key_id = i + KEY_ID_START;  // key pressed
        }
    }
    return KEY_ID_NONE;
}

////////////////////////////////////////////////////////////////////////

u8g2_t u8g2;

#define StrW(str) u8g2_GetStrWidth(&u8g2, str)

////////////////////////////////////////////////////////////////////////

void painter_text(container_t*);
void painter_icon(container_t*);

////////////////////////////////////////////////////////////////////////

#define CONFIG_PADDING_X 4   // x_padding
#define CONFIG_PADDING_Y 12  // y_padding

#define SPEED 3

#define ICON_SPEED 12
#define ICON_SPACE (ICON_SPEED * 4)  // 图标间隔

enum {
    M_LOGO,
    M_SELECT,
    M_PID,
    M_PID_EDIT,
    M_ICON,
    M_CHART,
    M_TEXT,
    M_VIDEO,
    M_ABOUT,
};

const menu_t menu_main[] = {
    {M_LOGO, "MainUI", 0},
    {M_PID, "+PID Editor", 0},
    {M_ICON, "-Icon Test", 0},
    {M_CHART, "-Chart Test", 0},
    {M_TEXT, "-Text Edit", 0},
    {M_VIDEO, "-Play Video", 0},
    {M_ABOUT, "{ About }", 0},
};

const menu_t menu_pid[] = {
    {0, "-Proportion", 0},
    {1, "-Integral", 0},
    {2, "-Derivative", 0},
    {3, "Return", 0},
};

const menu_t menu_icon[] = {
    {0, "Likes", 0},
    {1, "Collection", 0},
    {2, "Slot", 0},
    {3, "Share", 0},
};

void handler_main(menu_t* menu, uint8_t index, uint8_t size, key_type_t key);
void handler_pid(menu_t* menu, uint8_t index, uint8_t size, key_type_t key);

uint8_t container_index = 0;

container_t containers[] = {

    {
        .painter = painter_text,
        .handler = handler_main,

        .menus = menu_main,
        .size  = ARRAR_SIZE(menu_main),

        .count_display = 4,

        .menu_height   = CONFIG_SCREEN_HEIGHT / 4,                      // CONFIG_SCREEN_HEIGHT / count_display
        .slider_height = CONFIG_SCREEN_HEIGHT / ARRAR_SIZE(menu_main),  // CONFIG_SCREEN_HEIGHT / size

        .repaint = true,

    },

    {
        .painter = painter_text,
        .handler = handler_pid,

        .menus = menu_pid,
        .size  = ARRAR_SIZE(menu_pid),

        .count_display = 4,

        .menu_height   = CONFIG_SCREEN_HEIGHT / 4,
        .slider_height = CONFIG_SCREEN_HEIGHT / ARRAR_SIZE(menu_pid),

        .repaint = true,

    },

    {
        .painter = painter_text,
        .handler = handler_pid,

        .menus = menu_icon,
        .size  = ARRAR_SIZE(menu_icon),

        .count_display = 4,  // < size

        .menu_height   = CONFIG_SCREEN_HEIGHT / 4,
        .slider_height = CONFIG_SCREEN_HEIGHT / ARRAR_SIZE(menu_icon),

        .repaint = true,

    },

};

////////////////////////////////////////////////////////////////////////

void effect_disappear()
{
    for (uint16_t i = 0; i < buf_len; ++i)
        if (i % 2 == 0) buf_ptr[i] &= 0x55;
    u8g2_SendBuffer(&u8g2);

    for (uint16_t i = 0; i < buf_len; ++i)
        if (i % 2 != 0) buf_ptr[i] &= 0xAA;
    u8g2_SendBuffer(&u8g2);

    for (uint16_t i = 0; i < buf_len; ++i)
        if (i % 2 == 0) buf_ptr[i] &= 0x00;
    u8g2_SendBuffer(&u8g2);

    for (uint16_t i = 0; i < buf_len; ++i)
        if (i % 2 != 0) buf_ptr[i] &= 0x00;
    u8g2_SendBuffer(&u8g2);
}

////////////////////////////////////////////////////////////////////////

void painter_text(container_t* p)
{
    if (p->w_mask.current == 0)
        p->w_mask.current = StrW(p->menus[0].title) + CONFIG_PADDING_X * 2;

    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: {
            printf("prev\r\n");
            if (p->index_selected > 0) {
                // decrease selected index
                --p->index_selected;
                // check if masked index at top
                if (p->index_masked > 0) {
                    // decrease masked index
                    --p->index_masked;
                    // decrease mask ypos
                    easing_init(&p->y_mask, 0, -p->menu_height, p->y_mask.current, SPEED);
                } else {
                    // decrease text yoffset
                    easing_init(&p->y_text, 0, -p->menu_height, p->y_text.current, SPEED);
                }
                // decrease scroll ypos
                easing_init(&p->y_slider, 0, -p->slider_height, p->y_slider.current, SPEED);
                // change mask width
                easing_init(&p->w_mask, p->w_mask.current, StrW(p->menus[p->index_selected].title), 0, SPEED);
                // repaint
                p->repaint = true;
            }
            break;
        }
        case KEY_ID_NEXT: {
            printf("next\r\n");
            if (p->index_selected < p->size - 1) {
                // increase selected index
                ++p->index_selected;
                // check if masked index at bottom
                if (p->index_masked < p->count_display - 1) {
                    // increase masked index
                    ++p->index_masked;
                    // increase mask ypos
                    easing_init(&p->y_mask, 0, p->menu_height, p->y_mask.current, SPEED);
                } else {
                    // increase text yoffset
                    easing_init(&p->y_text, 0, p->menu_height, p->y_text.current, SPEED);
                }
                // increase scroll ypos
                easing_init(&p->y_slider, 0, p->slider_height, p->y_slider.current, SPEED);
                // change mask width
                easing_init(&p->w_mask, p->w_mask.current, StrW(p->menus[p->index_selected].title), 0, SPEED);
                // repaint
                p->repaint = true;
            }
            break;
        }
        // other keys
        default: {
            printf("other\r\n");
            if (p->handler) p->handler(p->menus, p->index_selected, p->size, key_id);
            containers[container_index].repaint = true;
        }
    }

    if (p->repaint) {
        // easing

        do_easing_linear(&p->w_mask);
        do_easing_linear(&p->y_mask);
        do_easing_linear(&p->y_text);
        do_easing_linear(&p->y_slider);

        p->repaint = p->w_mask.running || p->y_mask.running || p->y_text.running || p->y_slider.running;

        // new frame

        u8g2_ClearBuffer(&u8g2);

        // titles

        uint8_t index  = p->y_text.current / p->menu_height;
        int8_t  offset = index * p->menu_height - p->y_text.current;
        while (index < p->size && offset < CONFIG_SCREEN_HEIGHT) {
            u8g2_DrawStr(&u8g2, CONFIG_PADDING_X, offset + CONFIG_PADDING_Y, p->menus[index].title);
            ++index, offset += p->menu_height;
        }

        // scroll

        u8g2_DrawVLine(&u8g2, 126, 0, CONFIG_SCREEN_HEIGHT);  // 轴

        for (uint8_t i = 0, y = 0; i <= p->size; ++i, y += p->slider_height) {  // 两侧小点
            u8g2_DrawPixel(&u8g2, 125, y);
            u8g2_DrawPixel(&u8g2, 127, y);
        }

        u8g2_DrawVLine(&u8g2, 125, p->y_slider.current, p->slider_height);  // 滑块
        u8g2_DrawVLine(&u8g2, 127, p->y_slider.current, p->slider_height);

        // mask

        u8g2_SetDrawColor(&u8g2, 2);
        u8g2_DrawRBox(&u8g2, 0, p->y_mask.current, p->w_mask.current + CONFIG_PADDING_X * 2, p->menu_height, 1);
        u8g2_SetDrawColor(&u8g2, 1);

        // update screen

        u8g2_SendBuffer(&u8g2);
    }
}

void painter_icon(container_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: {
            printf("prev\r\n");
            if (p->index_selected > 0) {
                // decrease selected index
                --p->index_selected;
                // check if masked index at top
                if (p->index_masked > 0) {
                    // decrease masked index
                    --p->index_masked;
                    // decrease mask ypos
                    easing_init(&p->y_mask, 0, -p->menu_height, p->y_mask.current, SPEED);
                } else {
                    // decrease text yoffset
                    easing_init(&p->y_text, 0, -p->menu_height, p->y_text.current, SPEED);
                }
                // decrease scroll ypos
                easing_init(&p->y_slider, 0, -p->slider_height, p->y_slider.current, SPEED);
                // change mask width
                easing_init(&p->w_mask, p->w_mask.current, StrW(p->menus[p->index_selected].title), 0, SPEED);
                // repaint
                p->repaint = true;
            }
            break;
        }
        case KEY_ID_NEXT: {
            printf("next\r\n");
            if (p->index_selected < p->size - 1) {
                // increase selected index
                ++p->index_selected;
                // check if masked index at bottom
                if (p->index_masked < p->count_display - 1) {
                    // increase masked index
                    ++p->index_masked;
                    // increase mask ypos
                    easing_init(&p->y_mask, 0, p->menu_height, p->y_mask.current, SPEED);
                } else {
                    // increase text yoffset
                    easing_init(&p->y_text, 0, p->menu_height, p->y_text.current, SPEED);
                }
                // increase scroll ypos
                easing_init(&p->y_slider, 0, p->slider_height, p->y_slider.current, SPEED);
                // change mask width
                easing_init(&p->w_mask, p->w_mask.current, StrW(p->menus[p->index_selected].title), 0, SPEED);
                // repaint
                p->repaint = true;
            }
            break;
        }
        // other keys
        default: {
            printf("other\r\n");
            if (p->handler) p->handler(p->menus, p->index_selected, p->size, key_id);
            containers[container_index].repaint = true;
        }
    }
}

void handler_pid(menu_t* menu, uint8_t index, uint8_t size, key_type_t key)
{
    println("%s", menu[index].title);

    uint8_t i = 0;

    switch (menu[index].id) {
        case 0:
        case 1:
        case 2:
            for (uint16_t i = 0; i < buf_len; ++i)
                buf_ptr[i] &= (i % 2 == 0 ? 0x55 : 0xAA);
            u8g2_SendBuffer(&u8g2);
            pid_select = menu[index].id;
            while (1) {
                switch (key_scan()) {
                    case KEY_ID_NEXT:
                        if (Kpid[pid_select] < PID_MAX)
                            Kpid[pid_select] += 1;
                        break;
                    case KEY_ID_PREV:
                        if (Kpid[pid_select] > 0)
                            Kpid[pid_select] -= 1;
                        break;

                    case KEY_ID_OK: return;
                }

                u8g2_DrawBox(&u8g2, 16, 16, 96, 31);
                u8g2_SetDrawColor(&u8g2, 2);
                u8g2_DrawBox(&u8g2, 17, 17, 94, 29);
                u8g2_SetDrawColor(&u8g2, 1);

                u8g2_DrawFrame(&u8g2, 18, 36, 60, 8);
                u8g2_DrawBox(&u8g2, 20, 38, (uint8_t)(Kpid[pid_select] / PID_MAX * 56), 4);

                switch (pid_select) {
                    case 0: u8g2_DrawStr(&u8g2, 22, 30, "Editing Kp"); break;
                    case 1: u8g2_DrawStr(&u8g2, 22, 30, "Editing Ki"); break;
                    case 2: u8g2_DrawStr(&u8g2, 22, 30, "Editing Kd"); break;
                    default: break;
                }

                static char buff[16];
                sprintf(buff, "%d", Kpid[pid_select]);
                u8g2_DrawStr(&u8g2, 81, 44, buff);

                u8g2_SendBuffer(&u8g2);
            }

            break;

        case 3: {
            effect_disappear();
            container_index = 0;
            return;
        }

        default:
            break;
    }
}

void handler_main(menu_t* menu, uint8_t index, uint8_t size, key_type_t key)
{
    println("%s", menu[index].title);

    effect_disappear();

    uint8_t i = 0;

    switch (menu[index].id) {
        case M_LOGO: {
            while (1) {
                switch (key_scan()) {
                    case KEY_ID_PREV:
                    case KEY_ID_OK: {
                        effect_disappear();
                        return;
                    }
                    default: break;
                }
                if (i == 0) {
                    i = 1;
                    u8g2_ClearBuffer(&u8g2);
                    u8g2_SetDrawColor(&u8g2, 0);
                    u8g2_DrawStr(&u8g2, x, 12, "Hello World");
                    u8g2_SetDrawColor(&u8g2, 1);
                    u8g2_DrawStr(&u8g2, x, 24, "Hello World");
                    u8g2_SetDrawColor(&u8g2, 2);
                    u8g2_DrawStr(&u8g2, x, 36, "Hello World");
                    u8g2_SendBuffer(&u8g2);
                }
            }
            break;
        }
        case M_PID: {
            effect_disappear();
            container_index = 1;
            break;
        }
        case M_ICON: {
            effect_disappear();
            container_index = 2;
            break;
        }
        case M_CHART: {
            break;
        }
        case M_TEXT: {
            break;
        }
        case M_VIDEO: {
            break;
        }
        case M_ABOUT: {
            while (1) {
                switch (key_scan()) {
                    case KEY_ID_PREV:
                    case KEY_ID_OK: {
                        effect_disappear();
                        return;
                    }
                    default: break;
                }
                if (i == 0) {
                    i = 1;
                    u8g2_ClearBuffer(&u8g2);
                    u8g2_SetDrawColor(&u8g2, 1);
                    u8g2_DrawStr(&u8g2, 2, 12, "MCU : STM32");
                    u8g2_DrawStr(&u8g2, 2, 28, "FLASH : 512KB");
                    u8g2_DrawStr(&u8g2, 2, 44, "SRAM : 256KB");
                    u8g2_DrawStr(&u8g2, 2, 60, "RTC SRAM : 16KB");
                    u8g2_SendBuffer(&u8g2);
                }
            }

            break;
        }
    }
}
