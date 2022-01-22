/* -*- coding: utf-8 -*- */

/**
 * @file gpio.h
 * @brief 汎用 I/O ポート (GPIO) に関する定義・宣言
 */

#ifndef __GPIO_H__
#define __GPIO_H__

void gpio_init(void);
int8_t gpio_set_dir(uint8_t pno, uint8_t nthbit, uint8_t dir);
int8_t gpio_write(uint8_t pno, uint8_t nthbit, uint8_t v);
int8_t gpio_read(uint8_t pno, uint8_t nthbit);

#endif
