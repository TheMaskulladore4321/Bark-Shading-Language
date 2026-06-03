#pragma once
#include <string>
#include "ast.h"

class RaylibEmitter
{
public:
    std::string Emit(
        const Program& program);
};