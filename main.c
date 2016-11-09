#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("argument is not correct\n");
        return 0;
    }
    int max_step = atoi(argv[1]);
    int field_x = 10000;
    int field_y = 10000;
    int fish_amount = 50;
    double small_fish[fish_amount][3][2]; // fish0[[x, y], [v_x, v_y], [\omega, \alpha]]

    for (int i = 0; i < max_step; i++) {
        for (int j = 0; j < fish_amount; j++) {
            small_fish[j][0][0] += small_fish[j][1][0]; // x = x + v_x
            small_fish[j][0][1] += small_fish[j][1][1]; // y = y + v_y

            // 並進速度に\omega回る回転行列をかける
            small_fish[j][1][0] = cos(small_fish[j][2][0]) * small_fish[j][1][0] + sin(small_fish[j][2][0]) * small_fish[j][1][1];
            small_fish[j][1][1] = -1 * sin(small_fish[j][2][0]) * small_fish[j][1][0] + cos(small_fish[j][2][0]) * small_fish[j][1][1];

            small_fish[j][2][0] += small_fish[j][2][1]; //\omega = \omega + \alpha
        }

        char fname[50];
        sprintf(fname, "dat/%d.dat", i);
        FILE* fp = fopen(fname, "w");
        if (fp == NULL) {
            printf("failed to open file.\n");
            exit(1);
        }
        for (int j = 0; j < fish_amount; j++) {
            if (fabs(small_fish[j][0][0]) < field_x && fabs(small_fish[j][0][1] < field_y)) {
                fprintf(fp, "%f, %f, %f, %f, x, y, v_x, v_y\n", small_fish[j][0][0], small_fish[j][0][1], small_fish[j][1][0], small_fish[j][1][1]);
            }
        }
        fclose(fp);
    }

    return 0;
}

