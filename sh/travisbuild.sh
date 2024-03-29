#!/bin/bash -eux


if test -e target ; then
  rm -rf ./target/
fi

mkdir target

pushd target

#cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++-5 -DCMAKE_C_COMPILER=gcc-5 ..
cmake -DCMAKE_BUILD_TYPE=Debug ..

make all
#make test ARGS='-V'
./testall -v
make report

popd
