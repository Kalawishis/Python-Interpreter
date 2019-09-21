//
// Created by Aeddon Chipman on 12/6/2018.
//

#include "tokenizer.h"

int program_offset = 0;
char* program = NULL;

// for help with identifying tokens
const char* KEYWORDS[] = {
        "False",
        "None",
        "True",
        "and",
        "as",
        "assert",
        "break",
        "class",
        "continue",
        "def",
        "del",
        "elif",
        "else",
        "except",
        "finally",
        "for",
        "from",
        "global",
        "if",
        "import",
        "in",
        "not in", //handled by tokenizer
        "is",
        "is not", //handled by tokenizer
        "lambda",
        "nonlocal",
        "not",
        "or",
        "pass",
        "raise",
        "return",
        "try",
        "while",
        "with",
        "yield",
        "await",
        "print"
};

bool streq(const char* first, const char* second) {
    int index = 0;
    while (first[index] || second[index]) {
        if (first[index] != second[index]) {
            return false;
        }
        index++;
    }
    return true;
}

// checks if a given word is a keyword
TokenType match_word(const char* word) {
    for (int i = KEYWORDS_START + 1; i < KEYWORDS_END; i++) {
        if (streq(word, KEYWORDS[i - KEYWORDS_START - 1])) {
            return (TokenType)i;
        }
    }
    return VAR_LIT;
}

// checks if the character can be a starting character for a variable name
bool var_start_valid(char c) {
    return c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == 95;
};

// checks if the character can be a character inside a variable name
bool var_valid(char c) {
    return c >= 65 && c <= 90 || c >= 97 && c <= 122 || c == 95 || c >= 48 && c <= 57;
};

// checks if a character is a number
bool num_valid(char c) {
    return c >= 48 && c <= 57;
}

// converts a ASCII numeric string into a Token
Token process_num(const char* program_text) {
    int num_len = 1;
    bool is_float = program_text[0] == '.';
    while (num_valid(program_text[num_len]) || program_text[num_len] == '.') {
        if (program_text[num_len] == '.') {
            if (is_float) {
                printf("tokenization error, multiple decimals in floating point literal\n");
                exit(-1);
            }
            is_float = true;
        }
        num_len++;
    }
    char* num_name = (char*)malloc(num_len + 1);
    for (int i = 0; i < num_len; i++) {
        num_name[i] = program_text[i];
    }
    num_name[num_len] = 0;
    if (is_float) {
        return (Token){FLOAT_LIT, num_name, num_len};
    }
    return (Token){INT_LIT, num_name, num_len};
}

