/* -*- coding: utf-8 -*- */

/**
 * @file timer32.c
 * @brief 32-bit タイマ/カウンタ (CT32B0/1) を操作するための関数群
 */

#include "system.h"
#include "timer32.h"

#define NUM_TIMER32 2

/**
 * @brief タイマ初期化
 * @param[in] tno タイマ番号 (0 または 1)
 * @return なし
 */
void tmr32_init(uint8_t tno)
{
    if (tno >= NUM_TIMER32) return;
    reg_set_bit(SYSCON(SYSAHBCLKCTRL), 9);
    nvic_enable_irq(IRQ_TIMER32_0 + tno);
}

/**
 * @brief ミリ秒単位で遅延を発生させる
 * @param[in] tno タイマ番号 (0 または 1)
 * @param[in] t 遅延時間 [ms]
 * @return なし
 */
void tmr32_delay_ms(uint8_t tno, uint32_t t)
{
    if (tno >= NUM_TIMER32) return;

    t *= ((sys_clock() / reg_read(SYSCON(SYSAHBCLKDIV))) / 1000);    /* 遅延時間をクロックカウント値に換算 */

    reg_write(TMR32Bn(tno, TCR), 0x02);    /* タイマカウンタリセット */
    reg_write(TMR32Bn(tno, PR), 0x00);     /* プリスケーラは使用しない */
    reg_write(TMR32Bn(tno, MR0), t);       /* タイマカウンタとの比較値をセット */
    reg_write(TMR32Bn(tno, IR), 0xFF);     /* すべての割り込みをリセットする */
    reg_write(TMR32Bn(tno, MCR), 0x04);    /* MR0 の値に一致したらタイマを止める */
    reg_write(TMR32Bn(tno, TCR), 0x01);    /* タイマ開始 */

    while (reg_read_bit(TMR32Bn(tno, TCR), 0));
}

/**
 * @brief マイクロ秒単位で遅延を発生させる
 * @param[in] tno タイマ番号 (0 または 1)
 * @param[in] t 遅延時間 [us]
 * @return なし
 */
void tmr32_delay_us(uint8_t tno, uint32_t t)
{
    if (tno >= NUM_TIMER32) return;

    t *= ((sys_clock() / reg_read(SYSCON(SYSAHBCLKDIV))) / 1000000);    /* 遅延時間をクロックカウント値に換算 */

    reg_write(TMR32Bn(tno, TCR), 0x02);    /* タイマカウンタリセット */
    reg_write(TMR32Bn(tno, PR), 0x00);     /* プリスケーラは使用しない */
    reg_write(TMR32Bn(tno, MR0), t);       /* タイマカウンタとの比較値をセット */
    reg_write(TMR32Bn(tno, IR), 0xFF);     /* すべての割り込みをリセットする */
    reg_write(TMR32Bn(tno, MCR), 0x04);    /* MR0 の値に一致したらタイマを止める */
    reg_write(TMR32Bn(tno, TCR), 0x01);    /* タイマ開始 */

    while (reg_read_bit(TMR32Bn(tno, TCR), 0));
}
