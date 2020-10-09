#pragma once

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

class Lexer {
    public: 
        Lexer(wstring str);        
        enum class Kind {
            Header, 
            Open, 
            Close, 
            Secondary,
            None,
        };
        enum class LexemType{
            Id = 0, 
            String = 1, 
            Char = 2, 
            Int = 3,
            Float = 4, 
            
            If = 5, 
            Else = 6, 
            While = 7, 
            Return = 8, 
            Break = 9, 
            Function = 10,

            Plus = 11, 
            Minus = 12, 
            Asterisk = 13, 
            Slash = 14, 
            Percent = 15,
            Colon = 16, 
            Semicolon = 17,
            Power = 18,
            
            DoubleEqual = 19,
            NotEqual = 20,
            Assign = 21,
            
            Begin = 22,
            End = 23,

            LeftParenthesis = 24, 
            RightParenthesis = 25,
            LeftBracket = 26,
            RightBracket = 27,

            Comma = 28,
            }; 

        typedef struct {
            size_t   start_pos;
            size_t   lexem_length;
            LexemType type;
            Kind kind;
        } lexem_t;

        void process_text();
    private: 
        std::unordered_map<wstring, LexemType> classifier;
        wstring text;
        wchar_t * position;
        size_t position_num;
        std::wregex separator_finder;
        std::vector <lexem_t> tokens;
        const char* start = nullptr;
        // TODO: logically reorder parts of the regex to make it more readable
        const wstring regex_text =
            LR"(\bӘГӘР\b|\bБАШКАЧА\b|\bБУЛГАНДА\b|\bБАШ\b)"
            LR"(|\bОЧ\b|\bЮЛ\b|\bБӨТЕН\b|\bВАКЛАНМА\b|\bСИМВОЛ\b)"
            LR"(|\bКАЙТАР\b|\bЯЗ\b|\bУКЫ\b|\bФУНКЦИЯ\b|\bЯКИ\b|)"
            LR"(\bҺӘМ\b|\bТҮГӘЛ\b|\[|\]|\(|\)|\,|:=|==|\-|\+|\\|\^)";

        void read_token();
        lexem_t classify_by_lexem_type(wstring str);
};
