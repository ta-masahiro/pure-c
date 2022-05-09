#include "array.h"
/*
array * array_init(unsigned long size) {
    array * a = (array*)malloc(sizeof(array));
    void ** table = (void**)malloc(size*sizeof(void));
    a->size = size; a->sp = 0; a->table = table;
    return a;
}

i8_array * i8_array_init(unsigned long size) {
    i8_array * a = (i8_array*)malloc(sizeof(array));
    unsigned char * table = (unsigned char *)malloc(size*sizeof(char));
    a->size = size; a->sp = 0; a->table = table;
    return a;
}

array_2 * array_2_init(unsigned int xsize, unsigned int ysize) {
    array_2 * a = (array_2*)malloc(sizeof(array_2));
    void ** table = (void **)malloc(xsize*ysize*sizeof(void*));
    a->xsize = xsize; a->ysize = ysize;
    a->xsp = 0; a->ysp =0;
    a->table = table;
    return a;
}
*/
array * array_init(unsigned int type, unsigned int dim, unsigned int * sizes) {
    unsigned int size  = 1,i;
    int * ss = (int *)malloc(dim*sizeof(int));
    int * sp = (int *)malloc(dim*sizeof(int)); 
    for(i = 0; i < dim; i ++ ) {
        size *= sizes[i]; 
        ss[i] = sizes[i];
        sp[i] = sizes[i];
    }
    array * a = (array * )malloc(sizeof(array)); 
    if (type == OBJ_CMPLX) a->table._cmplx = (complex*)malloc(size*sizeof(complex));
    else a->table._ptr = (void**)malloc(size * sizeof(void * ));
    a -> type = type; a -> dim = dim; a -> sizes = ss;a->sps =sp;
    //printf("type:%d dim :%d size:", type, dim);for(i=0;i< dim ;i++) printf(" %d",sizes[i]);
    //printf("total size:%d",size);printf("\n");
    return a;    
}
/*
array * array_init(unsigned int type, unsigned int dim, unsigned int * sizes) {
    unsigned int size  = 1,i;
    int * ss = (int *)malloc(dim*sizeof(int));
    int * sp = (int *)malloc(dim*sizeof(int)); 
    for(i = 0; i < dim; i ++ ) {
        size *= sizes[i]; 
        ss[i] = sizes[i];
        sp[i] = sizes[i];
    }
    array * a = (array * )malloc(sizeof(array)); 
    void ** table = (void**)malloc(size * sizeof(void * ));
    a -> type = type; a -> dim = dim; a -> sizes = ss; a -> table = table; a->sps =sp;
    //printf("type:%d dim :%d size:", type, dim);for(i=0;i< dim ;i++) printf(" %d",sizes[i]);
    //printf("total size:%d",size);printf("\n");
    return a;    
}
*/
/*
void * array_ref(array * a, unsigned long idx) {
    return a->table[idx];
}

void * array_2_ref(array_2 *a, unsigned int xid, unsigned int yid) {
    return a->table[xid*a->xsize + yid];
}
*/
void * array_ref(array * a, int * ind) {
    unsigned int index = ind[0]; 
    for(int i =1 ; i < a -> dim; i ++ ) {
        index = (index * a -> sizes[i - 1]) + ind[i]; 
    }
    if (a->type == OBJ_CMPLX) return &(a->table._cmplx[index]);
    return a -> table._ptr[index]; 
}

array * array_copy(array * a) {
    array * r = array_init(a->type, a->dim, a->sizes);
    int N =1; for(int i=0;i<a->dim;i++) N*=a->sizes[i];
    if (a->type == OBJ_CMPLX) cblas_zcopy(N, (complex*)a->table._cmplx, 1, (complex*)r->table._cmplx,1 );
    else cblas_dcopy(N, (double*)a->table._ptr, 1, (double*)r->table._ptr,1 );
    return r;
}

extern void *change_objtype(void *, obj_type, obj_type);

