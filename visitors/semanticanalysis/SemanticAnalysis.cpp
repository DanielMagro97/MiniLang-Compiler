#include "SemanticAnalysis.h"

SemanticAnalysis::SemanticAnalysis() {
    symbolTableStack = vector<SymbolTable*>();
    functionParameters = multimap<string, Type>();
    hasReturnStatement = false;

    // push a new symbol table to the symbol table stack, representing the global scope
    symbolTableStack.push_back(new SymbolTable());
}

SemanticAnalysis::~SemanticAnalysis() {
    // pop the first (the global scope) symbol table from the symbol table stack
    symbolTableStack.pop_back();
}

void SemanticAnalysis::visitAST(vector<ASTNode *> *ast) {
    // loop through every node inside the AST
    for (auto const &childNode: *ast) {
        childNode->accept(this);
    }

    // if the Semantic Analysis Visitor has reached this point of execution, then no problems were found
    cout << "No Errors found during Semantic Analysis" << endl;
}



void SemanticAnalysis::visit(ASTVariableDeclaration *astNode) {
    currentSymbolTableScopeIndex = symbolTableStack.size()-1;

    string identifier = astNode->astIdentifier->identifier;
    if (symbolTableStack.at(currentSymbolTableScopeIndex)->isVariableInSymbolTable(identifier)) {
        cout << "the variable " << identifier << " has already been declared in this scope";
        exit(1);
    }

    astNode->astExpressionNode->accept(this);
    if (typeOfCurrentVariable == astNode->astType->type) {
        VariableProperties variableProperties = VariableProperties(typeOfCurrentVariable, VariableType::VARIABLE);
        symbolTableStack.at(currentSymbolTableScopeIndex)->add(identifier,variableProperties);
    } else {
        cout << "the variable type must match the expression type" << endl;
        exit(1);
    }
}

// this visitor makes sure that a value of a certain type is being assigned to a variable of the same type
void SemanticAnalysis::visit(ASTAssignment *astNode) {
    Type identifierType, expressionType;

    astNode->astIdentifier->accept(this);
    identifierType = typeOfCurrentVariable;

    astNode->astExpressionNode->accept(this);
    expressionType = typeOfCurrentVariable;

    if (identifierType != expressionType) {
        cout << "Identifier " << astNode->astIdentifier->identifier  << " is not of the same type as the Expression" << endl;
        exit(1);
    }
}

void SemanticAnalysis::visit(ASTPrint *astNode) {
    astNode->astExpressionNode->accept(this);
}

void SemanticAnalysis::visit(ASTIf *astNode) {
    astNode->expression->accept(this);
    if (typeOfCurrentVariable != Type::BOOL) {
        cout << "The expression inside the if statement must be of Type Bool" << endl;
        exit(1);
    }

    astNode->ifBlock->accept(this);
    astNode->elseBlock->accept(this);
}

void SemanticAnalysis::visit(ASTWhile *astNode) {
    astNode->astExpressionNode->accept(this);
    if (typeOfCurrentVariable != Type::BOOL) {
        cout << "The expression inside the while statement must be of Type Bool" << endl;
        exit(1);
    }

    astNode->astBlock->accept(this);
}

void SemanticAnalysis::visit(ASTReturn *astNode) {
    astNode->astExpressionNode->accept(this);
}

void SemanticAnalysis::visit(ASTFunctionDeclaration *astNode) {
    // the name of the function
    string identifier = astNode->identifier->identifier;
    // the return type of the function (bool, int, real or string)
    Type valueType = astNode->type->type;
    // the order of the parameter types in the function (the signature)
    vector<Type> formalParameterTypes;
    for (int i = 0; i < astNode->formalParameters->formalParameters->size(); i++) {
        formalParameterTypes.push_back(astNode->formalParameters->formalParameters->at(i)->astType->type);
        // adding the formal parameter to the map of Formal Parameters so that they can be added to the block's symbol table
        functionParameters.insert(pair<string, Type>(
                astNode->formalParameters->formalParameters->at(i)->astIdentifier->identifier,
                formalParameterTypes.at(i) ));
    }
    // storing all the details of the method signature in a VariableProperties instance
    VariableProperties variableProperties = VariableProperties(valueType, VariableType::FUNCTION, formalParameterTypes);

    currentSymbolTableScopeIndex = symbolTableStack.size() - 1;
    if (symbolTableStack.at(currentSymbolTableScopeIndex)->isFunctionInSymbolTable(identifier, variableProperties)) {
        cout << "the function " << identifier << " has already been declared inside this scope" << endl;
        exit(1);
    } else {
        // if the function has not already been declared, add it to the symbol table
        symbolTableStack.at(currentSymbolTableScopeIndex)->add(identifier, variableProperties);
    }

    // perform semantic analysis on the block
    astNode->block->accept(this);
    if (!hasReturnStatement) {
        cout << "return statement expected inside declaration of function " << identifier << endl;
        exit(1);
    }
    if (returnType != astNode->type->type) {
        cout << "return type specified in function declaration and the type of variable returned do not match in function ";
        cout << identifier << endl;
    }

    // set back to false since it is no longer needed in this method
    hasReturnStatement = false;
}

