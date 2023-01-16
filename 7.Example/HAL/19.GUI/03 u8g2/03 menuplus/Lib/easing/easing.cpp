
#include "easing.hpp"

////////////////////////////////////////////////////////////////////////////////////

#ifdef TESTMODE
/**
 *

 * pos = {}:
 *
 *      0.000000        10.000000       20.000000       0.000000        0.000000        0.000000        0.000000        0.000000        0.000000
 *      2.500000        12.500000       22.500000       2.500000        25.000000       250.000000      2.500000        25.000000       250.000000
 *      5.000000        15.000000       25.000000       5.000000        50.000000       500.000000      5.000000        50.000000       500.000000
 *      7.500000        17.500000       27.500000       7.500000        75.000000       750.000000      7.500000        75.000000       750.000000
 *      10.000000       20.000000       30.000000       10.000000       100.000000      1000.000000     10.000000       100.000000      1000.000000
 *
 * pos = {
 *      easing_pos(10, 5), // absolute
 *      easing_pos(10, 10),
 *      easing_pos(20, 10),
 *      easing_pos(10, 5), // relative
 *      easing_pos(10, 10),
 *      easing_pos(20, 10),
 *      easing_pos(10, 5), // moveto
 *      easing_pos(10, 10),
 *      easing_pos(20, 10),
 * }
 *
 *      5.000000        20.000000       30.000000       15.000000       20.000000       30.000000       15.000000       20.000000       30.000000
 *      7.500000        22.500000       32.500000       17.500000       45.000000       280.000000      15.000000       42.500000       275.000000
 *      10.000000       25.000000       35.000000       20.000000       70.000000       530.000000      15.000000       65.000000       520.000000
 *      12.500000       27.500000       37.500000       22.500000       95.000000       780.000000      15.000000       87.500000       765.000000
 *      15.000000       30.000000       40.000000       25.000000       120.000000      1030.000000     15.000000       110.000000      1010.000000
 *
 */

int main()
{
#if 0  // unused

    char  buff[32];
    FILE* f = fopen("data.csv", "w");
    while (!easing_isok(&e)) {
        easing_update(&e);
        float x = easing_curpos(&e);
        sprintf(buff, "%f\n", y);
        fputs(buff, f);
    }
    fclose(f);

#endif

#if 0

    easing_pos pos[10] = {
#if 0
        // absolute
        easing_pos(10, 5),
        easing_pos(10, 10),
        easing_pos(20, 10),
        // relative
        easing_pos(10, 5),
        easing_pos(10, 10),
        easing_pos(20, 10),
        // moveto(absolute)
        easing_pos(10, 5),
        easing_pos(10, 10),
        easing_pos(20, 10),
        // disable
        easing_pos(10, 10),
#endif
    };
    pos[4] = 2000;  // set current pos

    pos[0].move(00, 10);
    pos[1].move(10, 20);
    pos[2].move(20, 30);
    pos[3].move(10);
    pos[4].move(100);
    pos[5].move(1000);
    pos[6].moveto(10);
    pos[7].moveto(100);
    pos[8].moveto(1000);
    pos[9].moveto(10);

    printf("--------------------\r\n");
    pos[8].print();  // enable
    printf("--------------------\r\n");
    pos[9].print();  // disable
    printf("--------------------\r\n");

    // const size_t len = 1;
    const size_t len = ARRAY_SIZE(pos);

    // easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_FORWARD));
    // easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_REVERSE));
    // easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_FORWARD | easing_core::mode::DIR_BACKANDFORTH));
    easing_core easing(pos, len, 5, EASING_MODE_NTIMES(1, easing_core::mode::DIR_REVERSE | easing_core::mode::DIR_BACKANDFORTH));

    easing.start();
    while (!easing.isok()) {
        easing.update();
        for (size_t i = 0; i < len; ++i) {
            printf("%f\t", easing[i].curpos());
            // printf("%f\t", pos[i].curpos());
        }
        printf("\n");
    }

#endif

#if 0
    easing_fast easing(10, 10, 5);

    easing.move(20);
    easing.start();
    while (!easing.isok()) {
        easing.update();
        printf("%f\n", easing.curpos());
    }

#endif

    return 0;
}

#endif