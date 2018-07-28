#pragma once

#include "../ASTStatementNode.h"
#include "../ASTExpressionNode.h"
#include "ASTBlock.h"

class ASTIf : public ASTStatementNode {
public:
    ASTExpressionNode *expression;
    ASTBlock *ifBlock;
    ASTBlock *elseBlock;

    ASTIf(ASTExpressionNode *expression, ASTBlock *ifBlock, ASTBlock *elseBlock);
    ~ASTIf();

    void accept(Visitor *visitor) override;
};
