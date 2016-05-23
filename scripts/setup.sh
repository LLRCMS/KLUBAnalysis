paths=$(echo $PATH | tr ":" "\n") # split on :
pathunset=true
for pp in $paths
do
    if [[ $pp == *"KLUBAnalysis/bin"* ]] ; then
        echo "Warning: path to KLUB binaries already set to: $pp"
        echo "         ... Ignoring this setup command"
        pathunset=false
    fi
done

if [ "$pathunset" = true ] ; then
    export THISDIR=`pwd`
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/lib
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/../HHKinFit2

    if [ -n "${DYLD_LIBRARY_PATH}" ] ; then
    export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/lib
    export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/../HHKinFit2
    fi

    export PATH=${PATH}:${THISDIR}/bin
fi
