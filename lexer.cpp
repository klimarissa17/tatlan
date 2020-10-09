#include "lexer.h"

Lexer::Lexer(wstring str) {
    std::locale::global(std::locale("tt_RU.UTF-8"));
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
        {L"БУЛГАНДА",   Lexer::LexemType::While},
        {L"БАШ",        Lexer::LexemType::Begin},
        {L"ОЧ",         Lexer::LexemType::End},
        {L"ЮЛ",         Lexer::LexemType::String},
        {L"БӨТЕН",      Lexer::LexemType::Int},
        {L"ВАКЛАНМА",   Lexer::LexemType::Float},
        {L"СИМВОЛ",     Lexer::LexemType::Char},
        {L"КАЙТАР",     Lexer::LexemType::Return},
        {L"ФУНКЦИЯ",    Lexer::LexemType::Function},
        {L"ӘГӘР",       Lexer::LexemType::If},
        {L"БАШКАЧА",    Lexer::LexemType::Else}
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
        
    lexem_t token = classify_by_lexem_type(match[0]);
    position_num = position_num + match[0].length();
    tokens.push_back(token);
}

Lexer::lexem_t Lexer::classify_by_lexem_type(wstring str) {
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