void change_array_type(array * a, obj_type t) {
    int i;
    int N =1; for(i = 0; i < a->dim; i++) N *= a->sizes[i];
    complex *c;
    if (t == OBJ_CMPLX) {
        complex *cmplx_table = (complex*)malloc(N*sizeof(complex));
        for(i = 1; i < N; i++) {
            c = change_objtype(a->table._ptr[i],a->type,t);
            cmplx_table[i] = *c + 0*I;
        }
    } else {
         for(i = 0; i < N; i++) {
            a->table._ptr[i] = change_objtype(a->table._ptr[i], a->type, t);
        }
    }
    a->type = t;
}

array * array_eye(int dim) {
    int  sizes[2];
    sizes[0] = dim; sizes[1] = dim;
    array * r = array_init(OBJ_FLT, 2, sizes);
    for(int i=0;i<dim;i++) r->table._flt[i*dim+i] = 1.0;
    return r;
} 
/*
array * array_2_mul(array * A, array * B) {
    if (A->ysp != B->xsp) return NULL;
    array_2 * C = array_2_init(A->xsize, B->ysize);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A->xsize, B->xsize, C->xsize, 1.0, (double*)(A->table), A->xsp, (double*)(B->table), B->xsp, 0.0, (double*)(C->table), C->xsp);
    // dgemm_(order, TransA, TransB, M, N, K, alpha, A, LDA, B, LDB, beta, C, LDC)
	// C = alpha * A * B + beta * C
	// A=M*K, B=K*N, C=M*N
    // order: CblasRowMajor/CblasColMajor
	// Trans: CblasNoTrans/CblasTrans/CblasConjTrans
	// LDA = number of row of A
    C->xsp = A->xsp; C->ysp = B->ysp;
    return C;
}
*/

array * array_scler_mull(array *A, void * value) {
    int N=1;
    long alpha = (long)value;
    if (A->dim == 1) N=A->sizes[0] ;
    else if (A->dim ==2) N=A->sizes[0]*A->sizes[1];
    else {for(int i=0;i<A->dim;i++) N*=A->sizes[i];}
    array * C = array_init(A->type, A->dim, A->sizes);
    cblas_dcopy(N, (double*)A->table._flt, 1, (double*)C->table._flt,1 );
    cblas_dscal(N,*(double*)&alpha, (double*)C->table._flt,1);
    // void cblas_dscal(const int N,const double alpha,double *X,const int incX)
    // X = αX
    // N:Xのサイズ
    // alpha:α倍する値
    // X:データのpoinnter
    // incX:データの増分(通常1)
    return C;
}

array * array_scler_mull_set(array *A, void * value) {
    int N=1;
    long alpha = (long)value;
    if (A->dim == 1) N=A->sizes[0] ;
    else if (A->dim ==2) N=A->sizes[0]*A->sizes[1];
    else {for(int i=0;i<A->dim;i++) N*=A->sizes[i];}
    cblas_dscal(N,*(double*)&alpha, (double*)A->table._flt,1);
    // void cblas_dscal(const int N,const double alpha,double *X,const int incX)
    // X = αX
    // N:Xのサイズ
    // alpha:α倍する値
    // X:データのpoinnter
    // incX:データの増分(通常1)
    return A;
}
array * array_array_add(array *A, array * B) {
    int N = 1;
    if (A->dim ==1) N = A->sizes[0];
    else if(A->dim ==2) N = A->sizes[0]*A->sizes[1];
    else { for(int i=0;i<A->dim;i++) N*=A->sizes[i];}
    array * C = array_init(A->type, A->dim, A->sizes);
    cblas_dcopy(N, (double*)B->table._flt, 1, (double*)C->table._flt,1 );
    cblas_daxpy(N, 1, (double*)A->table._flt, 1, (double*)C->table._flt, 1);
    // void cblas_daxpy(const int N, const double alpha, const double *X, const int incX, double *Y, const int incY)
    // Y = αX + Y
    // N:X,Yのサイズ
    // alpha:α倍する値
    // X,Y:データのpoinnter
    // incX,incY:データの増分(通常1)
    return C;
}

