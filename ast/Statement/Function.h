
#pragma once

#include <string>
#include <vector>

#include "./../Node.h"

namespace HLVM { namespace AST { namespace Statement {
    class Function {
    protected:
        std::string name;
        std::vector<HLVM::AST::Node> statements;
    public:
        Function(std::string name, std::vector<HLVM::AST::Node> statements);
    };
}}}
