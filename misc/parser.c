#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

typedef struct TokenQueue_prototype
{

    Token *TokenArray;
    int    Front;
    int    Rear;
    int    Count;

} TokenQueue;

typedef struct TokenStack_prototype
{

    int top;
    Token *items;

} TokenStack;

Token Peek(TokenQueue *queue)
{
    return queue->TokenArray[queue->Front];
}

int IsEmpty(TokenQueue *queue)
{
    return queue->Count == 0;
}

int IsFull(TokenQueue *queue)
{
    return queue->Count == 2048;
}

int Size(TokenQueue *queue)
{
    return queue->Count;
}

void Insert(TokenQueue *queue, Token token)
{
    if (!IsFull(queue))
    {
        if (queue->Rear == 2048 - 1) queue->Rear = -1;
        queue->TokenArray[++queue->Rear] = token;
        queue->Count++;
    }
}

Token Remove(TokenQueue *queue)
{
    Token t = queue->TokenArray[queue->Front++];

    if (queue->Front == 2048)
        queue->Front = 0;
    
    queue->Count--;
    return t;
}

int SSize(TokenStack *stack)
{
    return stack->top + 1;
}

int SIsEmpty(TokenStack *stack)
{
    return stack->top == -1;
}

int SIsFull(TokenStack *stack)
{
    return stack->top == 2048 - 1;
}

void SPush(TokenStack *stack, Token element)
{
    if (SIsFull(stack))
        return;
    
    stack->items[++stack->top] = element;
}

Token *SPeek(TokenStack *stack)
{
    if (!SIsEmpty(stack))
        return &(stack->items[stack->top]);
    else return 0;
}

Token Nulltok = {0};

Token SPop(TokenStack *stack)
{
    if (!SIsEmpty(stack))
        return stack->items[stack->top--];
    else
    {
        return Nulltok;
    }
    
}

Token t;

int offset = 0;

int ParsePrimary(TokenStack *Queue, ParseTree *Tree)
{
    if (!Queue || !Tree)
        return 0;
    t = SPop(Queue);

    if (!memcmp(&t, &Nulltok, sizeof(Token)))
        return 1;

    Tree->Left = (ParseTree*) calloc(sizeof(ParseTree), 1);
    Tree->Right = (ParseTree*) calloc(sizeof(ParseTree), 1); 

    Tree->Token = &t;
    Tree->Left->Token = Queue->items;

    if (!ParsePrimary(Queue, Tree->Right)) return 0;

    return 1;
}

int Parse(Token *TokenBuffer, ParseTree *Tree)
{
    if (!TokenBuffer) return 0;
    if (!Tree) if (!(Tree = (ParseTree*) malloc(sizeof(ParseTree)))) return 0;

    Token c;

    TokenStack outputQueue = {0};
    outputQueue.items = (Token*) calloc(sizeof(Token), 2048);
    TokenStack operatorStack = {0};
    operatorStack.items = (Token*) calloc(sizeof(Token), 2048);

    if (!outputQueue.items || !operatorStack.items) return 0;

    for (int i = 0; i < sizeof(TokenBuffer)/sizeof(Token); i++) // pointer arithmetic instead of sizeof
    {
        c = TokenBuffer[i];

        if (c.TokenType == LNUMTOK)
            SPush(&outputQueue, c);
        else if (c.TokenType >= 5)
        {
            while (SPeek(&operatorStack) && (SPeek(&operatorStack)->TokenType <= c.TokenType) /* left paren missing */)
                SPush(&outputQueue, SPop(&operatorStack));
            SPush(&operatorStack, c);
        }
        /* parens */
    }

    

    while (SPeek(&operatorStack))
    {
        SPush(&outputQueue, SPop(&operatorStack));
    }
    
    
    Tree->Left = (ParseTree*) malloc(sizeof(ParseTree));
    Tree->Right = (ParseTree*) malloc(sizeof(ParseTree));

    //      +
    //     / \
    //    1   *
    //       / \
    //      2   /
    //         / \
    //        21  9
    // 1 2 21 9 / * +
    Tree->Left->Token = outputQueue.items;
    Tree->Token = &(outputQueue.items
        [(&outputQueue.items)[1] - outputQueue.items]);
    ParsePrimary(&outputQueue, Tree);

    return 1;
}