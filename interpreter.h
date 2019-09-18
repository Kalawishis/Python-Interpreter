//
// Created by Aeddon Chipman on 12/8/2018.
//

#ifndef CPYINTERPRETER_INTERPRETER_H
#define CPYINTERPRETER_INTERPRETER_H

#include "parser.h"

void deduce_print(PyType* p);
PyType* interpret(ASN* asn);
void run_program();

typedef struct VarNode VarNode;
struct VarNode {
    String* name;
    PyType* obj;
    VarNode* next;
};
void var_assign(VarNode* var_list, String* var_name, PyType* obj);
PyType* var_retrieve(VarNode* var_list, String* var_name);

#endif //CPYINTERPRETER_INTERPRETER_H
