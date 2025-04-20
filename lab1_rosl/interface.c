#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interface.h"
#include "integer.h"
#include "double.h"

void printUsage() {
    printf("\nUsage:\n");
    printf("Enter polynomials in format: <size> <type> <coefficients...>\n");
    printf("  size - number of coefficients\n");
    printf("  type - i (int) or f (double)\n");
    printf("  coefficients - space-separated values\n");
    printf("Operations:\n");
    printf("  + : add polynomials\n");
    printf("  - : subtract polynomials\n");
    printf("  * : multiply by scalar\n");
    printf("  = : calculate result and exit\n");
    printf("After '=' you'll be asked to input variable values\n\n");
}

TypeInfo* getTypeInfoFromChar(char type, PolynomialErrors* error) {
    switch(type) {
        case 'i':
            return GetIntTypeInfo();
        case 'f':
            return GetDoubleTypeInfo();
        default:
            *error = INCOMPATIBLE_POLYNOMIAL_TYPES;
            return NULL;
    }
}

void readPolynomial(Polynomial** poly, PolynomialErrors* operationResult) {
    int size;
    char type;
    
    if (scanf("%d %c", &size, &type) != 2) {
        *operationResult = POLYNOMIAL_NOT_DEFINED;
        return;
    }
    
    PolynomialErrors typeError = POLYNOMIAL_OPERATION_OK;
    TypeInfo* typeInfo = getTypeInfoFromChar(type, &typeError);
    if (typeInfo == NULL) {
        *operationResult = typeError;
        return;
    }
    
    *poly = createPolynomial(typeInfo, size, operationResult);
    if (*operationResult != POLYNOMIAL_OPERATION_OK) {
        return;
    }
    
    for (int i = 0; i < size; ++i) {
        if (typeInfo == GetIntTypeInfo()) {
            int value;
            if (scanf("%d", &value) != 1) {
                *operationResult = POLYNOMIAL_NOT_DEFINED;
                return;
            }
            memcpy((*poly)->coefficients[i], &value, sizeof(int));
        } else if (typeInfo == GetDoubleTypeInfo()) {
            double value;
            if (scanf("%lf", &value) != 1) {
                *operationResult = POLYNOMIAL_NOT_DEFINED;
                return;
            }
            memcpy((*poly)->coefficients[i], &value, sizeof(double));
        }
    }
    
    *operationResult = POLYNOMIAL_OPERATION_OK;
}

void handleEvaluation(const Polynomial* poly) {
    void* xValues = malloc(poly->typeInfo->size * poly->size);
    void* result = malloc(poly->typeInfo->size);
    
    if (!xValues || !result) {
        fprintf(stderr, "Memory allocation failed in evaluation\n");
        free(xValues);
        free(result);
        return;
    }

    printf("Enter %zu values ", poly->size);
    
    for (size_t i = 0; i < poly->size; i++) {
        if (poly->typeInfo == GetIntTypeInfo()) {
            int value;
            if (scanf("%d", &value) != 1) {
                fprintf(stderr, "Invalid input for x\n");
                free(xValues);
                free(result);
                return;
            }
            memcpy((char*)xValues + i * poly->typeInfo->size, &value, sizeof(int));
        } else {
            double value;
            if (scanf("%lf", &value) != 1) {
                fprintf(stderr, "Invalid input for x\n");
                free(xValues);
                free(result);
                return;
            }
            memcpy((char*)xValues + i * poly->typeInfo->size, &value, sizeof(double));
        }
    }

    memset(result, 0, poly->typeInfo->size);
    PolynomialErrors err = evaluatePolynomial(poly, xValues, result);
    
    if (err == POLYNOMIAL_OPERATION_OK) {
        printf("Result: ");
        poly->typeInfo->print(result);
        printf("\n");
    } else {
        fprintf(stderr, "%s\n", polynomialErrorToString(err));
    }

    free(xValues);
    free(result);
}

