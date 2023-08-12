#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <iostream>
#include <vector>

#include "block/block.h"
#include "../frontend/icg/env.h"


std::vector<Block *> generate_basic_blocks(Environment *env);

#endif // !_COMPILER_H_