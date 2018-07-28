#pragma once

#include <vector>
#include <iostream>
#include "../../parser/ast/expression/ASTType.h"
// the following include statements are made use of during the interpreter execution pass
#include "../../parser/ast/statement/ASTFormalParameters.h"
#include "../../parser/ast/statement/ASTBlock.h"

enum class VariableType {
    VARIABLE,
    FUNCTION
};

class VariableProperties {
public:
    // storing whether the variable is of type Bool, Int, Real or String
    Type valueType;
    // storing whether the variable is a variable (value) or a function call
    VariableType variableType;
    // if the identifier refers to a function, then the method signature is stored (as the data type of the parameters)
    std::vector<Type> parameterTypes;


    VariableProperties(Type valueType, VariableType variableType) {
        this->valueType = valueType;
        this->variableType = variableType;
        // set the vector of parameter types as an empty vector
        this->parameterTypes = std::vector<Type>();
    }

    VariableProperties(Type valueType, VariableType variableType, std::vector<Type> parameterTypes) {
        this->valueType = valueType;
        this->variableType = variableType;
        this->parameterTypes = parameterTypes;
    }

    // the following are only made use of by the Interpreter Execution Pass

    // variables which will store the value according to the identifier's data type
    // (only one should ever be used for each instance of Variable Properties)
    bool boolValue;
    int intValue;
    double realValue;
    std::string stringValue;

    // variables used to store information about a function
    // stores the parameters of a function
    ASTFormalParameters *formalParameters;
    // stores the statements inside the body of a function
    ASTBlock *functionBody;


    std::string toString() {
        std::string stringVP;

        if (valueType == Type::BOOL) {
            stringVP += "Bool";
        } else if (valueType == Type::INT) {
            stringVP += "Int";
        } else if (valueType == Type::REAL) {
            stringVP += "Real";
        } else if (valueType == Type::STRING) {
            stringVP += "String";
        }

        stringVP += " ";

        if (variableType == VariableType::VARIABLE) {
            stringVP += "Variable";
        } else if (variableType == VariableType::FUNCTION) {
            stringVP += "Function";

            stringVP += "\t(";
            for (int i = 0; i < parameterTypes.size(); i++) {
                if (parameterTypes.at(i) == Type::BOOL) {
                    stringVP += "Bool";
                } else if (parameterTypes.at(i) == Type::INT) {
                    stringVP +=  "Int";
                } else if (parameterTypes.at(i) == Type::REAL) {
                    stringVP +=  "Real";
                } else if (parameterTypes.at(i) == Type::STRING) {
                    stringVP +=  "String";
                }

                if (i != parameterTypes.size()-1) {
                    stringVP += ", ";
                }

            }
            stringVP += ")";
        }

        return stringVP;
    }

};
