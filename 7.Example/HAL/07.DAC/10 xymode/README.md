# stm32-xy-mode

STM32 DAC example: drawing on oscilloscope in XY-mode.

```C
uint8_t xs[] = {
    2, 2, 22, 22, 2, 22,                     // R
    27, 48, 48, 27, 27, 27, 48, 48, 27, 48,  // 2
    52, 52, 73, 73, 52, 73, 73,              // A
    85, 77, 77, 77, 85, 91, 98, 98, 98, 91,  // U
    102, 102, 102, 123, 102, 123,            // K
};
uint8_t ys[] = {
    42, 84, 84, 63, 63, 42,                  // R
    42, 63, 84, 84, 63, 84, 84, 63, 42, 42,  // 2
    42, 84, 84, 63, 63, 63, 42,              // A
    42, 50, 84, 50, 42, 42, 50, 84, 50, 42,  // U
    42, 84, 63, 84, 63, 42,                  // K
};

int16_t idx       = 0;
int16_t prev_idx  = 0;
uint8_t curr_x    = 0;
uint8_t curr_y    = 0;
int16_t dx        = 0;
int16_t dy        = 0;
int8_t  direction = 1;

void next_line()
{
    const uint8_t speed          = 32;
    int8_t        next_direction = direction;
    idx += direction;
    if (idx < 0) {
        idx            = 0;
        next_direction = -direction;
    } else if (idx >= sizeof(xs) / sizeof(xs[0])) {
        idx            = sizeof(xs) / sizeof(xs[0]) - 1;
        next_direction = -direction;
    }

    prev_idx = idx - direction;
    if (prev_idx < 0) {
        prev_idx = sizeof(xs) / sizeof(xs[0]) - 1;
    } else if (prev_idx >= sizeof(xs) / sizeof(xs[0])) {
        prev_idx = 0;
    }

    direction = next_direction;
    curr_x    = xs[prev_idx];
    curr_y    = ys[prev_idx];

    dx = (((int16_t)xs[idx]) - ((int16_t)xs[prev_idx])) / speed;
    dy = (((int16_t)ys[idx]) - ((int16_t)ys[prev_idx])) / speed;
    if (dx == 0) {
        if (xs[idx] > xs[prev_idx]) {
            dx = 1;
        } else if (xs[idx] < xs[prev_idx]) {
            dx = -1;
        }
    }

    if (dy == 0) {
        if (ys[idx] > ys[prev_idx]) {
            dy = 1;
        } else if (ys[idx] < ys[prev_idx]) {
            dy = -1;
        }
    }
}

void init()
{
    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
    next_line();
    HAL_Delay(1);
}

void loop()
{
    curr_x += dx;
    curr_y += dy;
    if (((dx > 0) && (curr_x > xs[idx])) ||
        ((dx < 0) && (curr_x < xs[idx]))) {
        curr_x = xs[idx];
    }
    if (((dy > 0) && (curr_y > ys[idx])) ||
        ((dy < 0) && (curr_y < ys[idx]))) {
        curr_y = ys[idx];
    }

    uint32_t x = ((uint32_t)curr_x) * 0xFFF / 128;
    uint32_t y = ((uint32_t)curr_y) * 0xFFF / 128;
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, x);
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, y);

    if ((curr_x == xs[idx]) && (curr_y == ys[idx])) {
        next_line();
    }
}

int main(void)
{
    init();
    while (1) {
        loop();
    }
}
```
