#include "stm32f10x.h"  // Device header
#include "OLED.h"
#include "math.h"
#include <stdlib.h>

int8_t Cube[8][3] = {
    {17, 17, 17},
    {-17, 17, 17},
    {-17, -17, 17},
    {17, -17, 17},  // A B C D
    {17, 17, -17},
    {-17, 17, -17},
    {-17, -17, -17},
    {17, -17, -17}};  // E F G H
double Cube_display[8][3] = {
    {17, 17, 17},
    {-17, 17, 17},
    {-17, -17, 17},
    {17, -17, 17},  // A B C D
    {17, 17, -17},
    {-17, 17, -17},
    {-17, -17, -17},
    {17, -17, -17}};
;

uint8_t Cor_TransX(double x)
{
    return x + 64;
}

uint8_t Cor_TransY(double y)
{
    return y + 32;
}

void Cube_rotate(double thetaX, double thetaY, double thetaZ)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        Cube_display[i][0] = Cube[i][0];
        Cube_display[i][1] = Cube[i][1] * cos(thetaX) - Cube[i][2] * sin(thetaX);
        Cube_display[i][2] = Cube[i][1] * sin(thetaX) + Cube[i][2] * cos(thetaX);

        Cube_display[i][0] = Cube_display[i][0] * cos(thetaY) - Cube_display[i][2] * sin(thetaY);
        Cube_display[i][1] = Cube_display[i][1];
        Cube_display[i][2] = Cube_display[i][0] * sin(thetaY) + Cube_display[i][2] * cos(thetaY);

        Cube_display[i][0] = Cube_display[i][0] * cos(thetaZ) - Cube_display[i][1] * sin(thetaZ);
        Cube_display[i][1] = Cube_display[i][0] * sin(thetaZ) + Cube_display[i][1] * cos(thetaZ);
        Cube_display[i][2] = Cube_display[i][2];
    }
}

void DrawCube()
{
    DrawLine(Cor_TransX(Cube_display[0][0]), Cor_TransY(Cube_display[0][1]),  // AB
             Cor_TransX(Cube_display[1][0]), Cor_TransY(Cube_display[1][1]));

    DrawLine(Cor_TransX(Cube_display[1][0]), Cor_TransY(Cube_display[1][1]),  // BC
             Cor_TransX(Cube_display[2][0]), Cor_TransY(Cube_display[2][1]));

    DrawLine(Cor_TransX(Cube_display[2][0]), Cor_TransY(Cube_display[2][1]),  // CD
             Cor_TransX(Cube_display[3][0]), Cor_TransY(Cube_display[3][1]));

    DrawLine(Cor_TransX(Cube_display[3][0]), Cor_TransY(Cube_display[3][1]),  // DA
             Cor_TransX(Cube_display[0][0]), Cor_TransY(Cube_display[0][1]));

    DrawLine(Cor_TransX(Cube_display[4][0]), Cor_TransY(Cube_display[4][1]),  // EF
             Cor_TransX(Cube_display[5][0]), Cor_TransY(Cube_display[5][1]));

    DrawLine(Cor_TransX(Cube_display[5][0]), Cor_TransY(Cube_display[5][1]),  // FG
             Cor_TransX(Cube_display[6][0]), Cor_TransY(Cube_display[6][1]));

    DrawLine(Cor_TransX(Cube_display[6][0]), Cor_TransY(Cube_display[6][1]),  // GH
             Cor_TransX(Cube_display[7][0]), Cor_TransY(Cube_display[7][1]));

    DrawLine(Cor_TransX(Cube_display[7][0]), Cor_TransY(Cube_display[7][1]),  // HE
             Cor_TransX(Cube_display[4][0]), Cor_TransY(Cube_display[4][1]));

    DrawLine(Cor_TransX(Cube_display[0][0]), Cor_TransY(Cube_display[0][1]),  // AE
             Cor_TransX(Cube_display[4][0]), Cor_TransY(Cube_display[4][1]));

    DrawLine(Cor_TransX(Cube_display[1][0]), Cor_TransY(Cube_display[1][1]),  // BF
             Cor_TransX(Cube_display[5][0]), Cor_TransY(Cube_display[5][1]));

    DrawLine(Cor_TransX(Cube_display[2][0]), Cor_TransY(Cube_display[2][1]),  // CG
             Cor_TransX(Cube_display[6][0]), Cor_TransY(Cube_display[6][1]));

    DrawLine(Cor_TransX(Cube_display[3][0]), Cor_TransY(Cube_display[3][1]),  // DH
             Cor_TransX(Cube_display[7][0]), Cor_TransY(Cube_display[7][1]));
}

int main(void)
{
    double   PI = 3.141592;
    uint16_t i;
    OLED_Init();
    while (1) {
        for (i = 0; i < 360; i++) {
            Cube_rotate(PI / 180 * i, PI / 90 * i, 0);
            DrawCube();
            OLED_display();
        }
    }
}
