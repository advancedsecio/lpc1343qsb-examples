/* -*- coding: utf-8 -*- */

/**
 * @file lpc1343.h
 * @brief LPC1343 全般に関する定義・宣言
 */

#ifndef __LPC1343_H__
#define __LPC1343_H__

#include <stdint.h>
#include "cortexm3.h"

/**
 * LPC1343 IRQ 番号 (0..239(max))
 */
typedef enum lpc1343_irq {
    IRQ_WAKEUP0 = 0,
    IRQ_WAKEUP1,
    IRQ_WAKEUP2,
    IRQ_WAKEUP3,
    IRQ_WAKEUP4,
    IRQ_WAKEUP5,
    IRQ_WAKEUP6,
    IRQ_WAKEUP7,
    IRQ_WAKEUP8,
    IRQ_WAKEUP9,
    IRQ_WAKEUP10,
    IRQ_WAKEUP11,
    IRQ_WAKEUP12,
    IRQ_WAKEUP13,
    IRQ_WAKEUP14,
    IRQ_WAKEUP15,
    IRQ_WAKEUP16,
    IRQ_WAKEUP17,
    IRQ_WAKEUP18,
    IRQ_WAKEUP19,
    IRQ_WAKEUP20,
    IRQ_WAKEUP21,
    IRQ_WAKEUP22,
    IRQ_WAKEUP23,
    IRQ_WAKEUP24,
    IRQ_WAKEUP25,
    IRQ_WAKEUP26,
    IRQ_WAKEUP27,
    IRQ_WAKEUP28,
    IRQ_WAKEUP29,
    IRQ_WAKEUP30,
    IRQ_WAKEUP31,
    IRQ_WAKEUP32,
    IRQ_WAKEUP33,
    IRQ_WAKEUP34,
    IRQ_WAKEUP35,
    IRQ_WAKEUP36,
    IRQ_WAKEUP37,
    IRQ_WAKEUP38,
    IRQ_WAKEUP39,
    IRQ_I2C,
    IRQ_TIMER16_0,
    IRQ_TIMER16_1,
    IRQ_TIMER32_0,
    IRQ_TIMER32_1,
    IRQ_SSP0,
    IRQ_UART,
    IRQ_USBIRQ,
    IRQ_USBFIQ,
    IRQ_ADC,
    IRQ_WDT,
    IRQ_BOD,
    IRQ_PIO3,
    IRQ_PIO2,
    IRQ_PIO1,
    IRQ_PIO0,
    IRQ_SSP1,
} lpc1343_irq_t;

/*
 * System control レジスタ
 */

/**
 * @def SYSCON(reg)
 * System control レジスタのアドレスを得るためのマクロ。\n
 * 例えば SYSCON(SYSOSCCTRL) とすると 0x40048020 (SYSCON_SYSOSCCTRL) を得る。
 */
#define SYSCON(reg) (SYSCON_BASE + SYSCON_##reg)

