#include <stdio.h>
#include "bit_ops.h"

/* Returns the Nth bit of X. Assumes 0 <= N <= 31. */
unsigned get_bit(unsigned x, unsigned n) {
    /* YOUR CODE HERE */
    unsigned mask = 1 << n;
    return (x & mask) >> n; 
}

/* Set the nth bit of the value of x to v. Assumes 0 <= N <= 31, and V is 0 or 1 */
void set_bit(unsigned *x, unsigned n, unsigned v) {
    /* YOUR CODE HERE */
    unsigned bit = get_bit(*x,n);
    // if bit == 1, v == 1  (~v) << n
    //    bit == 1, v == 0  (~v) << n
    //    bit == 0, v == 1  v << n
    //    bit == 0, v == 0  v << n
    unsigned mask = (bit ^ v) << n;
    *x = mask ^ *x;
    return;
}

/* Flips the Nth bit in X. Assumes 0 <= N <= 31.*/
void flip_bit(unsigned *x, unsigned n) {
    /* YOUR CODE HERE */
    unsigned origin = get_bit(*x,n);
    unsigned flip = (~origin) & 1;
    set_bit(x,n,flip);
}

