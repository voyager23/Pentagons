#!/bin/bash
echo "Pentagon log" > scan.log
TARGET=$1
LIMIT=$2
while [ $TARGET -le $LIMIT ]; do
	echo $TARGET
	./penta $TARGET >> scan.log
	let TARGET=TARGET+2
done
