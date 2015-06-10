CC = g++
CFLAGS = -O3 -ffast-math -o
LIBS = -L/sw/taurus/libraries/cuda/6.5.14/lib64
INCLUDE = -I/sw/taurus/libraries/cuda/6.5.14/include
LDFLAGS = -ltiff -lcuda -lcudart -lcufft
NVFLAGS = -O3 --use_fast_math -c -arch=sm_35

all:fft

fft: fft.o
	$(CC) $(CFLAGS) fft $(LIBS) $(INCLUDE) $(LDFLAGS) src/tiff_handler.cpp fft.o

fft.o:
	nvcc $(NVFLAGS) src/fft.cu

clean:
		rm *.o fft
