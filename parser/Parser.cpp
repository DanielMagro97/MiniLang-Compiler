#include "Parser.h"

Parser::Parser() {
    ast = new vector<ASTNode*>();
}

Parser::~Parser() {
    ast->clear();
}

using namespace std;

vector<ASTNode*> *Parser::parse() {
    while (Lexer::peekNextToken().tokenIdentifier != TokenIdentifier::TOK_EOF) {
        // retrieve the next token from the Lexer class and set it as the currentToken
        currentToken = Lexer::getNextToken();
        // parse the statement which begins with the current token and add it to the vector of ASTNodes
        ast->push_back(parseStatement());
    }

    return ast;
}

ASTStatementNode *Parser::parseStatement() {
    if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "var") {
        // and call the parseVariableDeclarationStatement which will parse this statement,
        // which is a variable declaration statement (since it starts with var)
        return parseVariableDeclarationStatement();
    } else if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "set") {
        return parseAssignmentStatement();
    } else if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "print") {
        return parsePrintStatement();
    } else if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "if") {
        return parseIfStatement();
    } else if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "while") {
        return parseWhileStatement();
    } else if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "return") {
        return parseReturnStatement();
    } else if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "def") {
        return parseFunctionDeclarationStatement();
    } else if (currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == "{") {
        return parseBlock();
    } else {
        cout << "statement expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}



// Statements
ASTVariableDeclaration *Parser::parseVariableDeclarationStatement() {
    ASTIdentifier *identifier;
    ASTType *type;
    ASTExpressionNode *expression;

    // since the keyword token storing "var" has been consumed, move on to the next
    currentToken = Lexer::getNextToken();
    identifier = parseIdentifier();

    currentToken = Lexer::getNextToken();
    // if the next token is not a semicolon, throw an error and exit the program
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ":") ) {
        cout << "semicolon ':' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    type = parseType();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Equals) ) {
        cout << "equals '=' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    expression = parseExpression();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ";") ) {
        cout << "semicolon ';' expected at the end of line " << Lexer::lineNumber << endl;
        exit(1);
    }

    return new ASTVariableDeclaration(identifier, type, expression);
}

ASTAssignment *Parser::parseAssignmentStatement() {
    ASTIdentifier *identifier;
    ASTExpressionNode *expression;

    // since the keyword token storing "set" has been consumed, move on to the next
    currentToken = Lexer::getNextToken();
    identifier = parseIdentifier();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Equals) ) {
        cout << "equals '=' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    expression = parseExpression();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ";") ) {
        cout << "semicolon ';' expected at the end of line " << Lexer::lineNumber << endl;
        exit(1);
    }

    return new ASTAssignment(identifier, expression);
}

ASTPrint *Parser::parsePrintStatement() {
    ASTExpressionNode *expression;

    currentToken = Lexer::getNextToken();
    expression = parseExpression();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ";") ) {
        cout << "semicolon ';' expected at the end of line " << Lexer::lineNumber << endl;
        exit(1);
    }

    return new ASTPrint(expression);
}

ASTIf *Parser::parseIfStatement() {
    ASTExpressionNode *expression;
    ASTBlock *ifBlock;
    ASTBlock *elseBlock;

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == "(") ) {
        cout << "open bracket '(' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    expression = parseExpression();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ")") ) {
        cout << "closed bracket ')' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    ifBlock = parseBlock();

    // if there is no "else" keyword after the 'if' block, then return the if statement with an empty else block
    if ( !(Lexer::peekNextToken().lexeme == "else") ) {
        elseBlock = new ASTBlock(new vector<ASTStatementNode*>);
        return new ASTIf(expression, ifBlock, elseBlock);
    }

    // consume the else token
    Lexer::getNextToken();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == "{") ) {
        cout << "open curly bracket '{' expected after else at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    elseBlock = parseBlock();

    return new ASTIf(expression, ifBlock, elseBlock);
}

ASTWhile *Parser::parseWhileStatement() {
    ASTExpressionNode *expression;
    ASTBlock *block;

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == "(") ) {
        cout << "open bracket '(' expected after 'while' at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    expression = parseExpression();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ")") ) {
        cout << "closed bracket ')' expected after expression at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == "{") ) {
        cout << "open curly bracket '{' expected before block at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    block = parseBlock();

    return new ASTWhile(expression, block);
}

