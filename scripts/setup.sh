#!/usr/bin/env bash

THISFILE="$( [ ! -z "$ZSH_VERSION" ] && echo "${(%):-%x}" || echo "${BASH_SOURCE[0]}" )"
KLUBDIR="$( cd "$( dirname "$THISFILE" )/.." && pwd )"
CMSSWDIR="$( cd "$( dirname "$THISFILE" )/../.." && pwd )"

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${KLUBDIR}/lib"
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${CMSSWDIR}/HHKinFit2"

if [ -n "${DYLD_LIBRARY_PATH}" ]; then
    export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${KLUBDIR}/lib"
    export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${CMSSWDIR}/HHKinFit2"
fi

export PATH="${PATH}:${KLUBDIR}/bin"

