//
// Created by Aeddon Chipman on 12/6/2018.
//

#include "types.h"

const char* PRIMITIVE_LIST[] = {
        "~py_bool",
        "~py_int",
        "~py_float",
        "~py_string",
        "~py_none"
};

const char* OP_NAMES[] = {
        "~py_exp",
        "~py_mono_plus",
        "~py_mono_minus",
        "~py_bit_not",
        "~py_mul",
        "~py_div",
        "~py_floor_div",
        "~py_mod",
        "~py_plus",
        "~py_minus",
        "~py_l_shift",
        "~py_r_shift",
        "~py_bit_and",
        "~py_bit_xor",
        "~py_bit_or",
        "~py_in",
        "~py_not_in",
        "~py_is",
        "~py_is_not",
        "~py_lt",
        "~py_ltoe",
        "~py_gt",
        "~py_gtoe",
        "~py_ne",
        "~py_eqeq",
        "~py_not",
        "~py_and",
        "~py_or"
};

double double_exp(double a, double b) {
    return a + b; // IMPROVE LATER
}
int int_exp(int a, int b) {
    return a + b; // IMPROVE LATER
}
double double_mod(double a, double b) {
    double floor_div = (double)(int)(a/b);
    return a - b*floor_div;
}

bool bool_type(const char* typename) {
    return streq(typename, PRIMITIVE_LIST[0]);
}
bool int_type(const char* typename) {
    return (streq(typename, PRIMITIVE_LIST[0]) ||
            streq(typename, PRIMITIVE_LIST[1]));
}
bool float_type(const char* typename) {
    return (streq(typename, PRIMITIVE_LIST[2]));
}
bool numeric_type(const char* typename) {
    return int_type(typename) || float_type(typename);
}
bool string_type(const char* typename) {
    return streq(typename, PRIMITIVE_LIST[3]);
}
bool none_type(const char* typename) {
    return streq(typename, PRIMITIVE_LIST[4]);
}

