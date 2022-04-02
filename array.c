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
    void ** table = (void**)malloc(size * sizeof(void * ));
    array * a = (array * )malloc(sizeof(array)); 
    a -> type = type; a -> dim = dim; a -> sizes = ss; a -> table = table; a->sps =sp;
    //printf("type:%d dim :%d size:", type, dim);for(i=0;i< dim ;i++) printf(" %d",sizes[i]);
    //printf("total size:%d",size);printf("\n");
    return a;    
}
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

    return a -> table[index]; 
}

array * array_copy(array * a) {
    array * r = array_init(a->type, a->dim, a->sizes);
    int N =1; for(int i=0;i<a->dim;i++) N*=a->sizes[i];
    cblas_dcopy(N, (double*)r->table, 1, (double*)a->table,1 );
    return r;
}

array * array_eye(int dim) {
    int  sizes[2];
    sizes[0] = dim; sizes[1] = dim;
    array * r = array_init(4, dim, sizes);
    for(int i=0;i<dim;i+=dim) r->table[i*dim+i] = (void*)0x3ff0000000000000;
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
    cblas_dcopy(N, (double*)A->table, 1, (double*)C->table,1 );
    cblas_dscal(N,*(double*)&alpha, (double*)C->table,1);
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
    cblas_dscal(N,*(double*)&alpha, (double*)A->table,1);
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
    cblas_dcopy(N, (double*)B->table, 1, (double*)C->table,1 );
    cblas_daxpy(N, 1, (double*)A->table, 1, (double*)C->table, 1);
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
    cblas_daxpy(N, alpha, (double*)A->table, 1, (double*)B->table, 1);
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
            double r = cblas_ddot(A->sps[0], (double*)A->table, 1, (double*)B->table, 1);
            // cblas_ddot (const int N, const double *X, const int incX, const double *Y, const int incY)
            // N:X,Yのサイズ（計算サイズ) 
            // X,Y:データのpointer
            // incX,incY:データの増分(通常1)
            C->table[0] = (void*)&(r);return C;
        } else if (B->dim ==2) { // ベクターAと行列Bの積
            if (B->sizes[1] != A->sizes[0]) {printf("SizeError!\n");Throw(3);}
            sizes[0] = B->sizes[1];
            C = array_init(A->type, 1, sizes);
            cblas_dgemv(CblasRowMajor, CblasNoTrans, B->sizes[0], B->sizes[1], 1.0, (double*)B->table, B->sps[1],
                        (double *)A->table, 1, 1.0, (double*)C->table, 1);
            // gblas_dgemv(order, Trans, const int M, const int N, const double alpha, const double * A, const int lda, 
            //            const double *X, const int incX, const double beta, double * Y, const int incY)
            // Y = alpha * A * X + beta * Y 
            return C;
        }
    } else if (A->dim ==2 && B->dim ==1) {   // 行列AとベクターBの積
        if (A->sizes[1] != B->sizes[0]) {printf("SizeError!\n");Throw(3);}
        sizes[0] = A->sizes[1];
        C = array_init(A->type, 1, sizes);
        cblas_dgemv(CblasRowMajor, CblasNoTrans, A->sizes[0], A->sizes[1], 1.0, (double*)A->table, A->sps[1],
                    (double *)B->table, 1, 1.0, (double*)C->table, 1);
        return C;
    }
    else if (A->dim != 2 || B->dim != 2) {printf("MatrixDIMMiss!\n");Throw(3);}
    // 行列Aと行列Bの積
    if (A->sps[1] != B->sps[0]) {printf("MatrixSizeMiss!\n");Throw(3);}
    sizes[0] = A->sizes[0]; sizes[1] =B->sizes[1];
    C = array_init(A->type, A->dim, sizes);
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
        A->sizes[0], C->sizes[1], B->sizes[0], 1.0, (double*)(A->table), A->sps[1], (double*)(B->table), B->sps[1], 0.0, (double*)(C->table), C->sps[1]);
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
        A->sizes[0], C->sizes[1], B->sizes[0], alpha, (double*)(A->table), A->sps[1], (double*)(B->table), B->sps[1], 1.0, (double*)(C->table), C->sps[1]);
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
    cblas_dcopy(N, (double*)X->table, 1, (double*)Y->table,1 );
    int *pivot = (int *)malloc(A->sizes[0]*sizeof(int));
    LAPACKE_dgesv(LAPACK_ROW_MAJOR, A->sizes[0], A->sizes[1], (double *)A->table, A->sps[1], pivot, (double *)X->table, (X->dim  == 1) ? X->sps[0]: X->sps[1]);
    // int LAPACK_dgesv(order, int n, int nrhs, double *a, int lda, int *ipiv, double *b, int ldb)
    return X;
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