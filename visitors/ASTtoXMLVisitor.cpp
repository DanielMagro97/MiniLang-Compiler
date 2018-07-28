#include "ASTtoXMLVisitor.h"

ASTtoXMLVisitor::ASTtoXMLVisitor() {
    // when the ASTtoXMLVisitor is initialised, set the indentation to no indentation at all
    indentation = "";
}

void ASTtoXMLVisitor::indent() {
    // add another tab (\t) to the indentation string
    indentation += "\t";
}

void ASTtoXMLVisitor::outdent() {
    // remove the last character (\t) from the indentation string
    indentation.pop_back();
}

void ASTtoXMLVisitor::visitAST(vector<ASTNode *> *ast) {
    // print the opening XML Tag
    cout << "<AbstractSyntaxTree>" << endl;

    // indent before entering the tree
    indent();

    // loop through every node inside the AST
    for (auto const &childNode: *ast) {
        childNode->accept(this);
    }

    // outdent after reading through the tree
    outdent();
    // print the closing XML Tag
    cout << "</AbstractSyntaxTree>" << endl;
}

void ASTtoXMLVisitor::visit(ASTVariableDeclaration *astNode) {
    // print the opening tag and indent
    cout << indentation << "<VariableDeclaration>" << endl;
    indent();

    astNode->astIdentifier->accept(this);
    astNode->astType->accept(this);
    astNode->astExpressionNode->accept(this);

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</VariableDeclaration>" << endl;
}

void ASTtoXMLVisitor::visit(ASTAssignment *astNode) {
    // print the opening tag and indent
    cout << indentation << "<Assignment>" << endl;
    indent();

    astNode->astIdentifier->accept(this);
    astNode->astExpressionNode->accept(this);

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</Assignment>" << endl;
}

void ASTtoXMLVisitor::visit(ASTPrint *astNode) {
    // print the opening tag and indent
    cout << indentation << "<Print>" << endl;
    indent();

    astNode->astExpressionNode->accept(this);

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</Print>" << endl;
}

void ASTtoXMLVisitor::visit(ASTIf *astNode) {
    // print the opening tag and indent
    cout << indentation << "<If>" << endl;
    indent();

    astNode->expression->accept(this);
    astNode->ifBlock->accept(this);
    // if the elseBlock (vector of statements) is not empty
    if (!astNode->elseBlock->astBlock->empty()) {
        astNode->elseBlock->accept(this);
    }

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</If>" << endl;
}

void ASTtoXMLVisitor::visit(ASTWhile *astNode) {
    // print the opening tag and indent
    cout << indentation << "<While>" << endl;
    indent();

    astNode->astExpressionNode->accept(this);
    astNode->astBlock->accept(this);

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</While>" << endl;
}

void ASTtoXMLVisitor::visit(ASTReturn *astNode) {
    // print the opening tag and indent
    cout << indentation << "<Return>" << endl;
    indent();

    astNode->astExpressionNode->accept(this);

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</Return>" << endl;
}

void ASTtoXMLVisitor::visit(ASTFunctionDeclaration *astNode) {
    // print the opening tag and indent
    cout << indentation << "<FunctionDeclaration>" << endl;
    indent();

    astNode->identifier->accept(this);
    astNode->formalParameters->accept(this);
    astNode->block->accept(this);
    astNode->type->accept(this);

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</FunctionDeclaration>" << endl;
}

void ASTtoXMLVisitor::visit(ASTBlock *astNode) {
    // print the opening tag and indent
    cout << indentation << "<Block>" << endl;
    indent();

    // loop through every statement inside the block
    for (auto &statement : *astNode->astBlock) {
        // and visit it
        statement->accept(this);
    }

    // outdent and print the closing tag
    outdent();
    cout << indentation << "</Block>" << endl;
}

void ASTtoXMLVisitor::visit(ASTFormalParameters *astNode) {
    cout << indentation << "<FormalParameters>" << endl;
    indent();
    // loop through every formal parameter
    for (auto &formalParameter : *astNode->formalParameters) {
        // and visit it
        formalParameter->accept(this);
    }
    outdent();
    cout << indentation << "</FormalParameters>" << endl;
}

