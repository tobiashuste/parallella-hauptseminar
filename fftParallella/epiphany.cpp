#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <e-lib.h>

#define PI M_PI
#define TWOPI (2.0*PI)

float data[2048] __attribute__ ((aligned(8), section (".data_bank1")));
float b[2048] __attribute__ ((aligned(8), section (".data_bank2")));
float c[2048] __attribute__ ((aligned(8), section (".data_bank3")));

int main(int argc, char **args){
	
	e_irq_global_mask(E_TRUE);
		
	e_ctimer_set(E_CTIMER_0, E_CTIMER_MAX);
	e_ctimer_set(E_CTIMER_1, E_CTIMER_MAX);

	e_ctimer_start(E_CTIMER_0, E_CTIMER_FPU_INST);
	e_ctimer_start(E_CTIMER_1, E_CTIMER_CLK);
	
	//FFT



	uint32_t cycles = (uint32_t) e_ctimer_get(E_CTIMER_1);
	uint32_t fpops  = (uint32_t) e_ctimer_get(E_CTIMER_0);

	*(UserInterrupt*) 0x24 = UserInterrupt::Done:

	return 0;
}


void fft(int nn, int inverse){
	
	int n, mmax, m, j, istep, i;
	float wtemp, wr, wpr, wpi, wi, theta;
	float tempr, tempi;	

	n = nn << 1;
	j = 1;
	//reordering
	for(i = 1; i < n; i += 2){
		if(j > i){
			tempr = data[j];
			data[j] = data[i];
			data[i] = tempr;
			tempr = data[j+1];
			data[j+1] = data[i+1];
			data[i+1] = tempr;
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
				tempr = wr*data[j] - wi*data[j+1];
				tempi = wr*data[j+1] + wi*data[j];
				data[j] = data[i] - tempr;
				data[j+1] = data[i+1] - tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr = (wtemp = wr)*wpr - wi*wpi + wr;
			wi = wi*wpr + wtemp*wpi + wi;	
		}
		mmax = istep;
	}
}
