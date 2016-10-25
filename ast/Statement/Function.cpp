
#pragma once

#include "function.h"

namespace HLVM { namespace AST { namespace Statement {
    Function::Function(std::string name, std::vector <HLVM::AST::Node> statements) {
        this->name = name;
        this->statements = statements;
    }
}}}
