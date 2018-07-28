#pragma once

#include "../ASTStatementNode.h"
#include "../expression/ASTIdentifier.h"
#include "../expression/ASTType.h"
#include "../ASTExpressionNode.h"

class ASTVariableDeclaration : public ASTStatementNode {
public:
    ASTIdentifier *astIdentifier;
    ASTType *astType;
    ASTExpressionNode *astExpressionNode;

    ASTVariableDeclaration(ASTIdentifier *astIdentifier, ASTType *astType, ASTExpressionNode *astExpressionNode);
    ~ASTVariableDeclaration();

    void accept(Visitor *visitor) override;
};
