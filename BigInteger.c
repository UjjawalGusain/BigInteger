#include "BigInteger.h"
#include<stdio.h>
#include<stdlib.h>

Node *newNode(int data)
{
    Node *n = (Node *)malloc(sizeof(Node));
    if(!n)  return NULL;

    n -> data = data;
    n -> next = NULL;
    return n;
}

struct BigInteger emptyInitialize()
{
    struct BigInteger num;
    num.head = NULL, num.tail = NULL;
    num.sign = 1;
    return num;
}

struct BigInteger initialize(char *s)
{
    struct BigInteger num;
    Node *head = NULL, *temp = NULL;
    int i = 0;

    if(s[i] == '-')
    {
        num.sign = 0;
        i++;
    }
    else
        num.sign = 1;
    temp = newNode(s[i] - '0');
    temp -> next = head;
    head = temp;
    i++;
    while(s[i] != '\0')
    {
        temp = newNode(s[i] - '0');
        temp -> next = head;
        head = temp;
        i++;
    }
    num.head = head;
    if(num.sign == 1)
        num.length = i;
    else num.length = i-1;

    return num;
}

void recursiveDisplay(Node *temp)
{
    if(!temp)
    {
        return;
    }

    recursiveDisplay(temp -> next);
    printf("%d", temp -> data);
}

void display(struct BigInteger a)
{
    Node *temp = a.head;
    if(!a.sign) 
        printf("-");
    recursiveDisplay(temp);
    printf("\n");
}

Node *reverse(Node *head)
{
    Node *prev = NULL, *curr = head, *next = NULL;

    while(curr)
    {
        next = curr -> next;
        curr -> next = prev;
        prev = curr;
        curr = next;
    }

    head = prev;

    return head;
}

int calculateLength(struct BigInteger *num)
{
    int len = 0;
    Node *temp = num -> head;
    while(temp)
    {
        len++;
        temp = temp -> next;
    }
    return len;
}

int isGreaterOrEqual(struct BigInteger a, struct BigInteger b)
{
    //returns 2 for equal  
    //returns 1 for a>b
    //returns 0 for b > a

    if(a.length > b.length) return 1;
    else if(b.length > a.length) return 0;

    a.head = reverse(a.head);
    b.head = reverse(b.head);
    int flag = 0;
    Node *temp1 = a.head, *temp2 = b.head;
    while(temp1)
    {
        if(temp1 -> data > temp2 -> data)
        {
            flag = 1;
            break;
        }
        else if(temp2 -> data > temp1 -> data)
        {
            flag = -1;
            break;
        }
        temp1 = temp1 -> next;
        temp2 = temp2 -> next;
    }

    a.head = reverse(a.head);
    b.head = reverse(b.head);

    if(flag == 1)
        return 1;
    else if(flag == 0)
        return 2;
    return 0;
}

struct BigInteger add(struct BigInteger a, struct BigInteger b)
{
    if((!a.sign && b.sign) || (a.sign && !b.sign))
    {   
        int sign;
        struct BigInteger temp;
        if(isGreaterOrEqual(a, b))
            sign = a.sign;
        else
            sign = b.sign;
        a.sign = b.sign = 1;
        temp = sub(a, b);
        temp.sign = sign;
        return temp;
    }

    Node *temp1 = a.head, *temp2 = b.head;
    struct BigInteger num;
    Node *res = NULL;
    int carry = 0, val = 0, data = 0, size = 0;

    while(temp1 && temp2)
    {
        val = (temp1 -> data)  + (temp2 -> data) + carry;

        carry = val/10;
        data = val%10;
        Node *n = newNode(data);

        n -> next = res;
        res = n;

        temp1 = temp1 -> next;
        temp2 = temp2 -> next;
        size++;
    }
    while(temp1)
    {
        val = (temp1 -> data) + carry;
        carry = val/10;
        data = val%10;

        Node *n = newNode(data);
        n -> next = res;
        res = n;

        temp1 = temp1 -> next;
        size++;
    }
    while(temp2)
    {
        val = (temp2 -> data) + carry;
        carry = val/10;
        data = val%10;

        Node *n = newNode(data);
        n -> next = res;
        res = n;

        temp2 = temp2 -> next;
        size++;
    }
    if(carry)
    {
        Node *n = newNode(carry);
        n -> next = res;
        res = n;
        size++;
    }

