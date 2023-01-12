

#include "stdbool.h"
#include "math.h"
#include "utils.h"

#include "easing/easing.h"
#include "gui.h"
#include "res/images.h"

#define ARRAR_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define min(a, b) ((a < b) ? (a) : (b))

////////////////////////////////////////////////////////////////////////

typedef struct {
    uint8_t id;
    char*   title;
    void (*callback)(void);
} item_menu_t;

extern struct container;
typedef struct container page_t;

struct container {
    void (*painter)(page_t*);
    // void painter(page_t* container);

    void (*handler)(page_t*);
    // void handler(page_t*);

    bool repaint;

    item_menu_t* menus;
    uint8_t      size;  // count of menu

    uint8_t count_display;   // number of menus can be displayed per page
    uint8_t index_masked;    // [0, min(count_display,size)), index of the masked menu in page
    uint8_t index_selected;  // [0, size), current selected menu index in all

    uint8_t h_line;    // height of menu / line ( CONFIG_SCREEN_HEIGHT / count_display )
    uint8_t h_slider;  // height of scroll ( CONFIG_SCREEN_HEIGHT / size)

    easing_t w_mask;
    easing_t y_mask;
    easing_t y_slider;
    easing_t y_title;
    easing_t x_icon;
};

////////////////////////////////////////////////////////////////////////

#define CONFIG_PADDING_X 4   // x_padding
#define CONFIG_PADDING_Y 12  // y_padding

#define SPEED 8

uint8_t pid_select;
uint8_t pid_max = 10.00;
uint8_t Kpid[3] = {9.97, 0.2, 0.01};  // Kp,Ki,Kd

void painter_text(page_t*);
void painter_icon(page_t*);
void painter_text_editor(page_t*);
void painter_chart(page_t* p);

void handler_main(page_t* p);
void handler_pid(page_t* p);
void handler_icon(page_t* p);
void handler_text_editor(page_t* p);

uint8_t container_index = 0;

page_t pages[] = {

    {
        .painter = painter_icon,
        .handler = handler_icon,

        .menus = menu_icon,
        .size  = ARRAR_SIZE(menu_icon),

        .count_display = 4,  // < size

        .h_line   = CONFIG_SCREEN_HEIGHT / 4,
        .h_slider = CONFIG_SCREEN_HEIGHT / ARRAR_SIZE(menu_icon),

        .repaint = true,

    },

    {
        .painter = painter_text_editor,
        .handler = handler_text_editor,

        .repaint = true,

        .size = 11,  // pos of button

        .h_line = 16,

    },

    {
        .painter = painter_chart,

        .repaint = true,

    },

};

////////////////////////////////////////////////////////////////////////

char name[] = "AZaz World ";

static bool    is_edit = false;  // 编辑中
static uint8_t blink   = false;  // 高亮

void handler_text_editor(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        case KEY_ID_PREV: {
            if (is_edit) {
                // space->Z->...->A->z->...->a->space
                char ch = name[p->index_selected];
                switch (ch) {
                    case 'a': ch = ' '; break;
                    case 'A': ch = 'z'; break;
                    case ' ': ch = 'Z'; break;
                    default: --ch; break;
                }
                name[p->index_selected] = ch;
            } else {
                // decrease selected
                if (p->index_selected > 0) {
                    --p->index_selected;
                } else {
                    p->index_selected = p->size;
                }
            }
            // repaint
            p->repaint = true;
            break;
        }
        case KEY_ID_NEXT: {
            if (is_edit) {
                // space->a->...->z->A->...->Z->space
                char ch = name[p->index_selected];
                switch (ch) {
                    case ' ': ch = 'a'; break;
                    case 'z': ch = 'A'; break;
                    case 'Z': ch = ' '; break;
                    default: ++ch; break;
                }
                name[p->index_selected] = ch;
            } else {
                // increase selected
                if (p->index_selected < p->size) {
                    ++p->index_selected;
                } else {
                    p->index_selected = 0;
                }
            }
            // repaint
            p->repaint = true;
            break;
        }
        case KEY_ID_OK: {
            if (p->index_selected == p->size) {
                // return
                effect_disappear();
                pages[container_index = 0].repaint = true;
                return;
            } else {
                // edit mode
                is_edit = !is_edit;
                // repaint
                p->repaint = true;
            }
            break;
        }
    }
}

void painter_text_editor(page_t* p)
{
    handler_text_editor(p);

    if (p->repaint) {
        p->repaint = false;

        _clear();

        // frame
        _fiil_rounded_rect(4, 6, 120, 52, 8);
        // title
        _draw_str((128 - _str_w_("--Text Editor--")) / 2, 20, "--Text Editor--");
        // text
        uint8_t box_x = 10;
        _draw_str(box_x, 38, name);
        // button
        _draw_str(80, 50, "-Return");

#define BLINK_SPEED 24  // 2的倍数

        // cursor
        if (p->index_selected < p->size) {
            if (blink < BLINK_SPEED / 2) {
                char ch                 = name[p->index_selected];
                name[p->index_selected] = '\0';
                box_x += _str_w_(name);
                name[p->index_selected] = ch;
                // char (hover)
                char temp[2] = {name[p->index_selected], '\0'};
                _set_color(2);
                _draw_rect(box_x, 26, _str_w_(temp) + 2, p->h_line);
                _set_color(1);
            }
        } else {
            // button (hover)
            _set_color(2);
            _draw_rounded_rect(78, 38, _str_w_("-Return") + 4, p->h_line, 1);
            _set_color(1);
        }

        if (is_edit) {
            blink      = (blink < BLINK_SPEED, 0) ? (blink + 1) : 0;
            p->repaint = true;
        }

        _update();
    }
}

