#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef VIS
#include "vis.h"
#endif

#ifndef ROWS
#define ROWS		2046
#endif
#ifndef COLS
#define COLS		2046
#endif
#ifndef MAX_ITER
#define MAX_ITER	4000
#endif
#ifndef TOL
#define TOL		1e-4
#endif
#ifndef MAX_TEMP
#define MAX_TEMP	100.0
#endif
#ifndef CHKPNT_ITER
#define CHKPNT_ITER	1000
#endif


float** allocate(int, int, float**);
void init_grid(int, int, float**, float);

int main(int argc, char *argv[]) {

	int rows = ROWS + 2, cols = COLS + 2;
	int i = 0, j = 0;
	int max_iter = MAX_ITER, iter = 0;
	int chkpnt_iter = CHKPNT_ITER, t_chkpoint = 0;

	float max_temp = MAX_TEMP, tol = TOL;
	if (argc > 1) {
		for (i = 0; i < argc; i++) {
			if (argv[i][0] == '-') {
				if (argv[i][1] == 'm') {
					max_iter = atoi(argv[i + 1]);
				} else if (argv[i][1] == 'r') {
					rows = atoi(argv[i + 1]) + 2;
				} else if (argv[i][1] == 'c') {
					cols = atoi(argv[i + 1]) + 2;
				} else if (argv[i][1] == 't') {
					max_temp = atof(argv[i + 1]);
				} else if (argv[i][1] == 'd') {
					tol = atof(argv[i + 1]);
				} else if (argv[i][1] == 'k') {
					chkpnt_iter = atoi(argv[i + 1]);
				}
			}
		}
	}
	float dT = max_temp;

	float **T_old = allocate(rows, cols, T_old);
	float **T_new = allocate(rows, cols, T_new);

	init_grid(rows, cols, T_old, max_temp);
	init_grid(rows, cols, T_new, max_temp);

	fprintf(stdout, "INPUT PARAMETERS\n================\n");

	fprintf(stdout, "ROWS = %d  , COLS = %d\n", rows - 2, cols - 2);
	fprintf(stdout, "WRITE RESULTS ON EVERY %d ITERATION TO FILES\n",
			chkpnt_iter);
	fprintf(stdout, "MAXIMUM ITERATIONS ALLOWED = %d\n", max_iter);
	fprintf(stdout, "TOLERANCE= %f\n", tol);
	fprintf(stdout, "MAX TEMP = %f\n", max_temp);
	fflush(stdout);

	while (dT > tol && iter <= max_iter) {

		int i = 0, j = 0;

#ifdef VIS
		if ( iter % chkpnt_iter == 0){
			write_dataset_vtk(rows,cols,T_new,iter);
		}
#endif

		for (i = 1; i < rows - 1; i++) {
			for (j = 1; j < cols - 1; j++) {
				T_new[i][j] = 0.25
						* (T_old[i - 1][j] + T_old[i + 1][j] + T_old[i][j - 1]
								+ T_old[i][j + 1]);

			}
		}

		dT = 0.0;
		for (i = 1; i < rows - 1; i++) {
			for (j = 1; j < cols - 1; j++) {
				dT = fmaxf(fabsf(T_new[i][j] - T_old[i][j]), dT);
				T_old[i][j] = T_new[i][j];
			}
		}
		iter++;
	}

#ifdef VIS

		write_dataset_vtk(rows,cols,T_new,iter);

#endif

	if ((iter - 1) == max_iter)
		printf("Reached maximum iterations %d. Error = %2.4f\n", iter - 1, dT);
	else
		printf(
				"[JOBID: %s]: Converged in %d iterations with and error of %2.4f\n",
				getenv("SLURM_JOBID"), iter - 1, dT);

	return 0;
}

float** allocate(int rows, int cols, float **T) {
	int i = 0, j = 0;
	T = (float**) malloc(rows * sizeof(float*));
	if (T != NULL) {
		T[0] = (float*) malloc(rows * cols * sizeof(float));
	}
	if (T[0] != NULL) {
		for (i = 0; i < rows; i++)
			T[i] = (*T + i * cols);
	}
	return T;
}

void init_grid(int rows, int cols, float **T, float max_temp) {
	int i = 0, j = 0;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {
			T[i][j] = max_temp * 0.75;
		}
	}

	T[0][0] 			= max_temp;
	T[0][cols-1] 		= max_temp;
	T[rows-1][0] 		= max_temp;

	// Set left and rigth boundary initial values
	for (i = 1; i < rows-1; i++) {
		T[i][0] = max_temp;
		T[i][cols - 1] = max_temp * 0.75;
	}
	// Set top and bottom boundary initial values
	for (j = 1; j < cols-1; j++) {
		T[0][j] = max_temp;
		T[rows - 1][j] = max_temp * 0.75;
	}

}
