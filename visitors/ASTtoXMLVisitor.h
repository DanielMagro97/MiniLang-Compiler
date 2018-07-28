#pragma once

#include <iostream>
#include <vector>

#include "Visitor.h"

using namespace std;

class ASTtoXMLVisitor : public Visitor {
public:
    // constructor method for the ASTtoXMLVisitor class
    ASTtoXMLVisitor();
    // method which visits every node in the Abstract Syntax Tree
    void visitAST(vector<ASTNode *> *ast);
    // statement visitors
    void visit(ASTVariableDeclaration *astNode) override;
    void visit(ASTAssignment *astNode) override;
    void visit(ASTPrint *astNode) override;
    void visit(ASTIf *astNode) override;
    void visit(ASTWhile *astNode) override;
    void visit(ASTReturn *astNode) override;
    void visit(ASTFunctionDeclaration *astNode) override;
    void visit(ASTBlock *astNode) override;

    void visit(ASTFormalParameters *astNode) override;
    void visit(ASTFormalParameter *astNode) override;


    // expression visitors
    void visit(ASTBinaryExpr *astNode) override;
    void visit(ASTOperator *astNode) override;
    void visit(ASTBoolean *astNode) override;
    void visit(ASTInteger *astNode) override;
    void visit(ASTReal *astNode) override;
    void visit(ASTString *astNode) override;
    void visit(ASTIdentifier *astNode) override;
    void visit(ASTType *astNode) override;
    void visit(ASTFunctionCall *astNode) override;
    void visit(ASTActualParameters *astNode) override;
    void visit(ASTSubExpr *astNode) override;
    void visit(ASTUnaryExpr *astNode) override;
    void visit(ASTUnaryOperator *astNode) override;

private:
    // string which stores the total indentation before the start of an XML entry. stores multiple of the '\t' character
    string indentation;
    // method which adds a tab '\t' to the indentation string
    void indent();
    // method which reduces a tab '\t' from the indentation string
    void outdent();
};
