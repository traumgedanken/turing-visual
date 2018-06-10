#pragma once

#include "common_global.h"

enum DIRECTION { LEFT, NONE, RIGHT };

enum ERROR_TYPE { ERROR, WARNING };

#define ANY_SYMBOL '*'

// to describe response status
enum {
    NETWORK_SPECIAL_STATUS = -665,
    NETWORK_ERROR_CODE = -666,
    NETWORK_FALSE = -667,
    NETWORK_TRUE = 0
};

#define NETWORK_SEPARATOR '~'

// for creating gif
enum {
    FRAME_WIDTH = 800,
    FRAME_HIGHT = 75
};
