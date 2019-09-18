//
// Created by Aeddon Chipman on 12/7/2018.
//

#ifndef CPYINTERPRETER_EVALUATOR_H
#define CPYINTERPRETER_EVALUATOR_H

#include "tokenizer.h"
#include "types.h"

struct ASN {
    bool is_op;
    TokenType op;
    PyType* obj;
    struct ASN* first;
    struct ASN* second;
    struct ASN* third;
};
typedef struct ASN ASN;

extern int current_indent;
extern ASN* program_tree;
extern void parse_program();
extern const char* PRIMITIVE_LIST[];

#endif //CPYINTERPRETER_EVALUATOR_H
