# HASHADD=true
# type root >/dev/null 2>&1 || { HASHADD=false; }

# if [ $HASHADD = true ] ; then
for dir in ./*/
do
  cd $dir
  pwd
  
  ### corrupted dir exists -- move here
  if [ -d corrupted ]; then
    if [ "$(ls -A corrupted)" ]; then
      mv corrupted/* .
    fi
    rm -r corrupted
  fi

  ### singleFiles dir does not exists -- do not do anything
  if [ -d singleFiles ]; then
    if [ "$(ls -A singleFiles)" ]; then
      mv singleFiles/* .
    fi
    rm -r singleFiles
  fi

  ### total.root exists -- delete it
  if [ -f total.root ]; then
    rm total.root
  fi

  ## go back
  cd -
done
# else
#    echo "hadd not available, please do cmsenv in a CMSSW release"
# fi