// converts the first available part of the program text into an
// applicable token
Token tokenize(const char* program_text) {
    switch(program_text[0]) {
        case '"': {
            int string_len = 1; // string length, including quotes, not including null
            while (program_text[string_len] != '"') {
                if (program_text[string_len] == 0) { // end with no closing quote
                    printf("tokenization error: unclosed string\n");
                    exit(-1);
                }
                string_len++;
            }
            string_len++;
            char *string_name = (char *)malloc(string_len);
            for (int i = 0; i < string_len; i++) {
                string_name[i] = program_text[i];
            }
            string_name[string_len] = 0;
            return (Token){STRING_LIT, string_name, string_len};
        }
        case '(':
            return (Token){L_PAR, NULL, 1};
        case ')':
            return (Token){R_PAR, NULL, 1};
        case '[':
            return (Token){L_BRACK, NULL, 1};
        case ']':
            return (Token){R_BRACK, NULL, 1};
        case '{':
            return (Token){L_BRACE, NULL, 1};
        case '}':
            return (Token){R_BRACE, NULL, 1};
        case '<': // LT or LTOE or L_SHIFT or L_SHIFT_EQ
            if (program_text[1] == '<') { // L_SHIFT OR L_SHIFT_EQ
                if (program_text[2] == '=') { // L_SHIFT_EQ
                    return (Token){L_SHIFT_EQ, NULL, 3};
                }
                return (Token){L_SHIFT, NULL, 2};
            }
            if (program_text[1] == '=') { // LTOE
                return (Token){LTOE, NULL, 2};
            }
            return (Token){LT, NULL, 1};
        case '>': // GT or GTOE or R_SHIFT or R_SHIFT_EQ
            if (program_text[1] == '>') { // R_SHIFT OR R_SHIFT_EQ
                if (program_text[2] == '=') { // R_SHIFT_EQ
                    return (Token) {R_SHIFT_EQ, NULL, 3};
                }
                return (Token){R_SHIFT, NULL, 2};
            }
            if (program_text[1] == '=') { // GTOE
                return (Token){GTOE, NULL, 2};
            }
            return (Token){GT, NULL, 1};
        case '=': // EQ or EQEQ
            if (program_text[1] == '=') { // EQEQ
                return (Token){EQEQ, NULL, 2};
            }
            return (Token){EQ, NULL, 1};
        case '!': // NE
            if (program_text[1] == '=') { // NE
                return (Token){NE, NULL, 2};
            }
            printf("tokenization error, random !\n");
            exit(-1);
        case '|': // BIT_OR or BIT_OR_EQ
            if (program_text[1] == '=') { // BIT_OR_EQ
                return (Token){BIT_OR_EQ, NULL, 2};
            }
            return (Token){BIT_OR, NULL, 1};
        case '^': // BIT_XOR or BIT_XOR_EQ
            if (program_text[1] == '=') { // BIT_XOR_EQ
                return (Token){BIT_XOR_EQ, NULL, 2};
            }
            return (Token){BIT_XOR, NULL, 1};
        case '&': // BIT_AND or BIT_AND_EQ
            if (program_text[1] == '=') { // BIT_AND_EQ
                return (Token){BIT_AND_EQ, NULL, 2};
            }
            return (Token){BIT_AND, NULL, 1};
        case '~':
            return (Token){BIT_NOT, NULL, 1};
        case '+': // PLUS or PLUS_EQ
            if (program_text[1] == '=') { // PLUS_EQ
                return (Token){PLUS_EQ, NULL, 2};
            }
            return (Token){PLUS, NULL, 1};
        case '-': // MINUS or MINUS_EQ
            if (program_text[1] == '=') { // MINUS_EQ
                return (Token){MINUS_EQ, NULL, 2};
            }
            return (Token){MINUS, NULL, 1};
        case '*': // MUL or EXP or MUL_EQ or EXP_EQ
            if (program_text[1] == '*') { // EXP OR EXP_EQ
                if (program_text[2] == '=') { // EXP_EQ
                    return (Token){EXP_EQ, NULL, 3};
                }
                return (Token){EXP, NULL, 2};
            }
            if (program_text[1] == '=') { // MUL_EQ
                return (Token){MUL_EQ, NULL, 2};
            }
            return (Token){MUL, NULL, 1};
        case '@': // AT
            return (Token){AT, NULL, 1};
        case '/': // DIV or FLOOR_DIV or DIV_EQ or FLOOR_DIV_EQ
            if (program_text[1] == '/') { // FLOOR_DIV OR FLOOR_DIV_EQ
                if (program_text[2] == '=') { // FLOOR_DIV_EQ
                    return (Token){FLOOR_DIV_EQ, NULL, 3};
                }
                return (Token){FLOOR_DIV, NULL, 2};
            }
            if (program_text[1] == '=') { // DIV_EQ
                return (Token){DIV_EQ, NULL, 2};
            }
            return (Token){DIV, NULL, 1};
        case '%': // MOD or MOD_EQ
            if (program_text[1] == '=') { // MOD_EQ
                return (Token){MOD_EQ, NULL, 2};
            }
            return (Token){MOD, NULL, 1};
        case ':': // COLON
            return (Token){COLON, NULL, 1};
        case ',': // COMMA
            return (Token){COMMA, NULL, 1};
        case '#': { // COMMENT
            int comment_len = 1; // string length, including #
            while (program_text[comment_len] && program_text[comment_len] != '\n') {
                comment_len++;
            }
            char *comment_name = (char*)malloc(comment_len);
            for (int i = 0; i < comment_len; i++) {
                comment_name[i] = program_text[i];
            }
            comment_name[comment_len] = 0;
            return (Token){COMMENT, comment_name, comment_len};
        }
        case '.': // PERIOD
            if (num_valid(program_text[1])) {
                return process_num(program_text);
            }
            return (Token){PERIOD, NULL, 1};
        case ' ': { // SPACE
            int spaces = 0;
            while (program_text[spaces] == ' ') {
                spaces++;
            }
            return (Token){SPACE, NULL, spaces};
        }
        case '\n': { // NEWLINE
            int newlines = 0;
            while (program_text[newlines] == '\n') {
                newlines++;
            }
            return (Token){NEWLINE, NULL, newlines};
        }
        case '\t': { // TAB
            int indent = 0;
            while (program_text[indent] == '\t') {
                indent++;
            }
            return (Token){TAB, NULL, indent};
        }
        case 0:
            return (Token){END, NULL, 0};
        default: { // keyword, variable name, or number
            if (var_start_valid(program_text[0])) { // if a variable name
                int var_len = 1; // var length, not including null
                while (var_valid(program_text[var_len])) {
                    var_len++;
                }
                char* var_name = (char*)malloc(var_len + 1);
                for (int i = 0; i < var_len; i++) {
                    var_name[i] = program_text[i];
                }
                var_name[var_len] = 0;
                if (streq(var_name, "is")) {
                    Token space = tokenize(program_text + 2);
                    if (space.type == SPACE) {
                        Token not = tokenize(program_text + 2 + space.length);
                        if (not.type == NOT) {
                            return (Token){IS_NOT, NULL, 5 + space.length};
                        }
                    }
                }
                if (streq(var_name, "not")) {
                    Token space = tokenize(program_text + 3);
                    if (space.type == SPACE) {
                        Token not = tokenize(program_text + 3 + space.length);
                        if (not.type == IN) {
                            return (Token){NOT_IN, NULL, 5 + space.length};
                        }
                    }
                }
                return (Token){match_word(var_name), var_name, var_len};
            }
            if (num_valid(program_text[0]) || program_text[0] == '.') {
                return process_num(program_text);
            }
            else {
                printf("tokenization error, unrecognized input %s\n", program_text);
                exit(-1);
            }
        }
    }
}

Token test_next() {
    return tokenize(&program[program_offset]);
}

Token next_NSP() {
    Token t = tokenize(&program[program_offset]);
    program_offset += t.length;
    if (t.type == SPACE) {
        t = tokenize(&program[program_offset]);
        program_offset += t.length;
    }
    return t;
}