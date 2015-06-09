#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "include/tiff_handler.h"



/*
* gets name of tiff-image
*
* reads data into ouput array linewise
*/
int readTIFF(char* dataName, float* returnData){
	unsigned int imageLength, imageWidth, row, col;
	float* data = NULL;
	tsize_t scanLine;
	tdata_t* buf;
	TIFF* tif = TIFFOpen(dataName, "rb");
	if (tif){
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);
		scanLine = TIFFScanlineSize(tif);
		buf = (tdata_t*)_TIFFmalloc(scanLine);
		data = (float*)malloc(scanLine);
		for (row = 0; row < imageLength; row++){
			TIFFReadScanline(tif, buf, row, 0);
			_TIFFmemcpy(data, buf, scanLine);
			for (col = 0; col < imageWidth; col++){
				returnData[col + imageWidth * row] = data[col];
			}
		}
		_TIFFfree(buf);
		free(data);
		TIFFClose(tif);
		return 0;
	}
	return -1;
}

int readTIFFu16(char* dataName, uint16_t* returnData){
	unsigned int imageLength, imageWidth, row, col;
	uint16_t* data = NULL;
	tsize_t scanLine;
	tdata_t* buf;
	TIFF* tif = TIFFOpen(dataName, "r");
	if (tif){
		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &imageLength);
		printf("%d \n", imageLength);
		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &imageWidth);
		scanLine = TIFFScanlineSize(tif);
		buf = (tdata_t*)_TIFFmalloc(scanLine);
		data = (uint16_t*)malloc(scanLine);
		for (row = 0; row < imageLength; row++){
			TIFFReadScanline(tif, buf, row, 0);
			_TIFFmemcpy(data, buf, scanLine);
			for (col = 0; col < imageWidth; col++){
				returnData[col + imageWidth * row] = data[col];
			}
		}
		_TIFFfree(buf);
		free(data);
		TIFFClose(tif);
		return 0;
	}
	return -1;
}


int writeTiff(char* dataName, float* inputData, unsigned int imageLength, unsigned int imageWidth){
	unsigned int row, col;
	tsize_t scanLine;
	TIFF* tif = TIFFOpen(dataName, "wb");
	float* data = NULL;
	if (tif){
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, imageLength);
		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, imageWidth);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 32);
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
		//noch anpassen!
		scanLine = sizeof(float) * imageWidth;
		data = (float*)malloc(scanLine);
		for (row = 0; row < imageLength; row++){
			for (col = 0; col < imageWidth; col++){
				data[col] = inputData[col + imageWidth * row];
			}
			TIFFWriteScanline(tif, data, row, 0);
		}
		free(data);
		TIFFClose(tif);
		return 0;
	}
	return -1;
}


/*
* pixel values stored linewise in inputarray
*
* creates output TIFF-Image in unsigned int 16 bit format
*/
int writeTiffu16(char* dataName, uint16_t* inputData, unsigned int imageLength, unsigned int imageWidth){
	unsigned int row, col;
	tsize_t scanLine;
	TIFF* tif = TIFFOpen(dataName, "w");
	uint16_t* data = NULL;
	if (tif){
		TIFFSetField(tif, TIFFTAG_IMAGELENGTH, imageLength);
		TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, imageWidth);
		TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, 1);
		TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, 16); 
		TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
		scanLine = sizeof(uint16_t) * imageWidth;
		data = (uint16_t*)malloc(scanLine);
		for (row = 0; row < imageLength; row++){
			for (col = 0; col < imageWidth; col++){
				data[col] = inputData[col + imageWidth * row];
			}
			TIFFWriteScanline(tif, data, row, 0);
		}
		free(data);
		TIFFClose(tif);
		return 0;
	}
	return -1;
}
