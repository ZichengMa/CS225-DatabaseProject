#!/bin/bash 
export LD_LIBRARY_PATH="./libs"
export QT_DEBUG_PLUGINS=1
#export QT_QPA_PLATFORM_PLUGIN_PATH="./platforms"
./dataUI "$@"
