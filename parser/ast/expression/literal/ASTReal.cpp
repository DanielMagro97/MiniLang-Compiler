#include "ASTReal.h"

ASTReal::ASTReal(double value) {
    this->value = value;
}

#include "../../../../visitors/Visitor.h"
void ASTReal::accept(Visitor *visitor) {
    visitor->visit(this);
}
