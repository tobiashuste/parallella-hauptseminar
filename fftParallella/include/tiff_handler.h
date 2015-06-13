#include <tiffio.h>

int readTIFF(char* dataName, float* returnData);
int readTIFFu16(char* dataName, uint16_t* returnData);
int writeTiff(char* dataName, float* inputData, unsigned int imageLength, unsigned int imageWidth);
int writeTiffu16(char* dataName, uint16_t* inputData, unsigned int imageLength, unsigned int imageWidth);
