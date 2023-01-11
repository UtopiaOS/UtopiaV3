/*
* The MIT License (MIT)
* Copyright (c) 2016 Peter Goldsborough
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
* $ Utopia $
*/

#ifndef COVENANT_LIBGENERIC_VECTOR_H
#define COVENANT_LIBGENERIC_VECTOR_H

#define __NEED_Size
#define __NEED_UniversalType
#define __NEED_Status
#define __NEED_USize
#include <covenant/status.h>
#include <covenant/bits/everytype.h>
typedef struct Vector Vector;

struct Vector {
    USize size;
    USize capacity;
    USize obj_size;

    UniversalType data; // Continous array of data
};

Status c_vec_init(Vector*, USize, USize);

Status c_vec_push_back(Vector*, UniversalType);
Status c_vec_push_front(Vector*, UniversalType);
Status c_vec_insert(Vector*, USize, UniversalType);
Status c_vec_assign(Vector*, USize, UniversalType);

Status c_vec_pop_back(Vector*);
Status c_vec_pop_front(Vector*);
Status c_vec_erase(Vector*, USize);
Status c_vec_clear(Vector*);

UniversalType c_vec_get(Vector*, USize);
UniversalType c_vec_front(Vector*);
UniversalType c_vec_back(Vector*);

Status c_vec_resize(Vector*, USize);

#endif