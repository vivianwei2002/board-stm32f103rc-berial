#include "graphics.h"

void gl_update(void)
{
    ssd1306_fill_img((uint8_t*)screen_buffer);
}

void gl_clear(void)
{
    char*    p   = (char*)screen_buffer;
    uint16_t len = SCREEN_BUFFER_SZIE;
    while (len-- > 0) *p++ = 0;
}

#if CONFIG_ENABLE_CLIPRECT

static gl_int clip_x1 = SCREEN_LEFT;
static gl_int clip_y1 = SCREEN_TOP;
static gl_int clip_x2 = SCREEN_RIGHT;
static gl_int clip_y2 = SCREEN_BOTTOM;

static inline uint8_t _hittest_rect(gl_int x1, gl_int y1, gl_int x2, gl_int y2)
{
    return ((x1 > clip_x2) || (y1 > clip_y2) || (x2 < clip_x1) || (y2 < clip_y1)) ? 0 : 1;
}

static inline void _intersect_with(gl_int* x1, gl_int* y1, gl_int* x2, gl_int* y2)
{
    if (*x1 < clip_x1) *x1 = clip_x1;
    if (*y1 < clip_y1) *y1 = clip_y1;
    if (*x2 > clip_x2) *x2 = clip_x2;
    if (*y2 > clip_x2) *y2 = clip_x2;
}

void gl_set_cliprect(gl_int x, gl_int y, gl_uint w, gl_uint h)
{
    gl_int x1 = x, y1 = y, x2 = x + w, y2 = y + h;
    _intersect_with(&x1, &y1, &x2, &y2);
    clip_x1 = x1, clip_y1 = y1, clip_x2 = x2, clip_y2 = y2;
}

void gl_reset_cliprect(void)
{
    clip_x1 = SCREEN_LEFT;
    clip_y1 = SCREEN_TOP;
    clip_x2 = SCREEN_RIGHT;
    clip_y2 = SCREEN_BOTTOM;
}

#endif

////////////////////////////////////////////////////

#define _set_color(row, subrow, col, color)                          \
    switch (color) {                                                 \
        case color_black: screen_buffer[row][col] &= ~subrow; break; \
        case color_white: screen_buffer[row][col] |= subrow; break;  \
        case color_xor: screen_buffer[row][col] ^= subrow; break;    \
    }

static inline void _set_pixel(gl_uint x, gl_uint y, gl_color cr)
{
    gl_uint col    = x;
    gl_uint row    = y / 8;
    gl_uint subrow = 1 << (y % 8);
    _set_color(row, subrow, col, cr);
}

static inline void _set_hline(gl_uint x, gl_uint y, gl_uint w, gl_color cr)
{
    gl_uint col    = x;
    gl_uint row    = y / 8;
    gl_uint subrow = 1 << (y % 8);
    while (w--) {
        _set_color(row, subrow, col, cr);
        ++col;
    }
}

static inline void _set_vline(gl_uint x, gl_uint y, gl_uint h, gl_color cr)
{
    gl_uint col = x;
    gl_uint row, subrow;
    while (h--) {
        row    = y / 8;
        subrow = 1 << (y % 8);
        _set_color(row, subrow, col, cr);
        ++y;
    }
}

////////////////////////////////////////////////////

void gl_draw_pixel(gl_int x, gl_int y, gl_color cr)
{
#if CONFIG_ENABLE_CLIPRECT
    if (clip_x1 <= x && x <= clip_x2 && clip_y1 <= y && y <= clip_y2)
#endif
        _set_pixel(x, y, cr);
}

void gl_draw_hline(gl_int x, gl_int y, gl_uint w, gl_color cr)
{
#if CONFIG_ENABLE_CLIPRECT
    gl_int ep = x + w - 1;
    if (ep < clip_x1 || x > clip_x2 || y < clip_y1 || y > clip_y2) return;
    if (ep > clip_x2) w = clip_x2 - x;
#endif
    _set_hline(x, y, w, cr);
}

