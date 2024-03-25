#!/bin/bash -eux


pushd target
make helgrindp
popd

