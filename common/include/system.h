/* -*- coding: utf-8 -*- */

/**
 * @file system.h
 * @brief ターゲットシステム全般に関する定義、宣言
 */

#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "lpc1343.h"
#include "gpio.h"
#include "timer32.h"

#define __XTAL 12000000UL    /* 外部水晶の発振周波数 */

void sys_init(void);
uint32_t sys_clock(void);

#endif
