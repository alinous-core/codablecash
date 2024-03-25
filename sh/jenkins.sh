#!/bin/bash

rm -rf ./target/

./sh/cmakeDebug.sh
./sh/maketest.sh
./sh/makecheck.sh

