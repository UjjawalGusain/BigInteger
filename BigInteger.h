#ifndef BigInteger_H
#define BigInteger_H

typedef struct Node
{
    int data;
    struct Node *next;
}Node;

typedef struct BigInteger
{
    Node *head, *tail;
    int length, sign;
}BigInteger;

struct BigInteger initialize(char *);
void display(struct BigInteger);
struct BigInteger add(struct BigInteger, struct BigInteger);
struct BigInteger sub(struct BigInteger, struct BigInteger);
struct BigInteger mul(struct BigInteger, struct BigInteger);
struct BigInteger div1(struct BigInteger, struct BigInteger);
struct BigInteger mod(struct BigInteger, struct BigInteger);
struct BigInteger div1(struct BigInteger, struct BigInteger);
#endif