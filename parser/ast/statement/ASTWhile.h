#pragma once

#include "../ASTStatementNode.h"
#include "../ASTExpressionNode.h"
#include "ASTBlock.h"

class ASTWhile : public ASTStatementNode {
public:
    ASTExpressionNode *astExpressionNode;
    ASTBlock *astBlock;

    ASTWhile(ASTExpressionNode *astExpressionNode, ASTBlock *astBlock);
    ~ASTWhile();

    void accept(Visitor *visitor) override;
};