void gl_draw_vline(gl_int x, gl_int y, gl_uint h, gl_color cr)
{
#if CONFIG_ENABLE_CLIPRECT
    gl_int ep = y + h - 1;
    if (x < clip_x1 || x > clip_x2 || ep < clip_y1 || y > clip_y2) return;
    if (ep > clip_y2) h = clip_y2 - y;
#endif
    _set_vline(x, y, h, cr);
}

void gl_fill_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_color cr)
{
    gl_int x1 = x, y1 = y, x2 = x + w, y2 = y + h;

#if CONFIG_ENABLE_CLIPRECT

    if (_hittest_rect(x1, y1, x2, y2) == 0) return;

    _intersect_with(&x1, &y1, &x2, &y2);

    w = x2 - x1;

#endif

    while (y1 < y2) {
        _set_hline(x1, y1, w, cr);
        ++y1;
    }
}

void gl_draw_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_color cr)
{
    gl_draw_hline(x, y, w, cr);
    gl_draw_vline(x, y, h, cr);
    gl_draw_hline(x, y + h, w, cr);
    gl_draw_vline(x + w, y, h, cr);
}

/**
 * @dir 0: counterclockwise, 1: clockwise
 */
void get_rotate_point(gl_int xc, gl_int yc, gl_int x1, gl_int y1, gl_int* x2, gl_int* y2, gl_real angle, bool dir)
{
    gl_real r  = gl_sqrt((y1 - yc) * (y1 - yc) + (x1 - xc) * (x1 - xc));
    gl_real a0 = atan2f(x1 - xc, y1 - yc);
    if (dir) {
        *x2 = gl_round(xc + r * gl_cos(a0 + angle));
        *y2 = gl_round(yc + r * gl_sin(a0 + angle));
    } else {
        *x2 = gl_round(xc + r * gl_cos(a0 - angle));
        *y2 = gl_round(yc + r * gl_sin(a0 - angle));
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if CONFIG_ENABLE_CIRCLE

void _draw_circle_section(gl_int x, gl_int y, gl_int xc, gl_int yc, gl_color cr, gl_selection opt)
{
    if (opt & Selection_RightTop) {
        gl_draw_pixel(xc + x, yc - y, cr);
        gl_draw_pixel(xc + y, yc - x, cr);
    }
    if (opt & Selection_LeftTop) {
        gl_draw_pixel(xc - x, yc - y, cr);
        gl_draw_pixel(xc - y, yc - x, cr);
    }
    if (opt & Selection_RightBottom) {
        gl_draw_pixel(xc + x, yc + y, cr);
        gl_draw_pixel(xc + y, yc + x, cr);
    }
    if (opt & Selection_LeftBottom) {
        gl_draw_pixel(xc - x, yc + y, cr);
        gl_draw_pixel(xc - y, yc + x, cr);
    }
}

void _fill_circle_section(gl_int x, gl_int y, gl_int xc, gl_int yc, gl_color cr, gl_selection opt)
{
    if (opt & Selection_RightTop) {
        gl_draw_vline(xc + x, yc - y, y + 1, cr);
        gl_draw_vline(xc + y, yc - x, x + 1, cr);
    }
    if (opt & Selection_LeftTop) {
        gl_draw_vline(xc - x, yc - y, y + 1, cr);
        gl_draw_vline(xc - y, yc - x, x + 1, cr);
    }
    if (opt & Selection_RightBottom) {
        gl_draw_vline(xc + x, yc, y + 1, cr);
        gl_draw_vline(xc + y, yc, x + 1, cr);
    }
    if (opt & Selection_LeftBottom) {
        gl_draw_vline(xc - x, yc, y + 1, cr);
        gl_draw_vline(xc - y, yc, x + 1, cr);
    }
}

void _draw_ellipse_section(gl_int x, gl_int y, gl_int xc, gl_int yc, gl_color cr, gl_selection opt)
{
    if (opt & Selection_RightTop)
        gl_draw_pixel(xc + x, yc - y, cr);
    if (opt & Selection_LeftTop)
        gl_draw_pixel(xc - x, yc - y, cr);
    if (opt & Selection_RightBottom)
        gl_draw_pixel(xc + x, yc + y, cr);
    if (opt & Selection_LeftBottom)
        gl_draw_pixel(xc - x, yc + y, cr);
}

void _fill_ellipse_section(gl_int x, gl_int y, gl_int xc, gl_int yc, gl_color cr, gl_selection opt)
{
    if (opt & Selection_RightTop)
        gl_draw_vline(xc + x, yc - y, y + 1, cr);
    if (opt & Selection_LeftTop)
        gl_draw_vline(xc - x, yc - y, y + 1, cr);
    if (opt & Selection_RightBottom)
        gl_draw_vline(xc + x, yc, y + 1, cr);
    if (opt & Selection_LeftBottom)
        gl_draw_vline(xc - x, yc, y + 1, cr);
}

void gl_draw_circle(gl_int xc, gl_int yc, gl_uint r, gl_color cr, gl_selection opt)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(xc - r, yc - r, xc + r + 1, yc + r + 1) == 0) return;
#endif

    gl_int  f  = 1 - r;
    gl_int  dx = 1, dy = -(r << 1);
    gl_uint x = 0, y = r;

    _draw_circle_section(x, y, xc, yc, cr, opt);

    while (x < y) {
        if (f >= 0) {
            --y;
            dy += 2;
            f += dy;
        }
        ++x;
        dx += 2;
        f += dx;
        _draw_circle_section(x, y, xc, yc, cr, opt);
    }
}

