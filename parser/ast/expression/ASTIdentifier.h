#pragma once

#include "../ASTExpressionNode.h"
#include <iostream>

class ASTIdentifier : public ASTExpressionNode {
public:
    std::string identifier;

    explicit ASTIdentifier(std::string identifier);
    ~ASTIdentifier();

    void accept(Visitor *visitor) override;
};
