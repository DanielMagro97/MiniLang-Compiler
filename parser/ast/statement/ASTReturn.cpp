#include "ASTReturn.h"

ASTReturn::ASTReturn(ASTExpressionNode *astExpressionNode) {
    this->astExpressionNode = astExpressionNode;
}

ASTReturn::~ASTReturn() {
    delete astExpressionNode;
}

#include "../../../visitors/Visitor.h"
void ASTReturn::accept(Visitor *visitor) {
    visitor->visit(this);
}
