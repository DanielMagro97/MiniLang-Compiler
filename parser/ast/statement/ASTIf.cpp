#include "ASTIf.h"

ASTIf::ASTIf(ASTExpressionNode *expression, ASTBlock *ifBlock, ASTBlock *elseBlock) {
    this->expression = expression;
    this->ifBlock = ifBlock;
    this->elseBlock = elseBlock;
}

ASTIf::~ASTIf() {
    delete expression;
    delete ifBlock;
    delete elseBlock;
}

#include "../../../visitors/Visitor.h"
void ASTIf::accept(Visitor *visitor) {
    visitor->visit(this);
}
