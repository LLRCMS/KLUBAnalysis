#!/usr/bin/env bash

## THIS TO CHECK ON LD_LYBRARY_PATH: will work on screen but leave previous path in the config

# paths=$(echo $LD_LIBRARY_PATH | tr ":" "\n") # split on :
# pathunset=1
# for pp in $paths
# do
#     if [[ $pp == *"`pwd`/lib"* ]] ; then
#         echo "Warning: path to KLUB binaries already set to: $pp"
#         echo "         ... Ignoring this setup command"
#         pathunset=0
#     fi
# done

# paths=$(echo $PATH | tr ":" "\n") # split on :
pathunset=1
# for pp in $paths 
# do
#     if [[ $pp == *KLUBAnalysis/bin ]] ; then
#         echo "Warning: path to KLUB binaries already set to: $pp"
#         echo "         ... Ignoring this setup command"
#         pathunset=0
#         break
#     fi
# done

if [ ${pathunset} -eq 0 ] ; then
    THISFILE="$( [ ! -z "$ZSH_VERSION" ] && echo "${(%):-%x}" || echo "${BASH_SOURCE[0]}" )"
    KLUBDIR="$( cd "$( dirname "$THISFILE" )/.." && pwd )"
    CMSSWDIR="$( cd "$( dirname "$THISFILE" )/../.." && pwd )"
	echo $KLUBDIR
	
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${KLUBDIR}/lib"
    export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${CMSSWDIR}/HHKinFit2"

    if [ -n "${DYLD_LIBRARY_PATH}" ]; then
        export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${KLUBDIR}/lib"
        export DYLD_LIBRARY_PATH="${DYLD_LIBRARY_PATH}:${CMSSWDIR}/HHKinFit2"
    fi

    export PATH="${PATH}:${KLUBDIR}/bin"
fi
