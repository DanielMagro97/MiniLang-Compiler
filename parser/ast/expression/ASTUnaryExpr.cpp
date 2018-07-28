#include "ASTUnaryExpr.h"

ASTUnaryExpr::ASTUnaryExpr(ASTUnaryOperator *unaryOperator, ASTExpressionNode *expression) {
    this->unaryOperator = unaryOperator;
    this->expression = expression;
}

ASTUnaryExpr::~ASTUnaryExpr() {
    delete unaryOperator;
    delete expression;
}

#include "../../../visitors/Visitor.h"
void ASTUnaryExpr::accept(Visitor *visitor) {
    visitor->visit(this);
}
