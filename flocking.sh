#!/bin/sh

video_length=5
frame_rate=10
frame_number=`expr $video_length \* $frame_rate`
field_x=50
field_y=50
fish_amount=50

rm out.mp4
rm -rf dat
rm -rf eps
rm -rf png
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
    echo "プログラムを実行し、連番ファイルを出力します."
    ./main.out $video_length $frame_rate $field_x $field_y $fish_amount
    echo "プログラムの実行が終了しました."
else
    echo "コンパイルに失敗したようです."
    exit 0
fi

rm main.out

echo "gnuplotで画像ファイルに変換します."
mkdir eps
touch plotimg.gp
i=0
while [ $i -lt $frame_number ]
do
    if [ -e dat/${i}.dat ]; then
        echo "set encoding iso_8859_1" >> plotimg.gp
        echo "set term postscript color eps enhanced" >> plotimg.gp
        echo "set output 'eps/$i.eps'" >> plotimg.gp
        echo "set size ratio 1" >> plotimg.gp
        echo "set xrange[-$field_x:$field_x]" >> plotimg.gp
        echo "set yrange[-$field_y:$field_y]" >> plotimg.gp
        echo "plot 'dat/${i}.dat' u 1:2:3:4 w vector ti ''" >> plotimg.gp
    else
        echo "${i}は存在しないです."
    fi
    gnuplot plotimg.gp
    rm plotimg.gp
    i=`expr $i + 1`
done

mkdir png
echo "epsからpngに変換します."
i=0
while [ $i -lt $frame_number ]
do
    if [ -e eps/${i}.eps ]; then
        convert -density 500x500 eps/${i}.eps png/${i}.png
    else
        echo "eps/${i}.eps は存在しません."
    fi
    i=`expr $i + 1`
done

echo "pngから動画を作ります."
if [ -e png/0.png ]; then
    ffmpeg -r $frame_rate -i png/%d.png -vcodec libx264 -pix_fmt yuv420p -r 60 out.mp4
fi

if [ -e out.mp4 ]; then
    echo "出力が完了しました."
    totem out.mp4 &
else
    echo "出力に失敗したようです."
    exit 0
fi
