#!/bin/bash -eux


pushd target
make all -j6
#make test ARGS='-V'
./testall -v
make report
popd
