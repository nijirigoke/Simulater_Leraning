//
// Created by Juntaro.otaka on 2021/03/18.
//

#ifndef SIMULATER_LERANING_WORKSPACE_H
#define SIMULATER_LERANING_WORKSPACE_H

#endif  // SIMULATER_LERANING_WORKSPACE_H

#include "simbase_test.h"

#define PINN  8
#define WALLS 8


typedef struct {
    int p1;
    int p2;

} WALL;


POSITION pin[PINN] = {{point,  point},
                      {-point, point},
                      {-point, -point},
                      {point,  -point}};

WALL wall[WALLS] = {{0, 1},
                    {1, 2},
                    {2, 3},
                    {3, 0}};

//POSITION DOTS
