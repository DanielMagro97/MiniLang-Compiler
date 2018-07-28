#include "ASTSubExpr.h"

ASTSubExpr::ASTSubExpr(ASTExpressionNode *expression) {
    this->expression = expression;
}

ASTSubExpr::~ASTSubExpr() {
    delete expression;
}

#include "../../../visitors/Visitor.h"
void ASTSubExpr::accept(Visitor *visitor) {
    visitor->visit(this);
}
