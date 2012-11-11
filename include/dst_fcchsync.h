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

#ifndef INCLUDED_DST_FCCHSYNC_H
#define INCLUDED_DST_FCCHSYNC_H
#define fcch_len 156.25 //length of fcch burst + guard times
#include <dst_api.h>
#include <gr_sync_block.h>
#include <vector>
#include <list>
#include <gr_firdes.h>
#include <gri_fft.h>
class dst_fcchsync;
typedef boost::shared_ptr<dst_fcchsync> dst_fcchsync_sptr;

DST_API dst_fcchsync_sptr dst_make_fcchsync (double rho = 0.03125, double G = 0.08, int D = 5, int taps = 17, int osr = 4, int fsamp = 1083333);

/*!
 * Calculates the Freqency offset of a GSM system by searching the FCCH <+description+>
 *
 *
 */
class DST_API dst_fcchsync : public gr_sync_block
{
    friend DST_API dst_fcchsync_sptr dst_make_fcchsync (double rho, double G, int D, int taps, int osr, int fsamp);

    double d_rho;
    double d_G;
    int d_D;
    int d_taps;
    int d_osr;
    int d_fsamp;
    double freq;
    std::vector<gr_complex> taps_buffer;
    std::vector<gr_complex> eavg_buffer;

    std::list<double> mea_list;

    gr_complex y;
    gr_complex e;
    int detectedago, detectedcounter;
    bool firstrun;
    dst_fcchsync (double rho, double G, int D, int taps, int osr, int fsamp);

public:

    double rho () const
    {
        return d_rho;
    }

    void set_osr (int osr)
    {
        d_osr = osr;
    }
    void set_rho (double rho)
    {
        d_rho = rho;
    }

    double G () const
    {
        return d_G;
    }
    void set_G (double G)
    {
        d_G = G;
    }

    int D () const
    {
        return d_D;
    }

    int fsamp () const
    {
        return d_fsamp;
    }
     void set_D (double D)
    {
        d_D = D;
    }

    int taps () const
    {
        return d_taps;
    }
    void set_taps (double taps)
    {
        d_taps = taps;
    }


    void calculate_fcch_firale(int noutput_items, const gr_complex input_buffer[], gr_complex out_buffer[], gr_complex *offset, std::vector<gr_complex> taps_buffer,std::vector<double> eavg_buffer, double alpha, double d_rho, double d_G, int d_D, int d_taps);

    ~dst_fcchsync ();


    int work (int noutput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items
              );
};

#endif /* INCLUDED_DST_FCCHSYNC_H */