void SemanticAnalysis::visit(ASTBlock *astNode) {
    // create a new symbol table for this scope, and push it to the top of the symbol table 'stack'
    symbolTableStack.push_back(new SymbolTable);

    currentSymbolTableScopeIndex = symbolTableStack.size()-1;

    // if there were parameters inside the function declaration, they must be added to this block's symbol table
    if (!functionParameters.empty()) {
        // set the iterator to the beginning of the map
        auto iterator = functionParameters.begin();
        // while the iterator hasn't reached the end of the map
        while (iterator != functionParameters.end()) {
            string identifier = iterator->first;
            VariableProperties variableProperties = VariableProperties(iterator->second, VariableType::VARIABLE);

            // if the parameter name was not already used inside the formal parameters
            if (symbolTableStack.at(currentSymbolTableScopeIndex)->isVariableInSymbolTable(identifier)) {
                cout << "variable " << identifier << " cannot be used twice as a formal parameter" << endl;
                exit(1);
            }
            // add the map entry to the current symbol table
            symbolTableStack.at(currentSymbolTableScopeIndex)->add(identifier, variableProperties);

            // increment the iterator to look at the next entry in the map
            iterator++;
        }
    }
    // clear the map of function parameters since the necessary data has been stored isnide the symbol table
    functionParameters.clear();

    // loop through every statement in the block
    for (auto &statement : *astNode->astBlock) {
        // and visit it
        statement->accept(this);

        // if the current statement is a return statement and a return statement has not yet been found
        if (typeid(*statement) == typeid(ASTReturn) && !hasReturnStatement) {
            hasReturnStatement = true;
            returnType = typeOfCurrentVariable;
        } else if (typeid(*statement) == typeid(ASTReturn) && typeOfCurrentVariable != returnType) {
            // all return statements in the block must be of the same type
            cout << "return statements in functions must all be of the same type" << endl;
            exit(1);
        }
    }

    // when exiting the block (scope), pop the last symbol table
    symbolTableStack.pop_back();
}

void SemanticAnalysis::visit(ASTFormalParameters *astNode) {
}

void SemanticAnalysis::visit(ASTFormalParameter *astNode) {
}



void SemanticAnalysis::visit(ASTBinaryExpr *astNode) {
    // checking that both operands are of the same type
    astNode->simpleExpression->accept(this);
    Type simpleExpression1Type = typeOfCurrentVariable;

    astNode->simpleExpression2->accept(this);
    Type simpleExpression2Type = typeOfCurrentVariable;

    if (simpleExpression1Type != simpleExpression2Type) {
        cout << "bianry operators must be applied to two variables of the same type" << endl;
        exit(1);
    }

    switch (astNode->anOperator->anOperator) {
        case Operator::MULTIPLICATION :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Multiplication '*' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = simpleExpression1Type;
            }
            break;
        case Operator::DIVISION :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Division '/' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = Type::REAL;
            }
            break;
        case Operator::AND :
            if (simpleExpression1Type != Type::BOOL) {
                cout << "AND can only be applied to variables of type Bool" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = Type::BOOL;
            }
            break;
        case Operator::ADDITION :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Addition '+' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = simpleExpression1Type;
            }
            break;
        case Operator::SUBTRACTION :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Subtraction '-' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = simpleExpression1Type;
            }
            break;
        case Operator::OR :
            if (simpleExpression1Type != Type::BOOL) {
                cout << "OR can only be applied to variables of type Bool" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = Type::BOOL;
            }
            break;
        case Operator::LESSTHAN :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Less Than '<' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = Type::BOOL;
            }
            break;
        case Operator::GREATERTHAN :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Greater Than '>' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = Type::BOOL;
            }
            break;
        case Operator::EQUALTO :
            typeOfCurrentVariable = Type::BOOL;
            break;
        case Operator::NOTEQUALTO :
            typeOfCurrentVariable = Type::BOOL;
            break;
        case Operator::LESSTHANOREQUALTO :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Less Than or Equal To '<=' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = Type::BOOL;
            }
            break;
        case Operator::GREATERTHANOREQUALTO :
            if (!(simpleExpression1Type == Type::INT || simpleExpression1Type == Type::REAL)) {
                cout << "Greater Than or Equal To '>=' can only be applied to variables of type Int or Real" << endl;
                exit(1);
            } else {
                typeOfCurrentVariable = Type::BOOL;
            }
            break;
    }
}

