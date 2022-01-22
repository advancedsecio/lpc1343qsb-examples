#!/bin/sh

#
# LPC1343 ファームウェア書き込みを行うスクリプト for FreeBSD
# root 権限で実行すること
# フロー:
#   1. ターゲットデバイスを USB マスストレージデバイスとして PC に接続する
#   2. geom コマンドで書き込み先フラッシュメモリのデバイスノードを探す
#   3. マウントする
#   4. 書き込み先に firmware.bin があれば削除する
#   5. cp コマンドでファームウェアのイメージファイルをコピーする
#   6. アンマウントする
# 詳細は LPC13xx ユーザマニュアル (UM10375) 21.4 節を参照
#

if [ $# -lt 1 ]; then
    echo "usage: lpcwrite firmwarefile" 1>&2
    exit 1
fi

DISKLIST=`geom disk list`

RE_GEOM="Geom name: "
RE_DESC="descr: "
RE_LPC="NXP LPC.* IFLASH"

NAME=""
DESC=""

while read line; do
    line=`echo $line`    # 前後の空白を削除する

    if [ `expr "$line" : "$RE_GEOM"` -ge ${#RE_GEOM} ]; then
        NAME=`echo $line | sed -e "s/$RE_GEOM//"`
        continue
    fi

    if [ `expr "$line" : "$RE_DESC"` -ge ${#RE_DESC} ]; then
        DESC=`echo $line | sed -e "s/$RE_DESC//"`
        if [ `expr "$DESC" : "$RE_LPC"` -ge ${#RE_LPC} ]; then
            break
        else
            DESC=""
        fi
    fi
done <<EOF
$DISKLIST
EOF

if [ "$DESC" = "" ]; then
    echo "device not found."
    exit 1
fi

echo -n "copy `basename $1` to /dev/$NAME ($DESC)... "

mount_msdosfs /dev/$NAME /media
if [ -e /media/firmware.bin ]; then
    rm /media/firmware.bin
fi
cp $1 /media
umount /media

echo "done."
