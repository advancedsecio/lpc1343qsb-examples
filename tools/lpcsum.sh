#!/bin/sh

#
# LPC13xx F/W イメージファイルのベクタテーブルにチェックサムを書き込む
# フロー:
#   1. テーブルエントリ 0 〜 6 を加算して 2 の補数をとる
#   2. テーブルエントリ 7 (予約) の位置に 1. で計算したチェックサムを書き込む
# 詳細は LPC13xx ユーザマニュアル (UM10375) 21.7 節を参照
#

NVECTS=7                             # チェックサム対象となるベクタの個数
WORDSIZE=4                           # 1 ワードのバイト数
TRGSIZE=$(($NVECTS * $WORDSIZE))     # チェックサム対象のバイト数
SUMOFFS=$((0 + $TRGSIZE))            # チェックサム付加位置
ENDIAN=little                        # エンディアン; little または big

#
# Usage を表示して終了する
#
usage() {
    echo "usage: lpcsum [-b] firmwarefile" 1>&2
    exit 1
}

#
# get_file_size(f)
# f で指定されたファイルのサイズを返す
#
get_file_size() {
    local ret=`wc -c $1`                # wc でファイルの名前とサイズを取得する
    ret=`echo $ret | cut -f1 -d ' '`    # echo で余分な空白を削除し、cut でファイルサイズだけ取り出す
    echo $(($ret))
}

#
# calc_sum(f, offs, sz, n [, e])
# ファイルの指定された場所についてワード単位でチェックサムを計算する
# ファイルサイズはチェック済みとする
# [in] f: 入力ファイル名
# [in] offs: 計算対象の先頭位置
# [in] sz: 1 ワードのサイズ [バイト]
# [in] n: 計算対象のワード数
# [in] e: エンディアン; little または big, 省略された場合は little
# return: チェックサム
#
# 計算例)
# f の先頭 32 バイト:
#     00 20 00 10 01 04 00 00 55 07 00 00 55 07 00 00
#     55 07 00 00 55 07 00 00 55 07 00 00 00 00 00 00
# offs=0, sz=4, n=7, e=little
# この場合、4 バイトごとに加算すると 0x10002000 + 0x401 + (0x755 * 5) = 0x100048AA,
# 2 の補数に直して NOT(0x100048AA) + 1 = 0xEFFFB756 が求めるチェックサムとなる
#
calc_sum() {
    local f=$1
    local offs=$2
    local sz=$3
    local n=$4
    local e=little
    if [ $# -ge 5 ]; then
        e=$5
    fi

    local mask=`printf "0x%.$(($sz * 2))s" FFFFFFFFFFFFFFFF`

    # 計算対象データをファイルから読み込んで 1 バイトずつ空白で区切る
    # ex) bytes = "00 20 00 10 01 04 00 00 55 07 00 00 55 07 00 00 55 07 00 00 55 07 00 00 55 07 00 00"
    local ndump=$(($sz * $n))
    local bytes=`od -A n -t x1 -v -j $offs -N $ndump $f | tr -Cd '0-9a-f ' | tr -s ' '`

    local sum=0
    local word=""

    for i in `seq 1 $ndump`
    do
        if [ $(($((i - 1)) % $sz)) -eq 0 ]; then
            word=""
        fi

        local b=`echo $bytes | cut -f$i -d ' '`

        if [ $e == "little" ]; then
            word=`echo $b$word`
        else
            word=`echo $word$b`
        fi

        if [ $(($i % $sz)) -eq 0 ]; then
            sum=$(($sum + `printf '%d' 0x$word`))
        fi
    done

    sum=$(($((~$sum)) + 1))
    sum=$(($sum & $mask))

    printf "0x%X" $sum
}

#
# to_octal_seq(x, sz [, e])
# 与えられた数 x をエスケープシーケンス付き 8 進数表現に直す
# ex) 0xEFFFB756 => \126\267\377\357 (リトルエンディアンの場合)
# [in] x: 変換したい数値
# [in] sz: 1 ワードのサイズ [バイト]
# [in] e: エンディアン; little または big, 省略された場合は little
# return: x のエスケープシーケンス付き 8 進数表現
#
to_octal_seq() {
    local x=$1
    local sz=$2
    local e=little
    if [ $# -ge 3 ]; then
        e=$3
    fi

    local ret=""

    for i in `seq 0 $(($sz - 1))`
    do
        if [ $e == "little" ]; then
            ret=`printf '%s' $ret; printf '\\'; printf '%o' $(($x & 0xFF))`
        else
            ret=`printf '\\'; printf '%o' $(($x & 0xFF)); printf '%s' $ret`
        fi
        x=$(($x >> 8))
    done

    printf '%s' $ret
}

#
# write_sum(f, oseq, offs)
# ファイルにチェックサムを書き込む
# ファイルサイズはチェック済みとする
# [in] f: 出力ファイル名
# [in] oseq: チェックサムのエスケープシーケンス付き 8 進数表現
# [in] offs: チェックサム付加位置
# return: なし
#
write_sum() {
    local f=$1
    local oseq=$2
    local offs=$3

    printf $oseq | dd of=$f bs=1 seek=$offs conv=notrunc > /dev/null 2>&1
}

#
# メイン関数
#
main() {
    while getopts b OPT
    do
        case $OPT in
            "b" ) ENDIAN=big;;
              * ) usage;;
        esac
    done

    shift `expr $OPTIND - 1`
    if [ $# -lt 1 ]; then
        usage
    fi

    local fwfile=$1

    if [ `get_file_size $fwfile` -lt $(($TRGSIZE + 4)) ]; then
        echo "error: file is too small."
        exit 1
    fi

    local sum=`calc_sum $fwfile 0 $WORDSIZE $NVECTS $ENDIAN`
    local oseq=`to_octal_seq $sum $WORDSIZE $ENDIAN`
    write_sum $fwfile $oseq $SUMOFFS
    echo "checksum:" $sum

    exit 0
}

main $*
