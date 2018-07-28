#pragma once

#include "../ASTExpressionNode.h"
#include "../expression/ASTIdentifier.h"
#include "../expression/ASTActualParameters.h"

class ASTFunctionCall : public ASTExpressionNode {
public:
    ASTIdentifier *astIdentifier;
    ASTActualParameters *astActualParameters;

    ASTFunctionCall(ASTIdentifier *astIdentifier, ASTActualParameters *astActualParameters);
    ~ASTFunctionCall();

    void accept(Visitor *visitor) override;
};