ASTReturn *Parser::parseReturnStatement() {
    ASTExpressionNode *expression;

    currentToken = Lexer::getNextToken();
    expression = parseExpression();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ";") ) {
        cout << "semicolon ';' expected at the end of line " << Lexer::lineNumber << endl;
        exit(1);
    }

    return new ASTReturn(expression);
}

ASTFunctionDeclaration *Parser::parseFunctionDeclarationStatement() {
    ASTIdentifier *identifier;
    ASTFormalParameters *formalParameters;
    ASTType *type;
    ASTBlock *block;

    currentToken = Lexer::getNextToken();
    identifier = parseIdentifier();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == "(") ) {
        cout << "open bracket '(' expected after identifier at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    vector<ASTFormalParameter*> *formalParams = new vector<ASTFormalParameter*>;
    currentToken = Lexer::getNextToken();
    // if the next token is not a closed bracket ')', then there are formal parameters
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ")") ) {
        parseFormalParameters(formalParams);

        currentToken = Lexer::getNextToken();
        if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ")") ) {
            cout << "closed bracket ')' expected after formal parameters at line " << Lexer::lineNumber << endl;
            exit(1);
        }
    }
    formalParameters = new ASTFormalParameters(formalParams);

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ":") ) {
        cout << "colon ':' expected after closed bracket at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    currentToken = Lexer::getNextToken();
    type = parseType();

    currentToken = Lexer::getNextToken();
    block = parseBlock();

    return new ASTFunctionDeclaration(identifier, formalParameters, type, block);
}

ASTBlock *Parser::parseBlock() {
    vector<ASTStatementNode*> *block = new vector<ASTStatementNode*>;

    // moving on to the next token since the open curly bracket '{' has been consumed
    currentToken = Lexer::getNextToken();

    // statement count is kept since if more than one statement in a block is read,
    // at the end of each statement the next block must be fetched
    int statementCount = 0;
    while (Lexer::peekNextToken().lexeme != "}") {
        if (statementCount > 0) {
            currentToken = Lexer::getNextToken();
        }
        block->push_back(parseStatement());
        statementCount++;
    }

    // consuming the closed curly bracket '}' at the end of the block.
    // This will always be a '}' since the loop will only stop when one is found
    Lexer::getNextToken();

    return new ASTBlock(block);
}



// Expressions
ASTExpressionNode *Parser::parseExpression() {
    ASTExpressionNode *simpleExpression;
    simpleExpression = parseSimpleExpression();

    if (Lexer::peekNextToken().tokenIdentifier == TOK_RelationalOp) {
        // getNextToken is used so that the RelationalOperator token is now the current token
        currentToken = Lexer::getNextToken();
        ASTOperator *anOperator;
        anOperator = parseOperator();

        currentToken = Lexer::getNextToken();
        ASTExpressionNode *simpleExpression2;
        simpleExpression2 = parseExpression();

        return new ASTBinaryExpr(simpleExpression, anOperator, simpleExpression2);
    } else {
        return simpleExpression;
    }
}

ASTExpressionNode *Parser::parseSimpleExpression() {
    ASTExpressionNode *term;
    term = parseTerm();

    if (Lexer::peekNextToken().tokenIdentifier == TOK_AdditiveOp) {
        // getNextToken is used so that the AdditiveOperator token is now the current token
        currentToken = Lexer::getNextToken();
        ASTOperator *anOperator;
        anOperator = parseOperator();

        currentToken = Lexer::getNextToken();
        ASTExpressionNode *term2;
        term2 = parseSimpleExpression();

        return new ASTBinaryExpr(term, anOperator, term2);
    } else {
        return term;
    }
}

ASTExpressionNode *Parser::parseTerm() {
    ASTExpressionNode *factor;
    factor = parseFactor();

    if (Lexer::peekNextToken().tokenIdentifier == TOK_MultiplicativeOp) {
        // getNextToken is used so that the MultiplicativeOperator token is now the current token
        currentToken = Lexer::getNextToken();
        ASTOperator *anOperator;
        anOperator = parseOperator();

        currentToken = Lexer::getNextToken();
        ASTExpressionNode *factor2;
        factor2 = parseTerm();

        return new ASTBinaryExpr(factor, anOperator, factor2);
    } else {
        return factor;
    }
}

