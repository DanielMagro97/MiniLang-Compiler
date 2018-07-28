#include "InterpreterExecution.h"

InterpreterExecution::InterpreterExecution() {
    symbolTableStack = vector<SymbolTable*>();

    boolValues = vector<bool>();
    intValues = vector<int>();
    realValues = vector<double>();
    stringValues = vector<string>();

    hasReturnStatement = false;

    functionParameters = nullptr;

    // push a new symbol table to the symbol table stack, representing the global scope
    symbolTableStack.push_back(new SymbolTable());
}

InterpreterExecution::~InterpreterExecution() {
    // pop the first (the global scope) symbol table from the symbol table stack
    symbolTableStack.pop_back();
}

void InterpreterExecution::visitAST(vector<ASTNode *> *ast) {
    // loop through every node inside the AST
    for (auto const &childNode: *ast) {
        childNode->accept(this);
    }
}


void InterpreterExecution::visit(ASTVariableDeclaration *astNode) {
    // visit the expression part of the variable declaration, so that the value is stored in its respective vector
    astNode->astExpressionNode->accept(this);
    // store the identifier from the astNode inside this string
    string identifier = astNode->astIdentifier->identifier;
    // store the 'type of the variable' in this variable.
    // this will be the same as what is stored inside typeOfCurrentVariable after visiting the expression
    Type typeOfVariable = astNode->astType->type;

    // set the current symbolTableIndex to the current scope
    currentSymbolTableScopeIndex = symbolTableStack.size()-1;
    // declare a new VariableProperties instance, and assign the type of the variable being declared,
    // and mark its VariableType as 'Variable'
    VariableProperties variableProperties = VariableProperties(typeOfVariable, VariableType::VARIABLE);
    switch (typeOfVariable) {
        case Type::BOOL :
            // since this variable is of type Bool, initialise its value to the Bool at the end of the boolValues vector
            variableProperties.boolValue = boolValues.back();
            // remove the Bool value at the end of the boolValues vector as it is no longer needed
            boolValues.pop_back();
            break;
        case Type::INT :
            variableProperties.intValue = intValues.back();
            intValues.pop_back();
            break;
        case Type::REAL :
            variableProperties.realValue = realValues.back();
            realValues.pop_back();
            break;
        case Type::STRING :
            variableProperties.stringValue = stringValues.back();
            stringValues.pop_back();
            break;
    }

    // add the current variable to the current scope's symbol table with the node's identifier and variableProperties
    symbolTableStack.at(currentSymbolTableScopeIndex)->add(identifier, variableProperties);
}

void InterpreterExecution::visit(ASTAssignment *astNode) {
    currentSymbolTableScopeIndex = symbolTableStack.size()-1;

    // retrieve the name of the variable being assigned
    string identifier = astNode->astIdentifier->identifier;

    // start from the current scope, and look through its symbol table for the variable with the identifier
    // if it is not found in the current scope, move to the outer scope to look for it
    while (currentSymbolTableScopeIndex >= 0) {
        // if the current scope contains the identifier
        if (symbolTableStack.at(currentSymbolTableScopeIndex)->isVariableInSymbolTable(identifier)) {
            // store its type
            typeOfCurrentVariable = symbolTableStack.at(currentSymbolTableScopeIndex)->typeOfVariable(identifier);
            // and stop looking since the variable has been found
            break;
        }

        currentSymbolTableScopeIndex--;
    }

    // visit the expression of the assignment statement. the expression visitor will have
    // access to the type of the identifier (since it is stored in typeOfCurrentVariable)
    astNode->astExpressionNode->accept(this);

    // depending on the type of the expression
    switch (typeOfCurrentVariable) {
        case Type::BOOL :
            // set the Bool value of the identifier (variable) as the Bool value at the end of the boolValues vector
            symbolTableStack.at(currentSymbolTableScopeIndex)->setBoolValueOfVariable(identifier, boolValues.back());
            // pop the Bool value at the end of the boolValues vector since it is no longer needed
            boolValues.pop_back();
            break;
        case Type::INT :
            symbolTableStack.at(currentSymbolTableScopeIndex)->setIntValueOfVariable(identifier, intValues.back());
            intValues.pop_back();
            break;
        case Type::REAL :
            symbolTableStack.at(currentSymbolTableScopeIndex)->setRealValueOfVariable(identifier, realValues.back());
            realValues.pop_back();
            break;
        case Type::STRING :
            symbolTableStack.at(currentSymbolTableScopeIndex)->setStringValueOfVariable(identifier, stringValues.back());
            stringValues.pop_back();
            break;

    }
}

