#include "ASTUnaryOperator.h"

ASTUnaryOperator::ASTUnaryOperator(UnaryOperator unaryOperator) {
    this->unaryOperator = unaryOperator;
}

ASTUnaryOperator::~ASTUnaryOperator() = default;

#include "../../../visitors/Visitor.h"
void ASTUnaryOperator::accept(Visitor *visitor) {
    visitor->visit(this);
}

std::string ASTUnaryOperator::toString() {
    if (unaryOperator == UnaryOperator::MINUS) {
        return "-";
    } else if (unaryOperator == UnaryOperator::NOT) {
        return "NOT";
    }
}
