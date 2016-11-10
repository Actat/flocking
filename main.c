#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/*
 * コマンドライン引数
 * argv[0] プログラムへのパス
 * argv[1] max_step 出力するファイルの個数
 * argv[2] field_x
 * argv[3] field_y
 * argv[4] fish_amount
 */

int main(int argc, char* argv[])
{
    if (argc != 5) {
        printf("argument is not correct\n");
        for (int i = 0;i < argc; i++) {
            printf("%d: %s\n", i, argv[i]);
        }
        exit(1);
    }
    int max_step = atoi(argv[1]);
    int field_x = atoi(argv[2]);
    int field_y = atoi(argv[3]);
    int fish_amount = atoi(argv[4]);
    double small_fish[fish_amount][3][2]; // fish0[[x, y], [v_x, v_y], [\omega, \alpha]]

    // small_fishの初期条件をランダムに与える
    srand((unsigned)time(NULL));
    for (int i = 0; i < fish_amount; i++) {
        small_fish[i][0][0] = (double)rand() / RAND_MAX * field_x;
        small_fish[i][0][1] = (double)rand() / RAND_MAX * field_y;
        small_fish[i][1][0] = (double)rand() / RAND_MAX * 100;
        small_fish[i][1][1] = (double)rand() / RAND_MAX * 100;
        small_fish[i][2][0] = (double)rand() / RAND_MAX;
        small_fish[i][0][0] = 0;
    }

    for (int i = 0; i < max_step; i++) {
        // 移動の処理
        for (int j = 0; j < fish_amount; j++) {
            // 並進
            small_fish[j][0][0] += small_fish[j][1][0]; // x = x + v_x
            small_fish[j][0][1] += small_fish[j][1][1]; // y = y + v_y

            // \omega回る回転行列をかける
            small_fish[j][1][0] = cos(small_fish[j][2][0]) * small_fish[j][1][0] - sin(small_fish[j][2][0]) * small_fish[j][1][1];
            small_fish[j][1][1] = sin(small_fish[j][2][0]) * small_fish[j][1][0] + cos(small_fish[j][2][0]) * small_fish[j][1][1];

            //\omega = \omega + \alpha
            small_fish[j][2][0] += small_fish[j][2][1];
        }

        // フロッキング処理(\alphaを決める処理)


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
                fprintf(fp, "%f, %f, %f, %f, x, y, v_x, v_y\n", small_fish[j][0][0], small_fish[j][0][1], small_fish[j][1][0], small_fish[j][1][1]);
            }
        }
        fclose(fp);
    }

    return 0;
}

