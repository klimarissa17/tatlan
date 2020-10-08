// std::locale::global(std::locale("tt_RU.UTF-8")) should be ran before ANYTHING else   
#include <unordered_set>
#include "lexer.cpp"
#include <vector>
#include <stack>


class Parser {
    public: 

        struct Node {
            Lexer::lexem_t lexem;
            std::vector<Node*> children;
            Node * parent;
            size_t number;
        };

        Parser(std::vector <Lexer::lexem_t>&);
        void parse();
        Node * root;

    private: 
        std::vector <Lexer::lexem_t> tokens;
        Lexer::Kind classify_by_kind(Lexer::lexem_t);
        std::stack<Lexer::lexem_t> parsing_stack;
        Node * add_child(Parser::Node *, Lexer::lexem_t);

        Node * curr_node;
        size_t curr_num;
};

Parser::Parser(std::vector<Lexer::lexem_t> &input_tokens) {
    root = new Node();
    tokens = input_tokens;
    root->parent = root;
    curr_num = 0;
    root->number = 0;
    curr_node = root;
}


Parser::Node * Parser::add_child(Parser::Node * node, Lexer::lexem_t lexem) {
    Parser::Node * child = new Node(); 
    assert(child != nullptr);
    assert(node != nullptr);
    child->parent = node;
    child->lexem = lexem;
    Parser::curr_num++;
    child->number = Parser::curr_num;
    node->children.push_back(child);
    return child;
}
        
Lexer::Kind Parser::classify_by_kind(Lexer::lexem_t token) {
    switch(token.type) {
        case(Lexer::LexemType::While):
            return Lexer::Kind::Header;
        case(Lexer::LexemType::Begin):
            return Lexer::Kind::Open;
        case(Lexer::LexemType::End):
            return Lexer::Kind::Close;
        case(Lexer::LexemType::Return):
            // TODO: process this as SECONDARY, NOT NONE!
            return Lexer::Kind::None;
        case(Lexer::LexemType::Function):
            return Lexer::Kind::Header;
        case(Lexer::LexemType::If):
            return Lexer::Kind::Header;
        case(Lexer::LexemType::Else):
            return Lexer::Kind::Header;
        default: 
            return Lexer::Kind::None;
    }
}

void Parser::parse() {
    Lexer::Kind curr_kind, prev_kind;
    
    for (int i = 0; i < tokens.size(); ++i) {
        tokens[i].kind = classify_by_kind(tokens[i]);
    }

    for (int i = 0; i < tokens.size(); ++i) {
        prev_kind = parsing_stack.empty() ? Lexer::Kind::None : parsing_stack.top().kind;
        curr_kind = tokens[i].kind;

        switch (prev_kind) {
            case (Lexer::Kind::Header):
                if (curr_kind == Lexer::Kind::None) {
                    continue;
                } else if (curr_kind == Lexer::Kind::Open) {
                    parsing_stack.push(tokens[i]);
                    add_child(curr_node, tokens[i]);
                } else {
                    wprintf(L"синтаксик тикшерү хатасы: ачылучы җәя табылмаган\n");
                    wprintf(L"parsing error: opening brace not found\n");
                    exit(1);
                }
                break;

            case (Lexer::Kind::Open):
                if (curr_kind == Lexer::Kind::None) {
                    continue;
                } else if (curr_kind == Lexer::Kind::Close) {
                    assert(!parsing_stack.empty());
                    parsing_stack.pop();                // deleting opening statement

                    if (parsing_stack.empty()) {
                        wprintf(L"синтаксик тикшерү хатасы: бүлек башы табылмаган\n");
                        wprintf(L"parsing error: header not found\n");
                        exit(1);
                    }
                    assert(parsing_stack.top().kind == Lexer::Kind::Header);
                    parsing_stack.pop();                // deleting header statement
                    curr_node = curr_node->parent;
                } else {
                    wprintf(L"синтаксик тикшерү хатасы: ябылучы җәя табылмаган\n");
                    wprintf(L"parsing error: closing brace not found\n");
                    exit(1);
                }
                break;
            case (Lexer::Kind::None):
                if (curr_kind == Lexer::Kind::None) {
                    continue;
                } else if (curr_kind == Lexer::Kind::Header) {
                    parsing_stack.push(tokens[i]);
                    add_child(curr_node, tokens[i]);
                } else {
                    // TODO: rewrite error messages here
                    wprintf(L"синтаксик тикшерү хатасы: ачылучы җәя табылмаган\n");
                    wprintf(L"parsing error: opening brace not found\n");
                    exit(1);
                }
                break;
                
            // TODO: process secondary tokens too

            default:
                continue;
        }
    }

    if (!parsing_stack.empty()) {
        wprintf(L"что за пиздец!");
        wprintf(L"синтаксик тикшерү хатасы: ябылучы җәя табылмаган\n");
        wprintf(L"parsing error: closing brace not found\n");
        exit(1);
    } 
}