void handleUserInput() {
    PolynomialErrors operationResult = POLYNOMIAL_OPERATION_OK;
    Polynomial* poly1 = NULL;
    
    printUsage();
    readPolynomial(&poly1, &operationResult);
    
    if (operationResult != POLYNOMIAL_OPERATION_OK) {
        fprintf(stderr, "%s\n",polynomialErrorToString(operationResult));
        exit(EXIT_FAILURE);
    }

    char operation[2];
    while (1) {
    
        if (scanf("%1s", operation) != 1) {
            fprintf(stderr, "Error reading operation\n");
            freePolynomial(poly1);
            exit(EXIT_FAILURE);
        }

        if (strcmp(operation, "=") == 0) {
            break;
        }

        if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0) {
            Polynomial* poly2 = NULL;
            readPolynomial(&poly2, &operationResult);
            
            if (operationResult != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(operationResult));
                freePolynomial(poly1);
                exit(EXIT_FAILURE);
            }

            if (poly1->typeInfo != poly2->typeInfo) {
                fprintf(stderr, "%s\n",polynomialErrorToString(INCOMPATIBLE_POLYNOMIAL_TYPES));
                freePolynomial(poly1);
                freePolynomial(poly2);
                exit(EXIT_FAILURE);
            }

            Polynomial* resultPoly = createPolynomial(poly1->typeInfo, 
                (poly1->size > poly2->size) ? poly1->size : poly2->size, 
                &operationResult);
                
            if (operationResult != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n",polynomialErrorToString(operationResult));
                freePolynomial(poly1);
                freePolynomial(poly2);
                exit(EXIT_FAILURE);
            }

            PolynomialErrors opResult;
            if (strcmp(operation, "+") == 0) {
                opResult = addPolynomials(poly1, poly2, resultPoly);
            } else {
                opResult = subtractPolynomials(poly1, poly2, resultPoly);
            }

            if (opResult != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n",polynomialErrorToString(opResult));
                freePolynomial(poly1);
                freePolynomial(poly2);
                freePolynomial(resultPoly);
                exit(EXIT_FAILURE);
            }

            freePolynomial(poly1);
            freePolynomial(poly2);
            poly1 = resultPoly;

        } else if (strcmp(operation, "*") == 0) {
            void* scalar = malloc(poly1->typeInfo->size);
            if (!scalar) {
                fprintf(stderr, "Memory error\n");
                freePolynomial(poly1);
                exit(EXIT_FAILURE);
            }

            if (poly1->typeInfo == GetIntTypeInfo()) {
                int value;
                if (scanf("%d", &value) != 1) {
                    fprintf(stderr, "Invalid scalar input\n");
                    free(scalar);
                    freePolynomial(poly1);
                    exit(EXIT_FAILURE);
                }
                memcpy(scalar, &value, sizeof(int));
            } else {
                double value;
                if (scanf("%lf", &value) != 1) {
                    fprintf(stderr, "Invalid scalar input\n");
                    free(scalar);
                    freePolynomial(poly1);
                    exit(EXIT_FAILURE);
                }
                memcpy(scalar, &value, sizeof(double));
            }

            Polynomial* resultPoly = createPolynomial(poly1->typeInfo, 
                poly1->size, &operationResult);
                
            if (operationResult != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(operationResult));
                free(scalar);
                freePolynomial(poly1);
                exit(EXIT_FAILURE);
            }

            PolynomialErrors opResult = multiplyPolynomial(poly1, scalar, resultPoly);
            free(scalar);
            
            if (opResult != POLYNOMIAL_OPERATION_OK) {
                fprintf(stderr, "%s\n", polynomialErrorToString(opResult));
                freePolynomial(poly1);
                freePolynomial(resultPoly);
                exit(EXIT_FAILURE);
            }

            freePolynomial(poly1);
            poly1 = resultPoly;

        } else {
            fprintf(stderr, "Invalid operation: %s\n", operation);
            freePolynomial(poly1);
            exit(EXIT_FAILURE);
        }
    }

    printf("\nFinal polynomial: ");
    printPolynomial(poly1);
    handleEvaluation(poly1);
    freePolynomial(poly1);
}