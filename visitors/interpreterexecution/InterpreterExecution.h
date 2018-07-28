#pragma once

#include "../Visitor.h"
#include "../semanticanalysis/SemanticAnalysis.h"

class InterpreterExecution : public Visitor {
private:

    vector<SymbolTable*> symbolTableStack;

    // this variable of type 'Type' will store the type of the last accessed variable
    // (i.e. whether it is Bool, Int, Real or String) used so that the value is stored in the correct vector
    Type typeOfCurrentVariable;

    // vector which stores all the values of type Bool
    vector<bool> boolValues;
    // vector which stores all the values of type Int
    vector<int> intValues;
    // vector which stores all the values of type Real
    vector<double> realValues;
    // vector which stores all the values of type String
    vector<string> stringValues;

    // boolean value which stores whether a block in a function declaration has a return statement
    bool hasReturnStatement;

    // pointer to ASTFormalParameters which is used when visiting a block so that the function parameters can be
    // added to the new scope's symbol table
    ASTFormalParameters *functionParameters;

    // number which stores the index of the current scope's symbol table - updated using symbolTableStack.size()-1
    unsigned long long int currentSymbolTableScopeIndex;
public:
    InterpreterExecution();
    ~InterpreterExecution();

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
};
