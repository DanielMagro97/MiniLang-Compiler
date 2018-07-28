#include "ASTWhile.h"

ASTWhile::ASTWhile(ASTExpressionNode *astExpressionNode, ASTBlock *astBlock) {
    this->astExpressionNode = astExpressionNode;
    this->astBlock = astBlock;
}

ASTWhile::~ASTWhile() {
    delete astExpressionNode;
    delete astBlock;
}

#include "../../../visitors/Visitor.h"
void ASTWhile::accept(Visitor *visitor) {
    visitor->visit(this);
}
