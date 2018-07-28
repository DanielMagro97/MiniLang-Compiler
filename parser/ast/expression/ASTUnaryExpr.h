#pragma once

#include "../ASTExpressionNode.h"
#include "ASTUnaryOperator.h"

class ASTUnaryExpr : public ASTExpressionNode {
public:
    ASTUnaryOperator *unaryOperator;
    ASTExpressionNode *expression;

    ASTUnaryExpr(ASTUnaryOperator *unaryOperator, ASTExpressionNode *expression);
    ~ASTUnaryExpr();

    void accept(Visitor *visitor) override;
};
