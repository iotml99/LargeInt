/**
 * @file large_integer.h
 * @version 0.1
 * @date 2021-01-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __LARGE_INTEGER_H_
#define __LARGE_INTEGER_H_

#include <stdio.h>
#include <stdint.h>

/* Supports 256 digits */
#define MAX_DIGITS 256

#define POSITIVE 1
#define NEGATIVE -1

/* Large number is stored in form of an array of digits, least significant first, which are zeropadded to the right.
An n digit number has 256-n zeroes padded to the right. 
num_digit identifies the number of valid digits in a number. 
Sign identifies the sign multiplier : 1 for positive, -1 for negative */

typedef struct large_integer
{
    unsigned int num_digits;
    char digits[MAX_DIGITS];
    int8_t sign;
} large_int_t;

/*********************Library Function Prototypes***************************/
large_int_t *create_large_integer(char *num_str);
void print_large_integer(large_int_t *ln);
large_int_t *add(large_int_t *a, large_int_t *b);
large_int_t *sub(large_int_t *a, large_int_t *b);
large_int_t *divide(large_int_t *a, large_int_t *b);
large_int_t *mod(large_int_t *a, large_int_t *b);
large_int_t *mul(large_int_t *a, large_int_t *b);
/***************************************************************************/

#endif