#define SYSCON_BASE          0x40048000
#define SYSCON_SYSMEMREMAP   0x000
#define SYSCON_PRESETCTRL    0x004
#define SYSCON_SYSPLLCTRL    0x008
#define SYSCON_SYSPLLSTAT    0x00C
#define SYSCON_USBPLLCTRL    0x010
#define SYSCON_USBPLLSTAT    0x014
#define SYSCON_SYSOSCCTRL    0x020
#define SYSCON_WDTOSCCTRL    0x024
#define SYSCON_IRCCTRL       0x028
#define SYSCON_SYSRESSTAT    0x030
#define SYSCON_SYSPLLCLKSEL  0x040
#define SYSCON_SYSPLLCLKUEN  0x044
#define SYSCON_USBPLLCLKSEL  0x048
#define SYSCON_USBPLLCLKUEN  0x04C
#define SYSCON_MAINCLKSEL    0x070
#define SYSCON_MAINCLKUEN    0x074
#define SYSCON_SYSAHBCLKDIV  0x078
#define SYSCON_SYSAHBCLKCTRL 0x080
#define SYSCON_SSP0CLKDIV    0x094
#define SYSCON_UARTCLKDIV    0x098
#define SYSCON_SSP1CLKDIV    0x09C
#define SYSCON_TRACECLKDIV   0x0AC
#define SYSCON_SYSTICKCLKDIV 0x0B0
#define SYSCON_USBCLKSEL     0x0C0
#define SYSCON_USBCLKUEN     0x0C4
#define SYSCON_USBCLKDIV     0x0C8
#define SYSCON_WDTCLKSEL     0x0D0
#define SYSCON_WDTCLKUEN     0x0D4
#define SYSCON_WDTCLKDIV     0x0D8
#define SYSCON_CLKOUTCLKSEL  0x0E0
#define SYSCON_CLKOUTUEN     0x0E4
#define SYSCON_CLKOUTDIV     0x0E8
#define SYSCON_PIOPORCAP0    0x100
#define SYSCON_PIOPORCAP1    0x104
#define SYSCON_BODCTRL       0x150
#define SYSCON_SYSTCKCAL     0x154
#define SYSCON_STARTAPRP0    0x200
#define SYSCON_STARTERP0     0x204
#define SYSCON_STARTRSRP0CLR 0x208
#define SYSCON_STARTSRP0     0x20C
#define SYSCON_STARTAPRP1    0x210
#define SYSCON_STARTERP1     0x214
#define SYSCON_STARTRSRP1CLR 0x218
#define SYSCON_STARTSRP1     0x21C
#define SYSCON_PDSLEEPCFG    0x230
#define SYSCON_PDAWAKECFG    0x234
#define SYSCON_PDRUNCFG      0x238
#define SYSCON_DEVICEID      0x3F4

/*
 * I/O Configuration レジスタ
 */

/**
 * @def IOCON(reg)
 * I/O Configuration レジスタのアドレスを得るためのマクロ。\n
 * 例えば IOCON(PIO0_7) とすると 0x40044050 (IOCON_PIO0_7) を得る。
 */
#define IOCON(reg) (IOCON_BASE + IOCON_##reg)

#define IOCON_BASE           0x40044000
#define IOCON_PIO2_6         0x000
/* Reserved                  0x004 */
#define IOCON_PIO2_0         0x008
#define IOCON_RESET_PIO0_0   0x00C
#define IOCON_PIO0_1         0x010
#define IOCON_PIO1_8         0x014
/* Reserved                  0x018 */
#define IOCON_PIO0_2         0x01C
#define IOCON_PIO2_7         0x020
#define IOCON_PIO2_8         0x024
#define IOCON_PIO2_1         0x028
#define IOCON_PIO0_3         0x02C
#define IOCON_PIO0_4         0x030
#define IOCON_PIO0_5         0x034
#define IOCON_PIO1_9         0x038
#define IOCON_PIO3_4         0x03C
#define IOCON_PIO2_4         0x040
#define IOCON_PIO2_5         0x044
#define IOCON_PIO3_5         0x048
#define IOCON_PIO0_6         0x04C
#define IOCON_PIO0_7         0x050
#define IOCON_PIO2_9         0x054
#define IOCON_PIO2_10        0x058
#define IOCON_PIO2_2         0x05C
#define IOCON_PIO0_8         0x060
#define IOCON_PIO0_9         0x064
#define IOCON_SWCLK_PIO0_10  0x068
#define IOCON_PIO1_10        0x06C
#define IOCON_PIO2_11        0x070
#define IOCON_R_PIO0_11      0x074
#define IOCON_R_PIO1_0       0x078
#define IOCON_R_PIO1_1       0x07C
#define IOCON_R_PIO1_2       0x080
#define IOCON_PIO3_0         0x084
#define IOCON_PIO3_1         0x088
#define IOCON_PIO2_3         0x08C
#define IOCON_SWDIO_PIO1_3   0x090
#define IOCON_PIO1_4         0x094
#define IOCON_PIO1_11        0x098
#define IOCON_PIO3_2         0x09C
#define IOCON_PIO1_5         0x0A0
#define IOCON_PIO1_6         0x0A4
#define IOCON_PIO1_7         0x0A8
#define IOCON_PIO3_3         0x0AC
#define IOCON_SCK0_LOC       0x0B0
#define IOCON_DSR_LOC        0x0B4
#define IOCON_DCD_LOC        0x0B8
#define IOCON_RI_LOC         0x0BC

