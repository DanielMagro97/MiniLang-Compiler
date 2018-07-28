#pragma once

#include "../lexer/Lexer.h"

// statements
#include "ast/ASTStatementNode.h"
#include "ast/statement/ASTVariableDeclaration.h"
#include "ast/statement/ASTAssignment.h"
#include "ast/statement/ASTPrint.h"
#include "ast/statement/ASTIf.h"
#include "ast/statement/ASTWhile.h"
#include "ast/statement/ASTReturn.h"
#include "ast/statement/ASTFunctionDeclaration.h"
#include "ast/statement/ASTBlock.h"

#include "ast/statement/ASTFormalParameters.h"
#include "ast/statement/ASTFormalParameter.h"

// expressions
#include "ast/ASTExpressionNode.h"
#include "ast/expression/ASTBinaryExpr.h"
#include "ast/expression/ASTIdentifier.h"
#include "ast/expression/ASTLiteral.h"
#include "ast/expression/literal/ASTBoolean.h"
#include "ast/expression/literal/ASTInteger.h"
#include "ast/expression/literal/ASTReal.h"
#include "ast/expression/literal/ASTString.h"
#include "ast/expression/ASTFunctionCall.h"
#include "ast/expression/ASTSubExpr.h"
#include "ast/expression/ASTUnaryExpr.h"
#include "ast/expression/ASTType.h"
#include "ast/expression/ASTOperator.h"
#include "ast/expression/ASTActualParameters.h"

class Parser {
private:
    Token currentToken;
    // Token nextToken; - this was removed and instead the peekNextToken method was implemented in the Lexer Class
    vector<ASTNode*> * ast;

    ASTStatementNode * parseStatement();

    // statements
    ASTVariableDeclaration * parseVariableDeclarationStatement();
    ASTAssignment * parseAssignmentStatement();
    ASTPrint * parsePrintStatement();
    ASTIf * parseIfStatement();
    ASTWhile* parseWhileStatement();
    ASTReturn * parseReturnStatement();
    ASTFunctionDeclaration * parseFunctionDeclarationStatement();
    ASTBlock * parseBlock();

    // expressions
    ASTExpressionNode * parseExpression();
    ASTExpressionNode * parseSimpleExpression();
    ASTExpressionNode * parseTerm();
    ASTExpressionNode * parseFactor();
    ASTIdentifier * parseIdentifier();
    ASTLiteral * parseLiteral();
    ASTFunctionCall * parseFunctionCall();
    ASTActualParameters * parseActualParameters(vector<ASTExpressionNode*> *actualParams);
    ASTSubExpr * parseSubExpression();
    ASTUnaryExpr * parseUnary();
    ASTType * parseType();
    ASTOperator * parseOperator();
    ASTUnaryOperator * parseUnaryOperator();
    ASTFormalParameters * parseFormalParameters(vector<ASTFormalParameter*> *formalParams);
    ASTFormalParameter * parseFormalParameter();

public:
    // constructor which initialises the currentToken to the first getNextToken from the Lexer
    Parser();
    // destructor
    ~Parser();
    // method which fetches the next token from the Lexer class until EOF is reached,
    // and adds each generated ast node to the end of the vector of ASTNodes
    vector<ASTNode*>* parse();

    // clearAST is only used by the REPL so that each time a new statement or program file is entered,
    // only the AST for the new code passes through semantic analysis and interpreter execution.
    void  clearAST() {
        ast->clear();
    }
};
