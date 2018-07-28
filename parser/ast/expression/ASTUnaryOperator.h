#pragma once

#include "../ASTExpressionNode.h"
#include <iostream>

enum class UnaryOperator {
    MINUS,
    NOT
};

class ASTUnaryOperator : public ASTExpressionNode {
public:
    UnaryOperator unaryOperator;

    explicit ASTUnaryOperator(UnaryOperator unaryOperator);
    ~ASTUnaryOperator();

    void accept(Visitor *visitor) override;

    std::string toString();
};
