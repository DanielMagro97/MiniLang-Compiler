#include "ASTBlock.h"

ASTBlock::ASTBlock(std::vector<ASTStatementNode *> *astBlock) {
    this->astBlock = astBlock;
}

ASTBlock::~ASTBlock() {
    astBlock->clear();
}

#include "../../../visitors/Visitor.h"
void ASTBlock::accept(Visitor *visitor) {
    visitor->visit(this);
}