array * array_add_set(array *A, array * B, double alpha ) {
    //A += B (A = A + B )
    int N = 1;
    if (A->dim ==1) N = A->sizes[0];
    else if(A->dim ==2) N = A->sizes[0]*A->sizes[1];
    else { for(int i=0;i<A->dim;i++) N*=A->sizes[i];}
    //array * C = array_init(A->type, A->dim, A->sizes);
    //cblas_dcopy(N, (double*)B->table, 1, (double*)C->table,1 );
    cblas_daxpy(N, alpha, (double*)A->table._flt, 1, (double*)B->table._flt, 1);
    // void cblas_daxpy(const int N, const double alpha, const double *X, const int incX, double *Y, const int incY)
    // Y = αX + Y
    // N:X,Yのサイズ
    // alpha:α倍する値
    // X,Y:データのpoinnter
    // incX,incY:データの増分(通常1)
    return B;
}

array * array_array_mul(array  * A, array * B) {
    array *C;
    unsigned int sizes[2];
    if (A->dim ==1) {
        if (B->dim ==1) {   // AとBの内積 ※結果はスカラーだが1X1の行列として返す
            if (A->sps[0] != B->sps[0]) {printf("SizeError!\n");Throw(3);}
            sizes[0] = 1;
            C = array_init(A->type,1,sizes);
            C->table._flt[0] = cblas_ddot(A->sps[0], (double*)A->table._flt, 1, (double*)B->table._flt, 1);
            // cblas_ddot (const int N, const double *X, const int incX, const double *Y, const int incY)
            // N:X,Yのサイズ（計算サイズ) 
            // X,Y:データのpointer
            // incX,incY:データの増分(通常1)
            return C;
        } else if (B->dim ==2) { // ベクターAと行列Bの積
            if (B->sizes[1] != A->sizes[0]) {printf("SizeError!\n");Throw(3);}
            sizes[0] = B->sizes[1];
            C = array_init(A->type, 1, sizes);
            cblas_dgemv(CblasRowMajor, CblasNoTrans, B->sizes[0], B->sizes[1], 1.0, (double*)B->table._flt, B->sps[1],
                        (double *)A->table._flt, 1, 1.0, (double*)C->table._flt, 1);
            // gblas_dgemv(order, Trans, const int M, const int N, const double alpha, const double * A, const int lda, 
            //            const double *X, const int incX, const double beta, double * Y, const int incY)
            // Y = alpha * A * X + beta * Y 
            return C;
        }
    } else if (A->dim ==2 && B->dim ==1) {   // 行列AとベクターBの積
        if (A->sizes[1] != B->sizes[0]) {printf("SizeError!\n");Throw(3);}
        sizes[0] = A->sizes[1];
        C = array_init(A->type, 1, sizes);
        cblas_dgemv(CblasRowMajor, CblasNoTrans, A->sizes[0], A->sizes[1], 1.0, (double*)A->table._flt, A->sps[1],
                    (double *)B->table._flt, 1, 1.0, (double*)C->table._flt, 1);
        return C;
    }
    else if (A->dim != 2 || B->dim != 2) {printf("MatrixDIMMiss!\n");Throw(3);}
    // 行列Aと行列Bの積
    if (A->sps[1] != B->sps[0]) {printf("MatrixSizeMiss!\n");Throw(3);}
    sizes[0] = A->sizes[0]; sizes[1] =B->sizes[1];
    C = array_init(A->type, A->dim, sizes);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A->sizes[0], C->sizes[1], B->sizes[0], 1.0, (double*)A->table._flt, A->sps[1], (double*)B->table._flt, B->sps[1], 0.0, (double*)C->table._flt, C->sps[1]);
    // dgemm_(order, TransA, TransB, M, N, K, alpha, A, LDA, B, LDB, beta, C, LDC)
	// C = alpha * A * B + beta * C
	// A=M*K, B=K*N, C=M*N
    // order: CblasRowMajor/CblasColMajor
	// Trans: CblasNoTrans/CblasTrans/CblasConjTrans
	// LDA = number of row of A
    C->sps[0] = A->sps[0]; C->sps[1] = B->sps[1];
    return C;
}
array * array_mul_add_set(array * C, double alpha, array  * A, array * B) {
    // C = C + alpha * A * B ( C += alpha * A * B )
    if (A->dim != 2 || B->dim != 2) {printf("MatrixDIMMiss!\n");Throw(3);}
    if (A->sps[1] != B->sps[0]) {printf("MatrixSizeMiss!\n");Throw(3);}
    //unsigned int sizes[2]; sizes[0] = A->sizes[0]; sizes[1] =B->sizes[1];
    //array * C = array_init(A->type, A->dim, sizes);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A->sizes[0], C->sizes[1], B->sizes[0], alpha, A->table._flt, A->sps[1], B->table._flt, B->sps[1], 1.0, C->table._flt, C->sps[1]);
    // dgemm_(order, TransA, TransB, M, N, K, alpha, A, LDA, B, LDB, beta, C, LDC)
	// C = alpha * A * B + beta * C
	// A=M*K, B=K*N, C=M*N
    // order: CblasRowMajor/CblasColMajor
	// Trans: CblasNoTrans/CblasTrans/CblasConjTrans
	// LDA = number of row of A
    C->sps[0] = A->sps[0]; C->sps[1] = B->sps[1];
    return C;
}

