#include "ASTIdentifier.h"

ASTIdentifier::ASTIdentifier(std::string identifier) {
    this->identifier = identifier;
}

ASTIdentifier::~ASTIdentifier() = default;

#include "../../../visitors/Visitor.h"
void ASTIdentifier::accept(Visitor *visitor) {
    visitor->visit(this);
}
