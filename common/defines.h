#pragma once

#include "common_global.h"

enum DIRECTION { LEFT, NONE, RIGHT };

enum ERROR_TYPE { ERROR, WARNING };

#define ANY_SYMBOL '*'

// to describe response status
#define NETWORK_SPECIAL_STATUS -665
#define NETWORK_ERROR_CODE -666
#define NETWORK_FALSE -667
#define NETWORK_TRUE 0
