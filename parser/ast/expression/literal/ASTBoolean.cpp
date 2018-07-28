#include "ASTBoolean.h"

ASTBoolean::ASTBoolean(bool value) {
    this->value = value;
}

#include "../../../../visitors/Visitor.h"
void ASTBoolean::accept(Visitor *visitor) {
    visitor->visit(this);
}
