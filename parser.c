//
// Created by Aeddon Chipman on 12/7/2018.
//

#include "parser.h"

#define ASCII_0 48

ASN* program_tree;
int current_indent = 0;
extern void deduce_print(PyType* p);
int power(int base, int exponent) {
    int res = 1;
    while (exponent--) {
        res *= base;
    }
    return res;
}
int str_to_int(char* text, int length) {
    int res = 0;
    int exponent = 0;
    for (int i = length - 1; i >= 0; i--) {
        int digit = text[i] - ASCII_0;
        int place_value = digit*power(10, exponent++);
        res += place_value;
    }
    return res;
}
double str_to_double(char* text) {
    double res = 0;
    int exponent = 1;
    int decimal_loc = 0;
    while (text[decimal_loc] != '.') {
        decimal_loc++;
    }
    res += str_to_int(text, decimal_loc);
    decimal_loc++;
    for (char* c = &text[decimal_loc]; *c; c++) {
        double digit = *c - ASCII_0;
        double place_value = digit/power(10, exponent++);
        res += place_value;
    }
    return res;
}
PyType* alloc_and_write(const char* name, Token t) {
    void* value_loc = NULL;
    if (streq(name, PRIMITIVE_LIST[0])) {
        value_loc = malloc(sizeof(bool));
        *(bool*)value_loc = (t.type == TRUE) ? true : false;
        return py_create(value_loc, PRIMITIVE_LIST[0]);
    }
    else if (streq(name, PRIMITIVE_LIST[1])) {
        value_loc = malloc(sizeof(int));
        *(int*)value_loc = str_to_int(t.text, t.length);
        return py_create(value_loc, PRIMITIVE_LIST[1]);
    }
    else if (streq(name, PRIMITIVE_LIST[2])) {
        value_loc = malloc(sizeof(double));
        *(double*)value_loc = str_to_double(t.text);
        return py_create(value_loc, PRIMITIVE_LIST[2]);
    }
    else if (streq(name, PRIMITIVE_LIST[3])) {
        value_loc = str_char_copy(t.text);
        return py_create(value_loc, PRIMITIVE_LIST[3]);
    }
    else if (streq(name, PRIMITIVE_LIST[4])) {
        return py_create(NULL, PRIMITIVE_LIST[4]);
    }

    /* and more */
    return NULL;
}
bool bool_literal(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == TRUE || t.type == FALSE) {
        *asn = (ASN){false,
                     0, // type of token doesn't matter
                     alloc_and_write(PRIMITIVE_LIST[0], t),
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool int_literal(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == INT_LIT) {
        *asn = (ASN){false,
                 0,
                 alloc_and_write(PRIMITIVE_LIST[1], t),
                 NULL,
                 NULL,
                 NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool float_literal(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == FLOAT_LIT) {
        *asn = (ASN){false,
                     0,
                     alloc_and_write(PRIMITIVE_LIST[2], t),
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool string_literal(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == STRING_LIT) {
        *asn = (ASN){false,
                     0,
                     alloc_and_write(PRIMITIVE_LIST[3], t),
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool none_literal(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == NONE) {
        *asn = (ASN){false,
                     0,
                     alloc_and_write(PRIMITIVE_LIST[4], t),
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool var_literal(ASN* asn) { // IMPROVE THIS PLZ
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == VAR_LIT) {
        *asn = (ASN){true,
                     VAR_LIT,
                     str_var_copy(t.text),
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool tuple_literal(ASN* asn) {
    return false;
}
bool list_literal(ASN* asn) {
    return false;
}
bool dict_literal(ASN* asn) {
    return false;
}
bool set_literal(ASN* asn) {
    return false;
}
void expression_emt(ASN* asn);
void atom_emt(ASN* asn) {
    bool res = bool_literal(asn) ||
               int_literal(asn) ||
               float_literal(asn) ||
               string_literal(asn) ||
               none_literal(asn) ||
               var_literal(asn) ||
               tuple_literal(asn) ||
               list_literal(asn) ||
               dict_literal(asn) ||
               set_literal(asn);
    if (!res) {
        Token t = next_NSP();
        if (t.type == L_PAR) {
            expression_emt(asn);
            t = next_NSP();
            if (!(t.type == R_PAR)) {
                printf("parsing error, unclosed parentheses\n");
                exit(-1);
            }
        }
        else {
            printf("parsing error, improper syntax\n");
            exit(-1);
        }
    }
}
void call_index_emt(ASN* asn) {
    atom_emt(asn);
}
void pre_emt(ASN* asn);
void exp_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    call_index_emt(first);
    int save = program_offset;
    while (next_NSP().type == EXP) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        EXP,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        pre_emt(parent->second);
        first = parent;
        save = program_offset;
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void pre_emt(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == PLUS ||
        t.type == MINUS ||
        t.type == BIT_NOT) {
        *asn = (ASN){true,
                     t.type,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     NULL,
                     NULL};
        pre_emt(asn->first);
        return;
    }
    program_offset = save;
    exp_emt(asn);
}
void scale_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    pre_emt(first);
    int save = program_offset;
    Token t = next_NSP();
    while (t.type == MUL ||
           t.type == DIV ||
           t.type == FLOOR_DIV ||
           t.type == MOD) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        t.type,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        pre_emt(parent->second);
        first = parent;
        save = program_offset;
        t = next_NSP();
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void add_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    scale_emt(first);
    int save = program_offset;
    Token t = next_NSP();
    while (t.type == PLUS ||
           t.type == MINUS) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        t.type,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        scale_emt(parent->second);
        first = parent;
        save = program_offset;
        t = next_NSP();
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void shift_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    add_emt(first);
    int save = program_offset;
    Token t = next_NSP();
    while (t.type == L_SHIFT ||
           t.type == R_SHIFT) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        t.type,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        add_emt(parent->second);
        first = parent;
        save = program_offset;
        t = next_NSP();
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void bit_and_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    shift_emt(first);
    int save = program_offset;
    while (next_NSP().type == BIT_AND) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        BIT_AND,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        shift_emt(parent->second);
        first = parent;
        save = program_offset;
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void bit_xor_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    bit_and_emt(first);
    int save = program_offset;
    while (next_NSP().type == BIT_XOR) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        BIT_XOR,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        bit_and_emt(parent->second);
        first = parent;
        save = program_offset;
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void bit_or_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    bit_xor_emt(first);
    int save = program_offset;
    while (next_NSP().type == BIT_OR) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        BIT_OR,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        bit_xor_emt(parent->second);
        first = parent;
        save = program_offset;
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void comp_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    bit_or_emt(first);
    int save = program_offset;
    Token t = next_NSP();
    while (t.type == IN ||
           t.type == NOT_IN ||
           t.type == IS ||
           t.type == IS_NOT ||
           t.type == LT ||
           t.type == LTOE ||
           t.type == GT ||
           t.type == GTOE ||
           t.type == NE ||
           t.type == EQEQ) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        t.type,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        bit_or_emt(parent->second);
        first = parent;
        save = program_offset;
        t = next_NSP();
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void not_emt(ASN* asn) {
    int save = program_offset;
    if (next_NSP().type == NOT) {
        *asn = (ASN){true,
                     NOT,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     NULL,
                     NULL};
        comp_emt(asn->first);
        return;
    }
    program_offset = save;
    comp_emt(asn);
}
void and_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    not_emt(first);
    int save = program_offset;
    while (next_NSP().type == AND) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        AND,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        not_emt(parent->second);
        first = parent;
        save = program_offset;
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void or_emt(ASN* asn) {
    ASN* first = (ASN*)malloc(sizeof(ASN));
    and_emt(first);
    int save = program_offset;
    while (next_NSP().type == OR) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        OR,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        and_emt(parent->second);
        first = parent;
        save = program_offset;
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void lambda_emt(ASN* asn) {
    or_emt(asn);
}
void expression_emt(ASN* asn) {
    lambda_emt(asn);
}
bool assign_stmt(ASN* asn) {
    int save = program_offset;
    Token t1 = next_NSP();
    if (t1.type == VAR_LIT) {
        Token t2 = next_NSP();
        if (t2.type == EQ ||
            t2.type == PLUS_EQ ||
            t2.type == MINUS_EQ ||
            t2.type == MUL_EQ ||
            t2.type == EXP_EQ ||
            t2.type == DIV_EQ ||
            t2.type == FLOOR_DIV_EQ ||
            t2.type == MOD_EQ ||
            t2.type == BIT_OR_EQ ||
            t2.type == BIT_AND_EQ ||
            t2.type == BIT_XOR_EQ ||
            t2.type == R_SHIFT_EQ ||
            t2.type == L_SHIFT_EQ) {
            *asn = (ASN) {true,
                          t2.type,
                          NULL,
                          (ASN*)malloc(sizeof(ASN)),
                          (ASN*)malloc(sizeof(ASN)),
                          NULL};
            *asn->first = (ASN) {false,
                                 VAR_LIT,
                                 str_var_copy(t1.text),
                                 NULL,
                                 NULL,
                                 NULL};
            expression_emt(asn->second);
            return true;
        }
    }
    program_offset = save;
    return false;
}
bool import_stmt(ASN* asn) {
    return false; // improve this later
}
bool return_stmt(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == RETURN) {
        *asn = (ASN){true,
                     RETURN,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     NULL,
                     NULL};
        expression_emt(asn->first);
        return true;
    }
    program_offset = save;
    return false;
}
bool pass_stmt(ASN* asn) {
    int save = program_offset;
    if (next_NSP().type == PASS) {
        *asn = (ASN){true,
                     PASS,
                     NULL,
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool break_stmt(ASN* asn) {
    int save = program_offset;
    if (next_NSP().type == BREAK) {
        *asn = (ASN){true,
                     BREAK,
                     NULL,
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
bool print_stmt(ASN* asn) {
    int save = program_offset;
    if (next_NSP().type == PRINT) {
        *asn = (ASN){true,
                     PRINT,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     NULL,
                     NULL};
        if (next_NSP().type != L_PAR) {
            printf("parser error, print must have parentheses\n");
            exit(-1);
        }
        expression_emt(asn->first);
        if (next_NSP().type != R_PAR) {
            printf("parser error, print must have parentheses\n");
            exit(-1);
        }
        return true;
    }
    program_offset = save;
    return false;
}
bool comment_literal(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == COMMENT) {
        *asn = (ASN){true,
                     COMMENT,
                     NULL,
                     NULL,
                     NULL,
                     NULL};
        return true;
    }
    program_offset = save;
    return false;
}
void singular_stmt(ASN* asn) {
    bool res = assign_stmt(asn) ||
               import_stmt(asn) ||
               return_stmt(asn) ||
               pass_stmt(asn) ||
               break_stmt(asn) ||
               print_stmt(asn) ||
               comment_literal(asn);
    if (!res) {
        expression_emt(asn);
    }
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == END) {
        program_offset = save;
        return;
    }
    if (t.type != NEWLINE) {
        printf("parser error, no newline\n");
        exit(-1);
    }
}
void newline_indented() {
    Token t = next_NSP();
    if (t.type == NEWLINE) {
        Token test = test_next();
        if (test.type == SPACE && test.length == (current_indent + 1)*4) {
            current_indent++;
            return;
        }
        t = next_NSP();
        if (t.type == TAB && t.length == current_indent + 1) {
            current_indent++;
        }
        else {
            printf("parser error, lack of indentation\n");
            printf("t.type is %d\n", t.type);
            exit(-1);
        }
    }
    else {
        printf("parser error, no newline\n");
        exit(-1);
    }
}
bool proper_tabbing(int my_indent) {
    int save = program_offset;
    int found_indent = 0;
    Token test = test_next();
    if (test.type == SPACE) {
        if (test.length%4) {
            printf("parser error, weird indentation\n");
            exit(-1);
        }
        found_indent = test.length/4;
    }
    else {
        Token t = next_NSP();
        if (t.type == TAB) {
            found_indent = t.length;
        }
        else {
            program_offset = save;
        }
    }
    if (found_indent < my_indent) {
        program_offset = save;
        current_indent = found_indent;
        return false;
    }
    if (found_indent > my_indent) {
        printf("parser error, excess indentation\n");
        exit(-1);
    }
    return true;
}
void general_stmt_group(ASN* asn);
bool elif_stmt(ASN* asn);
bool else_stmt(ASN* asn);
bool if_stmt(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == IF) {
        int my_indent = current_indent;
        *asn = (ASN){true,
                     IF,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN))};
        expression_emt(asn->first);
        t = next_NSP();
        if (!(t.type == COLON)) {
            printf("parser error, conditional without colon\n");
        }
        newline_indented(); // increases indent by 1
        general_stmt_group(asn->second);

        int save = program_offset;
        if (!proper_tabbing(my_indent)) {
            return true;
        }
        if (!(elif_stmt(asn->third) || else_stmt(asn->third))) {
            free(asn->third);
            program_offset = save;
            asn->third = NULL;
        }
        return true;
    }
    program_offset = save;
    return false;
}
bool elif_stmt(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == ELIF) {
        int my_indent = current_indent;
        *asn = (ASN){true,
                     ELIF,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN))};
        expression_emt(asn->first);
        t = next_NSP();
        if (!(t.type == COLON)) {
            printf("parser error, conditional without colon\n");
        }
        newline_indented(); // increases indent by 1
        general_stmt_group(asn->second);

        int save = program_offset;
        if (!proper_tabbing(my_indent)) {
            return true;
        }
        if (!(elif_stmt(asn->third) || else_stmt(asn->third))) {
            free(asn->third);
            program_offset = save;
            asn->third = NULL;
        }
        return true;
    }
    program_offset = save;
    return false;
}
bool else_stmt(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == ELSE) {
        t = next_NSP();
        if (!(t.type == COLON)) {
            printf("parser error, conditional without colon\n");
            exit(-1);
        }
        newline_indented();
        general_stmt_group(asn);
        return true;
    }
    program_offset = save;
    return false;
}
bool while_stmt(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == WHILE) {
        int my_indent = current_indent;
        *asn = (ASN){true,
                     WHILE,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN))};
        expression_emt(asn->first);
        t = next_NSP();
        if (!(t.type == COLON)) {
            printf("parser error, conditional without colon\n");
            exit(-1);
        }
        newline_indented();
        general_stmt_group(asn->second);

        int save = program_offset;
        if (!proper_tabbing(my_indent)) {
            return true;
        }
        if (!else_stmt(asn->third)) {
            free(asn->third);
            program_offset = save;
            asn->third == NULL;
        }
        return true;
    }
    program_offset = save;
    return false;
}
bool for_stmt(ASN* asn) {
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == FOR) {
        int my_indent = current_indent;
        *asn = (ASN){true,
                     FOR,
                     NULL,
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN)),
                     (ASN*)malloc(sizeof(ASN))};
        *asn->first = (ASN){true,
                            ITERATE,
                            NULL,
                            (ASN*)malloc(sizeof(ASN)),
                            (ASN*)malloc(sizeof(ASN)),
                            NULL};
        bool res = var_literal(asn->first->first);
        if (!res) {
            printf("parser error, bad for loop l-value formatting\n");
            exit(-1);
        }
        t = next_NSP();
        if (!(t.type == IN)) {
            printf("parser error, bad for loop l-value formatting\n");
            exit(-1);
        }
        expression_emt(asn->first->second);
        t = next_NSP();
        if (!(t.type == COLON)) {
            printf("parser error, for loop without colon\n");
        }
        newline_indented();
        general_stmt_group(asn->second);

        int save = program_offset;
        if (!proper_tabbing(my_indent)) {
            return true;
        }
        if (!else_stmt(asn->third)) {
            free(asn->third);
            program_offset = save;
            asn->third == NULL;
        }
        return true;
    }
    program_offset = save;
    return false;
}
bool def_stmt(ASN* asn) {
    return false;
}
bool class_stmt(ASN* asn) {
    return false;
}
void general_stmt(ASN* asn) {
    bool res = class_stmt(asn) ||
               def_stmt(asn) ||
               for_stmt(asn) ||
               while_stmt(asn) ||
               if_stmt(asn);
    if (!res) {
        singular_stmt(asn);
    }
}
void general_stmt_group(ASN* asn) {
    int my_indent = current_indent;
    ASN* first = (ASN*)malloc(sizeof(ASN));
    general_stmt(first);
    int save = program_offset;
    Token t = next_NSP();
    if (t.type == END) {
        *asn = *first;
        free(first);
        return;
    }
    program_offset = save;
    while (proper_tabbing(my_indent)) {
        ASN* parent = (ASN*)malloc(sizeof(ASN));
        *parent = (ASN){true,
                        SEQUENCE,
                        NULL,
                        first,
                        (ASN*)malloc(sizeof(ASN)),
                        NULL};
        general_stmt(parent->second);
        first = parent;

        save = program_offset;
        t = next_NSP();
        if (t.type != NEWLINE) {
            program_offset = save;
        }
        save = program_offset;
        t = next_NSP();
        if (t.type == END) {
            *asn = *first;
            free(first);
            return;
        }
        program_offset = save;
    }
    program_offset = save;
    *asn = *first;
    free(first);
}
void parse_program() {
    program_tree = (ASN*)malloc(sizeof(ASN));
    general_stmt_group(program_tree);
}
