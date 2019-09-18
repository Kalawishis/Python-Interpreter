//
// Created by Aeddon Chipman on 12/6/2018.
//

#ifndef CPYINTERPRETER_TOKENIZER_H
#define CPYINTERPRETER_TOKENIZER_H

#include <stdlib.h> // DO NOT INCLUDE THIS WHEN YOU PORT TO VIM
#include <stdio.h> // NOR THIS

#define true 1
#define false 0

typedef int bool;
extern bool streq(const char*, const char*);

enum TokenType{
    // literals
    STRING_LIT,
    VAR_LIT,
    BYTE_LIT,
    INT_LIT,
    FLOAT_LIT,
    IM_LIT,

    // enclosures
    QUOTE,
    L_PAR,
    R_PAR,
    L_BRACK,
    R_BRACK,
    L_BRACE,
    R_BRACE,

    // keywords
    KEYWORDS_START, // only used for locating purposes
    FALSE,
    NONE,
    TRUE,
    AND,
    AS,
    ASSERT,
    BREAK,
    CLASS,
    CONTINUE,
    DEF,
    DEL,
    ELIF,
    ELSE,
    EXCEPT,
    FINALLY,
    FOR,
    FROM,
    GLOBAL,
    IF,
    IMPORT,
    IN,
    NOT_IN,
    IS,
    IS_NOT,
    LAMBDA,
    NONLOCAL,
    NOT,
    OR,
    PASS,
    RAISE,
    RETURN,
    TRY,
    WHILE,
    WITH,
    YIELD,
    AWAIT,
    PRINT,
    KEYWORDS_END, // only used for locating purposes

    // expression operators
    LT,
    GT,
    LTOE,
    GTOE,
    EQEQ,
    NE,
    BIT_OR,
    BIT_XOR,
    BIT_AND,
    BIT_NOT,
    L_SHIFT,
    R_SHIFT,
    PLUS,
    MINUS,
    MUL,
    AT,
    DIV,
    FLOOR_DIV,
    MOD,
    EXP,
    COLON,
    COMMA,
    PERIOD,

    // assignment operators
    EQ,
    PLUS_EQ,
    MINUS_EQ,
    MUL_EQ,
    DIV_EQ,
    MOD_EQ,
    EXP_EQ,
    FLOOR_DIV_EQ,
    BIT_OR_EQ,
    BIT_XOR_EQ,
    BIT_AND_EQ,
    L_SHIFT_EQ,
    R_SHIFT_EQ,

    // whitespace
    SPACE,
    NEWLINE,
    TAB,

    // SPECIAL TOKENS
    COMMENT,
    ITERATE,
    SEQUENCE,
    END
};
typedef enum TokenType TokenType;

struct Token {
        TokenType type;
        char* text;
        int length; // token length, not string length
};
typedef struct Token Token;

extern int program_offset;
extern char* program;

Token test_next();
Token next_NSP();

#endif //CPYINTERPRETER_TOKENIZER_H
