#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

void create_raw_dir(int numranks){
	char dirname[256];
	sprintf(dirname,"raw_files");
	mkdir(dirname,S_IRWXU|S_IRWXG|S_IRWXO|S_ISUID);

	int i=0;
	for (i=0; i< numranks; i++){
		sprintf(dirname,"raw_files/%i",i);
		mkdir(dirname,S_IRWXU|S_IRWXG|S_IRWXO|S_ISUID);
	}

}
/* This function dumps into a file the following common data to 
 * be used by the reconstructor:
 * VTK header 
 * Structured grid coordinates
 * Header for cell data
 */
int record_common_content(int numranks,int num_chkpoint,int r, int c){
	char fname_header[512];
	sprintf(fname_header,"raw_files/common_header.vtk");
	FILE *fp = fopen(fname_header, "w+");	
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
	fclose(fp);

	// record the simulation information to for postprocessing
	char fname_parm[512];
	sprintf(fname_parm,"raw_files/parms.sim");
	fp = fopen(fname_parm, "w+");
	fprintf(fp,"numranks= %d\n",numranks);
	fprintf(fp,"num_checkpoint= %d\n",num_chkpoint);
	fclose(fp);
	return 0;
}

int dump_iter_bin(int rank,int numranks,int r, int c, float** A,int iter){

	char filename[512];
	sprintf(filename,"raw_files/%d/iter%d",rank,iter);
	FILE *fp = fopen(filename, "w+");
	int start,end;
	if (rank == 0){
		start = 0;
		end = r-2;
	}
	else if (rank == numranks-1){
		start = 1;
		end = r-1;
	}
	else {
		start = 1;
		end   = r-2;
	}
	for(int i=start; i<=end; i++){
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
