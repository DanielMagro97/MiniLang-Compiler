#pragma once

// statements
#include "../parser/ast/ASTStatementNode.h"
#include "../parser/ast/statement/ASTVariableDeclaration.h"
#include "../parser/ast/statement/ASTAssignment.h"
#include "../parser/ast/statement/ASTPrint.h"
#include "../parser/ast/statement/ASTIf.h"
#include "../parser/ast/statement/ASTWhile.h"
#include "../parser/ast/statement/ASTReturn.h"
#include "../parser/ast/statement/ASTFunctionDeclaration.h"
#include "../parser/ast/statement/ASTBlock.h"

#include "../parser/ast/expression/ASTBinaryExpr.h"
#include "../parser/ast/expression/literal/ASTBoolean.h"
#include "../parser/ast/expression/literal/ASTInteger.h"
#include "../parser/ast/expression/literal/ASTReal.h"
#include "../parser/ast/expression/literal/ASTString.h"
#include "../parser/ast/expression/ASTIdentifier.h"
#include "../parser/ast/expression/ASTFunctionCall.h"
#include "../parser/ast/expression/ASTSubExpr.h"
#include "../parser/ast/expression/ASTUnaryExpr.h"

class Visitor {
public:
    // statements
    virtual void visit(ASTVariableDeclaration *astNode)= 0;
    virtual void visit(ASTAssignment *astNode)= 0;
    virtual void visit(ASTPrint *astNode)= 0;
    virtual void visit(ASTIf *astNode)= 0;
    virtual void visit(ASTWhile *astNode)= 0;
    virtual void visit(ASTReturn *astNode)= 0;
    virtual void visit(ASTFunctionDeclaration *astNode)= 0;
    virtual void visit(ASTBlock *astNode)= 0;

    virtual void visit(ASTFormalParameters *astNode)= 0;
    virtual void visit(ASTFormalParameter *astNode)= 0;

    // expressions
    virtual void visit(ASTBinaryExpr *astNode)= 0;
    // literals
    virtual void visit(ASTBoolean *astNode) = 0;
    virtual void visit(ASTInteger *astNode) = 0;
    virtual void visit(ASTReal *astNode) = 0;
    virtual void visit(ASTString *astNode) = 0;
    // identifier
    virtual void visit(ASTIdentifier *astNode)= 0;
    // function call
    virtual void visit(ASTFunctionCall *astNode)= 0;
    virtual void visit(ASTActualParameters *astNode)= 0;
    // sub expression
    virtual void visit(ASTSubExpr *astNode)= 0;
    // unary expression
    virtual void visit(ASTUnaryExpr *astNode)= 0;

    virtual void visit(ASTType *astNode)= 0;
    virtual void visit(ASTOperator *astNode)= 0;
    virtual void visit(ASTUnaryOperator *astNode)= 0;
};