    num.head = reverse(res);
    num.length = size;
    if(a.sign && b.sign) num.sign = 1;
    else num.sign = 0;

    return num;
}

Node *removeLeadingZeroes(Node *headMSB, int *i)
{
    Node *temp = headMSB;

    while(temp && temp -> next && !(temp -> data))
    {
        Node *deleteNode = temp;
        temp = temp -> next;
        free(deleteNode);
        (*i)--;
    }
    return temp;
}

struct BigInteger sub(struct BigInteger a, struct BigInteger b)
{
    if(a.sign && !b.sign)
    {
        b.sign = 1;
        return add(a, b);
    }
    else if(!a.sign && b.sign)
    {
        b.sign = 0;
        return add(a, b);
    }
    struct BigInteger num;
    num.sign = 1;
    if(!isGreaterOrEqual(a, b))
    {
        struct BigInteger temp = a;
        a = b;
        b = temp;
        num.sign = 0;
    }

    Node *temp1 = a.head, *temp2 = b.head, *head = NULL;
    int val = 0, borrow = 0, data = 0, i = 0;
    while(temp1 && temp2)
    {
        int d1 = temp1 -> data + borrow, d2 = temp2 -> data;

        if(d1 < d2)
        {
            data = (d1 + 10) - d2;
            borrow = -1;
        }
        else
        {
            data = d1 - d2;
            borrow = 0;
        }

        Node *n = newNode(data);
        n -> next = head;
        head = n;
        temp1 = temp1 -> next;
        temp2 = temp2 -> next;   
        i++;
    }
    while(temp1)
    {
        int d1 = temp1 -> data + borrow;

        if(d1 < 0)
        {
            d1 = 9;
            borrow = -1;
        }
        else
            borrow = 0;

        Node *n = newNode(d1);
        n -> next = head;
        head = n;
        temp1 = temp1 -> next;
        i++;
    }
    head = removeLeadingZeroes(head, &i);
    head = reverse(head);
    num.head = head;
    num.length = i;

    return num;
}

Node *appendDigitLSB(Node *headLSB, int digit)
{
    Node *n = newNode(digit);
    n -> next = headLSB;
    headLSB = n;
    return headLSB;
}

void freeLinkedList(Node *head)
{
    Node *deleteNode;
    while(head)
    {
        deleteNode = head;
        head = head -> next;
        free(deleteNode);
    }
}

Node *multiplyByDigit(Node *head, int digit)
{
    int data = 0, val = 0, carry = 0;
    Node *temp = head, *res = NULL;
    while(temp)
    {
        val = (temp -> data * digit) + carry;
        carry = val/10;
        data = val%10;

        Node *n = newNode(data);
        n -> next = res;
        res = n;
        temp = temp -> next;
    }
    if(carry)
    {
        Node *n = newNode(carry);
        n -> next = res;
        res = n;
    }

    res = reverse(res);
    return res;
}

Node *sumLinkedList(Node *head1, Node *head2)
{
    Node *temp1 = head1, *temp2 = head2;
    Node *res = NULL;
    int carry = 0, val = 0, data = 0, size = 0;

    while(temp1 && temp2)
    {
        val = (temp1 -> data)  + (temp2 -> data) + carry;

        carry = val/10;
        data = val%10;
        Node *n = newNode(data);

        n -> next = res;
        res = n;

        temp1 = temp1 -> next;
        temp2 = temp2 -> next;
        size++;
    }
    while(temp1)
    {
        val = (temp1 -> data) + carry;
        carry = val/10;
        data = val%10;

        Node *n = newNode(data);
        n -> next = res;
        res = n;

        temp1 = temp1 -> next;
        size++;
    }
    while(temp2)
    {
        val = (temp2 -> data) + carry;
        carry = val/10;
        data = val%10;

        Node *n = newNode(data);
        n -> next = res;
        res = n;

        temp2 = temp2 -> next;
        size++;
    }
    if(carry)
    {
        Node *n = newNode(carry);
        n -> next = res;
        res = n;
        size++;
    }

    res = reverse(res);
    return res;
}

