/*
Name:
Student ID: 
Assignment 2, Question 2
*/

#include "q2.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * Pushes a new value onto the stack.
 * 
 * @param stack Pointer to the pointer of the stack's head.
 * @param value The value to be added to the stack.
 */
void push(Operand **stack, double value) {
    Operand *new_node = (Operand *)malloc(sizeof(Operand));

    if (!new_node) {
        printf("Memory allocation failed!\n");
        return;
    }

    new_node->value = value;
    new_node->next = *stack;
    *stack = new_node;
}

/**
 * Pops the top value from the stack and returns it.
 *
 * @param stack Pointer to the pointer of the stack's head.
 * @return The value that was at the top of the stack.
 */
double pop(Operand **stack) {
    if (*stack == NULL) {
        printf("Error: Stack is empty, cannot pop.\n");
        exit(1); // Exit since this shouldn't happen in valid postfix expressions
    }

    Operand *top = *stack;
    double value = top->value;
    *stack = top->next;
    free(top);
    return value;
}

/**
 * Prints all values in the stack.
 *
 * @param stack Pointer to the stack's head.
 */
void print(Operand *stack) {
    if (stack == NULL) {
        printf("Stack is empty.\n");
        return;
    }

    Operand *current = stack;
    printf("Stack contents: ");
    while (current != NULL) {
        printf("%f ", current->value);
        current = current->next;
    }
    printf("\n");
}