void InterpreterExecution::visit(ASTPrint *astNode) {
    astNode->astExpressionNode->accept(this);

    switch (typeOfCurrentVariable) {
        case Type::BOOL :
            // the vector of values will contain the value to be printed
            cout << boolValues.back() << endl;
            // pop the value since it is no longer needed
            boolValues.pop_back();
            break;
        case Type::INT :
            cout << intValues.back() << endl;
            intValues.pop_back();
            break;
        case Type::REAL :
            cout << realValues.back() << endl;
            realValues.pop_back();
            break;
        case Type::STRING :
            cout << stringValues.back() << endl;
            stringValues.pop_back();
            break;
    }
}

void InterpreterExecution::visit(ASTIf *astNode) {
    // visit the expression so that the boolean expression is evaluated to true or false
    // and stored at the end of the boolValues vector
    astNode->expression->accept(this);

    // if the expression was evaluated to true, visit the ifBlock
    if (boolValues.back()) {
        astNode->ifBlock->accept(this);
    } else {
        // // if the expression was evaluated to false, visit the elseBlock
        astNode->elseBlock->accept(this);
    }

    // remove the boolean value from the end of the boolValues vecor sine it is no longer required
    boolValues.pop_back();
}

void InterpreterExecution::visit(ASTWhile *astNode) {
    // visit the expression so that the boolean expression is evaluated to true or false
    // and stored at the end of the boolValues vector
    astNode->astExpressionNode->accept(this);

    // if the expression was evaluated to be true
    while (boolValues.back()) {
        // visit the block
        astNode->astBlock->accept(this);
        // remove the boolean value from the end of the boolValues vector, since it has already been used
        boolValues.pop_back();
        // reevaluate the expression, to decide whether to execute the next iteration
        astNode->astExpressionNode->accept(this);
    }

    // remove the boolean value from the end of the boolValues vector, since it is no longer needed
    boolValues.pop_back();
}

void InterpreterExecution::visit(ASTReturn *astNode) {
    // visit the expression which will be returned
    astNode->astExpressionNode->accept(this);
    // mark the boolean value hasReturnStatement as true (for the ASTBlock visitor
    hasReturnStatement = true;
}

void InterpreterExecution::visit(ASTFunctionDeclaration *astNode) {
    // store the node's formal parameters inside the global variable functionParameters, which will be used when
    // visiting a block so that the function parameters can be added to the new scope's symbol table
    functionParameters = astNode->formalParameters;

    // Declare a new VariableProperties instance
    // and initialise it to the same type as the function and mark its variable type as a 'function'
    VariableProperties variableProperties = VariableProperties(astNode->type->type, VariableType::FUNCTION);

    // loop through every formal parameter inside astFormalParameters
    for (auto const &formalParameter : *astNode->formalParameters->formalParameters) {
        // and add each parameter's type to the VariableProperties's vector of parameter types
        variableProperties.parameterTypes.push_back(formalParameter->astType->type);
    }
    // add the astFormalParameters node to the VariableProperties
    variableProperties.formalParameters = astNode->formalParameters;
    // add the ASTBlock to the VariableProperties functionBody
    variableProperties.functionBody = astNode->block;

    // add the function identifier together with the variable properties to this scope's symbol table
    currentSymbolTableScopeIndex = symbolTableStack.size()-1;
    symbolTableStack.at(currentSymbolTableScopeIndex)->add(astNode->identifier->identifier, variableProperties);
}

