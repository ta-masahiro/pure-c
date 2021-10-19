#include "general.h"
//   fermat : fermat法を使い素因数分解を行う
//
//   n 素因数分解を行う対象の数
//   a 発見された因数
//
void fermat(mpz_t n, mpz_t a) {
    mpz_t b2; mpz_init(b2);
    if (mpz_tstbit(n,0) ==0)                // nが偶数の時fermat法は止まらない
        {mpz_set_ui(a,2);
        return;}
    mpz_sqrt(a, n);                         // a=√n
    mpz_mul(b2,a,a); mpz_sub(b2,b2,n);      // b2=a*a-n
    while (mpz_perfect_square_p(b2) ==0 ) { // b2が平方数でない限り
        mpz_add_ui(a,a,1);                  // a =a+1
        mpz_mul(b2,a,a); mpz_sub(b2, b2, n);// b2 =a*a-n
    }
    mpz_sqrt(b2,b2);mpz_sub(a,a,b2);        // a=a-√b2
}
//   pollard_pm1 : ポラードの p-1 法を使い素因数分解を行う
//
//   n 素因数分解を行う対象の数
//   g 発見された因数
//   b 底(初期値として与える)
//   maxC 最大処理回数(指数の最大値 + 1)
//
//   戻り値 : 素因数分解に成功したらTRUEを返す
int pollard_pm1(mpz_t n, mpz_t g, unsigned long b, unsigned long maxC ) {
    mpz_t m; mpz_init_set_ui(m, b); 
    mpz_t mm1; mpz_init(mm1); 

    for ( unsigned long c = 0 ; c < maxC ; ++c ) {
        mpz_powm_ui(m, m, c + 1, n); 
        mpz_sub_ui(mm1, m, 1); 
        mpz_gcd(g, mm1, n); 
        if (mpz_cmp_ui(g, 1) == 0) continue; 
        if (mpz_cmp(g, n) == 0) return FALSE; 
        return TRUE;
    }
    return FALSE;
}
//   pollard_rho : ポラードのロー 法を使い素因数分解を行う
//
//   n 素因数分解を行う対象の数
//   d 発見された因数
//   x 初期値
//   cc 関数の初期パラメータ
//   max  最大処理回数
//   maxC 最大関数変更回数
//
//   戻り値 : 素因数分解に成功したらTRUEを返す
int pollard_rho(mpz_t n, mpz_t d, mpz_t x, mpz_t cc, unsigned long max, unsigned long maxc) {
    mpz_t y;mpz_init_set(y, x);;
    unsigned long trial, c;
    mpz_set_ui(d,1);
    for( c=1;c <=maxc; c++) {
        trial = 0;
        while (mpz_cmp_ui(d,1)==0 &&  trial++ < max ) {
            mpz_mul(x,x,x);mpz_add(x,x,cc);mpz_tdiv_r(x,x,n);
            mpz_mul(y,y,y);mpz_add(y,y,cc);mpz_tdiv_r(y,y,n);
            mpz_mul(y,y,y);mpz_add(y,y,cc);mpz_tdiv_r(y,y,n);
            mpz_sub(d,x,y);mpz_abs(d,d);
            mpz_gcd(d,d,n);
        }
        if (mpz_cmp(d,n) != 0 && trial<max) return TRUE;
        mpz_urandomm(cc,  RAND_STATE, n);
        mpz_urandomm(x,  RAND_STATE, n);
    }
    return FALSE;
}
