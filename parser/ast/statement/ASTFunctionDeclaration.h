#pragma once

#include "../ASTStatementNode.h"
#include "../expression/ASTIdentifier.h"
#include "ASTFormalParameters.h"
#include "../expression/ASTType.h"
#include "ASTBlock.h"


class ASTFunctionDeclaration : public ASTStatementNode {
public:
    ASTIdentifier *identifier;
    ASTFormalParameters *formalParameters;
    ASTType *type;
    ASTBlock *block;

    ASTFunctionDeclaration(ASTIdentifier *identifier, ASTFormalParameters *formalParameters,
                           ASTType *type, ASTBlock *block);
    ~ASTFunctionDeclaration();

    void accept(Visitor *visitor) override;
};
