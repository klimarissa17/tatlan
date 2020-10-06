#include <algorithm> 
#include <clocale>
#include <stdio.h> 
#include <vector> 
#include <string> 
#include <regex>
#include <unordered_map> 
#include <iostream> 
#include <locale.h>
#include <assert.h>

using std::string;
using std::wstring;

auto space_or_tab_or_endl = "\t\n ";

class Lexer {
    // TODO: unmess this and make some of the members private 
    public: 
        Lexer(wstring str);        

        enum class LexemType{
            Id, 
            String, 
            Char, 
            Int,
            Float, 
            
            If, 
            Else, 
            While, 
            Return, 
            Break, 
            Function,

            Plus, 
            Minus, 
            Asterisk, 
            Slash, 
            Percent,
            Colon, 
            Semicolon,
            Power,
            
            DoubleEqual,
            NotEqual,
            Assign,
            
            Begin,
            End,

            LeftParenthesis, 
            RightParenthesis,
            LeftBracket,
            RightBracket,

            Comma,

            ARITHM_OP, COMPARE_OP, NOT_OP, 
            UNARY_MINUS_OP, L_BRACE, R_BRACE, POSTFIX_WHILE_KW, READ_KW, WRITE_KW, LOGICAL_OP}; 

        std::unordered_map<wstring, LexemType> classifier;
        typedef struct {
            size_t   start_pos;
            size_t   lexem_length;
            LexemType type;
        } lexem_t;

        wstring text;
        wchar_t * position;
        size_t position_num;
        void read_token();
        lexem_t classify_token(wstring str);
        std::wregex separator_finder;
        std::vector <lexem_t> tokens;
        const char* start = nullptr;
        const wstring regex_text = LR"(\bӘГӘР\b|\bБАШКАЧА\b|\bБУЛГАНДА\b|\bБАШ\b|\bОЧ\b|\bюл\b|\bбөтен\b|\bвакланма\b|\bсимвол\b|\bкайтар\b|\bЯЗ\b|\bУКЫ\b|\bФУНКЦИЯ\b|\bЯКИ\b|\bҺӘМ\b|\bТҮГӘЛ\b|\[|\]|\(|\)|\,|:=|==|\-|\+|\\|\^)";

        void process_text();
};

Lexer::Lexer(wstring str) {
    text = str;
    position_num = 0;
    separator_finder.assign(regex_text);
    classifier = {
        {L"+",     Lexer::LexemType::Plus},
        {L"-",     Lexer::LexemType::Minus},
        {L"/",     Lexer::LexemType::Slash},
        {L"*",     Lexer::LexemType::Asterisk},
        {L"%",     Lexer::LexemType::Percent},
        {L"^",     Lexer::LexemType::Power},
        {L"==",    Lexer::LexemType::DoubleEqual},
        {L":=",    Lexer::LexemType::Assign},
        {L"!=",    Lexer::LexemType::NotEqual},
        {L"(",     Lexer::LexemType::LeftParenthesis},
        {L")",     Lexer::LexemType::RightParenthesis},
        {L"[",     Lexer::LexemType::LeftBracket},
        {L"]",     Lexer::LexemType::RightBracket},
        {L":",     Lexer::LexemType::Colon},
        {L";",     Lexer::LexemType::Semicolon},
        {L",",     Lexer::LexemType::Comma},
        {L"булганда",   Lexer::LexemType::While},
        {L"баш",        Lexer::LexemType::Begin},
        {L"оч",         Lexer::LexemType::End},
        {L"юл",         Lexer::LexemType::String},
        {L"бөтен",      Lexer::LexemType::Int},
        {L"вакланма",   Lexer::LexemType::Float},
        {L"символ",     Lexer::LexemType::Char},
        {L"кайтар",     Lexer::LexemType::Return},
        {L"функция",    Lexer::LexemType::Function},
        {L"әгәр",       Lexer::LexemType::If},
        {L"башкача",    Lexer::LexemType::Else}
    };
}


void Lexer::read_token() {
    using std::regex;
    std::wcmatch match;
    std::regex_search(&text[position_num], match, separator_finder); 
    
    size_t prev = position_num;
    
    position_num = position_num + match.position(0);
    size_t id_beg = text.find_first_not_of(L" \n\r\t", prev);

    if (id_beg != wstring::npos) {
        size_t id_end = text.find_first_not_of(L" \n\r\t", id_beg);
        if (id_end == wstring::npos) {
            // probably it means we reached the end of the text
            // but actually the following out is never printed; instead, a non-exising token is being found.
            // TODO: fix the bug above
            // TODO: do smth meaningful there instead of just printing
            std::wcout << "this is the end\n";
            return;
        } else {
            lexem_t res;
            res.lexem_length = id_end - id_beg + 1;
            res.start_pos = id_beg;
            res.type = Lexer::LexemType::Id;
            tokens.push_back(res);
        }
        
    }
        
    lexem_t token = classify_token(match[0]);
    position_num = position_num + match[0].length();
    tokens.push_back(token);
}

Lexer::lexem_t Lexer::classify_token(wstring str) {
    lexem_t res;
    res.lexem_length = str.length();
    res.start_pos = position_num;
    auto elem = classifier.find(str);
    if (elem != classifier.end()) {
        res.type = elem->second;
        return res;
    }

    // TODO: raise an exception or smth here if such keyword was not found
    return res;
}

void Lexer::process_text() {
    while (position_num < text.length()) {
        read_token();
    }
}


int main() {

    std::locale::global(std::locale("tt_RU.UTF-8"));
    std::wstring a = L"БАШ ОЧ  ";
    Lexer l(a);
    l.process_text(); 
    return 0;
}
