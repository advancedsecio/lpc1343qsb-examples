/* -*- coding: utf-8 -*- */

/**
 * @file cortexm3_reg.h
 * @brief Cortex-M3 レジスタアクセス関連のヘッダファイル
 */

#ifndef __CORTEXM3_REG_H__
#define __CORTEXM3_REG_H__

#define __BB_PMEM_START 0x40000000        /* ペリフェラルメモリのビットバンド先頭アドレス */
#define __BB_PMEM_END   0x400FFFFC        /* ペリフェラルメモリのビットバンド終了アドレス */

static inline void __reg_set_bit(uint32_t addr, uint8_t nthbit);
static inline void __reg_clr_bit(uint32_t addr, uint32_t nthbit);
static inline void __reg_write_bit(uint32_t addr, uint32_t nthbit, uint8_t v);
static inline uint8_t __reg_read_bit(uint32_t addr, uint32_t nthbit);
static inline void __bb_set_bit(uint32_t addr, uint8_t nthbit);
static inline void __bb_clr_bit(uint32_t addr, uint32_t nthbit);
static inline void __bb_write_bit(uint32_t addr, uint32_t nthbit, uint8_t v);
static inline uint8_t __bb_read_bit(uint32_t addr, uint32_t nthbit);
static inline uint32_t __bb_alias(uint32_t addr, uint8_t nthbit);
static inline uint8_t __is_pmem_bb(uint32_t addr);

/**
 * @def __deref(addr)
 * addr で指定された番地への逆参照を得るためのマクロ
 */
#define __deref(addr) (*(volatile uint32_t *)(addr))

/**
 * @brief レジスタへの書き込み
 * @param[in] addr レジスタアドレス
 * @param[in] val 書き込むデータ
 * @return なし
 */
static inline void reg_write(uint32_t addr, uint32_t val)
{
    __deref(addr) = val;
}

/**
 * @brief レジスタからの読み出し
 * @param[in] addr レジスタアドレス
 * @return 読み出したデータ
 */
static inline uint32_t reg_read(uint32_t addr)
{
    return __deref(addr);
}

/**
 * @brief レジスタの第 n ビットを立てる
 * @param[in] addr レジスタアドレス
 * @param[in] nthbit ビット位置
 * @return なし
 */
static inline void reg_set_bit(uint32_t addr, uint8_t nthbit)
{
    __is_pmem_bb(addr) ? __bb_set_bit(addr, nthbit) : __reg_set_bit(addr, nthbit);
}

/**
 * @brief レジスタの第 n ビットをクリアする
 * @param[in] addr レジスタアドレス
 * @param[in] nthbit ビット位置
 * @return なし
 */
static inline void reg_clr_bit(uint32_t addr, uint32_t nthbit)
{
    __is_pmem_bb(addr) ? __bb_clr_bit(addr, nthbit) : __reg_clr_bit(addr, nthbit);
}

/**
 * @brief レジスタの第 n ビットを書き換える
 * @param[in] addr レジスタアドレス
 * @param[in] nthbit ビット位置
 * @param[in] 書き込む値
 * @return なし
 */
static inline void reg_write_bit(uint32_t addr, uint32_t nthbit, uint8_t v)
{
    __is_pmem_bb(addr) ? __bb_write_bit(addr, nthbit, v) : __reg_write_bit(addr, nthbit, v);
}

/**
 * @brief レジスタの第 n ビットを読み出す
 * @param[in] addr レジスタアドレス
 * @param[in] nthbit ビット位置
 * @return 読み出した値
 */
static inline uint8_t reg_read_bit(uint32_t addr, uint32_t nthbit)
{
    return __is_pmem_bb(addr) ? __bb_read_bit(addr, nthbit) : __reg_read_bit(addr, nthbit);
}

/**
 * @brief 非ビットバンド領域にあるレジスタの第 n ビットを立てる
 * @param[in] addr レジスタアドレス (非ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @return なし
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline void __reg_set_bit(uint32_t addr, uint8_t nthbit)
{
    __deref(addr) |= (1 << nthbit);
}

/**
 * @brief 非ビットバンド領域にあるレジスタの第 n ビットをクリアする
 * @param[in] addr レジスタアドレス (非ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @return なし
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline void __reg_clr_bit(uint32_t addr, uint32_t nthbit)
{
    __deref(addr) &= ~(1 << nthbit);
}

/**
 * @brief 非ビットバンド領域にあるレジスタの第 n ビットを書き換える
 * @param[in] addr レジスタアドレス (非ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @param[in] 書き込む値
 * @return なし
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline void __reg_write_bit(uint32_t addr, uint32_t nthbit, uint8_t v)
{
    __reg_clr_bit(addr, nthbit);
    if (v & 1) {
        __reg_set_bit(addr, nthbit);
    }
}

/**
 * @brief 非ビットバンド領域にあるレジスタの第 n ビットを読み出す
 * @param[in] addr レジスタアドレス (非ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @return 読み出した値
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline uint8_t __reg_read_bit(uint32_t addr, uint32_t nthbit)
{
    return (__deref(addr) >> nthbit) & 1;
}

/**
 * @brief ビットバンド領域にある値の第 n ビットを立てる
 * @param[in] addr アドレス (ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @return なし
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline void __bb_set_bit(uint32_t addr, uint8_t nthbit)
{
    __deref(__bb_alias(addr, nthbit)) = 1;
}

/**
 * @brief ビットバンド領域にある値の第 n ビットをクリアする
 * @param[in] addr アドレス (ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @return なし
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline void __bb_clr_bit(uint32_t addr, uint32_t nthbit)
{
    __deref(__bb_alias(addr, nthbit)) = 0;
}

/**
 * @brief ビットバンド領域にある値の第 n ビットを書き換える
 * @param[in] addr アドレス (ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @param[in] 書き込む値
 * @return なし
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline void __bb_write_bit(uint32_t addr, uint32_t nthbit, uint8_t v)
{
    __bb_clr_bit(addr, nthbit);
    if (v & 1) {
        __bb_set_bit(addr, nthbit);
    }
}

/**
 * @brief ビットバンド領域にある値の第 n ビットを読み出す
 * @param[in] addr アドレス (ビットバンド領域)
 * @param[in] nthbit ビット位置
 * @return 読み出した値
 * @note アドレスチェックはしないので呼び出し側で責任を持つこと
 */
static inline uint8_t __bb_read_bit(uint32_t addr, uint32_t nthbit)
{
    return __deref(__bb_alias(addr, nthbit)) & 1;
}

/**
 * @brief addr で指定されたアドレスの第 n ビットに対するビットバンドエイリアスを算出する
 * @param[in] addr レジスタアドレス
 * @param[in] nthbit ビット位置
 * @return ビットバンドエイリアス
 * @note ビットバンドの対象領域は 0x20000000..0x200FFFFC および 0x40000000..0x400FFFFC だが\n
 *       特にチェックしないので、呼び出し側が責任を持つこと
 */
static inline uint32_t __bb_alias(uint32_t addr, uint8_t nthbit)
{
    return (addr & 0xF0000000) + 0x02000000 + ((addr & 0x000FFFFF) << 5) + (nthbit << 2);
}

/**
 * @brief addr で指定されたアドレスがペリフェラルメモリのビットバンド領域かを判定する
 * @param[in] addr レジスタアドレス
 * @return !0: 是, 0: 否
 */
static inline uint8_t __is_pmem_bb(uint32_t addr)
{
    return (__BB_PMEM_START <= addr) && (addr <= __BB_PMEM_END);
}

#endif
