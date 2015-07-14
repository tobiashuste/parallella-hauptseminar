#!/bin/bash

ELIBS="${EPIPHANY_HOME}/tools/host/lib"
EHDF="${EPIPHANY_HOME}/bsps/current/platform.hdf"

sudo -E LD_LIBRARY_PATH=${ELIBS} EPIPHANY_HDF=${EHDF} ./memory
