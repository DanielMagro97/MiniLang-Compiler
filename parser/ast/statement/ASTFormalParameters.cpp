#include "ASTFormalParameters.h"

ASTFormalParameters::ASTFormalParameters(std::vector<ASTFormalParameter*> *formalParameters) {
    this->formalParameters = formalParameters;
}

ASTFormalParameters::~ASTFormalParameters() {
    formalParameters->clear();
}

#include "../../../visitors/Visitor.h"
void ASTFormalParameters::accept(Visitor *visitor) {
    visitor->visit(this);
}
