#include "ASTPrint.h"

ASTPrint::ASTPrint(ASTExpressionNode *astExpressionNode) {
    this->astExpressionNode = astExpressionNode;
}

ASTPrint::~ASTPrint() {
    delete astExpressionNode;
}

#include "../../../visitors/Visitor.h"
void ASTPrint::accept(Visitor *visitor) {
    visitor->visit(this);
}
