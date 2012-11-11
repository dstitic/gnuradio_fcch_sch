#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/denis/Thesis/gr-dst/lib
export PATH=/home/denis/Thesis/gr-dst/lib:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
qa_dst_stream_tag_file_sink 
