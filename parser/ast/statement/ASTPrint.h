#pragma once

#include "../ASTStatementNode.h"
#include "../ASTExpressionNode.h"

class ASTPrint : public ASTStatementNode {
public:
    ASTExpressionNode *astExpressionNode;

    explicit ASTPrint(ASTExpressionNode *astExpressionNode);
    ~ASTPrint();

    void accept(Visitor *visitor) override;
};
