#ifndef INTEGER_H
#define INTEGER_H

#include "TypeInfo.h"

static TypeInfo* INT_TYPE_INFO = NULL;

void intAdd(const void* arg1, const void* arg2, void* result);
void intSubtract(const void* arg1, const void* arg2, void* result);
void intMultiply(const void* arg, void* result);
void intPrint(const void* data);
TypeInfo* GetIntTypeInfo();

#endif // INTEGER_H