array * solv_liner(array * A, array * Y) {
    // Y = A * X を解く
    array * X = array_init(Y->type, Y->dim, Y->sizes);
    int N = 1;for(int i=0;i<Y->dim;i++) N*=Y->sizes[i];
    cblas_dcopy(N, Y->table._flt, 1, X->table._flt,1 );
    int *pivot = (lapack_int *)malloc(A->sizes[0]*sizeof(lapack_int));
    int info = LAPACKE_dgesv(LAPACK_ROW_MAJOR, A->sizes[0], X->dim == 1 ? 1 : X->sizes[1], (double *)A->table._flt, A->sps[0], pivot, (double *)X->table._flt, X->dim == 1 ? 1: X->sps[1]);
    //LAPACKE_dgesv(LAPACK_ROW_MAJOR, A->sizes[0], A->sizes[1], (double *)A->table, A->sizes[0], pivot, (double *)X->table,3);
    //for(int i=0;i<A->sizes[0];i++) printf("%d\n",pivot[i]);
    // int LAPACK_dgesv(order, int n, int nrhs, double *a, int lda, int *ipiv, double *b, int ldb)
    if (info != 0) {printf("pivot 0!\n");Throw(3);}
    return X;
}

array * array_inv(array *A) {
    array * X = array_init(A->type, A->dim, A->sizes);
    int i, N = 1;for(i=0;i<A->dim;i++) N*=A->sizes[i];
    cblas_dcopy(N, (double*)A->table._flt, 1, (double*)X->table._flt,1 );
    int    m       = A->sizes[0] ;  // 行のサイズ
    int    n       = A->sizes[1] ;  // 列のサイズ
    int    lda     = A->sps[0] ;    // 行の実効数(m以下)
    //double A[SIZE*SIZE]   ; // m x n の行列成分。この行列の逆行列を求める。
    //A[0] = 2.0 ;A[2] = 3.0;
    //A[1] = 1.0 ;A[3] = 0.5;
    int    info           ;         // 計算が成功すれば0を返す
    int    *ipiv = (int *)malloc(MIN(n,m)*sizeof(int))     ; // 要素数はm,nのうち小さい方で良い
    //int    lwork   = n ; // nと同じ値
    //double *work = (double*)malloc(lwork*sizeof(double))     ; // 要素数はlworkと同じ値

    // LAPACKのdgetrfサブルーチンを呼んで、行列AをLU分解
    // 引数は全て参照渡し
    info = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, m, n, (double*)X->table._flt, lda, ipiv);
    if (info != 0) {printf("Pivot is 0!\n");Throw(3);}
    // LU分解後の行列から逆行列を求める
    // 逆行列は元の配列Aに入る
    info = LAPACKE_dgetri(LAPACK_ROW_MAJOR, n, (double*)X->table._flt, lda, ipiv);
    //for (i = 0; i < MIN(n,m); i++) printf("%d ",ipiv[i]);printf("\n");
    //printf("%+8.5lf %+8.5lf\n", A[0], A[2]);
    //printf("%+8.5lf %+8.5lf\n", A[1], A[3]);
    return X;
}

