#include "ASTString.h"

ASTString::ASTString(std::string value) {
    this->value = value;
}

#include "../../../../visitors/Visitor.h"
void ASTString::accept(Visitor *visitor) {
    visitor->visit(this);
}
