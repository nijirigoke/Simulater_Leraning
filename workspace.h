//
// Created by Juntaro.otaka on 2021/03/18.
//

#ifndef SIMULATER_LERANING_WORKSPACE_H
#define SIMULATER_LERANING_WORKSPACE_H

#endif  // SIMULATER_LERANING_WORKSPACE_H

#include "simbase.h"

#define PINN  4
#define WALLS 4

typedef struct {
    int p1;
    int p2;
} WALL;

POSITION pin[PINN] = {{0,   0},
                      {0,   250},
                      {250, 250},
                      {-0,  -250}};

WALL wall[WALLS] = {{0, 1},
                    {1, 2},
                    {2, 3},
                    {3, 0}};
