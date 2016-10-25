
#pragma once

#include "Return.h"

namespace HLVM { namespace AST { namespace Statement {
    Return::Return(HLVM::AST::Node *node) {
        this->expr = node;
    }
}}}