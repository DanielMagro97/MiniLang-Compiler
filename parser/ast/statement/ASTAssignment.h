#pragma once

#include "../ASTStatementNode.h"
#include "../expression/ASTIdentifier.h"
#include "../ASTExpressionNode.h"

class ASTAssignment : public ASTStatementNode {
public:
    ASTIdentifier *astIdentifier;
    ASTExpressionNode *astExpressionNode;

    ASTAssignment(ASTIdentifier *astIdentifier, ASTExpressionNode *astExpressionNode);
    ~ASTAssignment();

    void accept(Visitor *visitor) override;
};
