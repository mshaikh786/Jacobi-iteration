#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef VIS
#include "vis.h"
#endif

#ifndef ROWS
#define ROWS		1000
#endif
#ifndef COLS
#define COLS		1000
#endif
#ifndef MAX_ITER
#define MAX_ITER	1000
#endif
#ifndef TOL
#define TOL		1e-2
#endif
#ifndef MAX_TEMP
#define MAX_TEMP	100.0
#endif
#ifndef CHKPNT_ITER
#define CHKPNT_ITER	100
#endif

float** allocate(int,int,float**);
void init_grid(int,int,float**,float);


int main(int argc, char* argv[]){

	int rows = ROWS+2,
	    cols = COLS+2,
	    max_iter = MAX_ITER,
	    iter = 0;
	float max_temp = MAX_TEMP;
	if (argc > 1){
		for (int i=0; i < argc; i++){
			if (argv[i][0] == '-') {
				if (argv[i][1] == 'm'){
					max_iter = atoi(argv[i+1]);
				}
				else if (argv[i][1] == 'r'){
					rows = atoi(argv[i+1]) + 2;
				}
				else if (argv[i][1] == 'c'){
					cols = atoi(argv[i+1]) + 2;
				}
				else if (argv[i][1] == 'h'){
					max_temp = atof(argv[i+1]);
				}
			}
		}
	}

	float dT = MAX_TEMP;


	float**  T_old = allocate (rows,cols,T_old);
	float**  T_new = allocate (rows,cols,T_new);

	init_grid(rows,cols,T_old,max_temp);
	init_grid(rows,cols,T_new,max_temp);

	while ( dT > TOL && iter <= max_iter ){

		int i=0,j=0;
#ifdef VIS
		if ( iter % CHKPNT_ITER == 0){
			write_dataset_vtk(rows,cols,T_new,iter);
		}
#endif

		for (i=1 ; i < rows-1; i++){
			for(j=1; j < cols-1; j++){
				T_new[i][j] = 0.25 * (T_old[i-1][j] + T_old[i+1][j] + T_old[i][j-1] + T_old[i][j+1]); 
			}
		}

		dT = 0.0;
		for (i=1 ; i < rows-1; i++){
			for(j=1; j < cols-1; j++){
				dT = fmaxf(fabsf(T_new[i][j] - T_old[i][j]),dT);
				T_old[i][j] = T_new[i][j];
			}
		}
		iter++;
	}

	printf ( "Converged in %d iterations with and error of %2.4f\n",iter-1,dT);
	return 0;
}

float** allocate(int rows, int cols, float** T){
	int i=0,j=0;
	T = (float**) malloc(rows * sizeof(float*));
	if (T !=NULL) {
		T[0] = (float*) malloc(rows*cols*sizeof(float)); 
	}	
	if (T[0] != NULL){
		for (i =0 ; i< rows ; i++)
			T[i] = (*T + i*cols);
	}
	return T;
}

void init_grid(int rows, int cols, float** T, float max_temp){
	int i=0,j=0;

	for (i = 0; i < rows; i++){
		for (j = 0; j < cols; j++){
			T[i][j] = 0.0;
		}
	}
	// Set top and bottom boundary initial values
	for (j = 0; j < cols; j++){
		T[0][j] = MAX_TEMP;
		T[rows-1][j] = MAX_TEMP;
	}
	// Set left and rigth boundary initial values
	for (i = 0; i < rows; i++){
		T[i][0] = MAX_TEMP;
		T[i][cols-1] = MAX_TEMP;
	}

}