void InterpreterExecution::visit(ASTBlock *astNode) {
    // push a new Symbol Table to the symbolTableStack since a new scope has been entered
    symbolTableStack.push_back(new SymbolTable);

    currentSymbolTableScopeIndex = symbolTableStack.size()-1;

    // if there are any function parameters, they must be added to the new block's (new scope's) symbol table
    if (functionParameters != nullptr) {
        // calculate how many of each type of parameter there are so that they can be popped correctly from the vector of results
        int boolParams = 0;
        int intParams = 0;
        int realParams = 0;
        int stringParams = 0;
        for (auto const &paramType : *functionParameters->formalParameters) {
            switch (paramType->astType->type) {
                case Type::BOOL :
                    boolParams++;
                    break;
                case Type::INT :
                    intParams++;
                    break;
                case Type::REAL :
                    realParams++;
                    break;
                case Type::STRING :
                    stringParams++;
                    break;
            }
        }

        for (auto const &parameter : *functionParameters->formalParameters) {
            string identifier = parameter->astIdentifier->identifier;
            VariableProperties variableProperties = VariableProperties(parameter->astType->type, VariableType::VARIABLE);

            switch (parameter->astType->type) {
                case Type::BOOL :
                    variableProperties.boolValue = boolValues.at(boolValues.size()-boolParams);
                    boolParams--;
                    break;
                case Type::INT :
                    variableProperties.intValue = intValues.at(intValues.size()-intParams);
                    intParams--;
                    break;
                case Type::REAL :
                    variableProperties.realValue = realValues.at(realValues.size()-realParams);
                    realParams--;
                    break;
                case Type::STRING :
                    variableProperties.stringValue = stringValues.at(stringValues.size()-stringParams);
                    stringParams--;
                    break;
            }

            symbolTableStack.at(currentSymbolTableScopeIndex)->add(identifier, variableProperties);
        }

        // since the function parameters used by this function have been stored in this symbol table,
        // remove the values from their respective vectors
        for (auto const &paramType : *functionParameters->formalParameters) {
            switch (paramType->astType->type) {
                case Type::BOOL :
                    boolValues.pop_back();
                    break;
                case Type::INT :
                    intValues.pop_back();
                    break;
                case Type::REAL :
                    realValues.pop_back();
                    break;
                case Type::STRING :
                    stringValues.pop_back();
                    break;
            }
        }
        // set the global variable back to the null ptr
        functionParameters = nullptr;
    }

    // loop through every statement in the function's body,
    for (auto const &statement : *astNode->astBlock) {
        statement->accept(this);

        if (hasReturnStatement) {
            break;
        }
    }

    // remove the current scope from the symbol table since the program is exiting from it
    symbolTableStack.pop_back();
}


void InterpreterExecution::visit(ASTFormalParameters *astNode) {
}

void InterpreterExecution::visit(ASTFormalParameter *astNode) {
}



