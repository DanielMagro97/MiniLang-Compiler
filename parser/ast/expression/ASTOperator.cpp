#include "ASTOperator.h"

ASTOperator::ASTOperator(Operator anOperator) {
    this->anOperator = anOperator;
}

ASTOperator::~ASTOperator() = default;

#include "../../../visitors/Visitor.h"
void ASTOperator::accept(Visitor *visitor) {
    visitor->visit(this);
}

std::string ASTOperator::toString() {
    if (anOperator == Operator::MULTIPLICATION) {
        return "*";
    } else if (anOperator == Operator::DIVISION) {
        return "/";
    } else if (anOperator == Operator::AND) {
        return "AND";
    } else if (anOperator == Operator::ADDITION) {
        return "+";
    } else if (anOperator == Operator::SUBTRACTION) {
        return "-";
    } else if (anOperator == Operator::OR) {
        return "OR";
    } else if (anOperator == Operator::LESSTHAN) {
        return "<";
    } else if (anOperator == Operator::GREATERTHAN) {
        return ">";
    } else if (anOperator == Operator::EQUALTO) {
        return "==";
    } else if (anOperator == Operator::NOTEQUALTO) {
        return "!=";
    } else if (anOperator == Operator::LESSTHANOREQUALTO) {
        return "<=";
    } else if (anOperator == Operator::GREATERTHANOREQUALTO) {
        return ">=";
    }
}
