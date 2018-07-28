#pragma once

#include "../ASTExpressionNode.h"

class ASTLiteral : public ASTExpressionNode {
public:
    void accept(Visitor *visitor) override = 0;
};