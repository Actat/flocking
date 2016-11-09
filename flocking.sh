#!/bin/sh

video_length=5
frame_rate=10
frame_number=`expr $video_length \* $frame_rate`

rm main.out
rm -rf dat
mkdir dat

if [ -f main.c ]; then
    echo "コンパイルします."
    gcc -o main.out main.c -lm
else
    echo "main.c が存在しないようです."
    exit 0
fi

if [ -f main.out ]; then
    echo "コンパイルに成功したようです."
#    echo "プログラムを実行し、連番ファイルを出力します."
else
    echo "コンパイルに失敗したようです."
    exit 0
fi

