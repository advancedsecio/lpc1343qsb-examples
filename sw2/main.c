/* -*- coding: utf-8 -*- */

/**
 * @file main.c
 * @brief ユーザプログラムの main 関数
 */

#include <stdint.h>
#include "system.h"

#define TMRNO 0    /* タイマ番号 (0 または 1) */

/**
 * @brief スイッチ SW2 を押している間だけ LED を点灯させる
 * @return 0: 正常終了
 */
int main(void)
{
    reg_write(IOCON(PIO0_1), 0xD0);    /* PIO0_1: Pull-up */

    gpio_init();
    gpio_set_dir(0, 1, 0);             /* GPIO0_1: SW 入力 */
    gpio_set_dir(0, 7, 1);             /* GPIO0_7: LED 出力 */

    tmr32_init(TMRNO);

    gpio_write(0, 1, 0);
    gpio_write(0, 7, 0);

    while (1) {
        gpio_write(0, 7, !gpio_read(0, 1));
        tmr32_delay_ms(TMRNO, 10);
    }

    return 0;
}
