/* -*- coding: utf-8 -*- */

/**
 * @file system.c
 * @brief システムクロック等の初期設定
 * @noop コメント内の [x.x.x] は LPC13xx ユーザマニュアル (UM10375) の章節番号を示す。
 * @noop 当面使用予定のない機能に関する設定はいいかげんです。
 */

#include <stdint.h>
#include "system.h"

#define CLOCK_SETUP  1
#define SYSCLK_SETUP 1
#define SYSOSC_SETUP 1
#define WDTOSC_SETUP 0
#define SYSPLL_SETUP 1
#define USBCLK_SETUP 1
#define USBPLL_SETUP 1

#define __SYS_OSC_CLK (__XTAL)        /* システムオシレータ sys_osc_clk */
#define __IRC_OSC_CLK 12000000UL      /* IRC (Internal Resonant Crystal) オシレータ irc_osc_clk */
#define __WDT_OSC_CLK 0               /* ウォッチドッグオシレータ wdt_osc_clk */

#define SYSOSCCTRL_BYPASS 0
#if (__SYS_OSC_CLK > 20000000UL)
  #define SYSOSCCTRL_FRANGE 1
#else
  #define SYSOSCCTRL_FRANGE 0
#endif
#define SYSOSCCTRL_VAL       ((SYSOSCCTRL_FRANGE << 1) | SYSOSCCTRL_BYPASS)    /* [3.5.7] */
#define WDTOSCCTRL_VAL       0x000000A0    /* [3.5.8] */
#define SYSPLLCTRL_VAL       0x00000025    /* [3.5.3][3.11.4] */
#define USBPLLCTRL_VAL       0x00000003    /* [3.5.5] */
#define SYSAHBCLKCTRL_VAL    0x0001005F    /* [3.5.18] */
#define SYSAHBCLKDIV_VAL     0x00000001    /* [3.5.17] */

#define SYSPLLCLKSEL_VAL     0x00000001    /* [3.5.11] */
#define MAINCLKSEL_VAL       0x00000003    /* [3.5.15] */
#define USBPLLCLKSEL_VAL     0x00000001    /* [3.5.13] */

/* sys_pllclkin [3.5.11] */
#if ((SYSPLLCLKSEL_VAL & 3) == 1)
  #define __SYS_PLLCLKIN (__SYS_OSC_CLK)
#else
  #define __SYS_PLLCLKIN (__IRC_OSC_CLK)
#endif

/* sys_pllclkout [3.5.7][3.5.3] */
#if (SYSOSCCTRL_BYPASS == 0)
  #define __SYS_PLLCLKOUT (__SYS_PLLCLKIN * ((SYSPLLCTRL_VAL & 0x1f) + 1))
#else
  #define __SYS_PLLCLKOUT (__SYS_OSC_CLK)
#endif

/* main_clk [3.5.15] */
#if ((MAINCLKSEL_VAL & 3) == 1)
  #define __MAIN_CLOCK (__SYS_PLLCLKIN)
#elif ((MAINCLKSEL_VAL & 3) == 2)
  #define __MAIN_CLOCK (__WDT_OSC_CLK)
#elif ((MAINCLKSEL_VAL & 3) == 3)
  #define __MAIN_CLOCK (__SYS_PLLCLKOUT)
#else
  #define __MAIN_CLOCK (__IRC_OSC_CLK)
#endif

/* system_clk */
#define __SYSTEM_CLOCK  (__MAIN_CLOCK / SYSAHBCLKDIV_VAL)

static void init_sysclk(void);
static void init_usbclk(void);
static inline void update_enable(uint32_t regaddr);
static inline void nop(int n);
static uint32_t s_sysclk;

/**
 * @brief ターゲットシステムの初期化
 * @return なし
 */
void sys_init(void)
{
    #if (CLOCK_SETUP)
        init_sysclk();
        init_usbclk();
    #else
        reg_write(SYSCON(SYSAHBCLKDIV), SYSAHBCLKDIV_VAL);
        reg_write(SYSCON(SYSAHBCLKCTRL), SYSAHBCLKCTRL_VAL);
    #endif

    s_sysclk = __SYSTEM_CLOCK;
}

/**
 * @brief システムクロックの周波数を返す
 * @return システムクロックの周波数
 */
