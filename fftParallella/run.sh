#!/bin/bash

set -e

if [[ "$1" == "-d" ]]; then
	Config='Debug'
else
	Config='Release'
fi

if [[ "$1" == "" ]]; then
	Img=00000.tif
else
	Ing=$@
fi

ELIBS="${EPIPHANY_HOME}/tools/host/lib"
EHDF="${EPIPHANY_HOME}/bsps/current/platform.hdf"

sudo -E LD_LIBRARY_PATH=${ELIBS} EPIPHANY_HDF=${EHDF} ./FFT_PARALLELLA-host ${Img} 512 500
