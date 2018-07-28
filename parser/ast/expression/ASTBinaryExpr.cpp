#include "ASTBinaryExpr.h"

ASTBinaryExpr::ASTBinaryExpr(ASTExpressionNode *simpleExpression, ASTOperator *anOperator,
                             ASTExpressionNode *simpleExpression2) {
    this->simpleExpression = simpleExpression;
    this->anOperator = anOperator;
    this->simpleExpression2 = simpleExpression2;
}

ASTBinaryExpr::~ASTBinaryExpr() {
    delete simpleExpression;
    delete anOperator;
    delete simpleExpression2;
}

#include "../../../visitors/Visitor.h"
void ASTBinaryExpr::accept(Visitor *visitor) {
    visitor->visit(this);
}
