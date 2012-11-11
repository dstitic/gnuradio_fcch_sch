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

#ifndef INCLUDED_DST_FCCH_H
#define INCLUDED_DST_FCCH_H

#include <dst_api.h>
#include <gr_block.h>

class dst_fcch;
typedef boost::shared_ptr<dst_fcch> dst_fcch_sptr;

DST_API dst_fcch_sptr dst_make_fcch (double rho = 0.03125, double G = 0.08, int D = 5, int taps = 17);

/*!
 * \brief <+description+>
 *
 */
class DST_API dst_fcch : public gr_block
{
	friend DST_API dst_fcch_sptr dst_make_fcch (double rho, double G, int D, int taps);
	double d_rho;
	double d_G;
	int d_D;
	int d_taps;
	dst_fcch (double rho, double G, int D, int taps);

 public:

  public:
  double rho () const { return d_rho; }
  void set_rho (double rho) { d_rho = rho; }

  double G () const { return d_G; }
  void set_G (double G) { d_G = G; }

  int D () const { return d_D; }
  void set_D (double D) { d_D = D; }

  int taps () const { return d_taps; }
  void set_taps (double taps) { d_taps = taps; }

	~dst_fcch ();


  int general_work (int noutput_items,
		    gr_vector_int &ninput_items,
		    gr_vector_const_void_star &input_items,
		    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DST_FCCH_H */