void handler_pid(page_t* p)
{
    uint8_t i = 0;

    switch (p->menus[p->index_selected].id) {
        case 0:
        case 1:
        case 2:
            for (uint16_t i = 0; i < buf_len; ++i)
                buf_ptr[i] &= (i % 2 == 0 ? 0x55 : 0xAA);
            _update();
            pid_select = p->menus[p->index_selected].id;
            while (1) {
                switch (key_scan()) {
                    case KEY_ID_NONE:
                        break;
                    case KEY_ID_NEXT:
                        if (Kpid[pid_select] < pid_max)
                            Kpid[pid_select] += 1;
                        break;
                    case KEY_ID_PREV:
                        if (Kpid[pid_select] > 0)
                            Kpid[pid_select] -= 1;
                        break;
                    case KEY_ID_OK:
                        p->repaint = true;
                        return;
                }

                _draw_rect(16, 16, 96, 31);
                _set_color(2);
                _draw_rect(17, 17, 94, 29);
                _set_color(1);

                _fiil_rect(18, 36, 60, 8);
                _draw_rect(20, 38, (uint8_t)((float)Kpid[pid_select] / pid_max * 56), 4);

                switch (pid_select) {
                    case 0: _draw_str(22, 30, "Editing Kp"); break;
                    case 1: _draw_str(22, 30, "Editing Ki"); break;
                    case 2: _draw_str(22, 30, "Editing Kd"); break;
                    default: break;
                }

                static char buff[16];
                sprintf(buff, "%d", Kpid[pid_select]);
                _draw_str(81, 44, buff);

                _update();
            }

            break;

        case 3: {
            effect_disappear();
            pages[container_index = 0].repaint = true;
            return;
        }

        default:
            break;
    }
}

void handler_main(page_t* p)
{
    effect_disappear();

    uint8_t i = 0;

    switch (p->menus[p->index_selected].id) {
        case M_LOGO: {
            while (1) {
                switch (key_scan()) {
                    case KEY_ID_PREV:
                    case KEY_ID_OK: {
                        effect_disappear();
                        pages[container_index = 0].repaint = true;
                        return;
                    }
                    default: break;
                }
                if (i == 0) {
                    i = 1;
                    _clear();
                    _set_color(0);
                    _draw_str(CONFIG_PADDING_X, 12, "Hello World");
                    _set_color(1);
                    _draw_str(CONFIG_PADDING_X, 24, "Hello World");
                    _set_color(2);
                    _draw_str(CONFIG_PADDING_X, 36, "Hello World");
                    _update();
                }
            }
            break;
        }
        case M_PID: {
            effect_disappear();
            pages[container_index = 1].repaint = true;
            return;
        }
        case M_ICON: {
            effect_disappear();
            pages[container_index = 2].repaint = true;
            return;
        }
        case M_CHART: {
            effect_disappear();
            pages[container_index = 4].repaint = true;
            return;
        }
        case M_TEXT: {
            effect_disappear();
            pages[container_index = 3].repaint = true;
            return;
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
                        pages[container_index = 0].repaint = true;
                        return;
                    }
                    default: break;
                }
                if (i == 0) {
                    i = 1;
                    _clear();
                    _set_color(1);
                    _draw_str(2, 12, "MCU : STM32");
                    _draw_str(2, 28, "FLASH : 512KB");
                    _draw_str(2, 44, "SRAM : 256KB");
                    _draw_str(2, 60, "RTC SRAM : 16KB");
                    _update();
                }
            }

            break;
        }
    }
    pages[container_index = 0].repaint = true;
}

void handler_icon(page_t* p)
{
    effect_disappear();
    pages[container_index = 0].repaint = true;
    return;
}

float   angle, angle_last;
uint8_t chart_x;

float analogRead()
{
    HAL_ADC_Start(&hadc1);
    return (float)abs((HAL_ADC_GetValue(&hadc1) - 3700)) / 300 * 100;
}

void painter_chart(page_t* p)
{
    switch (key_scan()) {
        case KEY_ID_NONE: break;
        default: {
            effect_disappear();
            pages[container_index = 0].repaint = true;
            return;
        }
    }

    if (p->repaint) {
        // fraw frame

        _clear();

        _draw_str(4, 12, "Real time angle :");
        _draw_rounded_rect(4, 18, 120, 46, 8);
        _set_color(2);
        _draw_hline(10, 58, 108);
        _draw_vline(10, 24, 34);
        // 箭头
        _draw_pixel(7, 27);
        _draw_pixel(8, 26);
        _draw_pixel(9, 25);

        _draw_pixel(116, 59);
        _draw_pixel(115, 60);
        _draw_pixel(114, 61);
        _set_color(1);

        angle_last = analogRead();

        p->repaint = false;
    }

    _draw_rect(96, 0, 30, 14);

    _draw_vline(chart_x + 10, 59, 3);
    if (chart_x == 100) chart_x = 0;

    // u8g2_DrawBox(&u8g2,chart_x+11,24,8,32);

    _draw_vline(chart_x + 11, 24, 34);
    _draw_vline(chart_x + 12, 24, 34);
    _draw_vline(chart_x + 13, 24, 34);
    _draw_vline(chart_x + 14, 24, 34);

    // 异或绘制
    _set_color(2);
    angle = analogRead();
    _draw_line(chart_x + 11, 58 - (int)angle_last / 2, chart_x + 12, 58 - (int)angle / 2);
    _draw_vline(chart_x + 12, 59, 3);
    angle_last = angle;
    chart_x += 2;
    _draw_rect(96, 0, 30, 14);
    _set_color(1);

    _draw_num(96, 12, "%.2f", angle);

    _update();
}