void gl_fill_circle(gl_int xc, gl_int yc, gl_uint r, gl_color cr, gl_selection opt)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(xc - r, yc - r, xc + r + 1, yc + r + 1) == 0) return;
#endif

    gl_int  f  = 1 - r;
    gl_int  dx = 1, dy = -(r << 1);
    gl_uint x = 0, y = r;

    _fill_circle_section(x, y, xc, yc, cr, opt);
    while (x < y) {
        if (f >= 0) {
            --y;
            dy += 2;
            f += dy;
        }
        ++x;
        dx += 2;
        f += dx;
        _fill_circle_section(x, y, xc, yc, cr, opt);
    }
}

void gl_draw_ellipse(gl_int xc, gl_int yc, gl_uint rx, gl_uint ry, gl_color cr, gl_selection opt)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(xc - rx, yc - ry, xc + rx + 1, yc + ry + 1) == 0) return;
#endif

    gl_uint x, y;
    gl_int  xchg, ychg;
    gl_int  err;
    gl_int  stopx, stopy;

    gl_int rxrx2 = (rx * rx) << 1,
           ryry2 = (ry * ry) << 1;

    x = rx;
    y = 0;

    xchg = (1 - (rx << 1)) * ry * ry;
    ychg = rx * rx;

    err = 0;

    stopx = ryry2 * rx;
    stopy = 0;

    while (stopx >= stopy) {
        _draw_ellipse_section(x, y, xc, yc, cr, opt);
        ++y;
        stopy += rxrx2;
        err += ychg;
        ychg += rxrx2;
        if ((err << 1) + xchg > 0) {
            --x;
            stopx -= ryry2;
            err += xchg;
            xchg += ryry2;
        }
    }

    x = 0;
    y = ry;

    xchg = ry * ry;
    ychg = (1 - (ry << 1)) * rx * rx;

    err = 0;

    stopx = 0;
    stopy = rxrx2 * ry;

    while (stopx <= stopy) {
        _draw_ellipse_section(x, y, xc, yc, cr, opt);
        ++x;
        stopx += ryry2;
        err += xchg;
        xchg += ryry2;
        if ((err << 1) + ychg > 0) {
            --y;
            stopy -= rxrx2;
            err += ychg;
            ychg += rxrx2;
        }
    }
}

