#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <libgen.h>

#include <e-hal.h>

#include "include/tiff_handler.h"

int main(int argc, char* argv[]){

	float *img;
	int sizeX, sizeY;
	char *s;	

	//TODO: nutzereingabe lesen
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
	//char *epiphanyExecutable (char*)malloc(sizeof(char) * (strlen(hostExecutable) + strlen(E_EXECUTABLE) + 1 + 1));
	//sprintf(epiphanyExecutable, "%s/%s", dirname(hostExecutable), E_EXECUTABLE);

	//initialize epiphany device
	e_platform_t platform;
	e_epiphany_t dev;

	e_init(nullptr); //identisch zu NULL?
	
	writeTiff((char*)"out.tif", img, sizeY, sizeX);

	free(img);
	

}