void ASTtoXMLVisitor::visit(ASTFormalParameter *astNode) {
    cout << indentation << "<FormalParameter>" << endl;
    indent();

    astNode->astIdentifier->accept(this);
    astNode->astType->accept(this);

    outdent();
    cout << indentation << "</FormalParameter>" << endl;
}



void ASTtoXMLVisitor::visit(ASTBinaryExpr *astNode) {
    //cout << indentation << "<BinaryExpression Operator=";
    //cout << "'" << astNode->anOperator->toString() << "'";
    //cout << ">" << endl;
    cout << indentation << "<BinaryExpression>" << endl;
    indent();

    astNode->anOperator->accept(this);
    astNode->simpleExpression->accept(this);
    astNode->simpleExpression2->accept(this);

    outdent();
    cout << indentation << "</BinaryExpression>" << endl;
}

void ASTtoXMLVisitor::visit(ASTOperator *astNode) {
    cout << indentation << "<Operator>";
    cout << "'" << astNode->toString() << "'";
    cout << "</Operator>" << endl;
}

void ASTtoXMLVisitor::visit(ASTBoolean *astNode) {
    cout << indentation << "<BooleanLiteral>";
    cout << astNode->value;
    cout << "</BooleanLiteral>" << endl;
}

void ASTtoXMLVisitor::visit(ASTInteger *astNode) {
    cout << indentation << "<IntegerLiteral>";
    cout << astNode->value;
    cout << "</IntegerLiteral>" << endl;
}

void ASTtoXMLVisitor::visit(ASTReal *astNode) {
    cout << indentation << "<RealLiteral>";
    cout << astNode->value;
    cout << "</RealLiteral>" << endl;
}

void ASTtoXMLVisitor::visit(ASTString *astNode) {
    cout << indentation << "<StringLiteral>";
    cout << astNode->value;
    cout << "</StringLiteral>" << endl;
}

void ASTtoXMLVisitor::visit(ASTIdentifier *astNode) {
    cout << indentation << "<Identifier>";
    cout << astNode->identifier;
    cout << "</Identifier>" << endl;
}

void ASTtoXMLVisitor::visit(ASTType *astNode) {
    cout << indentation << "<Type>";
    cout << astNode->toString();
    cout << "</Type>" << endl;
}

void ASTtoXMLVisitor::visit(ASTFunctionCall *astNode) {
    cout << indentation << "<FunctionCall>" << endl;
    indent();

    astNode->astIdentifier->accept(this);
    astNode->astActualParameters->accept(this);

    outdent();
    cout << indentation << "</FunctionCall>" << endl;
}

void ASTtoXMLVisitor::visit(ASTActualParameters *astNode) {
    cout << indentation << "<ActualParameters>" << endl;
    indent();

    // loop through every actual parameter
    for (auto &actualParameter : *astNode->astActualParameters) {
        // and visit it
        actualParameter->accept(this);
    }

    outdent();
    cout << indentation << "</ActualParameters>" << endl;
}

void ASTtoXMLVisitor::visit(ASTSubExpr *astNode) {
    cout << indentation << "<SubExpression>" << endl;
    indent();

    astNode->expression->accept(this);

    outdent();
    cout << indentation << "</SubExpression>" << endl;
}

void ASTtoXMLVisitor::visit(ASTUnaryExpr *astNode) {
    //cout << indentation << "<UnaryExpression Operator=";
    //cout << "'" << astNode->unaryOperator->toString() << "'";
    //cout << ">" << endl;
    cout << indentation << "<UnaryExpression>";
    indent();

    astNode->unaryOperator->accept(this);
    astNode->expression->accept(this);

    outdent();
    cout << indentation << "</UnaryExpression>" << endl;
}

void ASTtoXMLVisitor::visit(ASTUnaryOperator *astNode) {
    cout << indentation << "<UnaryOperator>";
    cout << "'" << astNode->toString() << "'";
    cout << "</UnaryOperator>" << endl;
}
