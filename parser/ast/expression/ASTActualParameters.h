#pragma once

#include "../ASTExpressionNode.h"
#include <vector>

class ASTActualParameters : public ASTExpressionNode {
public:
    std::vector<ASTExpressionNode*> *astActualParameters;

    explicit ASTActualParameters(std::vector<ASTExpressionNode*> *astActualParameters);
    ~ASTActualParameters();

    void accept(Visitor *visitor) override;
};