/* PYTYPE METHODS */
// MUST HANDLE OVERFLOW BETTER
bool truthify(PyType* p) {
    if (numeric_type(p->name)) {
        if (float_type(p->name)) {
            return (bool)(*(double*)p->data);
        }
        else if (int_type(p->name)) {
            return (bool)(*(int*)p->data);
        }
    }
    else if (string_type(p->name)) {
        return (bool)((String*)p->data)->index;
    }
    else if (none_type(p->name)) {
        return false;
    }
        // PLEASE IMPLEMENT
    else {
        printf("type error, bool conversion is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_create(void* data, char* name) {
    PyType* p = (PyType*)malloc(sizeof(PyType));
    p->data = data;
    p->name = name;
    return p;
}
PyType* py_copy(PyType* p) {
    if (bool_type(p->name)) {
        bool* value_loc = (bool*)malloc(sizeof(bool));
        *value_loc = *(bool*)p->data;
        return py_create(value_loc, PRIMITIVE_LIST[0]);
    }
    else if (int_type(p->name)) {
        int* value_loc = (int*)malloc(sizeof(int));
        *value_loc = *(int*)p->data;
        return py_create(value_loc, PRIMITIVE_LIST[1]);
    }
    else if (float_type(p->name)) {
        double* value_loc = (double*)malloc(sizeof(double));
        *value_loc = *(double*)p->data;
        return py_create(value_loc, PRIMITIVE_LIST[2]);
    }
    else if (string_type(p->name)) {
        String* value_loc = str_copy((String*)p->data);
        return py_create(value_loc, PRIMITIVE_LIST[3]);
    }
    else if (none_type(p->name)) {
        return py_create(NULL, PRIMITIVE_LIST[4]);
    }
    /* and more */
    return NULL;
}
PyType* py_exp(PyType* p, PyType* other) { // FIX THIS SO IT DOESN'T JUST ADD
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = double_exp(*(double*)p->data, *(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = double_exp(*(int*)p->data, *(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = double_exp(*(double*)p->data, *(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else {
            int *data = (int*)malloc(sizeof(int));
            *data = int_exp(*(int*)p->data, *(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, ** is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_mono_plus(PyType* p) {
    if (numeric_type(p->name)) {
        if (float_type(p->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = *(double*)p->data;
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name)) {
            int* data = (int*)malloc(sizeof(int));
            *data = *(int*)p->data;
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    //PLEASE IMPLEMENT
    else {
        printf("type error, unary + is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_mono_minus(PyType* p) {
    if (numeric_type(p->name)) {
        if (float_type(p->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = -*(double*)p->data;
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name)) {
            int* data = (int*)malloc(sizeof(int));
            *data = -*(int*)p->data;
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, unary - is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_bit_not(PyType* p) {
    if (int_type(p->name)) {
        int* data = (int*)malloc(sizeof(int));
        *data = ~*(int*)p->data;
        return py_create(data, PRIMITIVE_LIST[1]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, ~ is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_mul(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data)*(*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(int*)p->data)*(*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data)*(*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else {
            int *data = (int*)malloc(sizeof(int));
            *data = (*(int*)p->data)*(*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    else if (string_type(p->name) && int_type(other->name)) {
        String* data = str_copy((String*)p->data);
        for (int i = 1; i < *(int*)other->data; i++) {
            str_extend(data, (String*)p->data);
        }
        return py_create(data, PRIMITIVE_LIST[3]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, * is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_div(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data)/(*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(int*)p->data)/(*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data)/(*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else {
            double *data = (double*)malloc(sizeof(double));
            *data = (double)(*(int*)p->data)/(*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, / is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_floor_div(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (double)(int)((*(double*)p->data)/(*(int*)other->data));
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (double)(int)((*(int*)p->data)/(*(double*)other->data));
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (double)(int)((*(double*)p->data)/(*(double*)other->data));
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else {
            int *data = (int*)malloc(sizeof(int));
            *data = (*(int*)p->data)/(*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, // is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_mod(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = double_mod(*(double*)p->data, *(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = double_mod(*(int*)p->data, *(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = double_mod(*(double*)p->data, *(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else {
            int *data = (int*)malloc(sizeof(int));
            *data = (*(int*)p->data)%(*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, %% is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_plus(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data) + (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(int*)p->data) + (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data) + (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else {
            int *data = (int*)malloc(sizeof(int));
            *data = (*(int*)p->data) + (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    else if (string_type(p->name) && string_type(other->name)) {
        String* data = str_copy((String*)p->data);
        str_extend(data, (String*)other->data);
        return py_create(data, PRIMITIVE_LIST[3]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, + is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_minus(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data) - (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(int*)p->data) - (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            double* data = (double*)malloc(sizeof(double));
            *data = (*(double*)p->data) - (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[2]);
        }
        else {
            int *data = (int*)malloc(sizeof(int));
            *data = (*(int*)p->data) - (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[1]);
        }
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, // is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_l_shift(PyType* p, PyType* other) {
    if (int_type(p->name)) {
        int* data = (int*)malloc(sizeof(int));
        *data = (*(int*)p->data) << (*(int*)other->data);
        return py_create(data, PRIMITIVE_LIST[1]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, << is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_r_shift(PyType* p, PyType* other) {
    if (int_type(p->name)) {
        int* data = (int*)malloc(sizeof(int));
        *data = (*(int*)p->data) >> (*(int*)other->data);
        return py_create(data, PRIMITIVE_LIST[1]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, >> is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_bit_and(PyType* p, PyType* other) {
    if (int_type(p->name)) {
        int* data = (int*)malloc(sizeof(int));
        *data = (*(int*)p->data) & (*(int*)other->data);
        return py_create(data, PRIMITIVE_LIST[1]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, & is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_bit_xor(PyType* p, PyType* other) {
    if (int_type(p->name)) {
        int* data = (int*)malloc(sizeof(int));
        *data = (*(int*)p->data) ^ (*(int*)other->data);
        return py_create(data, PRIMITIVE_LIST[1]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, ^ is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_bit_or(PyType* p, PyType* other) {
    if (int_type(p->name)) {
        int* data = (int*)malloc(sizeof(int));
        *data = (*(int*)p->data) | (*(int*)other->data);
        return py_create(data, PRIMITIVE_LIST[1]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, | is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_in(PyType* p, PyType* other) {
    if (string_type(p->name) && string_type(other->name)) {
        int* data = (int*)malloc(sizeof(int));
        //printf("here reached\n");
        *data = str_contains((String*)other->data, (String*)p->data);
        //printf("here reached 2\n");
        return py_create(data, PRIMITIVE_LIST[0]);
    }
    // PLEASE IMPELEMENT
    printf("type error, \"in\" is not defined for type %s\n", p->name);
    exit(-1);
}
PyType* py_not_in(PyType* p, PyType* other) {
    if (string_type(p->name) && string_type(other->name)) {
        int* data = (int*)malloc(sizeof(int));
        *data = 1 - str_contains((String*)other->data, (String*)p->data);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
    // PLEASE IMPELEMENT
    printf("type error, \"not in\" is not defined for type %s\n", p->name);
    exit(-1);
}
PyType* py_is(PyType* p, PyType* other) {
    int* data = (int*)malloc(sizeof(int));
    *data = p->data == other->data;
    return py_create(data, PRIMITIVE_LIST[0]);
}
PyType* py_is_not(PyType* p, PyType* other) {
    int* data = (int*)malloc(sizeof(int));
    *data = p->data != other->data;
    return py_create(data, PRIMITIVE_LIST[0]);
}
PyType* py_lt(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) < (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) < (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) < (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) < (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
    }
    else if (string_type(p->name) && string_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = str_less((String*)p->data, (String*)other->data);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, < is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_ltoe(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) <= (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) <= (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) <= (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) <= (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
    }
    else if (string_type(p->name) && string_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = str_less((String*)p->data, (String*)other->data) ||
                str_eq((String*)p->data, (String*)other->data);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
        // PLEASE IMPLEMENT
    else {
        printf("type error, <= is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_gt(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) > (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) > (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) > (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) > (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
    }
    else if (string_type(p->name) && string_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = str_greater((String*)p->data, (String*)other->data);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
        // PLEASE IMPLEMENT
    else {
        printf("type error, > is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_gtoe(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) >= (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) >= (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) >= (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) >= (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
    }
    else if (string_type(p->name) && string_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = str_greater((String*)p->data, (String*)other->data) ||
                str_eq((String*)p->data, (String*)other->data);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
        // PLEASE IMPLEMENT
    else {
        printf("type error, >= is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_ne(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) != (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) != (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) != (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) != (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
    }
    else if (string_type(p->name) && string_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = !str_eq((String*)p->data, (String*)other->data);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
    else if (none_type(p->name) || none_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = !(none_type(p->name) && none_type(other->name));
        return py_create(data, PRIMITIVE_LIST[0]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, != is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_eqeq(PyType* p, PyType* other) {
    if (numeric_type(p->name) && numeric_type(other->name)) {
        if (float_type(p->name) && int_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) == (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (int_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) == (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else if (float_type(p->name) && float_type(other->name)) {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(double*)p->data) == (*(double*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
        else {
            bool* data = (bool*)malloc(sizeof(bool));
            *data = (*(int*)p->data) == (*(int*)other->data);
            return py_create(data, PRIMITIVE_LIST[0]);
        }
    }
    else if (string_type(p->name) && string_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = str_eq((String*)p->data, (String*)other->data);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
    else if (none_type(p->name) || none_type(other->name)) {
        bool* data = (bool*)malloc(sizeof(bool));
        *data = none_type(p->name) && none_type(other->name);
        return py_create(data, PRIMITIVE_LIST[0]);
    }
    // PLEASE IMPLEMENT
    else {
        printf("type error, == is not defined for type %s\n", p->name);
        exit(-1);
    }
}
PyType* py_not(PyType* p) {
    bool* data = (bool*)malloc(sizeof(bool));
    *data = !truthify(p);
    return py_create(data, PRIMITIVE_LIST[0]);
}
PyType* py_and(PyType* p, PyType* other) { // reminder to interpreter: py_and and py_or do not create new objects
    if (!truthify(p)) {
        return p;
    }
    return other;
}
PyType* py_or(PyType* p, PyType* other) { // reminder to interpreter: py_and and py_or do not create new objects
    if (truthify(p)) {
        return p;
    }
    return other;
}

/* LIST METHODS */
List* list_create() {
    List* l = (List*)malloc(sizeof(List));
    l->capacity = LIST_SZ;
    l->index = 0;
    l->internal = (PyType**)malloc(sizeof(void*)*LIST_SZ);
    return l;
}
List* list_copy(List* other) {
    List* l = list_create();
    list_extend(l, other);
    return l;
}
PyType* list_get(List* l, int i) {
    if (i >= l->index) {
        printf("list error, index out of bounds\n");
        exit(-1);
    }
    return l->internal[i];
}
void list_set(List* l, int i, PyType* value) {
    if (i >= l->index) {
        printf("list error, index out of bounds\n");
        exit(-1);
    }
    l->internal[i] = value;
}
void list_append(List* l, PyType* value) {
    if (l->index >= l->capacity) {
        int new_capacity = l->capacity*2;
        PyType** new_internal = (PyType**)malloc(sizeof(PyType*)*new_capacity);
        for (int i = 0; i < l->capacity; i++) {
            new_internal[i] = l->internal[i];
        }
        free(l->internal);
        l->internal = new_internal;
        l->capacity = new_capacity;
    }
    l->internal[l->index++] = value;
}
void list_extend(List* l, List* other) {
    for (int i = 0; i < other->index; i++) {
        list_append(l, list_get(other, i));
    }
}
bool list_gt(List* l, List* other) {
    return false;
}
bool list_eq(List* l, List* other) {
    return false;
}
bool list_lt(List* l, List* other) {
    return false;
}

/* STRING METHODS */
String* str_create() {
    String* s = (String*)malloc(sizeof(String));
    s->capacity = LIST_SZ;
    s->index = 0;
    s->internal = (char*)malloc(sizeof(void*)*STR_SZ);
    s->internal[0] = 0;
    return s;
}
// copy string without quotes
String* str_char_copy(const char* other) {
    String* s = str_create();
    for (int i = 1; other[i] != '"'; i++) {
        str_append(s, other[i]);
    }
    return s;
}
String* str_var_copy(const char* other) {
    String* s = str_create();
    for (int i = 0; other[i] != 0; i++) {
        str_append(s, other[i]);
    }
    return s;
}
String* str_copy(String* other) {
    String* s = str_create();
    str_extend(s, other);
    return s;
}
char str_get(String* s, int i) {
    if (i > s->index) {
        printf("list error, index out of bounds\n");
        exit(-1);
    }
    return s->internal[i];
}
void str_set(String* s, int i, char value) {
    if (i > s->index) {
        printf("list error, index out of bounds\n");
        exit(-1);
    }
    s->internal[i] = value;
}
void str_append(String* s, char value) {
    if (s->index >= s->capacity) {
        int new_capacity = s->capacity*2;
        char* new_internal = (char*)malloc(sizeof(PyType*)*new_capacity);
        for (int i = 0; i < s->index; i++) {
            new_internal[i] = s->internal[i];
        }
        free(s->internal);
        s->internal = new_internal;
        s->capacity = new_capacity;
    }
    s->internal[s->index++] = value;
    s->internal[s->index] = 0;
}
void str_extend(String* s, String* other) {
    for (int i = 0; str_get(other, i) != 0; i++) {
        str_append(s, str_get(other, i));
    }
}
bool str_contains(String* s, String* other) {
    if (s->index < other->index) {
        return false;
    }
    for (int i = 0; i < s->index; i++) {
        bool flag = true;
        for (int j = 0; j < other->index; j++) {
            if (str_get(s, i + j) != str_get(other, j)) {
                flag = false;
                break;
            }
        }
        if (flag) {
            return true;
        }
    }
    return false;
}
bool str_greater(String* s, String* other) {
    for (int i = 0; i < s->index && i < other->index; i++) {
        if (str_get(s, i) > str_get(other, i)) {
            return true;
        }
        if (str_get(s, i) < str_get(other, i)) {
            return false;
        }
    }
    if (s->index > other->index) {
        return true;
    }
    return false;
}
bool str_eq(String* s, String* other) {
    return streq(s->internal, other->internal);
}
bool str_less(String* s, String* other) {
    for (int i = 0; i < s->index && i < other->index; i++) {
        if (str_get(s, i) < str_get(other, i)) {
            return true;
        }
        if (str_get(s, i) > str_get(other, i)) {
            return false;
        }
    }
    if (s->index < other->index) {
        return true;
    }
    return false;
}