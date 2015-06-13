#!/bin/bash

set -e

if [[ "$1" == "-d" ]]; then
	Config='Debug'
else
	Config='Release'
fi

if [[ "$1" == "" ]]; then
	Img=img.tif
else
	Ing=$@
fi

ELIBS="${EPIPHANY_HOME}/tools/host/lib"
EHDF="${EPIPHANY_HOME}/bsps/current/platform.hdf"

sudo -E LD_LIBRARY_PATH=${ELIBS} EPIPHANY_HDF=${EHDF} ./FFT_PARALLELA-host ${Img} 256 256