void InterpreterExecution::visit(ASTBinaryExpr *astNode) {
    // visit both expressions, starting with the second, such that the first will be at the end of the vector of values
    astNode->simpleExpression2->accept(this);
    astNode->simpleExpression->accept(this);

    switch (astNode->anOperator->anOperator) {
        case Operator::MULTIPLICATION :
            if (typeOfCurrentVariable == Type::INT) {
                // set the first operand as the value at the end of the vector of integer values
                int op1 = intValues.back();
                // pop that value as it is no longer needed
                intValues.pop_back();
                // set the second operand as the value that is now at the end of the vector of integer values
                int op2 = intValues.back();
                // pop that value as it is no longer needed
                intValues.pop_back();
                // push the result of the operation to the vector of integer values
                intValues.push_back(op1 * op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                realValues.push_back(op1 * op2);
            }
            break;
        case Operator::DIVISION :
            if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                intValues.push_back(op1 / op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                realValues.push_back(op1 / op2);
            }
            break;
        case Operator::AND :
            // when the operator is an AND, after the semantic analysis pass, the type will always be Bool,
            // the if condition was placed so that the bool variables would be contained within their own scope
            if (typeOfCurrentVariable == Type::BOOL) {
                bool op1 = boolValues.back();
                boolValues.pop_back();
                bool op2 = boolValues.back();
                boolValues.pop_back();
                boolValues.push_back(op1 && op2);
            }
            break;
        case Operator::ADDITION :
            if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                intValues.push_back(op1 + op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                realValues.push_back(op1 + op2);
            }
            break;
        case Operator::SUBTRACTION :
            if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                intValues.push_back(op1 - op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                realValues.push_back(op1 - op2);
            }
            break;
        case Operator::OR :
            if (typeOfCurrentVariable == Type::BOOL) {
                bool op1 = boolValues.back();
                boolValues.pop_back();
                bool op2 = boolValues.back();
                boolValues.pop_back();
                boolValues.push_back(op1 || op2);
            }
            break;
        case Operator::LESSTHAN :
            if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                boolValues.push_back(op1 < op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                boolValues.push_back(op1 < op2);
            }
            // since the expressions were of type Int or Real, and the result is of type Bool,
            // store the change inside typeOfCurrentVariable
            typeOfCurrentVariable = Type::BOOL;
            break;
        case Operator::GREATERTHAN :
            if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                boolValues.push_back(op1 > op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                boolValues.push_back(op1 > op2);
            }
            typeOfCurrentVariable = Type::BOOL;
            break;
        case Operator::EQUALTO :
            if (typeOfCurrentVariable == Type::BOOL) {
                bool op1 = boolValues.back();
                boolValues.pop_back();
                bool op2 = boolValues.back();
                boolValues.pop_back();
                boolValues.push_back(op1 == op2);
            } else if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                boolValues.push_back(op1 == op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                boolValues.push_back(op1 == op2);
            } else if (typeOfCurrentVariable == Type::STRING) {
                string op1 = stringValues.back();
                stringValues.pop_back();
                string op2 = stringValues.back();
                stringValues.pop_back();
                boolValues.push_back(op1 == op2);
            }
            typeOfCurrentVariable = Type::BOOL;
            break;
        case Operator::NOTEQUALTO :
            if (typeOfCurrentVariable == Type::BOOL) {
                bool op1 = boolValues.back();
                boolValues.pop_back();
                bool op2 = boolValues.back();
                boolValues.pop_back();
                boolValues.push_back(op1 != op2);
            } else if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                boolValues.push_back(op1 != op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                boolValues.push_back(op1 != op2);
            } else if (typeOfCurrentVariable == Type::STRING) {
                string op1 = stringValues.back();
                stringValues.pop_back();
                string op2 = stringValues.back();
                stringValues.pop_back();
                boolValues.push_back(op1 != op2);
            }
            typeOfCurrentVariable = Type::BOOL;
            break;
        case Operator::LESSTHANOREQUALTO :
            if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                boolValues.push_back(op1 <= op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                boolValues.push_back(op1 <= op2);
            }
            typeOfCurrentVariable = Type::BOOL;
            break;
        case Operator::GREATERTHANOREQUALTO :
            if (typeOfCurrentVariable == Type::INT) {
                int op1 = intValues.back();
                intValues.pop_back();
                int op2 = intValues.back();
                intValues.pop_back();
                boolValues.push_back(op1 >= op2);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double op1 = realValues.back();
                realValues.pop_back();
                double op2 = realValues.back();
                realValues.pop_back();
                boolValues.push_back(op1 >= op2);
            }
            typeOfCurrentVariable = Type::BOOL;
            break;
    }
}

void InterpreterExecution::visit(ASTOperator *astNode) {
}

void InterpreterExecution::visit(ASTBoolean *astNode) {
    // push the value of the Bool expression to the end of the vector boolValues
    boolValues.push_back(astNode->value);
    // set the type of the current variable to Bool
    typeOfCurrentVariable = Type::BOOL;
}

void InterpreterExecution::visit(ASTInteger *astNode) {
    intValues.push_back(astNode->value);
    typeOfCurrentVariable = Type::INT;
}

void InterpreterExecution::visit(ASTReal *astNode) {
    realValues.push_back(astNode->value);
    typeOfCurrentVariable = Type::REAL;
}

void InterpreterExecution::visit(ASTString *astNode) {
    stringValues.push_back(astNode->value);
    typeOfCurrentVariable = Type::STRING;
}

void InterpreterExecution::visit(ASTIdentifier *astNode) {
    string identifier = astNode->identifier;

    // start looking for the variable with the identifier in the current scope, and if it's not found look in outer scopes
    // until the global scope is reached
    currentSymbolTableScopeIndex = symbolTableStack.size()-1;
    while (currentSymbolTableScopeIndex >= 0) {
        // if the current scope contains the identifier
        if (symbolTableStack.at(currentSymbolTableScopeIndex)->isVariableInSymbolTable(identifier)) {
            // store its type
            typeOfCurrentVariable = symbolTableStack.at(currentSymbolTableScopeIndex)->typeOfVariable(identifier);
            // and break out of the loop
            break;
        }
        // if the variable is not found in this scope, look for it in an outer scope in the next iteration
        currentSymbolTableScopeIndex--;
    }

    // retrieve the variable's properties (which include its value)
    VariableProperties variableProperties = symbolTableStack.at(currentSymbolTableScopeIndex)->getVariablePropertiesOfVariable(identifier);

    // depending on the type of the identifier, push its value to the respective vector
    switch (typeOfCurrentVariable) {
        case Type::BOOL :
            boolValues.push_back(variableProperties.boolValue);
            break;
        case Type::INT :
            intValues.push_back(variableProperties.intValue);
            break;
        case Type::REAL :
            realValues.push_back(variableProperties.realValue);
            break;
        case Type::STRING :
            stringValues.push_back(variableProperties.stringValue);
            break;
    }
}

void InterpreterExecution::visit(ASTType *astNode) {
}

void InterpreterExecution::visit(ASTFunctionCall *astNode) {
    // store the identifier of the function being called
    string identifier = astNode->astIdentifier->identifier;

    // variable which stores a pointer to the ASTBlock of statements that need to be executed when a function is called
    ASTBlock *functionBody;

    bool functionFound = false;

    // look for the function being called in every scope's symbol table, starting from the deepest scope and moving outwards
    unsigned long long int currentSymbolTableScopeIndex = symbolTableStack.size() - 1;
    while (currentSymbolTableScopeIndex >= 0) {
        // if a scope's symbol table contains at least one function with the specified identifier
        if (symbolTableStack.at(currentSymbolTableScopeIndex)->isFunctionIdentifierInSymbolTable(identifier)) {
            // initialise a vector with the variable properties of all the functions with the specified identifier
            vector<VariableProperties> variableProperties =
                    symbolTableStack.at(currentSymbolTableScopeIndex)->getVariablePropertiesOfFunction(identifier);

            // look through all the functions' variable properties
            for (auto const &functionProperties : variableProperties) {
                // set the global variable functionParameters to the formal parameters of the functionProperties
                // this might be changed if this is not the correct function (eg in case of mismatching signature)
                functionParameters = functionProperties.formalParameters;
                // store the pointer to the block of functions of this function from function properties
                functionBody = functionProperties.functionBody;

                // if the number of parameters of the current function found in the symbol table
                // do not match the number of parameters of the function being interpreted
                if (functionProperties.parameterTypes.size()
                    != astNode->astActualParameters->astActualParameters->size()) {
                    // then this is surely not this function that needs to be executed
                    continue;
                }

                // declare a vector which will store the order of the types of all the evaluated parameters
                vector<Type> parametersEvaluated = vector<Type>();

                // loop through every parameter inside the function call
                int i = 0;
                for (auto const &parameter : *astNode->astActualParameters->astActualParameters) {
                    // visit the parameter, to evaluate it and store its type inside typeOfCurrentVariable
                    parameter->accept(this);

                    // store the type of the current parameter inside parametersEvaluated vector
                    parametersEvaluated.push_back(typeOfCurrentVariable);
                    // if the type of the current parameter does not equal the type of the function
                    // found inside the symbol table
                    if (typeOfCurrentVariable != functionProperties.parameterTypes.at(i)) {
                        // remove the evaluated values from their respective vectors
                        for (auto const &paramType : parametersEvaluated) {
                            switch (paramType) {
                                case Type::BOOL :
                                    boolValues.pop_back();
                                    break;
                                case Type::INT :
                                    intValues.pop_back();
                                    break;
                                case Type::REAL :
                                    realValues.pop_back();
                                    break;
                                case Type::STRING :
                                    stringValues.pop_back();
                                    break;
                            }
                        }
                        // set the pointer to the functionParameters back to null
                        functionParameters = nullptr;
                        // break from this function's loop to try other functions found with the same identifier,
                        // in this scope's symbol table, or any other outer scope
                        break;
                    }
                    i++;

                    if (i == astNode->astActualParameters->astActualParameters->size()) {
                        functionFound = true;
                    }
                }

                if (functionFound) {
                    break;
                }
            }
        }

        if (functionFound) {
            break;
        }
        // decrement the scope index so that in the next iteration the function is looked for
        // in the next outer scope's symbol table
        currentSymbolTableScopeIndex--;
    }

    // visit the body of statements of this function and execute them
    functionBody->accept(this);
    // set hasReturnStatement back to false
    hasReturnStatement = false;
}

void InterpreterExecution::visit(ASTActualParameters *astNode) {
}

void InterpreterExecution::visit(ASTSubExpr *astNode) {
    astNode->expression->accept(this);
}

void InterpreterExecution::visit(ASTUnaryExpr *astNode) {
    astNode->expression->accept(this);

    switch (astNode->unaryOperator->unaryOperator) {
        case UnaryOperator::MINUS :
            if (typeOfCurrentVariable == Type::INT) {
                int value = intValues.back();
                intValues.pop_back();
                intValues.push_back(-value);
            } else if (typeOfCurrentVariable == Type::REAL) {
                double value = realValues.back();
                realValues.pop_back();
                realValues.push_back(-value);
            }
            break;
        case UnaryOperator::NOT :
            if (typeOfCurrentVariable == Type::BOOL) {
                bool value = boolValues.back();
                boolValues.pop_back();
                boolValues.push_back(!value);
            }
            break;
    }
}

void InterpreterExecution::visit(ASTUnaryOperator *astNode) {
}
