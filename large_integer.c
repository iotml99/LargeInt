#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "large_integer.h"

#define MINUS_SIGN '-'

#define A_GREATER_B 1
#define A_LESS_B -1
#define A_EQUAL_B 0

#define NUM_MULTIPLES 10

/** Get max out of two given integers */
static int get_max(int x, int y)
{
    int max = x > y ? x : y;
    return max;
}

/** Right justify number */
static void check_length(large_int_t *x)
{
    while ((x->num_digits > 0) && (x->digits[x->num_digits - 1] == 0))
    {
        x->num_digits--;
    }

    if ((x->num_digits == 0) && (x->digits[0] == 0))
    {
        x->sign = POSITIVE;
    }
}

/** Create large number from a string */
large_int_t *create_large_integer(char *num_str)
{
    int i = 0;
    int length = strlen(num_str);

    large_int_t *ln = (large_int_t *)malloc(sizeof(large_int_t));

    if (num_str[i] == MINUS_SIGN)
    {
        ln->sign = NEGATIVE;
        ln->num_digits = length - 1;
        i++;
    }
    else
    {
        ln->sign = POSITIVE;
        ln->num_digits = length;
    }

    if (ln->num_digits > MAX_DIGITS)
    {
        return NULL;
    }

    memset(ln->digits, 0, MAX_DIGITS);

    for (int j = ln->num_digits - 1; j >= 0; j--, i++)
    {
        if (!isdigit(num_str[i]))
        {
            return NULL;
        }
        ln->digits[j] = num_str[i] - '0';
    }
    check_length(ln);
    return ln;
}

/** Print large number */
void print_large_integer(large_int_t *ln)
{
    printf("\nLargeInt at %p: ", ln);
    if (ln == NULL)
    {
        printf("NULL");
        return;
    }
    if (ln->num_digits == 0)
    {
        printf("0");
        return;
    }
    if (ln->sign == NEGATIVE)
    {
        printf("%c", MINUS_SIGN);
    }
    for (int i = ln->num_digits - 1; i >= 0; i--)
    {
        printf("%d", ln->digits[i]);
    }
}

/** Compare abs value of large numbers */
static int compare_abs(large_int_t *a, large_int_t *b)
{
    check_length(a);
    check_length(b);
    if (a->num_digits > b->num_digits)
    {
        return A_GREATER_B;
    }
    else if (a->num_digits < b->num_digits)
    {
        return A_LESS_B;
    }
    else
    {
        /* Equal Digits */
        for (int i = a->num_digits - 1; i >= 0; i--)
        {
            if (a->digits[i] > b->digits[i])
                return A_GREATER_B;
            else if (a->digits[i] < b->digits[i])
                return A_LESS_B;
        }
    }
    return A_EQUAL_B;
}

/* Assign integer value to large number */
void set_value(large_int_t *a, int val)
{
    memset(a->digits, 0, MAX_DIGITS);
    if (val < 0)
    {
        a->sign = NEGATIVE;
        val = -val;
    }
    else
    {
        a->sign = POSITIVE;
    }
    a->num_digits = 0;
    while (val > 0)
    {
        a->digits[a->num_digits] = val % 10;
        val = val / 10;
        a->num_digits++;
    }
}

/* Copy large number to another */
void copy(large_int_t *src, large_int_t *dst)
{
    memcpy(dst, src, sizeof(large_int_t));
}

/* Compare if two large numbers are equal*/
static bool is_equal(large_int_t *a, large_int_t *b)
{
    if (memcmp(a, b, sizeof(large_int_t)) == 0)
    {
        return true;
    }
    return false;
}

/* Initialize a large number to 0 */
void set_zero(large_int_t *a)
{
    a->sign = POSITIVE;
    a->num_digits = 0;
    memset(a->digits, 0, MAX_DIGITS);
}

