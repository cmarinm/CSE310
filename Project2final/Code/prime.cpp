//
// Created by Cesar on 10/10/2016.
//
#include "prime.h"

bool prime::TestForPrime(int val) {
    int limit, factor = 2;

    limit = (long)( sqrtf( (float) val ) + 0.5f );
    while( (factor <= limit) && (val % factor) )
        factor++;

    return( factor > limit );
}