uint32_t sys_clock(void)
{
    return s_sysclk;
}

/**
 * @brief システムクロックの初期設定
 * @return なし
 */
static void init_sysclk(void)
{
    #if (SYSCLK_SETUP)
        #if (SYSOSC_SETUP)
            __bb_clr_bit(SYSCON(PDRUNCFG), 5);                         /* システムオシレータ パワーダウン解除 [3.5.47] */
            reg_write(SYSCON(SYSOSCCTRL), SYSOSCCTRL_VAL);           /* システムオシレータ 設定 [3.5.7] */
            nop(200);

            reg_write(SYSCON(SYSPLLCLKSEL), SYSPLLCLKSEL_VAL);       /* システム PLL クロックソース選択 [3.5.11] */
            update_enable(SYSCON(SYSPLLCLKUEN));                     /* システム PLL クロックアップデート待ち [3.5.12] */

            #if (SYSPLL_SETUP)
                reg_write(SYSCON(SYSPLLCTRL), SYSPLLCTRL_VAL);       /* システム PLL 設定 [3.5.3] */
                __bb_clr_bit(SYSCON(PDRUNCFG), 7);                     /* システム PLL パワーダウン解除 [3.5.47] */
                while (__bb_read_bit(SYSCON(SYSPLLSTAT), 0) == 0);     /* システム PLL ロック待ち [3.5.4] */
            #endif
        #endif

        #if (WDTOSC_SETUP)
            reg_write(SYSCON(WDTOSCCTRL), WDTOSCCTRL_VAL);           /* ウォッチドッグオシレータ 設定 [3.5.8] */
            __bb_clr_bit(SYSCON(PDRUNCFG), 6);                         /* ウォッチドッグオシレータ パワーダウン解除 [3.5.47] */
        #endif

        reg_write(SYSCON(MAINCLKSEL), MAINCLKSEL_VAL);               /* メインクロック クロックソース選択 [3.5.15] */
        update_enable(SYSCON(MAINCLKUEN));                           /* メインクロックアップデート待ち [3.5.16] */
    #endif
}

/**
 * @brief USB クロックの初期設定
 * @return なし
 */
static void init_usbclk(void)
{
    #if (USBCLK_SETUP)
        __bb_clr_bit(SYSCON(PDRUNCFG), 10);                          /* USB PHY チップ パワーダウン解除 [3.5.47] */

        #if (USBPLL_SETUP)
            __bb_clr_bit(SYSCON(PDRUNCFG), 8);                       /* USB PLL パワーダウン解除 [3.5.47] */
            reg_write(SYSCON(USBPLLCLKSEL), USBPLLCLKSEL_VAL);     /* USB PLL クロックソース選択 [3.5.13] */
            update_enable(SYSCON(USBPLLCLKUEN));                   /* USB PLL クロックアップデート待ち [3.5.14] */
            reg_write(SYSCON(USBPLLCTRL), USBPLLCTRL_VAL);         /* USB PLL 設定 [3.5.5] */
            while (__bb_read_bit(SYSCON(USBPLLSTAT), 0) == 0);       /* USB PLL ロック待ち [3.5.6] */
            reg_write(SYSCON(USBCLKSEL), 0);                       /* USB クロック選択; USB PLL out [3.5.24] */
        #else
            reg_write(SYSCON(USBCLKSEL), 1);                       /* USB クロック選択; メインクロック [3.5.24] */
        #endif
    #else
        __bb_set_bit(SYSCON(PDRUNCFG), 10);                          /* USB PHY チップ パワーダウン有効 [3.5.47] */
        __bb_set_bit(SYSCON(PDRUNCFG), 8);                           /* USB PLL パワーダウン有効 [3.5.47] */
    #endif
}

/**
 * @brief クロックアップデート待ち
 * @return なし
 */
static inline void update_enable(uint32_t regaddr)
{
    __bb_write_bit(regaddr, 0, 1);
    __bb_write_bit(regaddr, 0, 0);
    __bb_write_bit(regaddr, 0, 1);
    while (__bb_read_bit(regaddr, 0) == 0);
}

/**
 * @brief nop を n 回実行する
 * @return なし
 */
static inline void nop(int n)
{
    for (; n > 0; n--) {  __asm("nop"); }
}
