#pragma once

#include <map>

#include "VariableProperties.h"

using namespace std;

class SymbolTable {
public:
    // multimap is used since there might be multiple variables with the same name
    // where one may be a variable storing a value
    // and others being function calls with different parameter signatures
    multimap<string, VariableProperties> variables;

    SymbolTable();
    void add(string identifier, VariableProperties variableProperties);
    bool isVariableInSymbolTable(string identifier);
    bool isFunctionInSymbolTable(string identifier, VariableProperties variableProperties);
    // returns the data type of an identifier. primarily used by the semantic analysis visitor of the ASTIdentifier
    Type typeOfVariable(string identifier);
    // returns whether a function (given its identifier and parameters' data types) is in the symbol table
    // primarily used by the semantic analysis visitor of the ASTFunctionCall
    bool areFunctionIdandParamsInSymbolTable(string identifier, vector<Type> actualParameterTypes);
    // returns the data type returned by a function (given its identifier and parameters' data types)
    // primarily used by the semantic analysis visitor of the ASTFunctionCall
    Type typeOfFunction(string identifier, vector<Type> actualParameterTypes);


    // the following are only made use of by the Interpreter Execution Pass
    void setBoolValueOfVariable(string identifier, bool value);
    void setIntValueOfVariable(string identifier, int value);
    void setRealValueOfVariable(string identifier, double value);
    void setStringValueOfVariable(string identifier, string value);
    // method which returns the variable properties of a variable given its identifier
    VariableProperties getVariablePropertiesOfVariable(string identifier);
    // method which returns whether at least one function with the specified identifier exists in the symbol table
    bool isFunctionIdentifierInSymbolTable(string identifier);
    // method which returns a vector with the variable properties of all the functions in the symbol table
    // with the specified identifier
    vector<VariableProperties> getVariablePropertiesOfFunction(string identifier);
};
