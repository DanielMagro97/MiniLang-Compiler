#include "ASTType.h"

ASTType::ASTType(Type type) {
    this->type = type;
}

ASTType::~ASTType() = default;

#include "../../../visitors/Visitor.h"
void ASTType::accept(Visitor *visitor) {
    visitor->visit(this);
}

std::string ASTType::toString() {
    if (type == Type::BOOL) {
        return  "Bool";
    } else if (type == Type::INT) {
        return  "Int";
    } else if (type == Type::REAL) {
        return  "Real";
    } else if (type == Type::STRING) {
        return  "String";
    }
}
