#pragma once

#include "../ASTLiteral.h"

class ASTReal : public ASTLiteral {
public:
    double value;

    explicit ASTReal(double value);
    ~ASTReal() = default;

    void accept(Visitor *visitor) override;
};