/* Shift a large number to the left */
void shift_left(large_int_t *src, int shift, large_int_t *dst)
{
    copy(src, dst);

    if (dst->num_digits == 0)
        return;
    if (shift <= 0)
        return;

    for (int i = dst->num_digits - 1; i >= 0; i--)
        dst->digits[i + shift] = dst->digits[i];

    for (int i = 0; i < shift; i++)
        dst->digits[i] = 0;

    dst->num_digits = dst->num_digits + shift;
}

/* Perform addition */
large_int_t *add(large_int_t *a_op, large_int_t *b_op)
{
    large_int_t *c_res = create_large_integer("0");

    //if both a and b operands have same sign, c will have that sign as well
    if (a_op->sign == b_op->sign)
    {
        c_res->sign = a_op->sign;
    }

    //if a and b operands have different sign, subtraction will be carried out
    //result will have sign of operand with larger absolute value
    else
    {
        int compare_res = compare_abs(a_op, b_op);
        int c_sign;
        if (compare_res == A_GREATER_B)
        {
            c_sign = a_op->sign;
        }
        else if (compare_res == A_LESS_B)
        {
            c_sign = b_op->sign;
        }
        else
        {
            set_value(c_res, 0);
            return c_res;
        }
        if (a_op->sign == NEGATIVE)
        {
            //temporarily change sign
            a_op->sign = POSITIVE;
            // c = b - a
            c_res = sub(b_op, a_op);
            a_op->sign = NEGATIVE;
            c_res->sign = c_sign;
            return c_res;
        }
        else if (b_op->sign == NEGATIVE)
        {
            //temporarily change sign
            b_op->sign = POSITIVE;
            // c = a - b
            c_res = sub(a_op, b_op);
            b_op->sign = NEGATIVE;
            c_res->sign = c_sign;
            return c_res;
        }
    }
    c_res->num_digits = get_max(a_op->num_digits, b_op->num_digits) + 1;

    if (c_res->num_digits > MAX_DIGITS)
    {
        printf("\nERROR!! Max digit limit exceeded");
        return NULL;
    }
    /* sum_i = (a_i + b_i) % 10
       carry_i = (a_i + b_i) / 10 */

    int carry = 0;
    for (int i = 0; i < c_res->num_digits; i++)
    {
        int sum = a_op->digits[i] + b_op->digits[i] + carry;
        c_res->digits[i] = sum % 10;
        carry = sum / 10;
    }
    check_length(c_res);
    return c_res;
}

/* Perfor Subtraction */
large_int_t *sub(large_int_t *a_op, large_int_t *b_op)
{

    large_int_t *c_res = NULL;
    // if a is -ve and b is +ve, result is c = - (abs(a) + abs(b))

    if ((a_op->sign == NEGATIVE) && (b_op->sign == POSITIVE))
    {
        a_op->sign = POSITIVE;
        c_res = add(a_op, b_op);
        a_op->sign = NEGATIVE;
        c_res->sign = NEGATIVE;
        return c_res;
    }

    // If a is +ve and b is -ve result is c = a + b

    else if ((a_op->sign == POSITIVE) && (b_op->sign == NEGATIVE))
    {
        b_op->sign = POSITIVE;
        c_res = add(a_op, b_op);
        b_op->sign = NEGATIVE;
        c_res->sign = POSITIVE;
        return c_res;
    }

    // If a and b have same sign, abs(c) = abs(a - b), sign = sign of op with greater abs value
    c_res = create_large_integer("0");
    int compare_res = compare_abs(a_op, b_op);

    if (compare_res == A_EQUAL_B)
    {
        return c_res;
    }
    if (compare_res == A_LESS_B)
    {

        c_res = sub(b_op, a_op);
        c_res->sign = b_op->sign;
        return c_res;
    }

    int borrow = 0;
    c_res->num_digits = get_max(a_op->num_digits, b_op->num_digits);
    c_res->sign = a_op->sign;
    for (int i = 0; i < c_res->num_digits; i++)
    {
        int diff = a_op->digits[i] - b_op->digits[i] - borrow;
        if (a_op->digits[i] > 0)
            borrow = 0;
        if (diff < 0)
        {
            diff = diff + 10;
            borrow = 1;
        }
        c_res->digits[i] = diff % 10;
    }

    check_length(c_res);

    return c_res;
}

