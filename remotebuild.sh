#!/bin/bash 

#
# remotebuild.sh - script to perform remote build.
# Main usage - to check build in canonical environment
#

# Default values
REMOTE_HOST=
BUILD_TARGET=all
USER=$LOGNAME
KEY=

PREFIX=build_
DATE=$(date +"%Y-%m-%d_%H-%M-%S")
NAME=$PREFIX$DATE

# Get options
while getopts ":r:u:i:t:h" opt; do
    case $opt in
    r)
        REMOTE_HOST=$OPTARG
    ;;
    u)
        USER=$OPTARG
    ;;
    i)
        KEY=$OPTARG
    ;;
    t)
        BUILD_TARGET=$OPTARG
    ;;
    h)
        echo "Usage: remotebuild.sh [options]"
        echo "Script performs remote build of local source tree under svn."
        echo ""
        echo "Available options:"
        echo "  -r REMOTE_HOST    host on which run build"
        echo "  -u USER           user on remote host"
        echo "  -i KEY            SSH private key"
        echo "  -t TARGET         build target for make"
        echo "  -h                display this message"
        exit 0
    ;;
    \?)
        echo "Invalid option: -$OPTARG" >&2
        exit 3
    ;;
    esac
done

if [ -z "$KEY" ]; then
    echo "Key value may not be empty. Use option -i to specify key."
    exit 3
fi

echo "Build $BUILD_TARGET on remote host $REMOTE_HOST using user $USER and key $KEY"

if [ ! -e $KEY ]; then
    echo "Specified key not found: $KEY";
    exit 3
fi

echo "Test conection to build host..."
ssh -q -l $USER -i $KEY $REMOTE_HOST "echo 2>&1" || ( echo "Cannot connect to remote host"; exit 2 )


echo "Taring sources under svn to local tarbal..."
svn ls -R | tar -T - -czf $NAME.tar.gz || ( echo "Tar failed"; exit 2 )

echo "Copying sources to remote host..."
scp -q -i $KEY $NAME.tar.gz $USER@$REMOTE_HOST:"~/" || ( echo "Scp failed"; exit 2 )

echo "Extract sources on remote host..."
ssh -q -l $USER -i $KEY $REMOTE_HOST "mkdir $NAME; tar -xzf $NAME.tar.gz -C $NAME" || ( echo "Remote extract failed"; exit 2 )

echo "Run remote build..."

ssh -l $USER -i $KEY $REMOTE_HOST "cd $NAME; make $BUILD_TARGET" && echo "BUILD SUCCESSFUL!" || ( echo "BUILD FAILED!"; exit 1 )

exit 0
