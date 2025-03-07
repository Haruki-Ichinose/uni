#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define STACK_SIZE 10

char stack[STACK_SIZE];
int head_index = -1;

void push(char value)
{
    if (head_index < STACK_SIZE - 1)
    {
        head_index++;
        stack[head_index] = value;
    }
    else
    {
        printf("Stack overflow. Cannot push %c\n", value);
        exit(EXIT_FAILURE);
    }
}

char pop()
{
    if (head_index >= 0)
    {
        char value = stack[head_index];
        head_index--;
        return value;
    }
    else
    {
        printf("Stack underflow. Cannot pop from an empty stack.\n");
        exit(EXIT_FAILURE);
    }
}

int isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int getPrecedence(char operator)
{
    if (operator== '+' || operator== '-')
        return 1;
    else if (operator== '*' || operator== '/')
        return 2;
    else
        return 0;
}

void infixToRPN(char infix[], char rpn[])
{
    int i, j = 0;

    for (i = 0; infix[i] != '\0'; i++)
    {
        if (isdigit(infix[i]))
        {
            rpn[j++] = infix[i];
        }
        else if (infix[i] == '(')
        {
            push(infix[i]);
        }
        else if (infix[i] == ')')
        {
            // 括弧が閉じられるまでスタックから演算子を取り出す
            while (head_index >= 0 && stack[head_index] != '(')
            {
                rpn[j++] = pop();
            }

            // '(' を取り出す
            if (head_index >= 0 && stack[head_index] == '(')
            {
                pop();
            }
            else
            {
                printf("Mismatched parentheses.\n");
                exit(EXIT_FAILURE);
            }
        }
        else if (isOperator(infix[i]))
        {
            while (head_index >= 0 && isOperator(stack[head_index]) &&
                   getPrecedence(stack[head_index]) >= getPrecedence(infix[i]))
            {
                rpn[j++] = pop();
            }
            push(infix[i]);
        }
    }

    while (head_index >= 0)
    {
        rpn[j++] = pop();
    }

    rpn[j] = '\0';
}

int main()
{
    char infixExpression[] = "(3+2)*(10-2)";
    char rpnExpression[STACK_SIZE];

    infixToRPN(infixExpression, rpnExpression);

    printf("Infix Expression: %s\n", infixExpression);
    printf("RPN Expression: %s\n", rpnExpression);

    return 0;
}
