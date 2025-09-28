#!/bin/bash -eux

# ulimit -S -c unlimited
# ulimit -c
# 

pushd target
make all -j6
#make test ARGS='-V'
./testall -v
make report
popd