void gl_fill_ellipse(gl_int xc, gl_int yc, gl_uint rx, gl_uint ry, gl_color cr, gl_selection opt)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(xc - rx, yc - ry, xc + rx + 1, yc + ry + 1) == 0) return;
#endif

    gl_uint x, y;
    gl_int  xchg, ychg;
    gl_int  err;
    gl_int  stopx, stopy;

    gl_int rxrx2 = (rx * rx) << 1,
           ryry2 = (ry * ry) << 1;

    x = rx;
    y = 0;

    xchg = (1 - (rx << 1)) * ry * ry;
    ychg = rx * rx;

    err = 0;

    stopx = ryry2 * rx;
    stopy = 0;

    while (stopx >= stopy) {
        _fill_ellipse_section(x, y, xc, yc, cr, opt);
        ++y;
        stopy += rxrx2;
        err += ychg;
        ychg += rxrx2;
        if ((err << 1) + xchg > 0) {
            --x;
            stopx -= ryry2;
            err += xchg;
            xchg += ryry2;
        }
    }

    x = 0;
    y = ry;

    xchg = ry * ry;
    ychg = (1 - (ry << 1)) * rx * rx;

    err = 0;

    stopx = 0;
    stopy = rxrx2 * ry;

    while (stopx <= stopy) {
        _fill_ellipse_section(x, y, xc, yc, cr, opt);
        ++x;
        stopx += ryry2;
        err += xchg;
        xchg += ryry2;
        if ((err << 1) + ychg > 0) {
            --y;
            stopy -= rxrx2;
            err += ychg;
            ychg += rxrx2;
        }
    }
}

void gl_draw_ellipse_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_color cr)
{
    gl_int x1 = x, y1 = y, x2 = x + w, y2 = y + h;

#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(x1, y1, x2, y2) == 0) return;
#endif

    gl_int b1  = h & 1;
    gl_int dx  = 4 * (1 - w) * h * h,
           dy  = 4 * (b1 + 1) * w * w;
    gl_int err = dx + dy + b1 * w * w;
    gl_int e2;

    if (x1 > x2) {
        x1 = x2;
        x2 += w;
    }

    if (y1 > y2) y1 = y2;
    y1 += (h + 1) >> 1;
    y2 = y1 - b1;
    w *= 8 * w;
    b1 = 8 * h * h;

    do {
        gl_draw_pixel(x2, y1, cr);
        gl_draw_pixel(x1, y1, cr);
        gl_draw_pixel(x1, y2, cr);
        gl_draw_pixel(x2, y2, cr);
        e2 = err << 1;  // err * 2
        if (e2 >= dx) {
            ++x1;
            --x2;
            dx += b1;
            err += dx;
        }
        if (e2 <= dy) {
            ++y1;
            --y2;
            dy += w;
            err += dy;
        }
    } while (x1 <= x2);

    while (y1 - y2 < h) {
        gl_draw_pixel(x1 - 1, y1, cr);
        gl_draw_pixel(x2 + 1, y1++, cr);
        gl_draw_pixel(x1 - 1, y2, cr);
        gl_draw_pixel(x2 + 1, y2--, cr);
    }
}

void gl_draw_rounded_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_uint r, gl_color cr)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(x, y, x + w, y + h) == 0) return;
#endif

    gl_uint xl = x + r,
            yu = y + r,
            xr = x + w - r - 1,
            yl = y + h - r - 1;

    gl_draw_circle(xl, yu, r, cr, Selection_LeftTop);
    gl_draw_circle(xr, yu, r, cr, Selection_RightTop);
    gl_draw_circle(xl, yl, r, cr, Selection_LeftBottom);
    gl_draw_circle(xr, yl, r, cr, Selection_RightBottom);

    gl_uint ww = w - (r << 1),
            hh = h - (r << 1);

    ++xl;
    ++yu;

    if (ww >= 3) {
        ww -= 2;
        --h;
        gl_draw_hline(xl, y, ww, cr);
        gl_draw_hline(xl, y + h, ww, cr);
    }

    if (hh >= 3) {
        hh -= 2;
        --w;
        gl_draw_vline(x, yu, hh, cr);
        gl_draw_vline(x + w, yu, hh, cr);
    }
}

