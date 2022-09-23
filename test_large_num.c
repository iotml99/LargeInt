/*** This program tests functions defined in large_integer*/
#include <stdio.h>
#include <stdlib.h>
#include "test_large_num.h"
#include "large_integer.h"

static int assert_result(large_int_t *actual, large_int_t *expected)
{
    if (actual->sign != expected->sign)
        return FAILURE;
    if (actual->num_digits != expected->num_digits)
        return FAILURE;
    for (int i = 0; i < actual->num_digits; i++)
    {
        if (actual->digits[i] != expected->digits[i])
            return FAILURE;
    }
    return SUCCESS;
}

static int test_addition(void)
{
    int pass_count = 0;
    large_int_t *a, *b, *c, *r;
    for (int i = 0; i < NUM_TEST; i++)
    {
        printf("\n**********ADD Test %d**********", i + 1);
        a = create_large_integer(input_A[i]);
        b = create_large_integer(input_B_add[i]);
        r = create_large_integer(addition_results[i]);
        c = add(a, b);
        printf("\nOperands:");
        print_large_integer(a);
        print_large_integer(b);
        printf("\nResult:");
        print_large_integer(c);
        if (assert_result(c, r) == SUCCESS)
        {
            printf("\nSuccess! Test Passed");
            pass_count++;
        }
        else
        {
            printf("\nERROR!! Test Failed");
        }
        free(a);
        free(b);
        free(c);
        free(r);
    }
    return pass_count;
}

static int test_subtraction(void)
{
    int pass_count = 0;
    large_int_t *a, *b, *c, *r;
    for (int i = 0; i < NUM_TEST; i++)
    {
        printf("\n**********Subtract Test %d**********", i + 1);
        a = create_large_integer(input_A[i]);
        b = create_large_integer(input_B_sub[i]);
        r = create_large_integer(subtraction_result[i]);
        c = sub(a, b);
        printf("\nOperands:");
        print_large_integer(a);
        print_large_integer(b);
        printf("\nResult:");
        if (assert_result(c, r) == SUCCESS)
        {
            printf("\nSuccess! Test Passed");
            pass_count++;
        }
        else
        {
            printf("\nERROR!! Test Failed");
        }
        free(a);
        free(b);
        free(c);
        free(r);
    }
    return pass_count;
}

static int test_division(void)
{
    int pass_count = 0;
    large_int_t *a, *b, *c, *r;
    for (int i = 0; i < NUM_TEST; i++)
    {
        printf("\n**********Division Test %d**********", i + 1);
        a = create_large_integer(input_A[i]);
        b = create_large_integer(input_B_div[i]);
        r = create_large_integer(division_results[i]);
        c = divide(a, b);
        printf("\nOperands:");
        print_large_integer(a);
        print_large_integer(b);
        printf("\nResult:");
        if (assert_result(c, r) == SUCCESS)
        {
            printf("\nSuccess! Test Passed");
            pass_count++;
        }
        else
        {
            printf("\nERROR!! Test Failed");
        }
        free(a);
        free(b);
        free(c);
        free(r);
    }
    return pass_count;
}

static int test_mod(void)
{
    int pass_count = 0;
    large_int_t *a, *b, *c, *r;
    for (int i = 0; i < NUM_TEST; i++)
    {
        printf("\n**********Mod Test %d**********", i + 1);
        a = create_large_integer(input_A[i]);
        b = create_large_integer(input_B_mod[i]);
        r = create_large_integer(mod_results[i]);
        c = mod(a, b);
        printf("\nOperands:");
        print_large_integer(a);
        print_large_integer(b);
        printf("\nResult:");
        print_large_integer(c);
        if (assert_result(c, r) == SUCCESS)
        {
            printf("\nSuccess! Test Passed");
            pass_count++;
        }
        else
        {
            printf("\nERROR!! Test Failed");
        }
        free(a);
        free(b);
        free(c);
        free(r);
    }
    return pass_count;
}

static int test_mul(void)
{
    int pass_count = 0;
    large_int_t *a, *b, *c, *r;
    for (int i = 0; i < NUM_TEST; i++)
    {
        printf("\n**********Multiply Test %d**********", i + 1);
        a = create_large_integer(input_A[i]);
        b = create_large_integer(input_B_mul[i]);
        r = create_large_integer(mul_results[i]);
        c = mul(a, b);
        printf("\nOperands:");
        print_large_integer(a);
        print_large_integer(b);
        printf("\nResult:");
        print_large_integer(c);
        if (assert_result(c, r) == SUCCESS)
        {
            printf("\nSuccess! Test Passed");
            pass_count++;
        }
        else
        {
            printf("\nERROR!! Test Failed");
        }
        free(a);
        free(b);
        free(c);
        free(r);
    }
    return pass_count;
}

void main()
{
    printf("\n********Large Integer Test Begin**********");
    int pass_add = test_addition();
    int pass_sub = test_subtraction();
    int pass_div = test_division();
    int pass_mod = test_mod();
    int pass_mul = test_mul();

    printf("\n********Test Result Summary**********");
    printf("\nAdd Passed %d out of 5", pass_add);
    printf("\nSub Passed %d out of 5", pass_sub);
    printf("\nDiv Passed %d out of 5", pass_div);
    printf("\nMod Passed %d out of 5", pass_mod);
    printf("\nSub Passed %d out of 5", pass_mul);

    printf("\n********Large Integer Test End**********");
}