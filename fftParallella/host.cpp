#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>

#include <unistd.h>
#include <assert.h>

#include <e-hal.h>

#include "include/tiff_handler.h"

enum UserInterrupt{
	NotDone = 0x0,
	Done    = 0x1,
	Error   = 0x2,
};

int main(int argc, char* argv[]){

	float *img;
	int sizeX, sizeY;
	char *s;	

	if(argc != 4){
		printf("usage: executable <name of input data> <imageWidth> <imageLength>");
		exit(1);
	}else{
		s = argv[1];
		sizeX = atoi(argv[2]);
		sizeY = atoi(argv[3]);
	}

	img = (float*)malloc(sizeof(float)*sizeX*sizeY);
	readTIFF(s, img);
	printf("%f \n", img[25]);	

	//TODO: mit device verbinden + verarbeitung
	char *hostExecutable = strdup(argv[0]);
	char *epiphanyExecutable = (char*)malloc(sizeof(char) * (strlen(hostExecutable) + strlen(E_EXECUTABLE) + 1 + 1));
	sprintf(epiphanyExecutable, "%s/%s", dirname(hostExecutable), E_EXECUTABLE);

	//initialize epiphany device
	e_platform_t platform;
	e_epiphany_t dev;

	e_init(nullptr);
	e_reset_system();
	e_get_platform_info(&platform);
	
	//start workgroup with all e-cores
	if(e_open(&dev, 0, 0, 1, 1) != E_OK){
		printf("e_open failed\n");
		exit(1);
	}
    
	//load epiphany program into workgroup, but don't start it yet
	if(e_load_group(epiphanyExecutable, &dev, 0, 0, 1, 1, E_FALSE) == E_ERR){
     		printf("epiphany program could not be loaded into workgroup\n");
        	exit(1);
	}

	UserInterrupt init = UserInterrupt::NotDone;
	e_write(&dev, 0, 0, 0x24, &init, sizeof(init));
    
    	//copy data
	int nn = 512;
	int inverse = 1;

	float *dataComplex = (float*)calloc((2*nn+1), sizeof(float));
	for(int i = 0; i < nn; i++){
		dataComplex[2*i+1] = i;	
	}

	e_write(&dev, 0, 0, 0x40, &nn, sizeof(nn));
	e_write(&dev, 0, 0, 0x44, &inverse, sizeof(inverse));	

	e_write(&dev, 0, 0, 0x6000, dataComplex, sizeof(float)*(2*nn+1));
	
	//start device program
	e_start_group(&dev);

	//wait until the epiphany is done
	UserInterrupt epiphanyDone = UserInterrupt::NotDone;
	while(epiphanyDone == UserInterrupt::NotDone){
		e_read(&dev, 0, 0, 0x24, &epiphanyDone, sizeof(epiphanyDone));
		if(epiphanyDone == UserInterrupt::NotDone) 
			usleep(1000); 
	}

	assert(epiphanyDone == UserInterrupt::Done);

	e_read(&dev, 0, 0, 0x6000, dataComplex, sizeof(float)*(2*nn+1));

	printf("FFT:\n");
	for(int i = 0; i < nn; i++){
		printf("X[%d] = (%.2f + j %.2f)\n", i, dataComplex[2*i+1], dataComplex[2*i+2]);
	}
	
	uint32_t cycles, fpops;
	e_read(&dev, 0, 0, 0x48, &cycles, sizeof(uint32_t));
	e_read(&dev, 0, 0, 0x4C, &fpops, sizeof(uint32_t));

	printf("%d cycles, %d fpops\n", cycles, fpops);
	
	writeTiff((char*)"out.tif", img, sizeY, sizeX);

	free(img);
	
	return 0;
}
