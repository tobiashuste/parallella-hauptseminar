#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <math.h>

#include <e-lib.h>

#define PI 3.14159265358979323846
#define TWOPI (2.0*PI)

enum UserInterrupt{
        NotDone = 0x0,
        Done    = 0x1,
        Error   = 0x2,
};


float data[2048] __attribute__ ((aligned(8), section (".data_bank3")));

void fft(int nn, int inverse);
void normalize(int nn);

int main(){
	
	e_irq_global_mask(E_TRUE);
		
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX);
	e_ctimer_set(E_CTIMER_1, E_CTIMER_MAX);

	e_ctimer_start(E_CTIMER_0, E_CTIMER_FPU_INST);
	e_ctimer_start(E_CTIMER_1, E_CTIMER_CLK);

	//*(UserInterrupt*) 0x24 = UserInterrupt::Done:

	const uint32_t nn = *(uint32_t*) 0x40;
	const uint32_t inverse = *(uint32_t*) 0x44;

	fft(nn, inverse);	
	
	fft(nn, -1);
	
	normalize(nn);

	uint32_t cycles = (uint32_t) e_ctimer_get(E_CTIMER_1);
	uint32_t fpops = (uint32_t) e_ctimer_get(E_CTIMER_0);

	*(uint32_t*) 0x48 = E_CTIMER_MAX - cycles;
	*(uint32_t*) 0x4C = E_CTIMER_MAX - fpops;

	*(UserInterrupt*) 0x24 = UserInterrupt::Done;
	
	return 0;
}


void fft(int nn, int inverse){
	
	int n, mmax, m, j, istep, i;
	float wtemp, wr, wpr, wpi, wi, theta;
	float tempr, tempi;	
	
	float *dataTemp = data;
	
	n = nn << 1;
	j = 1;
	//reordering
	for(i = 1; i < n; i += 2){
		if(j > i){
			tempr = dataTemp[j];
			dataTemp[j] = dataTemp[i];
			dataTemp[i] = tempr;
			tempr = dataTemp[j+1];
			dataTemp[j+1] = dataTemp[i+1];
			dataTemp[i+1] = tempr;
		}
		m = n >> 1;
		while(m >= 2 && j > m){
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax = 2;
	while(n > mmax){
		istep = 2*mmax;
		theta = TWOPI/(inverse*mmax);
		wtemp = sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi = sin(theta);
		wr = 1.0;
		wi = 0.0;
		for(m = 1; m < mmax; m += 2){
			for(i = m; i <= n; i += istep){
				j = i + mmax;
				tempr = wr*dataTemp[j] - wi*dataTemp[j+1];
				tempi = wr*dataTemp[j+1] + wi*dataTemp[j];
				dataTemp[j] = dataTemp[i] - tempr;
				dataTemp[j+1] = dataTemp[i+1] - tempi;
				dataTemp[i] += tempr;
				dataTemp[i+1] += tempi;
			}
			wr = (wtemp = wr)*wpr - wi*wpi + wr;
			wi = wi*wpr + wtemp*wpi + wi;	
		}
		mmax = istep;
	}
}

void normalize(int nn){
	for(int i = 0; i < nn; i++){
		data[2*i+1] /= nn;
		data[2*i+2] /= nn;
	}
}
