#pragma once

#include "../ASTLiteral.h"
#include <iostream>

class ASTString : public ASTLiteral {
public:
    std::string value;

    explicit ASTString(std::string value);
    ~ASTString() = default;

    void accept(Visitor *visitor) override;
};
