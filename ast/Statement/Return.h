
#pragma once

namespace HLVM { namespace AST { namespace Statement {
    class Return : public HLVM::AST::Node {
    protected:
        HLVM::AST::Node *expr = nullptr;
    public:
        Return(HLVM::AST::Node *expr);
        Return() {};
    };
}}}
