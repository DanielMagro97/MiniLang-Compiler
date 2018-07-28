#pragma once

#include "../ASTExpressionNode.h"
#include "ASTOperator.h"

class ASTBinaryExpr : public ASTExpressionNode {
public:
    ASTExpressionNode *simpleExpression;
    ASTOperator *anOperator;
    ASTExpressionNode *simpleExpression2;

    ASTBinaryExpr(ASTExpressionNode *simpleExpression, ASTOperator *anOperator, ASTExpressionNode *simpleExpression2);
    ~ASTBinaryExpr();

    void accept(Visitor *visitor) override;
};
