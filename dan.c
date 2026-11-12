#include <stdio.h>
#include <stdlib.h>

struct test {
    int a;
    int b;
    int c;
    int prod;
} ;

#define MAX 63

struct test trial ;
struct test max ;

void multiply( struct test * t ) {
    t->prod = (t->a)*(t->b) + (t->a)*(t->c) + (t->b)*(t->c) ;
    printf( "prod: %d\n", t->prod ) ;
}     

void show( struct test * t ) {
    printf( "\t( a=%d, b=%d, c=%d ) = %d\n", t->a,t->b,t->c,t->prod ) ;
}

/* return 0=ok, 1=fail */
int increment_b() {
    (trial.b)++ ;
    trial.c = MAX - trial.b - trial.a ;
    multiply( &trial ) ;
    show( &trial ) ;
    return trial.c >0 ;
}

/* return 0=ok, 1=fail */
int increment() {
    if ( increment_b() ) {
        return 0 ; // ok
    }
    // increment a
    (trial.a)++ ;
    trial.b = 0 ;
    return increment_b() ;
}

int start() {
    /* trials will always be higher */
    max.prod = 0 ;
    
    /* initial trial */
    trial.a = 1 ;
    trial.b = 0 ;
    return increment_b() ; // ignore return value
}

void test_trial( void ) {
    if ( trial.prod > max.prod ) {
        max.a = trial.a ;
        max.b = trial.b ;
        max.c = trial.c ;
        multiply( &max ) ;
    }
}

void main( void ) {
    start() ;
    test_trial() ;
    
    while ( increment() ) {
        test_trial() ;
    }

    printf( "Solution\n") ;
    show( &max ) ;
}

    
