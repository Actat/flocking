#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
 * コマンドライン引数
 * argv[0] プログラムへのパス
 * argv[1] video_length
 * argv[2] frame_rate
 * argv[3] field_x
 * argv[4] field_y
 * argv[5] fish_amount
 */

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("argument is not correct\n");
        for (int i = 0;i < argc; i++) {
            printf("%d: %s\n", i, argv[i]);
        }
        exit(1);
    }
    int video_length = atoi(argv[1]);
    int frame_rate = atoi(argv[2]);
    int max_step = video_length * frame_rate;
    int field_x = atoi(argv[3]);
    int field_y = atoi(argv[4]);
    int fish_amount = atoi(argv[5]);
    double small_fish[fish_amount][3][3]; // fish0[[x, y, \theta], [v, a, 0], [\omega, \alpha, 0]]

    // small_fishの初期条件をランダムに与える
    srand((unsigned)time(NULL));
    for (int i = 0; i < fish_amount; i++) {
        small_fish[i][0][0] = ((double)rand() / RAND_MAX - 0.5) * 2 * field_x;
        small_fish[i][0][1] = ((double)rand() / RAND_MAX - 0.5) * 2 * field_y;
        small_fish[i][0][2] = (double)rand() / RAND_MAX;
        small_fish[i][1][0] = (double)rand() / RAND_MAX * 100;
        small_fish[i][1][1] = 0;
        small_fish[i][1][2] = 0;
        small_fish[i][2][0] = (double)rand() / RAND_MAX;
        small_fish[i][2][1] = 0;
        small_fish[i][2][2] = 0;
    }

    for (int i = 0; i < max_step; i++) {
        // 移動の処理
        for (int j = 0; j < fish_amount; j++) {
            // 並進
            small_fish[j][0][0] += cos(small_fish[j][0][2]) * small_fish[j][1][0]; // x = x + v_x
            small_fish[j][0][1] += sin(small_fish[j][0][2]) * small_fish[j][1][0]; // y = y + v_y

            // \omega回転
            small_fish[j][0][2] += small_fish[j][2][0];

            // v = v + a
            small_fish[j][1][0] += small_fish[j][1][1];

            // \omega = \omega + \alpha
            small_fish[j][2][0] += small_fish[j][2][1];
        }

        // フロッキング処理(a, \alphaを決める処理)


        // ファイル出力
        char fname[50];
        sprintf(fname, "dat/%d.dat", i);
        FILE* fp = fopen(fname, "w");
        if (fp == NULL) {
            printf("failed to open file.\n");
            exit(1);
        }
        for (int j = 0; j < fish_amount; j++) {
            if (fabs(small_fish[j][0][0]) < field_x && fabs(small_fish[j][0][1] < field_y)) {
                // 描画範囲内のもののみ出力
                fprintf(fp, "%f, %f, %f, %f, x, y, v_x, v_y\n"
                        , small_fish[j][0][0], small_fish[j][0][1], cos(small_fish[j][0][2]) * small_fish[j][1][0], sin(small_fish[j][0][2]) * small_fish[j][1][0]);
            }
        }
        fclose(fp);
    }

    return 0;
}

