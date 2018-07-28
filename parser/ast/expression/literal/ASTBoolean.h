#pragma once

#include "../ASTLiteral.h"

class ASTBoolean : public ASTLiteral {
public:
    bool value;

    explicit ASTBoolean(bool value);
    ~ASTBoolean() = default;

    void accept(Visitor *visitor) override;
};