void gl_fill_rounded_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_uint r, gl_color cr)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(x, y, x + w, y + h) == 0) return;
#endif

    gl_uint xl = x + r,
            yu = y + r,
            xr = x + w - r - 1,
            yl = y + h - r - 1;

    gl_fill_circle(xl, yu, r, cr, Selection_LeftTop);
    gl_fill_circle(xr, yu, r, cr, Selection_RightTop);
    gl_fill_circle(xl, yl, r, cr, Selection_LeftBottom);
    gl_fill_circle(xr, yl, r, cr, Selection_RightBottom);

    gl_uint ww = w - (r << 1),
            hh = h - (r << 1);

    ++xl;
    ++yu;

    if (ww >= 3) {
        ww -= 2;
        gl_fill_rect(xl, y, ww, r + 1, cr);
        gl_fill_rect(xl, yl, ww, r + 1, cr);
    }

    if (hh >= 3) {
        hh -= 2;
        gl_fill_rect(x, yu, w, hh, cr);
    }
}

// void gl_draw_rounded_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_uint r, gl_color cr)
// {
//     gl_int d = r << 1;  // d=2*r
//     if (w < d || h < d) return;
//     // four edge
//     gl_draw_hline(x + r, y, w - d, cr);
//     gl_draw_hline(x + r, y + h - 1, w - d, cr);
//     gl_draw_vline(x, y + r, h - d, cr);
//     gl_draw_vline(x + w - 1, y + r, h - d, cr);
//     // four corners
//     gl_draw_circle(x + r, y + r, r, cr, Selection_LeftTop);
//     gl_draw_circle(x + w - r - 1, y + r, r, cr, Selection_RightTop);
//     gl_draw_circle(x + w - r - 1, y + h - r - 1, r, cr, Selection_RightBottom);
//     gl_draw_circle(x + r, y + h - r - 1, r, cr, Selection_LeftBottom);
// }

// void gl_fill_rounded_rect(gl_int x, gl_int y, gl_uint w, gl_uint h, gl_uint r, gl_color cr)
// {
//     gl_int d = r << 1;  // d=2*r
//     if (w < d || h < d) return;
//     gl_fill_rect(x + r, y, w - d, r, cr);
//     gl_fill_rect(x, y + r, w, h - d, cr);
//     gl_fill_rect(x + r, y + h - r, w - d, r, cr);
//     // four corners
//     gl_fill_circle(x + r, y + r, r, cr, Selection_LeftTop);
//     gl_fill_circle(x + w - r - 1, y + r, r, cr, Selection_RightTop);
//     gl_fill_circle(x + w - r - 1, y + h - r - 1, r, cr, Selection_RightBottom);
//     gl_fill_circle(x + r, y + h - r - 1, r, cr, Selection_LeftBottom);
// }

void gl_draw_arc(gl_int xc, gl_int yc, gl_uint r, gl_real start_angle, gl_real end_angle, gl_color cr)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(xc - r, yc - r, xc + r, yc + r) == 0) return;
#endif

    if (end_angle < start_angle)
        end_angle += 2 * PI;

    gl_real dt = 0.4f / r;
    gl_int  n  = (int)((end_angle - start_angle) / dt);
    gl_real a  = start_angle;

    for (gl_int i = 0; i < n; ++i) {
        a += dt;
        gl_draw_pixel(
            roundf(xc + r * gl_cos(a)),
            roundf(yc + r * gl_sin(a)),
            cr);
    }
}

