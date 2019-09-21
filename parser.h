//
// Created by Aeddon Chipman on 12/7/2018.
//

// "parser" converts the text (currently ascii) of the program into an
// Abstract Syntax Tree via recursive descent for easy interpretation

#ifndef CPYINTERPRETER_PARSER_H
#define CPYINTERPRETER_PARSER_H

#include "tokenizer.h"
#include "types.h"

// Represents a node of an Abstract Syntax Tree
struct ASN {
    bool is_op;
    TokenType op;
    PyType* obj;

    // An operation in Python can have at most three children in the case
    // of conditionals (condition, operation if true, operation if false)
    struct ASN* first;
    struct ASN* second;
    struct ASN* third;
};
typedef struct ASN ASN;

extern int current_indent; // number of spaces divided by four, or number of tabs
extern ASN* program_tree; // root of parsed Python program
extern void parse_program();
extern const char* PRIMITIVE_LIST[];

#endif CPYINTERPRETER_PARSER_H
