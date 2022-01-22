/* -*- coding: utf-8 -*- */

/**
 * @file timer32.h
 * @brief 32-bit タイマに関する定義・宣言
 */

#ifndef __TIMER32_H__
#define __TIMER32_H__

#include <stdint.h>

void tmr32_init(uint8_t tno);
void tmr32_delay_ms(uint8_t tno, uint32_t ms);
void tmr32_delay_us(uint8_t tno, uint32_t us);

#endif