void SemanticAnalysis::visit(ASTOperator *astNode) {
}

void SemanticAnalysis::visit(ASTBoolean *astNode) {
    typeOfCurrentVariable = Type::BOOL;
}

void SemanticAnalysis::visit(ASTInteger *astNode) {
    typeOfCurrentVariable = Type::INT;
}

void SemanticAnalysis::visit(ASTReal *astNode) {
    typeOfCurrentVariable = Type::REAL;
}

void SemanticAnalysis::visit(ASTString *astNode) {
    typeOfCurrentVariable = Type::STRING;
}

void SemanticAnalysis::visit(ASTIdentifier *astNode) {
    // start from the current scope, and look through its symbol table for the variable with the identifier
    // if it is not found in the current scope, move to the outer scope to look for it, until the global scope is reached
    currentSymbolTableScopeIndex = symbolTableStack.size()-1;
    while (currentSymbolTableScopeIndex >= 0) {
        // if the current scope contains the identifier
        if (symbolTableStack.at(currentSymbolTableScopeIndex)->isVariableInSymbolTable(astNode->identifier)) {
            // store its type
            typeOfCurrentVariable = symbolTableStack.at(currentSymbolTableScopeIndex)->typeOfVariable(astNode->identifier);
            // and return execution
            return;
        }

        currentSymbolTableScopeIndex--;
    }

    // if this point is reached then the identifier was not found in any scope's symbol table
    cout << astNode->identifier << " was not declared in any scope" << endl;
    exit(1);
}

void SemanticAnalysis::visit(ASTType *astNode) {
}

void SemanticAnalysis::visit(ASTFunctionCall *astNode) {
    // boolean value storing whether a function was found in any scope or not
    bool functionFound = false;

    // retrieve the function's identifier
    string identifier = astNode->astIdentifier->identifier;

    // retrieve a vector with the function's parameters' data type
    vector<Type> actualParameterTypes;
    // loop through all the actual parameters in the function call
    for (auto &astActualParameter : *astNode->astActualParameters->astActualParameters) {
        // visit the actual parameter so that its type is stored inside typeOfCurrentVariable
        astActualParameter->accept(this);
        // add the type of the actual parameter to the vector of types
        actualParameterTypes.push_back(typeOfCurrentVariable);
    }

    currentSymbolTableScopeIndex = symbolTableStack.size()-1;
    while (currentSymbolTableScopeIndex >= 0) {
        // check if a function with the specified identifier and parameter types exists in the current scope
        if (symbolTableStack.at(currentSymbolTableScopeIndex)->areFunctionIdandParamsInSymbolTable(identifier, actualParameterTypes)) {
            // if so, indicate that the function has been found by marking the boolean value as true
            functionFound = true;
            // find its return type
            typeOfCurrentVariable = symbolTableStack.at(currentSymbolTableScopeIndex)->typeOfFunction(identifier,actualParameterTypes);
            // exit the loop that looks for the function since it has been found
            break;
        } else {
            // if the function was not found in the symbol table of this scope,
            // move to an outer scope in the next iteration and check that symbol table
            currentSymbolTableScopeIndex--;
            continue;
        }
    }

    if (!functionFound) {
        cout << "function " << identifier << " was not found in any scope" << endl;
        exit(1);
    }
}

void SemanticAnalysis::visit(ASTActualParameters *astNode) {
}

void SemanticAnalysis::visit(ASTSubExpr *astNode) {
    astNode->expression->accept(this);
}

void SemanticAnalysis::visit(ASTUnaryExpr *astNode) {
    astNode->expression->accept(this);
    if (typeOfCurrentVariable == Type::INT || typeOfCurrentVariable == Type::REAL) {
        if (astNode->unaryOperator->unaryOperator != UnaryOperator::MINUS) {
            cout << "NOT cannot be applied to variables of type Int or Real" << endl;
            exit(1);
        }
    } else if (typeOfCurrentVariable == Type::BOOL) {
        if (astNode->unaryOperator->unaryOperator != UnaryOperator::NOT) {
            cout << "MINUS '-' cannot be applied to variables of type Bool" << endl;
            exit(1);
        }
    } else if (typeOfCurrentVariable == Type::STRING) {
        cout << "Unary Operators cannot be applied to variables of type String" << endl;
        exit(1);
    }
}

void SemanticAnalysis::visit(ASTUnaryOperator *astNode) {
}
