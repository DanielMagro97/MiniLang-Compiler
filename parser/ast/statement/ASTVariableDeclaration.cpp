#include "ASTVariableDeclaration.h"

ASTVariableDeclaration::ASTVariableDeclaration(ASTIdentifier *astIdentifier, ASTType *astType,
                                               ASTExpressionNode *astExpressionNode) {
    this->astIdentifier = astIdentifier;
    this->astType = astType;
    this->astExpressionNode = astExpressionNode;
}

ASTVariableDeclaration::~ASTVariableDeclaration() {
    delete astIdentifier;
    delete astType;
    delete astExpressionNode;
}

#include "../../../visitors/Visitor.h"
void ASTVariableDeclaration::accept(Visitor *visitor) {
    visitor->visit(this);
}
