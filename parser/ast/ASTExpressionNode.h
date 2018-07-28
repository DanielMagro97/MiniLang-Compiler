#pragma once

#include "ASTNode.h"

class ASTExpressionNode : public ASTNode {
public:
    void accept(Visitor *visitor) override = 0;
};