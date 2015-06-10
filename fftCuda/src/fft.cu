#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cuda_runtime.h>
#include <cufft.h>

#include "include/tiff_handler.h"

#define BLOCK_SIZE 256

float *imageA_d, *result_d;
cufftComplex *data_d;

__global__ void normalize(int n, float *data); 
int convolve(int x, int y, float *imageA, float *result);
void finish(char *s);

int main(){

	float *img_inA, *img_out;

	cudaMallocHost((void**)&img_inA, sizeof(float)* 512 * 512);
	cudaMallocHost((void**)&img_out, sizeof(float)* 512 * 512);

	readTIFF((char*)"boy.tif", img_inA);

	convolve(512, 512, img_inA, img_out);

	writeTiff((char*)"out.tif", img_out, 512, 512);

	cudaFreeHost(img_inA); cudaFreeHost(img_out);

	return 0;
}

int convolve(int x, int y, float *imageA, float *result){

	cufftHandle planFwd, planInv;
	cudaError error1, error2, error3;
	cufftResult_t status;

	int tStart = clock();

	error1 = cudaMalloc((void**)&imageA_d, sizeof(float)*x*y);
	error2 = cudaMalloc((void**)&result_d, sizeof(float)*x*y);
	error3 = cudaMalloc((void**)&data_d, sizeof(cufftComplex)*x*y);
	if ((error1 != cudaSuccess) ||
		(error2 != cudaSuccess) ||
		(error3 != cudaSuccess)){
		finish((char*)"Memory Allocation failed\n");
		return EXIT_FAILURE;
	}
	
	error1 = cudaMemcpy(imageA_d, imageA, sizeof(float)*x*y, cudaMemcpyHostToDevice);
	if ((error1 != cudaSuccess)){
		finish((char*)"Memcpy host to device failed\n");
		return EXIT_FAILURE;
	}

	//transform image into frequency domain
	status = cufftPlan2d(&planFwd, x, y, CUFFT_R2C);
	if (status != CUFFT_SUCCESS){
		finish((char*)"creation of fft-plan failed\n");
		return EXIT_FAILURE;
	}

	status = cufftExecR2C(planFwd, imageA_d, data_d);
	if (status != CUFFT_SUCCESS){
		finish((char*)"cufftExecR2C failed\n");
		return EXIT_FAILURE;
	}
	
	//transform image back
	status = cufftPlan2d(&planInv, x, y, CUFFT_C2R);
	if (status != CUFFT_SUCCESS){
		finish((char*)"creation of fft-plan failed\n");
		return EXIT_FAILURE;
	}

	status = cufftExecC2R(planInv, data_d, result_d);
	if (status != CUFFT_SUCCESS){
		finish((char*)"cufftExecR2C failed\n");
		return EXIT_FAILURE;
	}
	normalize << <(int)ceil(x*y/(float)BLOCK_SIZE), BLOCK_SIZE >> >(x*y, result_d);
	
	error1 = cudaMemcpy(result, result_d, sizeof(float)*x*y, cudaMemcpyDeviceToHost);
	if (error1 != cudaSuccess){
		finish((char*)"(memcopy device to host failed\n");
		return EXIT_FAILURE;
	}	

	printf("Zeitdauer: %f\n", (clock() - tStart) / (float)CLOCKS_PER_SEC);

	return EXIT_SUCCESS;
}

void finish(char *s){
	if (s) printf(s);
	if (result_d)   cudaFree(result_d);
	if (imageA_d)   cudaFree(imageA_d);
	if (data_d)     cudaFree(data_d);
} 

__global__ void normalize(int n, float *data){
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	if (i < n){
		data[i] = data[i] / n;
	}
}
