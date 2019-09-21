//
// Created by Aeddon Chipman on 12/8/2018.
//

#ifndef CPYINTERPRETER_INTERPRETER_H
#define CPYINTERPRETER_INTERPRETER_H

#include "parser.h"

// determines how to print the PyType and then prints it
// used for representing __repr__ and __str__ in print()
void deduce_print(PyType* p);

// function called to run abstract syntax tree
PyType* interpret(ASN* asn);

// setup the tree and interpret it
void run_program();

// variables stored in linked list (will change to trie)
typedef struct VarNode VarNode;
struct VarNode {
    String* name;
    PyType* obj;
    VarNode* next;
};

/*
 * Variable operations.
 */
void var_assign(VarNode* var_list, String* var_name, PyType* obj);
PyType* var_retrieve(VarNode* var_list, String* var_name);

#endif CPYINTERPRETER_INTERPRETER_H
