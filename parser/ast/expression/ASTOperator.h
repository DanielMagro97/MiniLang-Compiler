#pragma once

#include "../ASTExpressionNode.h"
#include <iostream> // for string

enum class Operator {
    MULTIPLICATION,
    DIVISION,
    AND,
    ADDITION,
    SUBTRACTION,
    OR,
    LESSTHAN,
    GREATERTHAN,
    EQUALTO,
    NOTEQUALTO,
    LESSTHANOREQUALTO,
    GREATERTHANOREQUALTO
};

class ASTOperator : public ASTExpressionNode {
public:
    Operator anOperator;

    explicit ASTOperator(Operator anOperator);
    ~ASTOperator();

    void accept(Visitor *visitor) override;

    std::string toString();
};
