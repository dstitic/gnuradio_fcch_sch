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
#include <dst_fcchsync.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <string.h>
#define DEBUG

dst_fcchsync_sptr
dst_make_fcchsync (double rho, double G, int D, int taps, int osr, int fsamp)
{
    return dst_fcchsync_sptr (new dst_fcchsync (rho, G, D, taps, osr, fsamp));
}
static const int MIN_IN = 1;	// mininum number of input streams
static const int MAX_IN = 1;	// maximum number of input streams
static const int MIN_OUT = 2;	// minimum number of output streams
static const int MAX_OUT = 2;	// maximum number of output streams

dst_fcchsync::dst_fcchsync (double rho, double G, int D, int taps, int osr, int fsamp)
    : gr_sync_block ("fcchsync",
                     gr_make_io_signature (MIN_IN, MAX_IN, sizeof (gr_complex)),
                     gr_make_io_signature (MIN_OUT, MAX_OUT, sizeof (gr_complex))),
    d_rho (rho),
    d_G (G),
    d_D (D),
    d_taps (taps),
    d_osr(osr),
    d_fsamp(fsamp)
{
    this->freq=0;
    this->y=0;
    this->e=0;
    this->detectedago = 0;
    this->detectedcounter = 0;
    this->firstrun = true;
}

int nextpow2 (int input)
{
    if (!input){
        return 0;
    }
    int out = 1;
    while (out <  input){
        out <<= 1;
    }
    return out;
}

dst_fcchsync::~dst_fcchsync ()
{
}