Vector * eigen(array * A) {
    array * X = array_init(A->type, A->dim, A->sizes);
    int i, j, N = 1;for(i=0;i<A->dim;i++) N*=A->sizes[i];
    cblas_dcopy(N, (double*)A->table._flt, 1, (double*)X->table._flt,1 );
    int n  = A->sizes[0]; 
    int lda =A->sps[0];
    double * wr =(double *)malloc(n*sizeof(double));
    double * wi =(double *)malloc(n*sizeof(double));
    int ldvl = n;
    int ldvr = n;
    double * vl =(double *)malloc(n*ldvl*sizeof(double));
    double * vr =(double *)malloc(n*ldvr*sizeof(double));

    int info = LAPACKE_dgeev( LAPACK_ROW_MAJOR, 'V', 'V', n, (double*)X->table._flt, lda, wr, wi,vl, ldvl, vr, ldvr );
    int size[1] ;size[0]=n;
    array *eig_vect_l, *eig_vect_r, *eig_val = array_init(OBJ_CMPLX,1, size);
    Vector *eigenvect_l = vector_init(n);
    Vector *eigenvect_r = vector_init(n);
    for(i = 0; i < n; i++) {
        eig_val->table._cmplx[i] = wr[i] + wi[i]*I;
        if (wi[i] == 0.0) {
            eig_vect_l = array_init(OBJ_FLT, 1, size);
            eig_vect_r = array_init(OBJ_FLT, 1, size);
            for(j=0;j<n;j++) {
                eig_vect_l->table._flt[j] = vl[i*ldvl+j]; 
                eig_vect_r->table._flt[j] = vr[i*ldvr+j]; 
            } 

        } else {
            eig_vect_l = array_init(OBJ_CMPLX, 1, size);
            eig_vect_r = array_init(OBJ_CMPLX, 1, size);
            j=0;
            while (j<n) {
                eig_vect_l->table._cmplx[j] = vl[i*ldvl+j] + vl[i*ldvl+j+1]*I; 
                eig_vect_l->table._cmplx[j+1] = vl[i*ldvl+j] - vl[i*ldvl+j+1]*I; 
                eig_vect_r->table._cmplx[j] = vr[i*ldvr+j] + vr[i*ldvr+j+1]*I; 
                eig_vect_r->table._cmplx[j+1] = vr[i*ldvr+j] - vr[i*ldvr+j+1]*I;
                j+=2;
            } 
        }
        push(eigenvect_l,(void*)eig_vect_l);
        push(eigenvect_r,(void*)eig_vect_r);
    }
    Vector * r = vector_init(3);
    push(r,(void*)eig_val);
    push(r,(void*)eigenvect_l);
    push(r,(void*)eigenvect_r);//printf("OKOKOK!\n");
    return r;
}
/*
array * solv_liner_sq(array * A, array * x);
array * inv_array(array *A);
array * LU_decomp(array * A) ;
Vector * QR_decomp(array *A);
array * Hessen(array *A);
array * eigen_vectors(array *A);
array * diag(array *A);
*/
