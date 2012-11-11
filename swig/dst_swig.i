/* -*- c++ -*- */

#define DST_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "dst_swig_doc.i"


%{
#include "dst_fcch.h"
#include "dst_fcchsync.h"
#include "dst_sch.h"
%}


GR_SWIG_BLOCK_MAGIC(dst,fcch);
%include "dst_fcch.h"

GR_SWIG_BLOCK_MAGIC(dst,fcchsync);
%include "dst_fcchsync.h"

GR_SWIG_BLOCK_MAGIC(dst,sch);
%include "dst_sch.h"