ASTExpressionNode *Parser::parseFactor() {
    // if the current token is a literal
    if (currentToken.tokenIdentifier == TOK_BooleanLiteral || currentToken.tokenIdentifier == TOK_IntegerLiteral
            || currentToken.tokenIdentifier == TOK_RealLiteral || currentToken.tokenIdentifier == TOK_StringLiteral) {
        return parseLiteral();
    } else if (currentToken.tokenIdentifier == TOK_Identifier) {
        if (Lexer::peekNextToken().lexeme == "(") {
            return parseFunctionCall();
        } else {
            return parseIdentifier();
        }
    }  else if (currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == "(") {
        return parseSubExpression();
    } else if ( (currentToken.tokenIdentifier == TOK_AdditiveOp && currentToken.lexeme == "-")
            || (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "not") ) {
        return parseUnary();
    } else {
        cout << "factor expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}


ASTLiteral *Parser::parseLiteral() {
    if (currentToken.tokenIdentifier == TOK_BooleanLiteral) {
        if (currentToken.lexeme == "true") {
            return new ASTBoolean(true);
        } else if (currentToken.lexeme == "false") {
            return new ASTBoolean(false);
        } else {
            cout << "boolean literal expected at line " << Lexer::lineNumber << endl;
            exit(1);
        }
    } else if (currentToken.tokenIdentifier == TOK_IntegerLiteral) {
        return new ASTInteger(stoi(currentToken.lexeme));
    } else if (currentToken.tokenIdentifier == TOK_RealLiteral) {
        return new ASTReal(stof(currentToken.lexeme));
    } else if (currentToken.tokenIdentifier == TOK_StringLiteral) {
        return new ASTString(currentToken.lexeme);
    } else {
        cout << "literal expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}

ASTIdentifier *Parser::parseIdentifier() {
    if (currentToken.tokenIdentifier == TOK_Identifier) {
        return new ASTIdentifier(currentToken.lexeme);
    } else {
        cout << "identifier expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}

ASTFunctionCall *Parser::parseFunctionCall() {
    ASTIdentifier *identifier;
    identifier = parseIdentifier();

    currentToken = Lexer::getNextToken();
    if (currentToken.lexeme != "(") {
        cout << "open bracket '(' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    ASTActualParameters *actualParameters;
    vector<ASTExpressionNode*> *actualParams = new vector<ASTExpressionNode*>;
    currentToken = Lexer::getNextToken();
    // if the next token is not a ), then there are parameters which need to be parsed
    if (!(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ")") ) {
        parseActualParameters(actualParams);

        currentToken = Lexer::getNextToken();
        if (!(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ")") ) {
            cout << "closed bracket ')' expected after actual parameters at line " << Lexer::lineNumber << endl;
            exit(1);
        }
    }
    actualParameters = new ASTActualParameters(actualParams);

    return new ASTFunctionCall(identifier, actualParameters);
}

ASTActualParameters *Parser::parseActualParameters(vector<ASTExpressionNode*> *actualParams) {
    // parse the actual parameter, and add it to the vector of actual parameters
    actualParams->push_back(parseExpression());

    // if a comma is found this indicates that there are actual parameters which still need to be parsed
    if (Lexer::peekNextToken().lexeme == ",") {
        // consume the comma token
        Lexer::getNextToken();

        if (Lexer::peekNextToken().lexeme == ")") {
            cout << "actual parameter expected after comma ',' at line " << Lexer::lineNumber << endl;
            exit(1);
        }

        // fetch the next token so that the current token is the next actual parameter
        currentToken = Lexer::getNextToken();
        // call this method again to parse the rest of the actual parameters, if any
        parseActualParameters(actualParams);
    }
}

ASTSubExpr *Parser::parseSubExpression() {
    currentToken = Lexer::getNextToken();
    ASTExpressionNode *expression;
    expression = parseExpression();

    currentToken = Lexer::getNextToken();
    if ( !(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ")") ) {
        cout << "closed bracket ')' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }

    return new ASTSubExpr(expression);
}

ASTUnaryExpr *Parser::parseUnary() {
    ASTUnaryOperator *unaryOperator;
    unaryOperator = parseUnaryOperator();

    currentToken = Lexer::getNextToken();
    ASTExpressionNode *expression;
    expression = parseExpression();

    return new ASTUnaryExpr(unaryOperator, expression);
}

ASTUnaryOperator *Parser::parseUnaryOperator() {
    if (currentToken.tokenIdentifier == TOK_AdditiveOp && currentToken.lexeme == "-") {
        return new ASTUnaryOperator(UnaryOperator::MINUS);
    } else if (currentToken.tokenIdentifier == TOK_Keyword && currentToken.lexeme == "not") {
        return new ASTUnaryOperator(UnaryOperator::NOT);
    } else {
        cout << "unary operator '-' or 'not' expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}

ASTOperator *Parser::parseOperator() {
    if (currentToken.tokenIdentifier == TOK_MultiplicativeOp && currentToken.lexeme == "*") {
        return new ASTOperator(Operator::MULTIPLICATION);
    } else if (currentToken.tokenIdentifier == TOK_MultiplicativeOp && currentToken.lexeme == "/") {
        return new ASTOperator(Operator::DIVISION);
    } else if (currentToken.tokenIdentifier == TOK_MultiplicativeOp && currentToken.lexeme == "and") {
        return new ASTOperator(Operator::AND);
    } else if (currentToken.tokenIdentifier == TOK_AdditiveOp && currentToken.lexeme == "+") {
        return new ASTOperator(Operator::ADDITION);
    } else if (currentToken.tokenIdentifier == TOK_AdditiveOp && currentToken.lexeme == "-") {
        return new ASTOperator(Operator::SUBTRACTION);
    } else if (currentToken.tokenIdentifier == TOK_AdditiveOp && currentToken.lexeme == "or") {
        return new ASTOperator(Operator::OR);
    } else if (currentToken.tokenIdentifier == TOK_RelationalOp && currentToken.lexeme == "<") {
        return new ASTOperator(Operator::LESSTHAN);
    } else if (currentToken.tokenIdentifier == TOK_RelationalOp && currentToken.lexeme == ">") {
        return new ASTOperator(Operator::GREATERTHAN);
    } else if (currentToken.tokenIdentifier == TOK_RelationalOp && currentToken.lexeme == "==") {
        return new ASTOperator(Operator::EQUALTO);
    } else if (currentToken.tokenIdentifier == TOK_RelationalOp && currentToken.lexeme == "!=") {
        return new ASTOperator(Operator::NOTEQUALTO);
    } else if (currentToken.tokenIdentifier == TOK_RelationalOp && currentToken.lexeme == "<=") {
        return new ASTOperator(Operator::LESSTHANOREQUALTO);
    } else if (currentToken.tokenIdentifier == TOK_RelationalOp && currentToken.lexeme == ">=") {
        return new ASTOperator(Operator::GREATERTHANOREQUALTO);
    } else {
        cout << "operator expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}

ASTType *Parser::parseType() {
    if (currentToken.tokenIdentifier == TOK_Keyword) {
        if (currentToken.lexeme == "real") {
            return new ASTType(Type::REAL);
        } else if (currentToken.lexeme == "int") {
            return new ASTType(Type::INT);
        } else if (currentToken.lexeme == "bool") {
            return new ASTType(Type::BOOL);
        } else if (currentToken.lexeme == "string") {
            return new ASTType(Type::STRING);
        } else {
            cout << "type expected at line " << Lexer::lineNumber << endl;
            exit(1);
        }
    } else {
        cout << "type expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}

ASTFormalParameters *Parser::parseFormalParameters(vector<ASTFormalParameter*> *formalParams) {
    // parse the formal parameter, and add it to the vector of formal parameters
    formalParams->push_back(parseFormalParameter());

    // if a comma is found this indicates that there are formal parameters which still need to be parsed
    if (Lexer::peekNextToken().lexeme == ",") {
        // consume the comma token
        Lexer::getNextToken();

        if (Lexer::peekNextToken().lexeme == ")") {
            cout << "formal parameter expected after comma ',' at line " << Lexer::lineNumber << endl;
            exit(1);
        }

        // fetch the next token so that the current token is the next formal parameter
        currentToken = Lexer::getNextToken();
        // call this method again to parse the rest of the formal parameters, if any
        parseFormalParameters(formalParams);
    }
}

ASTFormalParameter *Parser::parseFormalParameter() {
    ASTIdentifier *identifier;
    ASTType *type;

    if (currentToken.tokenIdentifier == TOK_Identifier) {
        identifier = parseIdentifier();

        currentToken = Lexer::getNextToken();
        if (!(currentToken.tokenIdentifier == TOK_Punctuation && currentToken.lexeme == ":") ) {
            cout << "colon ':' expected at line " << Lexer::lineNumber << endl;
            exit(1);
        }

        currentToken = Lexer::getNextToken();
        type = parseType();

        return new ASTFormalParameter(identifier, type);
    } else {
        cout << "identifier expected at line " << Lexer::lineNumber << endl;
        exit(1);
    }
}
