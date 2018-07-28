#include "ASTFunctionCall.h"

ASTFunctionCall::ASTFunctionCall(ASTIdentifier *astIdentifier, ASTActualParameters *astActualParameters) {
    this->astIdentifier = astIdentifier;
    this->astActualParameters = astActualParameters;
}

ASTFunctionCall::~ASTFunctionCall() {
    delete astIdentifier;
    delete astActualParameters;
}

#include "../../../visitors/Visitor.h"
void ASTFunctionCall::accept(Visitor *visitor) {
    visitor->visit(this);
}
