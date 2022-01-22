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
    gpio_set_dir(0, 7, 1);    /* GPIO0_7: LED 出力 */

    tmr32_init(TMRNO);

    /* 500 ms 間隔で LED 出力を反転させる */
    while (1) {
        gpio_write(0, 7, led);
        led = !led;
        tmr32_delay_ms(TMRNO, 500);
    }

    return 0;
}