///
///fcch firale over input
///
void
dst_fcchsync::calculate_fcch_firale(int noutput_items, const gr_complex input_buffer[], gr_complex out_buffer[], gr_complex *offset, std::vector<gr_complex> taps_buffer, std::vector<double> eavg_buffer, double alpha, double d_rho, double d_G, int d_D, int d_taps)
{   int debugcounter1 = 0;
    double fcch_freq = 1625000 / 6 /4;
    bool fcch_found = false;
    double temp_offset=0;
    #ifdef DEBUG
    std::ofstream output1, output2, output3, output4;
    #endif

    int f_samp = d_fsamp;

    int fft_len = nextpow2(d_osr *fcch_len) *32;
    gr_complex fft_raw_in[fft_len];
    gr_complex *fft_input = NULL;
    gr_complex *fft_output = NULL;
    double tempmaxx, tempmaxy;
    double freqfrac;

    int MEA = (int)(fcch_len  * d_osr);

    int work_length = noutput_items;

    double error_average = 0;

    for (int i = 0; i < (work_length); i++)
    {
        error_average+=alpha*abs(input_buffer[i])*abs(input_buffer[i]);
    }
    error_average/=work_length;

    int detectedago = 0;

    for (int i = 0; i < (work_length); i++)
    {
        ///predictor
        for (int k = 0; k < this->d_taps; k++)
        {
           this->y = this->y + (conj(taps_buffer[k]) * input_buffer[i-(k)]);
        }

        ///error
        this->e = input_buffer[(i + this->d_D)] - this->y;

        ///average error power
        eavg_buffer[i] = ((1-this->d_rho) * eavg_buffer[i]) + (this->d_rho * abs(this->e)*abs(this->e));

        ///fir coefficients
        for (int k =0; k < this->d_taps; k++)
        {
            taps_buffer[k] = taps_buffer[k] + (gr_complex)this->d_G * conj(this->e) * input_buffer[i];
        }
        ///moving error average, cut off sample wise
        double temp_num = 0;
        mea_list.push_back(eavg_buffer[i]/error_average);

        if (mea_list.size() > MEA) {
            mea_list.pop_front();
        }
        for (std::list<double>::iterator it = mea_list.begin(); it != mea_list.end(); it++)
        {
            temp_num += *it;
        }

        ///error low point
        if (((temp_num) < 0.5  ) && i > 150 * d_osr)
        {
            fcch_found = true;
            debugcounter1++;

            int temp = abs(this->detectedago - i);
            if (temp > (d_osr * fcch_len) | (firstrun == true))
            {
                                                        //FCCH detected
                this->detectedago = i;
                this->firstrun = false;
                this->detectedcounter = this->detectedcounter + 1;

                gri_fft_complex * d_fft = new gri_fft_complex (fft_len, true, 1);

                fft_input = d_fft->get_inbuf();
                fft_output = d_fft->get_outbuf();
                double temp_fcch_start;

                #ifdef DEBUG
                output4.open ("gr_debug_4.txt", std::iostream::out | std::iostream::app);
                #endif
                    temp_fcch_start = (i - ((d_osr * fcch_len)-floor(50/4  * d_osr)));
                    //FFT Zero Padding
                    for(int tt = 0; tt<fft_len; tt++)
                    {

                        if (tt < (d_osr * fcch_len)-50)
                        {

                            fft_input[tt] = real(input_buffer[(int)temp_fcch_start+tt]);

                            #ifdef DEBUG
                            output4 << real(input_buffer[(int)temp_fcch_start+tt]);
                            output4 << ";";
                            #endif


                        }
                        else
                        {
                            fft_input[tt] = 0;

                        }
                    }

            #ifdef DEBUG
            output4.flush();
            output4.close();
            #endif

            d_fft->execute();

            #ifdef DEBUG
            output2.open ("gr_debug_2.txt", std::iostream::out | std::iostream::app);
            output2 << "\r\n";
            for(int op = 0; op < fft_len; op++){
            output2 << fft_output[op];
            output2 << ";";
             output2.flush();
            }
            output2.close();
            #endif

            //left side
            double half_fft[fft_len/2];
            for (int ii = 0; ii < fft_len/2; ii++){
                half_fft[ii]=abs(fft_output[ii]);
            }

            #ifdef DEBUG
            output1.open ("gr_debug_1.txt", std::iostream::out | std::iostream::app);
            output1 << "\r\n";
            for(int op = 0; op < fft_len/2; op++)
            {
            output1 << half_fft[op];
            output1 << ";";
            output1.flush();
            }
            #endif



            #ifdef DEBUG
            output1.close();
            #endif
            //find maximum
            tempmaxx = 0;
            tempmaxy = 0;
            float tempstart =  floor((2/100)*fft_len);
            for (int yy = (int)tempstart; yy < (fft_len/2); yy++)
            {
                if (half_fft[yy] > tempmaxy){
                    tempmaxy = half_fft[yy];
                    tempmaxx = yy;
                    }
            }

            #ifdef DEBUG
            output3.open ("gr_debug_3.txt", std::iostream::out | std::iostream::app);
            output3 << "\r\ntempstart: ";
            output3 << tempstart;  // 0
            output3 << "; tempmaxy: ";
            output3 << tempmaxy;                // 0
            output3 << "; tempmaxx: ";
            output3 << tempmaxx;                // 509
            #endif

//            freqfrac = tempmaxx / (fft_len/2);
            freqfrac = tempmaxx / (fft_len);
//            this->freq = freqfrac * f_samp/2;
            this->freq = freqfrac * f_samp;

            #ifdef DEBUG
            output3 << "; fft_len: ";
            output3 << fft_len;
            output3 << "; freqfrac: ";
            output3 << freqfrac;
            output3 << "; freq; ";
            output3 << freq;
            output3 << "; Offset: ";
            output3 << this->freq - (1625000 / 6 /4);
            output3 << "; temp fcch_start: ";
            output3 << temp_fcch_start;
            output3 << "; i: ";
            output3 << i;
            output3 << "; 0.5 * d_osr * fcch_len: ";
            output3 << 0.5 * d_osr * fcch_len;
            output3 << "; d_osr * fcch_len: ";
            output3 << d_osr * fcch_len;
            output3 << "; noutput_items: ";
            output3 << noutput_items;
            output3 << "; debugcounter1: ";
            output3 << debugcounter1;
            output3.flush();
            output3.close();
            #endif

            if (this->freq > 0)
            {
                temp_offset = this->freq - (1625000 / 6 /4);
            }
            offset[i] = temp_offset;
//
//
            }
            ///detected end of FCCH burst -> mark with Stream Tag
            const size_t item_index = i;
            const uint64_t offset = this->nitems_written(0) + (item_index - (140/2 * d_osr));
            pmt::pmt_t key = pmt::pmt_string_to_symbol("fcch_detected");
            pmt::pmt_t value = pmt::pmt_from_double((eavg_buffer[i] /error_average));
            this->add_item_tag(0, offset, key, value);

        }
        else {
            //Calculate Frequency from this
            if (this->freq > 0)
            {
                temp_offset = this->freq - (1625000 / 6 /4);
            }

           offset[i] = temp_offset;
           out_buffer[i] = input_buffer[i];

        }
    this->y = 0;
    }
}

int
dst_fcchsync::work (int noutput_items,
                    gr_vector_const_void_star &input_items,
                    gr_vector_void_star &output_items
                    )
{
    const gr_complex *in = (const gr_complex *) input_items[0];
    gr_complex *out = (gr_complex *) output_items[0];
    gr_complex *offset = NULL;
    offset = (gr_complex *) output_items[1];
    //secondary output stream
    std::vector<double> eavg_buffer(noutput_items); eavg_buffer[0] = 0;
    std::vector<gr_complex> taps_buffer(d_taps);
    calculate_fcch_firale(noutput_items, in, out, offset, taps_buffer, eavg_buffer, 1, d_rho, d_G, d_D, d_taps);
    // Tell runtime system how many output items we produced.
    return noutput_items;
}

