#ifndef DOUBLE_H
#define DOUBLE_H

#include "TypeInfo.h"

static TypeInfo* DOUBLE_TYPE_INFO = NULL;

void doubleAdd(const void* arg1, const void* arg2, void* result);
void doubleSubtract(const void* arg1, const void* arg2, void* result);
void doubleMultiply(const void* arg, void* result);
void doublePrint(const void* data);
TypeInfo* GetDoubleTypeInfo();

#endif // DOUBLE_H
