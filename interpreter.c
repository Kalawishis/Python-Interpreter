//
// Created by Aeddon Chipman on 12/8/2018.
//

#include "interpreter.h"

VarNode* var_list;
bool var_list_init = false;

extern bool truthify(PyType* p);

void deduce_print(PyType* p) {
    if (streq(p->name, PRIMITIVE_LIST[0])) {
        if (*(bool*)p->data == true) {
            printf("True\n");
        }
        else {
            printf("False\n");
        }
    }
    else if (streq(p->name, PRIMITIVE_LIST[1])) {
        printf("%d\n", *(int*)p->data);
    }
    else if (streq(p->name, PRIMITIVE_LIST[2])) {
        printf("%f\n", *(double*)p->data);
    }
    else if (streq(p->name, PRIMITIVE_LIST[3])) {
        printf("%s\n", ((String*)p->data)->internal);
    }
    else if (streq(p->name, PRIMITIVE_LIST[4])) {
        printf("None\n");
    }
}

// decides which abstract syntax node to free during a logical operation
void logical_free(PyType* res, PyType* left, PyType* right) {
    if (left == right) {
        return;
    }
    if (res == left) {
        free(right->data);
        free(right);
    }
    else {
        free(left->data);
        free(left);
    }
}

// handles variable assignment, initializes variable-storing data
// structure if necessary
void var_assign(VarNode* var_list, String* var_name, PyType* obj) {
    if (!var_list_init) {
        var_list_init = true;
        var_list->name = str_copy(var_name);
        var_list->obj = py_copy(obj);
        var_list->next = NULL;
        return;
    }
    VarNode* temp = var_list;
    while (true) {
        if (str_eq(var_name, temp->name)) {
            temp->obj = py_copy(obj);
            return;
        }
        if (!temp->next) {
            temp->next = (VarNode*)malloc(sizeof(VarNode));
            temp->next->name = str_copy(var_name);
            temp->next->obj = py_copy(obj);
            temp->next->next = NULL;
            return;
        }
        temp = temp->next;
    }
}

// handles variable access
PyType* var_retrieve(VarNode* var_list, String* var_name) {
    VarNode* temp = var_list;
    while (temp) {
        if (str_eq(var_name, temp->name)) {
            return py_copy(temp->obj);
        }
        temp = temp->next;
    }
    return NULL;
}

// operates correct unary operation on input PyType and returns output
// PyType
PyType* monop_resolve(TokenType op, PyType* p) {
    PyType* res;
    switch(op) {
        case PLUS:
            res = py_mono_plus(p);
            break;
        case MINUS:
            res = py_mono_minus(p);
            break;
        case BIT_NOT:
            res = py_bit_not(p);
            break;
        case NOT:
            res = py_not(p);
            break;
        default:
            printf("error, unrecognized monop in general\n");
            exit(-1);
    }
    free(p->data);
    free(p);
    return res;
}

// operates correct binary operation on input PyTypes and returns output
// PyType
PyType* binop_resolve(PyType* left, TokenType op, PyType* right) {
    PyType* res;
    switch(op) {
        case EXP:
        case EXP_EQ:
            res = py_exp(left, right);
            break;
        case MUL:
        case MUL_EQ:
            res = py_mul(left, right);
            break;
        case DIV:
        case DIV_EQ:
            res = py_div(left, right);
            break;
        case FLOOR_DIV:
        case FLOOR_DIV_EQ:
            res = py_floor_div(left, right);
            break;
        case MOD:
        case MOD_EQ:
            res = py_mod(left, right);
            break;
        case PLUS:
        case PLUS_EQ:
            res = py_plus(left, right);
            break;
        case MINUS:
        case MINUS_EQ:
            res = py_minus(left, right);
            break;
        case L_SHIFT:
        case L_SHIFT_EQ:
            res = py_l_shift(left, right);
            break;
        case R_SHIFT:
        case R_SHIFT_EQ:
            res = py_r_shift(left, right);
            break;
        case BIT_AND:
        case BIT_AND_EQ:
            res = py_bit_and(left, right);
            break;
        case BIT_XOR:
        case BIT_XOR_EQ:
            res = py_bit_xor(left, right);
            break;
        case BIT_OR:
        case BIT_OR_EQ:
            res = py_bit_or(left, right);
            break;
        case IN:
            res = py_in(left, right);
            break;
        case NOT_IN:
            res = py_not_in(left, right);
            break;
        case IS:
            res = py_is(left, right);
            break;
        case IS_NOT:
            res = py_is_not(left, right);
            break;
        case LT:
            res = py_lt(left, right);
            break;
        case LTOE:
            res = py_ltoe(left, right);
            break;
        case GT:
            res = py_gt(left, right);
            break;
        case GTOE:
            res = py_gtoe(left, right);
            break;
        case NE:
            res = py_ne(left, right);
            break;
        case EQEQ:
            res = py_eqeq(left, right);
            break;
        case AND:
            res = py_and(left, right);
            logical_free(res, left, right);
            return res;
        case OR:
            res = py_or(left, right);
            logical_free(res, left, right);
            return res;
        default:
            printf("error, unrecognized binop in general\n");
            exit(-1);
    }
    free(left->data);
    free(left);
    free(right->data);
    free(right);
    return res;
}