void gl_draw_pie(gl_int xc, gl_int yc, gl_uint r, gl_real start_angle, gl_real end_angle, gl_color cr)
{
#if CONFIG_ENABLE_CLIPRECT
    if (_hittest_rect(xc - r, yc - r, xc + r, yc + r) == 0) return;
#endif
    gl_draw_arc(xc, yc, r, start_angle, end_angle, cr);
    gl_draw_line(xc, yc, roundf(xc + r * gl_cos(start_angle)), roundf(yc + r * gl_sin(start_angle)), cr);
    gl_draw_line(xc, yc, roundf(xc + r * gl_cos(end_angle)), roundf(yc + r * gl_sin(end_angle)), cr);
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void gl_draw_triangle(gl_int x0, gl_int y0, gl_int x1, gl_int y1, gl_int x2, gl_int y2, gl_color cr)
{
    gl_draw_line(x0, y0, x1, y1, cr);
    gl_draw_line(x1, y1, x2, y2, cr);
    gl_draw_line(x2, y2, x0, y0, cr);
}

void gl_fill_triangle(gl_int x0, gl_int y0, gl_int x1, gl_int y1, gl_int x2, gl_int y2, gl_color cr)
{
    gl_int a, b, y, last;

    if (y0 > y1) {
        _swap_int(y0, y1);
        _swap_int(x0, x1);
    }
    if (y1 > y2) {
        _swap_int(y2, y1);
        _swap_int(x2, x1);
    }
    if (y0 > y1) {
        _swap_int(y0, y1);
        _swap_int(x0, x1);
    }

    if (y0 == y2) {
        a = b = x0;
        if (x1 < a) {
            a = x1;
        } else if (x1 > b) {
            b = x1;
        }
        if (x2 < a) {
            a = x2;
        } else if (x2 > b) {
            b = x2;
        }
        gl_draw_hline(a, y0, b - a + 1, cr);
        return;
    }

    gl_int dx01 = x1 - x0,
           dy01 = y1 - y0,
           dx02 = x2 - x0,
           dy02 = y2 - y0,
           dx12 = x2 - x1,
           dy12 = y2 - y1;

    gl_int sa = 0, sb = 0;

    if (y1 == y2) {
        last = y1;  // include y1 scanline
    } else {
        last = y1 - 1;  // skip it
    }

    for (y = y0; y <= last; ++y) {
        a = x0 + sa / dy01;
        b = x0 + sb / dy02;
        sa += dx01;
        sb += dx02;

        if (a > b) {
            _swap_int(a, b);
        }

        gl_draw_hline(a, y, b - a + 1, cr);
    }

    sa = dx12 * (y - y1);
    sb = dx02 * (y - y0);

    for (; y <= y2; ++y) {
        a = x1 + sa / dy12;
        b = x0 + sb / dy02;
        sa += dx12;
        sb += dx02;
        if (a > b) {
            _swap_int(a, b);
        }

        gl_draw_hline(a, y, b - a + 1, cr);
    }
}

void gl_draw_line(gl_int x1, gl_int y1, gl_int x2, gl_int y2, gl_color cr)
{
    uint8_t i;

    gl_int x = x1,
           y = y1;

    gl_int xerr = 0,
           yerr = 0;

    gl_int dx = x2 - x1,
           dy = y2 - y1;

    gl_int d;
    gl_int incx, incy;

    if (dx > 0) {
        incx = 1;
    } else if (dx == 0) {
        incx = 0;  // 垂直线
    } else {
        incx = -1;
        dx   = -dx;
    }

    if (dy > 0) {
        incy = 1;
    } else if (dy == 0) {
        incy = 0;  // 水平线
    } else {
        incy = -1;
        dy   = -dy;
    }

    d = dx > dy ? dx : dy;  // 基本增量坐标轴

    for (i = 0; i <= d + 1; ++i) {
        gl_draw_pixel(x, y, cr);
        xerr += dx;
        yerr += dy;
        if (xerr > d) {
            xerr -= d;
            x += incx;
        }
        if (yerr > d) {
            yerr -= d;
            y += incy;
        }
    }
}

void gl_draw_polygon(gl_int x[], gl_int y[], gl_uint len, bool closed, gl_color cr)
{
    uint8_t i;

    --len;

    for (i = 0; i < len; ++i)
        gl_draw_line(x[i], y[i], x[i + 1], y[i + 1], cr);

    if (closed)
        gl_draw_line(x[i], y[i], x[0], y[0], cr);
}
