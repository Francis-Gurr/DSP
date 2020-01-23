#include "structs.h"

float * demod(float *p_in, int size, struct Demod *osc){

	/* Pointers for input and output values */
	float *p_out;
	float *p_osc = osc->p_OSC;
	int index = osc->index;
	int inverse = osc->inverse;

	/* Count of how many samples have been demodulated */
	int counter = 0;

	while(counter < size) {

		/* Coherent demodulation: multiply signal by generated oscillator value */
		if (inverse == 1) {
			*p_out = -1 * (*p_in * *(p_osc + index));
		}
		else {
			*p_out = *p_in * *(p_osc + index);
		}

		/* Wrap around and flip the cos values */
		else {
			index = 0;
			inverse = abs(inverse - 1);
		}
		/* Move pointer to next oscillator value */
		if (index < osc->SIZE) {
			index++;
		}
		counter++;
		p_in++;
		p_out++;
	}

	osc->index = index;
	osc->inverse = inverse;

	return p_out;
}

