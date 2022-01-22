/* -*- coding: utf-8 -*- */

/**
 * @file cortexm3.h
 * @brief Cortex-M3 コアに関する定義・宣言
 */

#ifndef __CORTEXM3_H__
#define __CORTEXM3_H__

#include <stdint.h>
#include "cortexm3_reg.h"

/**
 * @def NVICn(reg, n)
 * @details NVIC レジスタのアドレスを得るためのマクロ。\n
 *          例えば NVICn(ISER, 1) とすると 0xE000E104 (NVIC_ISER1 のアドレス) を得る。
 */
#define NVICn(reg, n) ((NVIC_BASE + NVIC_##reg + ((n) << 2)))

/**
 * @def NVIC(regn)
 * @details NVIC レジスタのアドレスを得るためのマクロ。\n
 *          例えば NVIC(ISER1) とすると 0xE000E104 (NVIC_ISER1 のアドレス) を得る。
 */
#define NVIC(regn)    ((NVIC_BASE + NVIC_##regn))

#define NVIC_BASE  0xE000E000
#define NVIC_ISER  0x100
#define NVIC_ISER0 (NVIC_ISER + 0x00)
#define NVIC_ISER1 (NVIC_ISER + 0x04)
#define NVIC_ICER  0x180
#define NVIC_ICER0 (NVIC_ICER + 0x00)
#define NVIC_ICER1 (NVIC_ICER + 0x04)
#define NVIC_ISPR  0x200
#define NVIC_ISPR0 (NVIC_ISPR + 0x00)
#define NVIC_ISPR1 (NVIC_ISPR + 0x04)
#define NVIC_ICPR  0x280
#define NVIC_ICPR0 (NVIC_ICPR + 0x00)
#define NVIC_ICPR1 (NVIC_ICPR + 0x04)
#define NVIC_IABR  0x300
#define NVIC_IABR0 (NVIC_IABR + 0x00)
#define NVIC_IABR1 (NVIC_IABR + 0x04)
#define NVIC_IPR   0x400
#define NVIC_IPR0  (NVIC_IPR + 0x00)
#define NVIC_IPR1  (NVIC_IPR + 0x04)
#define NVIC_IPR2  (NVIC_IPR + 0x08)
#define NVIC_IPR3  (NVIC_IPR + 0x0C)
#define NVIC_IPR4  (NVIC_IPR + 0x10)
#define NVIC_IPR5  (NVIC_IPR + 0x14)
#define NVIC_IPR6  (NVIC_IPR + 0x18)
#define NVIC_IPR7  (NVIC_IPR + 0x1C)
#define NVIC_IPR8  (NVIC_IPR + 0x20)
#define NVIC_IPR9  (NVIC_IPR + 0x24)
#define NVIC_IPR10 (NVIC_IPR + 0x28)
#define NVIC_IPR11 (NVIC_IPR + 0x2C)
#define NVIC_IPR12 (NVIC_IPR + 0x30)
#define NVIC_IPR13 (NVIC_IPR + 0x34)
#define NVIC_IPR14 (NVIC_IPR + 0x38)
#define NVIC_STIR  0xF00

/**
 * Cortex-M3 IRQ 番号\n
 * 負の値のとき、プロセッサ内部の例外を表す。\n
 * 0 以上の値のとき、外部割込み入力 (IRQ 0..239) を表す。
 */
typedef enum irq {
    /* Cortex-M3 例外番号*/
    EX_NMI = -14,
    EX_HARDFALUT = -13,
    EX_MEMMANAGE = -12,
    EX_BUSFAULT = -11,
    EX_USAGEFAULT = -10,
    EX_SVCALL = -5,
    EX_PENDSV = -2,
    EX_SYSTICK = -1,
    /* 0..239: 外部割込み入力 (マイコンごとに別途定義する) */
} irq_t;

/**
 * @brief 外部割込み n 番を許可する
 * @param[in] n 許可する割込みの番号 (0..239)
 * @return なし
 */
static inline void nvic_enable_irq(irq_t n)
{
    __reg_set_bit(NVICn(ISER, (n >> 5)), n & 0x1F);
}

/**
 * @brief 外部割込み n 番を禁止する
 * @param[in] n 禁止する割込みの番号 (0..239)
 * @return なし
 */
static inline void nvic_disable_irq(irq_t n)
{
    __reg_set_bit(NVICn(ICER, (n >> 5)), n & 0x1F);
}

#endif