/* Division & Mod internal function */
void div_internal(large_int_t *a, large_int_t *b, large_int_t *q, large_int_t *r)
{
    if (q == NULL)
    {
        q = create_large_integer("0");
    }
    if (r == NULL)
    {
        r = create_large_integer("0");
    }
    int compare_res = compare_abs(a, b);

    if (compare_res == A_LESS_B)
    {
        /*A < B => Q = 0, R = A*/

        set_zero(q);
        copy(a, r);
        return;
    }
    else if (compare_res == A_EQUAL_B)
    {
        /* A == B => Q = 1, R = 0 */

        set_value(q, a->sign * b->sign);
        set_zero(r);
        return;
    }

    large_int_t *one = create_large_integer("1");
    if (is_equal(one, b))
    {
        /* If B is 1, q = A, r = 0 */
        copy(a, q);
        set_zero(r);
        return;
    }

    int a_sign = a->sign;
    int b_sign = b->sign;
    int q_sign = a_sign * b_sign;
    int r_sign = a_sign;

    a->sign = POSITIVE;
    b->sign = POSITIVE;

    large_int_t *a_copy = malloc(sizeof(large_int_t));
    large_int_t *temp;
    copy(a, a_copy);

    while (compare_abs(a_copy, b) != A_LESS_B)
    {
        a_copy = sub(a_copy, b);
        temp = add(q, one);
        copy(temp, q);
    }
    copy(a_copy, r);

    a->sign = a_sign;
    b->sign = b_sign;
    q->sign = q_sign;
    r->sign = r_sign;
}

/* Perform Division */
large_int_t *divide(large_int_t *a, large_int_t *b)
{
    large_int_t *q = create_large_integer("0");
    large_int_t *r = create_large_integer("0");
    div_internal(a, b, q, r);
    return q;
}

/* Perform Mod */
large_int_t *mod(large_int_t *a, large_int_t *b)
{
    large_int_t *q = create_large_integer("0");
    large_int_t *r = create_large_integer("0");
    div_internal(a, b, q, r);
    return r;
}

/* Perform Multiplication */
large_int_t *mul(large_int_t *a, large_int_t *b)
{
    large_int_t *c = create_large_integer("0");
    c->num_digits = a->num_digits + b->num_digits;
    if (c->num_digits > MAX_DIGITS)
    {
        printf("\nERROR!! Max digit limit exceeded");
        return NULL;
    }

    int a_sign = a->sign;
    int b_sign = b->sign;

    int comp_res = compare_abs(a, b);

    large_int_t *a_copy = malloc(sizeof(large_int_t));
    large_int_t *b_copy = malloc(sizeof(large_int_t));

    if (comp_res == A_LESS_B)
    {
        copy(a, b_copy);
        copy(b, a_copy);
    }
    else
    {
        copy(a, a_copy);
        copy(b, b_copy);
    }

    a_copy->sign = POSITIVE;
    b_copy->sign = POSITIVE;

    large_int_t multiples[NUM_MULTIPLES];
    large_int_t *temp, *temp2;

    for (int i = 0; i < NUM_MULTIPLES; i++)
    {
        set_value(&multiples[i], 0);
        for (int j = 0; j < i; j++)
        {
            temp = add(&multiples[i], a_copy);
            copy(temp, &multiples[i]);
        }
    }
    for (int i = b_copy->num_digits - 1; i >= 0; i--)
    {
        int digit = b_copy->digits[i];
        shift_left(&multiples[digit], i, temp);
        temp2 = add(c, temp);
        copy(temp2, c);
        free(temp2);
    }
    c->sign = a_sign * b_sign;

    check_length(c);
    return c;
}