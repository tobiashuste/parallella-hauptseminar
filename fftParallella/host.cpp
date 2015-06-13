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
    e_reset_system();
    e_get_platform_info(&platform);
    
    //start workgroup with all e-cores
    if(e_open(&dev, 0, 0, platform.rows, platform.cols) != E_OK){
        printf("e_open failed\n");
        exit(1);
    }
    
    //load epiphany program into workgroup, but don't start it yet
    /*if(e_load_group(epiphanyExecutable, &dev, 0, 0, platform.rows, platform.cols, E_FALSE) == E_ERR){
        printf("epiphany program could not be loaded into workgroup\n");
        exit(1);
    }*/
    
    //TODO: Korrekte Aufteilung der Daten
    //copy data to epiphany device
    /*for(int i = 0; i < sizeY/(platform.rows*platform.cols); i++){
        for(int row = 0; row < platform.rows; row++){
            for(int col = 0; col < platform.cols; col){
                e_write(&dev, row, col, 0x200, sizeof(float)*...*...);
            }
        }
    e_start_group(&dev);
     //TODO: check if epiphany is ready, copy results back
    }*/
	
	writeTiff((char*)"out.tif", img, sizeY, sizeX);

	free(img);
	

}
