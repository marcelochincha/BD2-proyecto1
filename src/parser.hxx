#include <string>
#include <unordered_map>

class Token {
   public:
    enum Type { CREATE = 0, SELECT, INSERT, DELETE, ID, NUM, STRING, COMMA, EQ, SEMICOLON, END, ERR, ANY,TABLE};
    static const char* token_name[14];
    Token(Type type, std::string value);
    Token(Type type);
    Type type;
    std::string lexema;
};

class Scanner {
   public:
    Scanner(std::string input);
    Token* nextToken();
    ~Scanner();

   private:
    std::string input;
    int first, current;
    std::unordered_map<std::string, Token::Type> reserved;
    char nextChar();
    void rollBack();
    void startLexema();
    std::string getLexema();
    Token::Type checkReserved(std::string);
};

class Parser {
   private:
    Scanner* scanner;
    Token *current, *previous;
    bool match(Token::Type ttype);
    bool check(Token::Type ttype);
    bool advance();
    bool isAtEnd();
    void parse();
    void parseProgram();
    void parseCreate();
    void parseSelect();
    void parseInsert();
    void parseDelete();

   public:
    Parser(Scanner* scanner);
    void parse();
};