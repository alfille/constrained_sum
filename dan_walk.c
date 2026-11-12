#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

int N = 5 ; // number of values

int S = 163 ; // Sum of values

int R = 10 ; // number of random passes

struct test {
    int plus ;
    int minus ;
    int val;
    int a[] ; // flexible array member
} ;

struct test * test_alloc( int plus, int minus ) {
    struct test * t = malloc( sizeof( struct test ) + N * sizeof( int ) ) ;
    if ( t ) {
        t->plus = plus ;
        t->minus = minus ;
        t->val = 0 ; // always lose
    }
    return t ;
}

void test_free( struct test * t ) {
    if ( t ) {
        free( t ) ;
    }
}

void test_copy( struct test * from, struct test * to ) {
    for ( int i = 0 ; i < N ; ++i ) {
        to->a[i] = from->a[i] ;
    }
    to->val = from->val ;
}

void test_value( struct test * t ) {
    if ( t ) {
        int val = 0 ;
        for ( int i = 0 ; i < N-1 ; ++i ) {
            val += t->a[i] * t->a[i+1] ;
        }
        t->val = val ;
    }
}

void test_randomize( struct test * t ) {
    for ( int i=0 ; i<S ; ++i ) {
        if ( i<N ) {
            t->a[i] = 1 ; // minimum value
        } else {
            int r = rand() % N ;
            ++ t->a[r] ;
        }
    }
}

void test_show( struct test * t ) {
    printf("\t( ");
    for ( int i = 0; i < N-1 ; ++i ) {
        printf( "%d, ", t->a[i] ) ;
    }
    printf("%d ) -> %d\n", t->a[N-1],t->val);
}

bool test_compare( struct test * current, struct test * candidate ) { 
    if ( candidate->val > current->val ) {
        test_copy( candidate, current ) ;
        return true ;
    }
    return false ;
}

void test_perturb( struct test * current, struct test * candidate ) {
    if ( current->a[candidate->minus] == 1 ) {
        candidate->val = 0 ; //loser
    } else {
        test_copy( current, candidate ) ;
        --candidate->a[candidate->minus] ;
        ++candidate->a[candidate->plus] ;
        test_value( candidate ) ;
    }
}

void test_run() {
    struct test * current = test_alloc(0,0) ;
    int NN = N * (N-1) ;
    struct test * trials[NN] ;

    // set up pertubations
    int counter = 0 ;
    for ( int p=0 ; p<N ; ++p ) {
        for ( int m=0 ; m<N ; ++m ) {
            if ( p!=m) {
                trials[counter] = test_alloc(p,m) ;
                ++ counter ;
            }
        }
    }
    
    // set up initial random
    test_randomize( current ) ;
    test_value( current ) ;
    //printf("Start") ;
    //test_show( current ) ;
    
    // loop through passes
    int passes = 0 ; 
    while( true ) {
        ++ passes ;
        int best_index = -1 ; // no index yet
        int best_val = current->val ;
        for ( int n=0 ; n<NN ; ++n ) {
            test_perturb( current, trials[n] ) ;
            if ( trials[n]->val > best_val ) {
                best_index = n ;
                best_val = trials[n]->val ;
            }
        }
        if ( best_index == -1 ) {
            printf("Solution (%d passes)\n",passes);
            test_show( current ) ;
            return ;
        }
        test_copy( trials[best_index] , current ) ;
    }
}

void help() {
    printf("constrained_sum: steepest gradient approach\n");
    printf("\tfind the best choice of N positive integers that:\n");
    printf("\t\tsUM TO S\n");
    printf("\t\tmaximize the function:\n");
    printf("\t\tab + bc + cd + de\n");
    printf("\tby Paul H Alfille 2026 -- MIT Licence\n");
    printf("\tSee https://github.com/alfille/constrained_sum\n");
    printf("\n");
    printf("steep_sum [options]\n");
    printf("\n");
    printf("Options\n");
    printf("\t-n | --number 5\t-- number of terms\n");
    printf("\t-s | --sum 63\t-- sum of terms\n");
    printf("\t-p | --passes 10\t-- repeat solution (random start)\n");
    printf("\t-c | --csv\t--Comma Separated Value output for spreadsheet\n",SIM_TYPE);
    printf("\t-h\t--help\t\tthis help\n");
    printf("\n");
    exit(1);
}

struct option long_options[] =
{
    {"passes" ,   required_argument, 0, 'p'},
    {"sum"    ,   required_argument, 0, 's'},
	{"number" ,   required_argument, 0, 'n'},
	{"csv"    ,   no_argument,       0, 'c'},
    {"help"   ,   no_argument,       0, 'h'},
    {0        ,   0          ,       0,   0}
};

void main( void ) {
    srand((unsigned int)time(NULL)); // seed random
    for ( int r = 0 ; r<R ; ++r ) {
        test_run() ;
    }
}

    
