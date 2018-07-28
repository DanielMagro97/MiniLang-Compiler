#pragma once

#include "../ASTStatementNode.h"
#include <vector>

class ASTBlock : public ASTStatementNode {
public:
    std::vector<ASTStatementNode*> *astBlock;

    explicit ASTBlock(std::vector<ASTStatementNode*> *astBlock);
    ~ASTBlock();

    void accept(Visitor *visitor) override;
};
