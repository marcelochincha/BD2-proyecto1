#include "parser.hxx"

#include <iostream>

//
// Token
//
const char* Token::token_name[14] = {"CREATE", "SELECT", "INSERT", "DELETE",    "ID",  "NUM",
                                     "STRING", "COMMA",  "EQ",     "SEMICOLON", "END", "ERR" ,"ANY", "TABLE"};

Token::Token(Type type, std::string value) : type(type), lexema(value) {}
Token::Token(Type type) : type(type) {}

//
// Scanner
//

Scanner::Scanner(std::string input) : input(input), first(0), current(0) {
    reserved["TABLE"] = Token::TABLE;
    reserved["CREATE"] = Token::CREATE;
    reserved["SELECT"] = Token::SELECT;
    reserved["INSERT"] = Token::INSERT;
    reserved["DELETE"] = Token::DELETE;
    reserved["STRING"] = Token::STRING;
    reserved[","] = Token::COMMA;
    reserved["="] = Token::EQ;
    reserved[";"] = Token::SEMICOLON;
}

Scanner::~Scanner() {}

Token* Scanner::nextToken() {
    Token* token;
    char c;
    // Consumir espacios y saltos de linea
    c = nextChar();
    while (c == ' ' || c == '\n') c = nextChar();
    if (c == '\0') return new Token(Token::END);

    // Empezar a leer lexema
    startLexema();
    if (isdigit(c)) {
        c = nextChar();
        while (isdigit(c)) c = nextChar();
        rollBack();
        token = new Token(Token::NUM, getLexema());
    } else if (isalpha(c)) {
        c = nextChar();
        while (isalpha(c)) c = nextChar();
        rollBack();
        std::string lex = getLexema();
        Token::Type ttype = checkReserved(lex);
        if (ttype != Token::ERR) token = new Token(ttype);
        else token = new Token(Token::ID, getLexema());
    } else if (c == '"')
    {
        c = nextChar();
        while (c != '"') c = nextChar();
        token = new Token(Token::STRING, getLexema());
    } 
    else if (strchr(";=,*", c)) {
        switch (c) {
            case '=':
                token = new Token(Token::EQ);
                break;
            case ';':
                token = new Token(Token::SEMICOLON);
                break;
            case ',':
                token = new Token(Token::COMMA);
                break;
            case '*':
                token = new Token(Token::ANY);
                break;
            default:
                std::cout << "No deberia llegar aca" << std::endl;
        }
    } else {
        token = new Token(Token::ERR, getLexema());
    }
    return token;
}

char Scanner::nextChar() {
    if (current >= input.size()) return '\0';
    return input[current++];
}

void Scanner::rollBack() {
    current--;
}

void Scanner::startLexema() {
    first = current - 1;
}

std::string Scanner::getLexema() {
    return input.substr(first, current - first);
}

Token::Type Scanner::checkReserved(std::string lex) {
    // Iterate over reserved words in HIGHERCASE
    for (auto& c : lex) c = toupper(c);
    if (reserved.find(lex) != reserved.end()) return reserved[lex];
}

//
// Parser
//

Parser::Parser(Scanner* scanner) : scanner(scanner), current(nullptr), previous(nullptr) {}
Parser::~Parser() {}

bool Parser::match(Token::Type ttype) {
    if (current->type == ttype) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    return current->type == ttype;
}

bool Parser::advance() {
  if (!isAtEnd()) {
    Token* temp = current;
    if (previous) delete previous;
    current = scanner->nextToken();
    previous = temp;
    if (check(Token::ERR)) {
      std::cerr << "syntax error." << std::endl;
      exit(0);
    }
    return true;
  }
  return false;
} 

bool Parser::isAtEnd() {
    return current->type == Token::END;
}

void Parser::parse() {
    current = this->scanner->nextToken();
    if (check(Token::ERR)) {
        std::cerr << "syntax error." << std::endl;
        exit(0);
    }
    parseProgram();
}


void Parser::parseProgram() {
    if(match(Token::CREATE)){
        parseCreate();
    }else if (match(Token::SELECT)){
        parseSelect();
    }else if (match(Token::INSERT)){
        parseInsert();
    }else if (match(Token::DELETE)){
        parseDelete();
    }else{
        std::cerr << "syntax error: " << current->lexema  << "unkown command" << std::endl;
        exit(0);
    }
}

void Parser::parseCreate(){
    //create table Customer from file “C:\data.csv” using index hash(“DNI”) 
    if(!match(Token::ID)){
        std::cerr << "syntax error: " << current->lexema  << "table name expected" << std::endl;
        exit(0);
    }
    if(!match(Token::TABLE)){
        std::cerr << "syntax error: " << current->lexema  << "table keyword expected" << std::endl;
        exit(0);
    }

    if(!match(Token::ID)){
        std::cerr << "syntax error: " << current->lexema  << "table name expected" << std::endl;
        exit(0);
    }
}

