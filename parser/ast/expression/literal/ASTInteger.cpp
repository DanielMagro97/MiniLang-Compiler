#include "ASTInteger.h"

ASTInteger::ASTInteger(int value) {
    this->value = value;
}

#include "../../../../visitors/Visitor.h"
void ASTInteger::accept(Visitor *visitor) {
    visitor->visit(this);
}