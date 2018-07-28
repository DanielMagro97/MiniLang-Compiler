#pragma once

#include "../ASTStatementNode.h"
#include "../ASTExpressionNode.h"

class ASTReturn : public ASTStatementNode {
public:
    ASTExpressionNode *astExpressionNode;

    explicit ASTReturn(ASTExpressionNode *astExpressionNode);
    ~ASTReturn();

    void accept(Visitor *visitor) override;
};