// runs the operations specified by the abstract syntax tree
PyType* interpret(ASN* asn) {
    if (asn->is_op) {
        switch(asn->op) {
            // special stuff
            case VAR_LIT: {
                PyType *var_value = var_retrieve(var_list, (String*)asn->obj);
                if (!var_value) {
                    printf("interpreter error, variable %s is not defined\n", ((String *) asn->obj)->internal);
                    exit(-1);
                }
                return var_retrieve(var_list, (String*)asn->obj);
            }
            case COMMENT:
                return NULL;
            // operators
            // unary operators
            case BIT_NOT:
            case NOT:
                return monop_resolve(asn->op, interpret(asn->first));
            // unary or binary operators
            case PLUS:
            case MINUS:
                if (!asn->second) { // if monop
                    return monop_resolve(asn->op, interpret(asn->first));
                }
                return binop_resolve(interpret(asn->first), asn->op, interpret(asn->second));
            // binary operators
            case EXP:
            case MUL:
            case DIV:
            case FLOOR_DIV:
            case MOD:
            case L_SHIFT:
            case R_SHIFT:
            case BIT_AND:
            case BIT_XOR:
            case BIT_OR:
            case IN:
            case NOT_IN:
            case IS:
            case IS_NOT:
            case LT:
            case LTOE:
            case GT:
            case GTOE:
            case NE:
            case EQEQ:
            case AND:
            case OR:
                return binop_resolve(interpret(asn->first), asn->op, interpret(asn->second));
            // advanced expression operators
            case LAMBDA:
                return NULL;
            // singular statements
            case EQ:
                var_assign(var_list, (String*)asn->first->obj, interpret(asn->second));
                return NULL;
            case PLUS_EQ:
            case MINUS_EQ:
            case MUL_EQ:
            case DIV_EQ:
            case MOD_EQ:
            case EXP_EQ:
            case FLOOR_DIV_EQ:
            case BIT_OR_EQ:
            case BIT_XOR_EQ:
            case BIT_AND_EQ:
            case L_SHIFT_EQ:
            case R_SHIFT_EQ: {
                String *var_name = (String*)asn->first->obj;
                PyType *var_value = var_retrieve(var_list, var_name);
                if (!var_value) {
                    printf("interpreter error, variable %s is not defined\n", ((String *) asn->obj)->internal);
                    exit(-1);
                }
                var_assign(var_list, var_name, binop_resolve(var_value, asn->op, interpret(asn->second)));
                return NULL;
            }
            case BREAK: // IMPROVE LOGIC LATER
                return NULL;
            case IMPORT:
                return NULL;
            case RETURN:
                return NULL;
            case PASS:
                return NULL;
            case PRINT:
                deduce_print(interpret(asn->first));
                return NULL;
            // compound statements
            case IF:
            case ELIF: {
                if (truthify(interpret(asn->first))) {
                    interpret(asn->second);
                }
                else {
                    if (asn->third) {
                        interpret(asn->third);
                    }
                }
                return NULL;
            }
            case WHILE:
                while (truthify(interpret(asn->first))) {
                    interpret(asn->second);
                }

                return NULL;
            case FOR:
                return NULL; // MAKE FOR LOOPS
            case SEQUENCE:
                interpret(asn->first);
                interpret(asn->second);
                return NULL;
            case DEF:
                return NULL;
            case CLASS:
                return NULL;
        }
    }
    else { // if asn is an object
        return py_copy(asn->obj);
    }
    return NULL;
}

// parses program, initializes variable list, and interprets abstract
// syntax tree
void run_program() {
    parse_program();
    var_list = (VarNode*)malloc(sizeof(VarNode));
    interpret(program_tree);
}