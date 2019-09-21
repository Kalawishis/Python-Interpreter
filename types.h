//
// Created by Aeddon Chipman on 12/6/2018.
//

#ifndef CPYINTERPRETER_TYPES_H
#define CPYINTERPRETER_TYPES_H

#include "tokenizer.h"

#define LIST_SZ 100
#define STR_SZ 100

// represents any object in Python, including user-defined
struct PyType {
    void* data;
    char* name;
};
typedef struct PyType PyType;

/*
 * Following methods apply operations to the PyType.
 * They check for compatibility as well.
 */
PyType* py_create(void* data, char* name); // object creation
PyType* py_copy(PyType* p); // object copying
PyType* py_exp(PyType* p, PyType* other);
PyType* py_mono_plus(PyType* p);
PyType* py_mono_minus(PyType* p);
PyType* py_bit_not(PyType* p);
PyType* py_mul(PyType* p, PyType* other);
PyType* py_div(PyType* p, PyType* other);
PyType* py_floor_div(PyType* p, PyType* other);
PyType* py_mod(PyType* p, PyType* other);
PyType* py_plus(PyType* p, PyType* other);
PyType* py_minus(PyType* p, PyType* other);
PyType* py_l_shift(PyType* p, PyType* other);
PyType* py_r_shift(PyType* p, PyType* other);
PyType* py_bit_and(PyType* p, PyType* other);
PyType* py_bit_xor(PyType* p, PyType* other);
PyType* py_bit_or(PyType* p, PyType* other);
PyType* py_in(PyType* p, PyType* other);
PyType* py_not_in(PyType* p, PyType* other);
PyType* py_is(PyType* p, PyType* other);
PyType* py_is_not(PyType* p, PyType* other);
PyType* py_lt(PyType* p, PyType* other);
PyType* py_ltoe(PyType* p, PyType* other);
PyType* py_gt(PyType* p, PyType* other);
PyType* py_gtoe(PyType* p, PyType* other);
PyType* py_ne(PyType* p, PyType* other);
PyType* py_eqeq(PyType* p, PyType* other);
PyType* py_not(PyType* p);
PyType* py_and(PyType* p, PyType* other);
PyType* py_or(PyType* p, PyType* other);

// Represents Python list primitive type
struct List {
    PyType** internal;
    int capacity;
    int index;
};
typedef struct List List;

/*
 * Methods for Python list primitive type.
 */
List* list_create();
List* list_copy(List* l);
PyType* list_get(List* l, int i);
void list_set(List* l, int i, PyType* value);
void list_append(List* l, PyType* value);
void list_extend(List* l, List* other);
bool list_greater(List* l, List* other);
bool list_eq(List* l, List* other);
bool list_less(List* l, List* other);

// Represents Python string primitive type
// Also used within interpreter for convenience
struct String {
    char* internal;
    int capacity;
    int index;
};
typedef struct String String;

/*
 * Methods for Python string primitive type.
 */
String* str_create();
String* str_char_copy(const char* other);
String* str_var_copy(const char* other);
String* str_copy(String* other);
char str_get(String* s, int i);
void str_set(String* s, int i, char value);
void str_append(String* s, char value);
void str_extend(String* s, String* other);
bool str_contains(String* s, String* other);
bool str_greater(String* l, String* other);
bool str_eq(String* l, String* other);
bool str_less(String* l, String* other);

#endif CPYINTERPRETER_TYPES_H
