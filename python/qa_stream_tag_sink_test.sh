#!/bin/sh
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/denis/Thesis/gr-dst/python
export PATH=/home/denis/Thesis/gr-dst/python:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DYLD_LIBRARY_PATH
export PYTHONPATH=/home/denis/Thesis/gr-dst/swig:$PYTHONPATH
/usr/bin/python /home/denis/Thesis/gr-dst/python/qa_dst_stream_tag_sink.py 
