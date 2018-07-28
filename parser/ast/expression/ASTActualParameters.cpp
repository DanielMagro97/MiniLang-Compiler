#include "ASTActualParameters.h"

ASTActualParameters::ASTActualParameters(std::vector<ASTExpressionNode*> *astActualParameters) {
    this->astActualParameters = astActualParameters;
}

ASTActualParameters::~ASTActualParameters() {
    astActualParameters->clear();
    /*
    for (auto const & parameter : astActualParameters) {
        delete parameter;
    }
     */
}

#include "../../../visitors/Visitor.h"
void ASTActualParameters::accept(Visitor *visitor) {
    visitor->visit(this);
}
