#pragma once

#include "../ASTStatementNode.h"
#include "ASTFormalParameter.h"
#include "vector"

class ASTFormalParameters : public ASTStatementNode {
public:
    std::vector<ASTFormalParameter*> *formalParameters;

    explicit ASTFormalParameters(std::vector<ASTFormalParameter*> *formalParameters);
    ~ASTFormalParameters();

    void accept(Visitor *visitor) override;
};
