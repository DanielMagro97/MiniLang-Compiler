#include "SymbolTable.h"

SymbolTable::SymbolTable() {
    variables = multimap<string, VariableProperties>();
}

void SymbolTable::add(string identifier, VariableProperties variableProperties) {
    variables.insert(pair<string,VariableProperties>(identifier,variableProperties));
}

bool SymbolTable::isVariableInSymbolTable(string identifier) {
    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the variable, in which case it will return true)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the variable identifier
        // and if the variable types are the same (i.e. both are variables or both are functions)
        if (iterator->first == identifier && iterator->second.variableType == VariableType::VARIABLE) {
            // then the variable is in the symbol table
            return true;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }

    return false;
}

bool SymbolTable::isFunctionInSymbolTable(string identifier, VariableProperties variableProperties) {
    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the function, in which case it will return true)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the variable identifier
        if (iterator->first == identifier) {
            // if the value types of the variables are the same
            if (iterator->second.valueType == variableProperties.valueType
                // and the variable types are the same (i.e. both functions)
                    && iterator->second.variableType == variableProperties.variableType
                // and the function call signature is the same
                    && iterator->second.parameterTypes == variableProperties.parameterTypes) {

                // if the both the identifier and properties of the function are the same
                // then the function is in the symbol table
                return true;
            }
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }

    return false;
}

#include <assert.h>
Type SymbolTable::typeOfVariable(string identifier) {
    // the variable being looked for must be in the symbol table
    assert(isVariableInSymbolTable(identifier));

    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the variable, in which case it will return its type)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the variable identifier
        // and if the variable type of the identifier in the map is 'VARIABLE'
        if (iterator->first == identifier && iterator->second.variableType == VariableType::VARIABLE) {
            // then return that variable's data type
            return iterator->second.valueType;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }
}

bool SymbolTable::areFunctionIdandParamsInSymbolTable(string identifier, vector<Type> actualParameterTypes) {
    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the function, in which case it will return true)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the function identifier
        if (iterator->first == identifier
            // and if the variable type of the identifier in the map is 'FUNCTION'
            && iterator->second.variableType == VariableType::FUNCTION
            // and the function's parameters' data types match
            && iterator->second.parameterTypes == actualParameterTypes) {

            // then return true (meaning the function was found)
            return true;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }

    return false;
}

Type SymbolTable::typeOfFunction(string identifier, vector<Type> actualParameterTypes) {
    // make sure that a function with the identifier and method signature passed exists
    assert(areFunctionIdandParamsInSymbolTable(identifier, actualParameterTypes));

    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the function, in which case it will return its type)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the function identifier
        if (iterator->first == identifier
            // and if the variable type of the identifier in the map is 'FUNCTION'
            && iterator->second.variableType == VariableType::FUNCTION
            // and the function's parameters' data types match
            && iterator->second.parameterTypes == actualParameterTypes) {

            // then return that function's data type
            return iterator->second.valueType;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }
}

void SymbolTable::setBoolValueOfVariable(string identifier, bool value) {
    // the variable being looked for must be in the symbol table
    assert(isVariableInSymbolTable(identifier));

    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the variable, in which case it will return its type)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the variable identifier
        // and if the variable type of the identifier in the map is 'VARIABLE'
        if (iterator->first == identifier && iterator->second.variableType == VariableType::VARIABLE) {
            // set the Bool value of this identifier in the symbol table's variableProperties struct
            iterator->second.boolValue = value;
            // return execution since the variable's value has been set
            return;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }
}

void SymbolTable::setIntValueOfVariable(string identifier, int value) {
// the variable being looked for must be in the symbol table
    assert(isVariableInSymbolTable(identifier));

    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the variable, in which case it will return its type)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the variable identifier
        // and if the variable type of the identifier in the map is 'VARIABLE'
        if (iterator->first == identifier && iterator->second.variableType == VariableType::VARIABLE) {
            // set the Int value of this identifier in the symbol table's variableProperties struct
            iterator->second.intValue = value;
            // return execution since the variable's value has been set
            return;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }
}

void SymbolTable::setRealValueOfVariable(string identifier, double value) {
// the variable being looked for must be in the symbol table
    assert(isVariableInSymbolTable(identifier));

    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the variable, in which case it will return its type)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the variable identifier
        // and if the variable type of the identifier in the map is 'VARIABLE'
        if (iterator->first == identifier && iterator->second.variableType == VariableType::VARIABLE) {
            // set the Real value of this identifier in the symbol table's variableProperties struct
            iterator->second.realValue = value;
            // return execution since the variable's value has been set
            return;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }
}

void SymbolTable::setStringValueOfVariable(string identifier, string value) {
    // the variable being looked for must be in the symbol table
    assert(isVariableInSymbolTable(identifier));

    // set the iterator to the beginning of the map
    auto iterator = variables.begin();
    // while the iterator hasn't reached the end of the map (or found the variable, in which case it will return its type)
    // it will keep looking
    while (iterator != variables.end()) {
        // if the key value matches the variable identifier
        // and if the variable type of the identifier in the map is 'VARIABLE'
        if (iterator->first == identifier && iterator->second.variableType == VariableType::VARIABLE) {
            // set the String value of this identifier in the symbol table's variableProperties struct
            iterator->second.stringValue = value;
            // return execution since the variable's value has been set
            return;
        }

        // increment the iterator to look at the next entry in the map
        iterator++;
    }
}

VariableProperties SymbolTable::getVariablePropertiesOfVariable(string identifier) {
    assert(isVariableInSymbolTable(identifier));

    auto iterator = variables.begin();
    while (iterator != variables.end()) {
        if (iterator->first == identifier
            && iterator->second.variableType == VariableType::VARIABLE) {

            return iterator->second;
        }

        iterator++;
    }
}

bool SymbolTable::isFunctionIdentifierInSymbolTable(string identifier) {
    auto iterator = variables.begin();

    while (iterator != variables.end()) {
        if (iterator->first == identifier
            && iterator->second.variableType == VariableType::FUNCTION) {

            return true;
        }

        iterator++;
    }

    return false;
}

vector<VariableProperties> SymbolTable::getVariablePropertiesOfFunction(string identifier) {
    assert(isFunctionIdentifierInSymbolTable(identifier));

    vector<VariableProperties> functionProperties = vector<VariableProperties>();

    auto iterator = variables.begin();
    while (iterator != variables.end()) {
        if (iterator->first == identifier
            && iterator->second.variableType == VariableType::FUNCTION) {

            functionProperties.push_back(iterator->second);
        }

        iterator++;
    }

    return functionProperties;
}
