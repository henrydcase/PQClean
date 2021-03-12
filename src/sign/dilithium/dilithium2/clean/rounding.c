#include "params.h"
#include "rounding.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_power2round
*
* Description: For finite field element a, compute a0, a1 such that
*              a mod^+ Q = a1*2^D + a0 with -2^{D-1} < a0 <= 2^{D-1}.
*              Assumes a to be standard representative.
*
* Arguments:   - int32_t a: input element
*              - int32_t *a0: pointer to output element a0
*
* Returns a1.
**************************************************/
int32_t PQCLEAN_DILITHIUM2_CLEAN_power2round(int32_t *a0, int32_t a)  {
    int32_t a1;

    a1 = (a + (1 << (D - 1)) - 1) >> D;
    *a0 = a - (a1 << D);
    return a1;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_decompose
*
* Description: For finite field element a, compute high and low bits a0, a1 such
*              that a mod^+ Q = a1*ALPHA + a0 with -ALPHA/2 < a0 <= ALPHA/2 except
*              if a1 = (Q-1)/ALPHA where we set a1 = 0 and
*              -ALPHA/2 <= a0 = a mod^+ Q - Q < 0. Assumes a to be standard
*              representative.
*
* Arguments:   - int32_t a: input element
*              - int32_t *a0: pointer to output element a0
*
* Returns a1.
**************************************************/
int32_t PQCLEAN_DILITHIUM2_CLEAN_decompose(int32_t *a0, int32_t a) {
    int32_t a1 = 0;
    uint64_t r;

    int32_t r0, r1;

    assert(a>0); assert(a<Q);

    // mod ALPHA
    static const uint32_t u = 360800;
    r = ((uint64_t)a)*u;
    r >>= 36;
    r *= 2 * GAMMA2;
    r = a - r;

    if (r>(2*GAMMA2)) {
        r -= 2*GAMMA2;
    }

    r1 = ((int32_t)r)*2*GAMMA2;

    // centrize
    if (r > GAMMA2) {
        *a0 = (int32_t)r - 2*GAMMA2;
    } else {
        *a0 = r;
    }


    // OLD
    a1  = (a + 127) >> 7;
    a1  = (a1 * 11275 + (1 << 23)) >> 24;
    a1 ^= ((43 - a1) >> 31) & a1;

    // CASE: r-r0 = q-1 => r1=0, r0 = r0-1
    uint64_t a2 = (uint64_t)a - *a0;
    if (a2 == (Q-1)) {
        a2 = 0;
        *a0--;
    }

    // divide (r-r0)/alpha
    // int32_t a2 = ((uint64_t)a-*a0)/(2*GAMMA2);
    if ( (a2 >= (2*GAMMA2))) {
        a2 = (a2*u) >> 36;
        // a2 is divisible by ALPHA=(2*GAMMA2) and hence
        // it will always be off by one.
        a2++;
    }



    //if (!a1) a2 = a1;

    //*a0  = a - a1 * 2 * GAMMA2;
    //*a0 -= (((Q - 1) / 2 - *a0) >> 31) & Q;
    if (a1 != (int32_t)a2)
        printf("OZAPTF: (A1=%d, A2=%d, A=%d R=%d)\n",
            a1, (int32_t)a2, a, (a-(*a0)));
//    printf("OZAPTF: %d %d %d\n", a, *a0, (a-*a0));
    return a2;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_make_hint
*
* Description: Compute hint bit indicating whether the low bits of the
*              input element overflow into the high bits.
*
* Arguments:   - int32_t a0: low bits of input element
*              - int32_t a1: high bits of input element
*
* Returns 1 if overflow.
**************************************************/
unsigned int PQCLEAN_DILITHIUM2_CLEAN_make_hint(int32_t a0, int32_t a1) {
    if (a0 > GAMMA2 || a0 < -GAMMA2 || (a0 == -GAMMA2 && a1 != 0)) {
        return 1;
    }

    return 0;
}

/*************************************************
* Name:        PQCLEAN_DILITHIUM2_CLEAN_use_hint
*
* Description: Correct high bits according to hint.
*
* Arguments:   - int32_t a: input element
*              - unsigned int hint: hint bit
*
* Returns corrected high bits.
**************************************************/
int32_t PQCLEAN_DILITHIUM2_CLEAN_use_hint(int32_t a, unsigned int hint) {
    int32_t a0, a1;

    a1 = PQCLEAN_DILITHIUM2_CLEAN_decompose(&a0, a);
    if (hint == 0) {
        return a1;
    }

    if (a0 > 0) {
        if (a1 == 43) {
            return 0;
        }
        return a1 + 1;
    }
    if (a1 == 0) {
        return 43;
    }
    return a1 - 1;
}
