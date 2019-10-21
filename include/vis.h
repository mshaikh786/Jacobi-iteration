#include <stdio.h>
#include <stdlib.h>
void print_stencil (int, int,float**);
int write_dataset_vtk(int r, int c, float** A, int iter){

	char filename[256];
	sprintf(filename,"iter%d.vtk",iter);
	FILE *fp = fopen(filename, "w+");	
	int X=c+1,
	    Y=r+1,
	    Z=1;
	fprintf(fp,"# vtk DataFile Version 3.0\n");
	fprintf(fp,"2D Laplacian gird solving for tempreature using Jacobian Iteration\n");
	fprintf(fp,"ASCII\n");
	fprintf(fp,"DATASET RECTILINEAR_GRID\n");
	fprintf(fp,"DIMENSIONS %d %d %d\n",X,Y,Z);
	fprintf(fp,"X_COORDINATES %d float\n",X);
	for (int j=0; j<X; j++){
		fprintf(fp,"%2.6f ",(float)(j)/(X-1));
	}
	fprintf(fp,"\nY_COORDINATES %d float\n",Y);
	for (int i=0; i<Y; i++){
		fprintf(fp,"%2.6f ",(float) (i)/(Y-1));
	}
	fprintf(fp,"\nZ_COORDINATES %d float\n",Z);
	fprintf(fp,"%2.2f\n",0.0);
	fprintf(fp,"CELL_DATA %d\n",(X-1)*(Y-1)*Z);
	fprintf(fp,"FIELD FieldData 1\n");
	fprintf(fp,"temperature 1 %d float\n",(X-1)*(Y-1)*Z);
	for(int i=0; i< r; i++){
		for (int j=0; j< c;j++){
			fprintf(fp,"%2.6f ",A[i][j]);
		}
	fprintf(fp,"\n");
	} 

	fclose(fp);	
	return 0;
}


//Helper function for debugging
void print_stencil (int r, int c,float** T){
	int i=0,j=0;
	for (i = 0; i < r; i++){
		for (j = 0; j < c; j++){

			printf("%2.4f ",T[i][j]);
		}
		printf("\n");
	}

}
