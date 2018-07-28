#pragma once

#include "../ASTExpressionNode.h"
#include <iostream>

enum class Type {
    REAL,
    INT,
    BOOL,
    STRING
};

class ASTType : public ASTExpressionNode {
public:
    Type type;

    explicit ASTType(Type type);
    ~ASTType();

    void accept(Visitor *visitor) override;

    std::string toString();
};
