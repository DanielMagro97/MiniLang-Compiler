#pragma once

#include "ASTNode.h"

class ASTStatementNode : public ASTNode {
public:
    void accept(Visitor *visitor) override = 0;
};