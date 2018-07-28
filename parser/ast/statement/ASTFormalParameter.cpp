#include "ASTFormalParameter.h"

ASTFormalParameter::ASTFormalParameter(ASTIdentifier *astIdentifier, ASTType *astType) {
    this->astIdentifier = astIdentifier;
    this->astType = astType;
}

ASTFormalParameter::~ASTFormalParameter() {
    delete astIdentifier;
    delete astIdentifier;
}

#include "../../../visitors/Visitor.h"
void ASTFormalParameter::accept(Visitor *visitor) {
    visitor->visit(this);
}
