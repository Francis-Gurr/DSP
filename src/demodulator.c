#include "demodulator.h"
#include "structs.h"
#include<stdlib.h>
#include<stdbool.h>

void demod(double *p_in, const int SIZE, struct Demod *osc){

	#pragma omp parallel shared(p_in, osc)
	{
		/* Pointers for input and output values */
		const double *p_osc = osc->p_OSC;
		int i_osc = osc->index;
		bool inverse = osc->inverse;

		/* Count of how many samples have been demodulated */
		#pragma omp for
		for (int i_s = 0; i_s < SIZE; i_s++) {
			double sample = *(p_in + i_s);
			/* Coherent demodulation: multiply signal by generated oscillator value */
			sample = sample * *(p_osc + i_osc);
			if (inverse) {
				sample = -1 * sample;
			}
			*(p_in + i_s) = sample;

			/* Move pointer to next oscillator value */
			if (++i_osc >= osc->SIZE) {
				i_osc = 0;
				inverse = !inverse;
			}
		}	
		osc->index = i_osc;
		osc->inverse = inverse;
	}
}