struct BigInteger mul(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger num;
    Node *res = NULL;
    Node *head1 = a.head, *head2 = b.head;
    int i = 1;
    res = multiplyByDigit(head1, head2 -> data);

    head2 = head2 -> next;

    while(head2)
    {
        Node *secondLL = multiplyByDigit(head1, head2 -> data);
        for(int appendTimes = 0; appendTimes < i; appendTimes++)
            secondLL = appendDigitLSB(secondLL, 0);
        res = sumLinkedList(res, secondLL);
        freeLinkedList(secondLL);
        head2 = head2 -> next;
        i++;
    }
    num.length = i;
    res = reverse(res);
    res = removeLeadingZeroes(res, &i);
    res = reverse(res);

    num.head = res;
    if((a.sign && b.sign) || (!a.sign && !b.sign))
        num.sign = 1;
    else
        num.sign = 0;
    num.length = calculateLength(&num);
    return num;
}

struct BigInteger plusOne(struct BigInteger a)
{
    struct BigInteger temp;
    temp.head = newNode(1);
    temp.length = 1;
    temp.sign = 1;
    a = add(a, temp);
    a.sign = 1;
    return a;
}

struct BigInteger divRepeatedAddition(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger res;
    if(b.head && b.head -> data == 0 && !(b.head -> next))
    {
        res.head = NULL;
        res.length = 0;
        res.sign = 1;
        printf("0 division error!\n");
        return res;
    }
    int sign = 0;
    if((a.sign == b.sign)) 
        sign = 1;
    res.head = newNode(0);
    a.sign = 1, b.sign = 1;
    if(isGreaterOrEqual(b, a) == 1) 
    {
        res.head = newNode(0);
        res.sign = 1;
        return res;
    }
    while(isGreaterOrEqual(a, b))
    {
        a = sub(a, b);
        res = plusOne(res);
    }
    res.sign = sign;
    res.length = calculateLength(&res);
    return res;
}

struct BigInteger mod(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger res = div1(a, b);
    res = sub(a, mul(b, res));
    return res;
}

void addZeroes(struct BigInteger *num, int n)
{
    Node *head = num -> head;

    for(int i = 0; i<n; i++)
    {
        Node *n = newNode(0);
        n -> next = head;
        head = n;
        num->length++;
    }

    num -> head = head;
}

struct BigInteger div1(struct BigInteger a, struct BigInteger b)
{
    struct BigInteger res, copyB = b;

    if(b.head && b.head -> data == 0 && !(b.head -> next))
    {
        res.head = NULL;
        res.length = 0;
        res.sign = 1;
        printf("0 division error!\n");
        return res;
    }
    int sign = 0;
    if((a.sign == b.sign)) 
        sign = 1;
    res.head = newNode(0);
    a.sign = 1, b.sign = 1;
    if(isGreaterOrEqual(b, a) == 1) 
    {
        res.head = newNode(0);
        res.sign = 1;
        return res;
    }

    //Base case for recursion
    int len = a.length - b.length;
    if(len < 2)
        return divRepeatedAddition(a, b);
    // Making the denominator of less or equal to numerator in length
    addZeroes(&copyB, len);

    //Condition if the value of denominator goes beyond numerator
    if(isGreaterOrEqual(copyB, a))
    {
        Node *temp = copyB.head;
        copyB.head = temp -> next;
        copyB.length--;
        len--;
        free(temp);
    }

    //Division by recursion
    struct BigInteger temp = copyB;
    copyB = div1(a, copyB);

    //Adding appropriate number of zeroes after the value
    addZeroes(&copyB, len);

    //Multiply result of the part quotient which is then subtracted by numberator
    struct BigInteger multiResult = mul(copyB, b);
    multiResult = sub(a, multiResult);

    //Division by recursion to optimize the complexity
    temp = div1(multiResult, b);
    copyB = add(copyB, temp);
    copyB.sign = sign;
    copyB.length = calculateLength(&copyB);
    return copyB;
}

