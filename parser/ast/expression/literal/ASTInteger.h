#pragma once

#include "../ASTLiteral.h"

class ASTInteger : public ASTLiteral {
public:
    int value;

    explicit ASTInteger(int value);
    ~ASTInteger() = default;

    void accept(Visitor *visitor) override;
};
