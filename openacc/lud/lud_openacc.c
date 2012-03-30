//#define rc(a, r, c, s) (a[(r)*(s)+(c)])
#define mat(row, col) matrix[(row) * (size) + (col)]

void lud_openacc(float * restrict matrix, int size){
  int i,j,k;
  float sum;

  #pragma acc data copy(matrix[:(size*size)])
  {
    for (i = 0; i < size; i++){
      //private(j,k,sum) shared(size,i,a) 
#pragma acc kernels loop gang, vector(8)
      for (j = i; j < size; j++){
	sum = mat(i, j);
#pragma acc loop gang, vector(8)
	for (k = 0; k < i; k++) 
	  sum -= mat(i, k) * mat(k, j);
	mat(i, j) = sum;
      }
      //private(j,k,sum) shared(size,i,a) 
#pragma acc kernels loop gang, vector(8)
      for (j = i + 1; j < size; j++){
	sum = mat(j, i);
#pragma acc loop gang, vector(8)
	for (k = 0; k < i; k++)
	  sum -= mat(j, k) * mat(k, i);
	mat(j, i) = sum / mat(i, i);
      }
    }
  }
}
