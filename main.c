#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

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
        small_fish[i][0][2] = ((double)rand() / RAND_MAX - 0.5) * 2 * M_PI;
        small_fish[i][1][0] = 1.67 / frame_rate;
        small_fish[i][1][1] = 0;
        small_fish[i][1][2] = 0;
        small_fish[i][2][0] = 0;//(double)rand() / RAND_MAX / frame_rate;
        small_fish[i][2][1] = 0;
        small_fish[i][2][2] = 0;
    }

    for (int i = 0; i < max_step; i++) {
        // フロッキング処理(a, \alphaを決める処理)
        for (int j = 0; j < fish_amount; j++) {
            int fish_in_view = 0;
            double view_r = 5;
            double view_theta = 2;
            double centroid_x = 0;
            double centroid_y = 0;
            double relative_posi_vect_r = 0;
            double relative_posi_vect_theta = 0;
            for (int k = 0; k < fish_amount; k++) {
                if (k != j) {
                    // 視野範囲の中に入っている同種の魚を抽出
                    double relative_posi_vect_x = small_fish[k][0][0] - small_fish[j][0][0];
                    double relative_posi_vect_y = small_fish[k][0][1] - small_fish[j][0][1];
                    relative_posi_vect_r = sqrt(relative_posi_vect_x * relative_posi_vect_x + relative_posi_vect_y * relative_posi_vect_y);
                    relative_posi_vect_theta = atan2(relative_posi_vect_y, relative_posi_vect_x);
                    if (relative_posi_vect_r < view_r && relative_posi_vect_theta - small_fish[j][0][2] < view_theta) {
                        fish_in_view++;
                        centroid_x += small_fish[k][0][0] + cos(small_fish[k][0][2]) * small_fish[k][1][0];
                        centroid_y += small_fish[k][0][1] + sin(small_fish[k][0][2]) * small_fish[k][1][0];
                    }
                }
            }
            if (fish_in_view == 0) {
                break;
            }
            centroid_x = centroid_x / fish_in_view;
            centroid_y = centroid_y / fish_in_view;
            // 相対位置に変換
            centroid_x = centroid_x - small_fish[j][0][0];
            centroid_y = centroid_y - small_fish[j][0][1];
            double dist_centroid = sqrt(centroid_x * centroid_x + centroid_y * centroid_y);
            double centroid_theta = small_fish[j][0][2];
            if(centroid_x != 0 || centroid_y != 0 && dist_centroid > 0.1) {
                centroid_theta = atan2(centroid_y, centroid_x);
            }
            // \alphaを決める
            small_fish[j][2][1] = 50 * (centroid_theta - small_fish[j][0][2]);
        }

        // 移動の処理
        for (int j = 0; j < fish_amount; j++) {
            // 並進
            small_fish[j][0][0] += cos(small_fish[j][0][2]) * small_fish[j][1][0]; // x = x + v_x
            small_fish[j][0][1] += sin(small_fish[j][0][2]) * small_fish[j][1][0]; // y = y + v_y

            // \omega回転
            small_fish[j][0][2] += small_fish[j][2][0];
            while(small_fish[j][0][2] > 2 * M_PI){
                small_fish[j][0][2] -= 2 * M_PI;
            }
            while(small_fish[j][0][2] < -1.0 * 2 * M_PI){
                small_fish[j][0][2] += 2 * M_PI;
            }

            // v = v + a
            small_fish[j][1][0] += small_fish[j][1][1];

            // \omega = \omega + \alpha
            small_fish[j][2][0] += small_fish[j][2][1];
        }

        // ファイル出力
        char fname[50];
        sprintf(fname, "dat/%d.dat", i);
        FILE* fp = fopen(fname, "w");
        if (fp == NULL) {
            printf("failed to open file.\n");
            exit(1);
        }
        for (int j = 0; j < fish_amount; j++) {
            if (fabs(small_fish[j][0][0]) < field_x && fabs(small_fish[j][0][1]) < field_y) {
                // 描画範囲内のもののみ出力
                fprintf(fp, "%f, %f, %f, %f, x, y, v_x, v_y\n"
                        , small_fish[j][0][0], small_fish[j][0][1]
                        , cos(small_fish[j][0][2]) * small_fish[j][1][0] * frame_rate, sin(small_fish[j][0][2]) * small_fish[j][1][0] * frame_rate);
            }
        }
        fclose(fp);
    }

    return 0;
}

