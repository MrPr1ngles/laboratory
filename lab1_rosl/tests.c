//честно, от балды


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "polynomial.h"
#include "integer.h"
#include "double.h"

void test_create_polynomial() {
    PolynomialErrors operationResult = POLYNOMIAL_OPERATION_OK;
    TypeInfo* intTypeInfo = GetIntTypeInfo();
    Polynomial* poly = createPolynomial(intTypeInfo, 3, &operationResult);

    assert(operationResult == POLYNOMIAL_OPERATION_OK);
    assert(poly != NULL);
    assert(poly->size == 3);
    assert(poly->typeInfo == intTypeInfo);

    for (size_t i = 0; i < poly->size; ++i) {
        assert(*(int*)poly->coefficients[i] == 0);
    }

    freePolynomial(poly);
    printf("test_create_polynomial passed.\n");
}

void test_add_polynomials() {
    PolynomialErrors operationResult = POLYNOMIAL_OPERATION_OK;
    TypeInfo* intTypeInfo = GetIntTypeInfo();

    Polynomial* poly1 = createPolynomial(intTypeInfo, 3, &operationResult);
    Polynomial* poly2 = createPolynomial(intTypeInfo, 3, &operationResult);
    Polynomial* result = createPolynomial(intTypeInfo, 3, &operationResult);

    //честно, от балды
    *(int*)poly1->coefficients[0] = 1;
    *(int*)poly1->coefficients[1] = 2;
    *(int*)poly1->coefficients[2] = 3;

    *(int*)poly2->coefficients[0] = 4;
    *(int*)poly2->coefficients[1] = 5;
    *(int*)poly2->coefficients[2] = 6;

    addPolynomials(poly1, poly2, result);

    assert(*(int*)result->coefficients[0] == 5);
    assert(*(int*)result->coefficients[1] == 7);
    assert(*(int*)result->coefficients[2] == 9);

    freePolynomial(poly1);
    freePolynomial(poly2);
    freePolynomial(result);
    printf("test_add_polynomials passed.\n");
}

void test_multiply_polynomial() {
    PolynomialErrors operationResult = POLYNOMIAL_OPERATION_OK;
    TypeInfo* intTypeInfo = GetIntTypeInfo();

    Polynomial* poly = createPolynomial(intTypeInfo, 3, &operationResult);
    Polynomial* result = createPolynomial(intTypeInfo, 3, &operationResult);

    *(int*)poly->coefficients[0] = 1;
    *(int*)poly->coefficients[1] = 2;
    *(int*)poly->coefficients[2] = 3;

    int scalar = 2;
    multiplyPolynomial(poly, &scalar, result);

    assert(*(int*)result->coefficients[0] == 2);
    assert(*(int*)result->coefficients[1] == 4);
    assert(*(int*)result->coefficients[2] == 6);

    freePolynomial(poly);
    freePolynomial(result);
    printf("test_multiply_polynomial passed.\n");
}

int main() {
    test_create_polynomial();
    test_add_polynomials();
    test_multiply_polynomial();
    printf("All tests passed.\n");
    return 0;
}
