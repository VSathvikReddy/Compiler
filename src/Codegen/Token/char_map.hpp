#pragma once

#include <unordered_map>
#include <string>


inline const std::unordered_map<char, std::string> KEYBOARD_CHAR_MAP = {
    // --- Special Symbols & Punctuation (Top Row / Shift Keys) ---
    {'~', "TILDE"},
    {'`', "GRAVE"},
    {'!', "EXCLAMATION"}, // or "BANG"
    {'@', "AT"},
    {'#', "HASH"}, // or "POUND"
    {'$', "DOLLAR"},
    {'%', "PERCENT"},
    {'^', "CARET"},
    {'&', "AMPERSAND"}, // or "AND"
    {'*', "STAR"},      // or "ASTERISK"
    {'(', "LPAREN"},
    {')', "RPAREN"},
    {'-', "DASH"},
    {'_', "UNDERSCORE"},
    {'+', "PLUS"},
    {'=', "EQUAL"},
    {'{', "LBRACE"},
    {'}', "RBRACE"},
    {'[', "LBRACKET"},
    {']', "RBRACKET"},
    {'|', "PIPE"},
    {'\\', "BACKSLASH"},
    {':', "COLON"},
    {';', "SEMICOLON"},
    {'"', "QUOTE"},
    {'\'', "APOSTROPHE"},
    {'<', "LESSER"},
    {'>', "GREATER"},
    {',', "COMMA"},
    {'.', "PERIOD"},
    {'/', "SLASH"},
    {'?', "QUESTION"},

    // --- Whitespace & Structural Control ---
    {' ', "SPACE"},
    {'\t', "TAB"},
    {'\n', "NEWLINE"},
    {'\r', "CR"},

    // --- Digits ---
    {'0', "ZERO"},
    {'1', "ONE"},
    {'2', "TWO"},
    {'3', "THREE"},
    {'4', "FOUR"},
    {'5', "FIVE"},
    {'6', "SIX"},
    {'7', "SEVEN"},
    {'8', "EIGHT"},
    {'9', "NINE"},

    // --- Lowercase Letters ---
    {'a', "a"}, {'b', "b"}, {'c', "c"}, {'d', "d"},
    {'e', "e"}, {'f', "f"}, {'g', "g"}, {'h', "h"},
    {'i', "i"}, {'j', "j"}, {'k', "k"}, {'l', "l"},
    {'m', "m"}, {'n', "n"}, {'o', "o"}, {'p', "p"},
    {'q', "q"}, {'r', "r"}, {'s', "s"}, {'t', "t"},
    {'u', "u"}, {'v', "v"}, {'w', "w"}, {'x', "x"},
    {'y', "y"}, {'z', "z"},

    // --- Uppercase Letters ---
    {'A', "A"}, {'B', "B"}, {'C', "C"}, {'D', "D"},
    {'E', "E"}, {'F', "F"}, {'G', "G"}, {'H', "H"},
    {'I', "I"}, {'J', "J"}, {'K', "K"}, {'L', "L"},
    {'M', "M"}, {'N', "N"}, {'O', "O"}, {'P', "P"},
    {'Q', "Q"}, {'R', "R"}, {'S', "S"}, {'T', "T"},
    {'U', "U"}, {'V', "V"}, {'W', "W"}, {'X', "X"},
    {'Y', "Y"}, {'Z', "Z"}
};