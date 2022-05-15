/* -*- coding: utf-8 -*- */

/**
 * @file main.c
 * @brief ユーザプログラムの main 関数
 */

#include <stdint.h>
#include "system.h"

#define TMRNO 0    /* タイマ番号 (0 または 1) */

/**
 * @brief LPC1343 QSB の LED3 をチカチカさせる
 * @return 0: 正常終了
 */
int main(void)
{
    uint8_t led = 0;

    gpio_init();
    gpio_set_dir(3, 0, 1);    /* GPIO3_0: LED 出力 */
    gpio_write(3, 0, led);

    tmr32_init(TMRNO);

    /* 1000 ms 間隔で LED 出力を反転させる */
    while (1) {
        tmr32_delay_ms(TMRNO, 1000);
        led = !led;
        gpio_write(3, 0, led);
    }

    return 0;
}
