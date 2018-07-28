#include "ASTAssignment.h"

ASTAssignment::ASTAssignment(ASTIdentifier *astIdentifier, ASTExpressionNode *astExpressionNode) {
    this->astIdentifier = astIdentifier;
    this->astExpressionNode = astExpressionNode;
}

ASTAssignment::~ASTAssignment() {
    delete astIdentifier;
    delete astExpressionNode;
}

#include "../../../visitors/Visitor.h"
void ASTAssignment::accept(Visitor *visitor) {
    visitor->visit(this);
}
