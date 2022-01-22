/* -*- coding: utf-8 -*- */

/**
 * @file startup.c
 * @brief ベクタテーブル, スタートアップルーチン
 */

extern void sys_init(void);
extern void main(void);

/* リンカスクリプトのロケーションカウンタを参照する */
extern unsigned long _main_sp;
extern unsigned long _data_org;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;

#define ATTR(defh) __attribute__ ((weak, alias (#defh)))

void reset_handler(void) __attribute__ ((noreturn));
void nmi_handler(void) ATTR(ex_handler);
void hardfault_handler(void) ATTR(ex_handler);
void memmanage_handler(void) ATTR(ex_handler);
void busfault_handler(void) ATTR(ex_handler);
void usagefault_handler(void) ATTR(ex_handler);
void svcall_handler(void) ATTR(ex_handler);
void debugmon_handler(void) ATTR(ex_handler);
void pendsv_handler(void) ATTR(ex_handler);
void systick_handler(void) ATTR(ex_handler);

void wakeup_handler(void) ATTR(irq_handler);
void i2c_handler(void) ATTR(irq_handler);
void tmr16b0_handler(void) ATTR(irq_handler);
void tmr16b1_handler(void) ATTR(irq_handler);
void tmr32b0_handler(void) ATTR(irq_handler);
void tmr32b1_handler(void) ATTR(irq_handler);
void ssp0_handler(void) ATTR(irq_handler);
void uart_handler(void) ATTR(irq_handler);
void usbirq_handler(void) ATTR(irq_handler);
void usbfiq_handler(void) ATTR(irq_handler);
void adc_handler(void) ATTR(irq_handler);
void wdt_handler(void) ATTR(irq_handler);
void bod_handler(void) ATTR(irq_handler);
void pio3_handler(void) ATTR(irq_handler);
void pio2_handler(void) ATTR(irq_handler);
void pio1_handler(void) ATTR(irq_handler);
void pio0_handler(void) ATTR(irq_handler);
void ssp1_handler(void) ATTR(irq_handler);

typedef void (* vector_entry)(void);

/* ベクタテーブル */
__attribute__ ((section(".vector")))
const vector_entry vectors[] = {
    /* メインスタックポインタ */
    (vector_entry)&_main_sp,

    /* システム例外ハンドラ */
    reset_handler,
    nmi_handler,
    hardfault_handler,
    memmanage_handler,
    busfault_handler,
    usagefault_handler,
    0,
    0,
    0,
    0,
    svcall_handler,
    debugmon_handler,
    0,
    pendsv_handler,
    systick_handler,

    /* IRQ (外部割込み) ハンドラ */
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,

    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,

    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,

    wakeup_handler,
    wakeup_handler,
    wakeup_handler,
    wakeup_handler,

    i2c_handler,
    tmr16b0_handler,
    tmr16b1_handler,
    tmr32b0_handler,
    tmr32b1_handler,
    ssp0_handler,
    uart_handler,
    usbirq_handler,
    usbfiq_handler,
    adc_handler,
    wdt_handler,
    bod_handler,
    pio3_handler,
    pio2_handler,
    pio1_handler,
    pio0_handler,
    ssp1_handler,
};

/*
 * なぜだかよくわからないが、出力先 ELF ファイルの .text セクションの
 * 先頭にスタートアップルーチンを置かないと、リセットがかかっても呼ばれない。
 * そうなるとプログラムの動作可否がオブジェクトファイルのリンク順に
 * 依存してしまって困るので、.reset セクションを設けてそこに
 * スタートアップルーチンを置き、リンク時に .text の先頭に強制的に
 * 配置されるようにリンカスクリプトを書いておく。
 */
void reset_handler(void) __attribute__ ((section(".reset")));

/**
 * @brief スタートアップルーチン
 * @return なし
 */
void reset_handler(void)
{
    unsigned long *src, *dst;

    /* RAM の DATA 領域を初期化 (初期値を ROM からコピーする) */
    src = &_data_org;
    for (dst = &_sdata; dst < &_edata;) {
        *dst++ = *src++;
    }

    /* BSS 領域の初期化 */
    for (dst = &_sbss; dst < &_ebss;) {
        *dst++ = 0;
    }

    sys_init();
    main();

    while (1);
}

/**
 * @brief システム例外のデフォルトハンドラ
 * @return なし
 */
void ex_handler(void)
{
    while (1);
}

/**
 * @brief IRQ (外部割込み) のデフォルトハンドラ
 * @return なし
 */
void irq_handler(void)
{
    while (1);
}
