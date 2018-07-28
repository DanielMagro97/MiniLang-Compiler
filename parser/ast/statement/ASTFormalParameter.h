#pragma once

#include "../ASTStatementNode.h"
#include "../expression/ASTIdentifier.h"
#include "../expression/ASTType.h"

class ASTFormalParameter : public ASTStatementNode {
public:
    ASTIdentifier *astIdentifier;
    ASTType *astType;

    ASTFormalParameter(ASTIdentifier *astIdentifier, ASTType *astType);
    ~ASTFormalParameter();

    void accept(Visitor *visitor) override;
};
