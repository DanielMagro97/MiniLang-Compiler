#include "ASTFunctionDeclaration.h"

ASTFunctionDeclaration::ASTFunctionDeclaration(ASTIdentifier *identifier, ASTFormalParameters *formalParameters,
                                               ASTType *type, ASTBlock *block) {
    this->identifier = identifier;
    this->formalParameters = formalParameters;
    this->type = type;
    this->block = block;
}

ASTFunctionDeclaration::~ASTFunctionDeclaration() {
    delete identifier;
    delete formalParameters;
    delete type;
    delete block;
}

#include "../../../visitors/Visitor.h"
void ASTFunctionDeclaration::accept(Visitor *visitor) {
    visitor->visit(this);
}
