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

#ifndef INCLUDED_DST_SCH_H
#define INCLUDED_DST_SCH_H

#include <dst_api.h>
#include <gr_sync_block.h>

class dst_sch;
typedef boost::shared_ptr<dst_sch> dst_sch_sptr;

DST_API dst_sch_sptr dst_make_sch (int osr = 4);

/*!
 * \brief <+description+>
 *
 */
class DST_API dst_sch : public gr_sync_block
{
	friend DST_API dst_sch_sptr dst_make_sch (int osr);
    int d_osr;
	dst_sch (int osr);

 public:
	~dst_sch ();

    void set_osr (int osr)
    {
        d_osr = osr;
    }

	int work (int noutput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);
};

#endif /* INCLUDED_DST_SCH_H */

