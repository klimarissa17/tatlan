#pragma once

#include <vector>
#include <stack>
#include <unordered_set>

#include "lexer.h"

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
