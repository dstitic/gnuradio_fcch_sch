/* -*- c++ -*- */
/*
 * Copyright 2012 <+YOU OR YOUR COMPANY+>.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gr_io_signature.h>
#include <dst_fcch.h>
#include <stdexcept>
#include <cstdio>
//#include <gr_buffer.h>
#include <gr_complex.h>

dst_fcch_sptr
dst_make_fcch (double rho, double G, int D, int taps)
{
	return dst_fcch_sptr (new dst_fcch (rho, G, D, taps));
}

static const int MIN_IN = 1;	// mininum number of input streams
static const int MAX_IN = 1;	// maximum number of input streams
static const int MIN_OUT = 1;	// minimum number of output streams
static const int MAX_OUT = 1;	// maximum number of output streams


dst_fcch::dst_fcch (double rho, double G, int D, int taps)
	: gr_block ("fcch",
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_complex)),
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_complex))),
		d_rho (rho),
		d_G (G),
		d_D (D),
		d_taps (taps)


{
}


dst_fcch::~dst_fcch ()
{
}
   //gr_complex work_buffer[work_length];

   //gr_complex Eavg_buffer[work_length];
   //gr_complex error_buffer[work_length];
   //gr_complex divisor = 2;

   ////firale extra variables
   //gr_complex e[work_length];

   //gr_complex alpha = 1;


   //Eavg_buffer[0] = 0;
   //error_buffer[0] = 0;
   //gr_complex y[d_taps];
   //y[0] = 0;
   //gr_complex tap_w[d_taps];
   //int single_error = 0;


   //double rho, double G, int D, int taps
void
old_calculate_fcch_firale(gr_complex work_buffer[], int work_length, double alpha, double d_rho, double d_G, int d_D, int d_taps, double out_buffer[]){

		//gr_complex Eavg_buffer[work_length];
		double eavg[work_length];
		eavg[0] = 0;
		gr_complex y[work_length];
		gr_complex tap_w[work_length];
		gr_complex e[work_length];


		//taps initialisieren
		for (int it = 0; it < d_taps; it++){
		   tap_w[it] = 0;
		}

		//predictor
		for (int i = d_taps; i < (work_length+d_taps); i++){
			for (int ii = 0; ii < d_taps; ii++){
				y[i]= conj(tap_w[ii]) * work_buffer[i-ii];
			}
		}

		//error
		for (int i = 0; i < work_length; i++){
		e[i] = work_buffer[i + d_D] - y[i];
		}

		//fir coefficients
		for (int i = 0; i < work_length; i++){
			tap_w[i] = tap_w[i] + (gr_complex)d_G * conj(e[i]) * work_buffer[i];
		}
		//average error power
		for (int i = 0; i < work_length; i++){
			out_buffer[i] = ((1-d_rho) * out_buffer[i] + d_rho * abs(e[i])*abs(e[i]));;

		}
		//

		//out_buffer = eavg;
}


int
dst_fcch::general_work (int noutput_items,
			       gr_vector_int &ninput_items,
			       gr_vector_const_void_star &input_items,
			       gr_vector_void_star &output_items)
{
  const gr_complex  *in = (const gr_complex  *) input_items[0];
  gr_complex  *out = (gr_complex  *) output_items[0];

   //basics,,
	int work_length = d_taps + d_D + 16;

	gr_complex work_buffer[work_length];
	double out_buffer[work_length];
	double alpha = 1;

	noutput_items = work_length;


	for (int i = 0; i < noutput_items; i++){
		work_buffer[i] = in[i];
	}

	//calculate_fcch_firale(work_buffer, work_length, alpha, d_rho, d_G, d_D, d_taps, out_buffer);

	for (int i = 0; i < noutput_items; i++){
		out[i] = work_buffer[i];
	}



  // Tell runtime system how many input items we consumed on
  // each input stream.
  consume_each (noutput_items);

  // Tell runtime system how many output items we produced.
  return noutput_items;


}
