#pragma once

#include "../ASTExpressionNode.h"

class ASTSubExpr : public ASTExpressionNode {
public:
    ASTExpressionNode *expression;

    explicit ASTSubExpr(ASTExpressionNode *expression);
    ~ASTSubExpr();

    void accept(Visitor *visitor) override;
};
