#pragma once

#include <typeinfo>
#include "../Visitor.h"
#include "SymbolTable.h"

class SemanticAnalysis : public Visitor {
private:
    vector<SymbolTable*> symbolTableStack;
    // this variable of type 'Type' will store the type of a particular node/variable which has just been visited
    // i.e. whether it is Bool, Int, Real or String
    Type typeOfCurrentVariable;
    // map of strings to variableTypes of the parameters of a function,
    // which will be added to the symbol table of its block
    multimap<string, Type> functionParameters;
    // boolean value which stores whether a block in a function declaration has a return statement
    bool hasReturnStatement;
    // Type which stores the return type of the variable returned inside a function declaration's block
    Type returnType;
    // number which stores the index of the current scope's symbol table - updated using symbolTableStack.size()-1
    unsigned long long int currentSymbolTableScopeIndex;
public:
    SemanticAnalysis();
    ~SemanticAnalysis();

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

    // used by the #st command inside MiniLangI
    vector<SymbolTable*> getSymbolTables() {
        return symbolTableStack;
    }
};
