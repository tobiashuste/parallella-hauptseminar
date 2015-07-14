#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <e-hal.h>

#define BILLION 1E9
#define X 1
#define Y 1
#define SIZE 8192
#define WRITE 0

int main(int argc, char* argv[]){

	float *temp = (float*)malloc(sizeof(float)*SIZE);
	float time;
	struct timespec start, end;

	e_platform_t platform;
	e_epiphany_t dev;

	e_init(nullptr);
	e_reset_system();
	e_get_platform_info(&platform);

	e_open(&dev, 0, 0, 1, 1);

	clock_gettime(CLOCK_REALTIME, &start);
	for (int i = 0; i < 1000; i++){
#if WRITE
		e_write(&dev, 0, 0, 0x0000, temp, sizeof(float)*SIZE);
#endif
#if !WRITE
		e_read(&dev, 0, 0, 0x0000, temp, sizeof(float)*SIZE);
#endif
	}

	clock_gettime(CLOCK_REALTIME, &end);

	time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec)/BILLION;

	printf("Gesamtzeit: %f s\n Bandbreite: %f MByte/s\n", time, 4*SIZE/(time / (2 * 1000))/(1024*1024));
	
	e_reset_system();

	return 0;

}