/*
 * GPIO レジスタ
 */

/**
 * @def GPIOn(n, reg)
 * GPIO[n] レジスタのアドレスを得るためのマクロ。n はポート番号 (0..3)。\n
 * 例えば GPIOn(1, DIR) とすると 0x50018000 (GPIO1 DIR レジスタ) を得る。
 */
#define GPIOn(n, reg) (GPIO_BASE + ((n) << 16) + GPIO_##reg)
#define GPIO0(reg)    (GPIO0_BASE + GPIO_##reg))
#define GPIO1(reg)    (GPIO1_BASE + GPIO_##reg))
#define GPIO2(reg)    (GPIO2_BASE + GPIO_##reg))
#define GPIO3(reg)    (GPIO3_BASE + GPIO_##reg))

/**
 * @def GPIOnDATA(n, mask)
 * GPIO[n]DATA レジスタのアドレスを得るためのマクロ。n はポート番号 (0..3)、
 * mask はリード/ライトデータに適用する 12-bit 幅のマスク (0x0000..0x0FFF)。
 * マスクについては LPC1311/13/42/43 ユーザマニュアル (UM10375) 9.5.1 節を参照。\n
 * 例えば GPIOnDATA(1, 0x000F) とすると、0x000F << 2 = 0x003C がオフセットとして加算されて
 * 0x5001003C (GPIO1 DATA レジスタ) を得る。
 * そしてこのアドレスへのリード/ライトデータにはマスク 0x000F が適用される。
 * マスクが全ビット 1 ならオフセットは 0x0FFF << 2 = 0x3FFC となる。
 */
#define GPIOnDATA(n, mask) (GPIO_BASE + ((n) << 16) + (mask << 2))
#define GPIO0DATA(mask)    (GPIO0_BASE + (mask << 2))
#define GPIO1DATA(mask)    (GPIO1_BASE + (mask << 2))
#define GPIO2DATA(mask)    (GPIO2_BASE + (mask << 2))
#define GPIO3DATA(mask)    (GPIO3_BASE + (mask << 2))

#define GPIO_BASE  0x50000000
#define GPIO0_BASE (GPIO_BASE + 0x00000)
#define GPIO1_BASE (GPIO_BASE + 0x10000)
#define GPIO2_BASE (GPIO_BASE + 0x20000)
#define GPIO3_BASE (GPIO_BASE + 0x30000)

#define GPIO_DATA 0x3FFC
#define GPIO_DIR  0x8000
#define GPIO_IS   0x8004
#define GPIO_IBE  0x8008
#define GPIO_IEV  0x800C
#define GPIO_IE   0x8010
#define GPIO_RIS  0x8014
#define GPIO_MIS  0x8018
#define GPIO_IC   0x801C

/*
 * 32-bit タイマレジスタ
 */

/**
 * @def TMR32Bn(n, reg)
 * TMR32B[n] レジスタのアドレスを得るためのマクロ。n はタイマ番号 (0, 1)。\n
 * 例えば TMR32Bn(1, TCR) とすると 0x40018004 (TMR32B1TCR) を得る。\n
 */
#define TMR32Bn(n, reg) (TMR32B_BASE + ((n) << 14) + TMR32B_##reg)
#define TMR32B0(reg)    (TMR32B0_BASE + TMR32B_##reg)
#define TMR32B1(reg)    (TMR32B1_BASE + TMR32B_##reg)

#define TMR32B_BASE  0x40014000
#define TMR32B0_BASE (TMR32B_BASE + 0x0000)
#define TMR32B1_BASE (TMR32B_BASE + 0x4000)

#define TMR32B_IR   0x000
#define TMR32B_TCR  0x004
#define TMR32B_TC   0x008
#define TMR32B_PR   0x00C
#define TMR32B_PC   0x010
#define TMR32B_MCR  0x014
#define TMR32B_MR0  0x018
#define TMR32B_MR1  0x01C
#define TMR32B_MR2  0x020
#define TMR32B_MR3  0x024
#define TMR32B_CCR  0x028
#define TMR32B_CR0  0x02C
#define TMR32B_EMR  0x03C
#define TMR32B_CTCR 0x070
#define TMR32B_RWMC 0x074

#endif
