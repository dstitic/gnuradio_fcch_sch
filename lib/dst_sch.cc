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
#include <dst_sch.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#define DEBUG

dst_sch_sptr
dst_make_sch (int osr)
{
	return dst_sch_sptr (new dst_sch (osr));
}
static const int MIN_IN = 1;	// mininum number of input streams
static const int MAX_IN = 1;	// maximum number of input streams
static const int MIN_OUT = 1;	// minimum number of output streams
static const int MAX_OUT = 1;	// maximum number of output streams

dst_sch::dst_sch (int osr)
	: gr_sync_block ("sch",
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_complex)),
		gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_complex))),
		d_osr(osr)
{
}

dst_sch::~dst_sch ()
{
}

gr_complex *fir_filter(gr_complex fir_taps[], gr_complex fir_in[])
{
int fir_taps_len = sizeof(fir_taps)/sizeof(fir_taps[0]);
int fir_in_len = sizeof(fir_in)/sizeof(fir_in[0]);

gr_complex *fir_out = new gr_complex[fir_in_len - fir_taps_len];

int fir_out_len = sizeof(fir_out)/sizeof(fir_out[0]);

for(int k = fir_taps_len; k < fir_out_len; k++)
{
    for (int i = 0; i < fir_taps_len; i++)
    {
        fir_out[k] +=  fir_taps[i] * fir_in[k - i];
    }
}
return fir_out;
}

gr_complex *flip_conj(gr_complex input_array[])
{
    int size_temp = sizeof(input_array)/sizeof(input_array[0]);
    gr_complex *flip_conj_out = new gr_complex[size_temp];

    for (int i = 0; i < size_temp; i++)
    {
        flip_conj_out[size_temp - i] = conj(input_array[i]);
    }

}


int
dst_sch::work (int noutput_items,
			gr_vector_const_void_star &input_items,
			gr_vector_void_star &output_items)
{
    std::ofstream output1, output2, output3, output4;
	const float *in = (const float *) input_items[0];
	float *out = (float *) output_items[0];
    std::vector<gr_tag_t> tags;
    const uint64_t nread = this->nitems_read(0); //number of items read on port 0
    const size_t ninput_items = noutput_items; //assumption for sync block, this can change
  //read all tags associated with port 0 for items in this work function
    pmt::pmt_t key = pmt::pmt_string_to_symbol("fcch_detected");
    this->get_tags_in_range(tags, 0, nread, nread+ninput_items, key);
    output1.open ("gr_debug_sch_1.txt", std::iostream::out | std::iostream::app);

        output1 << "\r\n";
        output1 << "New";
        output1 << " tags.size() ";
        output1 << tags.size();
    //gr_tag_t foo;
    int ext_tr [] =         {1, 0, 1, 1, 1, 0, 0, 1,
                            0, 1, 1, 0, 0, 0, 1, 0,
                            0, 0, 0, 0, 0, 1, 0, 0,
                            0, 0, 0, 0, 1, 1, 1, 1,
                            0, 0, 1, 0, 1, 1, 0, 1,
                            0, 1, 0, 0, 0, 1, 0, 1,
                            0, 1, 1, 1, 0, 1, 1, 0,
                            0, 0, 0, 1, 1, 0, 1, 1};

    for (int i = 0; i < tags.size(); i++)

    {
        output1 << "\r\n";
        output1 << "New";
        output1 << " tags[i].key: ";
        output1 << tags[i].key;
        output1 << " tags[i].value: ";
        output1 << tags[i].value;
        output1 << " tags[i].offset: ";
        output1 << tags[i].offset;
        output1 << "\r\n";
    }

    output1.flush();
    output1.close();
	// Do <+signal processing+>

	// Tell runtime system how many output items we produced.
	return noutput_items;
}

