#pragma once

class Visitor;

class ASTNode {
public:
    virtual void accept(Visitor *visitor) = 0;
};