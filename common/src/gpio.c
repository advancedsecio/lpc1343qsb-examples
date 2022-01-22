/* -*- coding: utf-8 -*- */

/**
 * @file gpio.c
 * @brief 汎用 I/O ポート (GPIO) を操作するための関数群
 */

#include "lpc1343.h"
#include "gpio.h"

#define NUM_PORT 4

/**
 * @brief GPIO 初期化
 * @return なし
 */
void gpio_init(void)
{
    reg_set_bit(SYSCON(SYSAHBCLKCTRL), 6);

    nvic_enable_irq(IRQ_PIO0);
    nvic_enable_irq(IRQ_PIO1);
    nvic_enable_irq(IRQ_PIO2);
    nvic_enable_irq(IRQ_PIO3);
}

/**
 * @brief 指定されたピンの入出力方向を設定する
 * @param[in] pno ポート番号 (0..3)
 * @param[in] nthbit ピン番号に対応するビット位置 (0..11)
 * @param[in] dir 0 なら入力, 1 なら出力
 * @return 0: 書き込み成功, -1: 失敗
 */
int8_t gpio_set_dir(uint8_t pno, uint8_t nthbit, uint8_t dir)
{
    if (pno >= NUM_PORT) return -1;
    __reg_write_bit(GPIOn(pno, DIR), nthbit, dir & 1);
    return 0;
}

/**
 * @brief 指定された GPIO DATA レジスタのビットを立てる/クリアする
 * @param[in] pno ポート番号 (0..3)
 * @param[in] nthbit ピン番号に対応するビット位置 (0..11)
 * @param[in] v 0 ならクリアする，1 なら立てる
 * @return 0: 書き込み成功, -1: 失敗
 */
int8_t gpio_write(uint8_t pno, uint8_t nthbit, uint8_t v)
{
    if (pno >= NUM_PORT) return -1;
    uint32_t mask = (1 << nthbit);
    __reg_write_bit(GPIOnDATA(pno, mask), nthbit, v & 1);
    return 0;
}

/**
 * @brief 指定された GPIO DATA レジスタのビットを読み出す
 * @param[in] pno ポート番号 (0..3)
 * @param[in] nthbit ピン番号に対応するビット位置 (0..11)
 * @return 読み出した値 (0 または 1), 失敗時は -1
 */
int8_t gpio_read(uint8_t pno, uint8_t nthbit)
{
    if (pno >= NUM_PORT) return -1;
    uint32_t mask = (1 << nthbit);
    return __reg_read_bit(GPIOnDATA(pno, mask), nthbit);
}
