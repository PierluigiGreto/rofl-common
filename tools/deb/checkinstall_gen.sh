#!/bin/bash

PKG_NAME="rofl"
PKG_NAME_DBG="rofl-dbg"
VERSION="0.4"
RELEASE="0"
REQUIRES+="libssl, libcrypto++"
LICENSE="Mozilla Public License, Version 2.0"
GROUP="net"
MAINTAINER="rofl@bisdn.de"
DESCRIPTION="Revised OpenFlow Libraries"
IS_DEBUG=""

check_root(){
	if [ "$(id -u)" != "0" ]; then
	   echo "This script must be run as root" 1>&2
	   exit 1
	fi
}

dump_result(){
	echo "Detected configuration"
	echo "Package name: $1"
	echo "Version: $VERSION"
	echo "Release: $RELEASE"
	echo "Requires: $REQUIRES"
	echo "License: $LICENSE"
	echo "Package group: $GROUP"
	echo "Maintainer: $MAINTAINER"
	echo "Replaces and conflicts: $2"

	read -p "Proceed? " -n 1 -r
	echo    # (optional) move to a new line
	if [[ $REPLY =~ ^[Yy]$ ]]
	then
		echo	
	else
		exit 0
	fi	
}

is_debug(){
	IS_NOT_DEBUG=`cat config.log| grep NDEBUG`
}

get_version(){
	VERSION=`cat ../VERSION | sed s/dev//g | sed s/RC.*//g | tr -d v`
}

get_release(){
	RELEASE=`cat ../VERSION`
}

generate_pkg(){
	echo $DESCRIPTION > description-pak
	sudo checkinstall -D -y --pkgname="$1" --pkgversion="$VERSION" --pkgrelease="$RELEASE" --pkglicense="$LICENSE" --pkggroup="$GROUP" --maintainer="$MAINTAINER" --provides="$1" --requires="$REQUIRES" --conflicts="$2" --replaces="$2"
}

#
# Main routine
#

#Detect stuff
check_root
is_debug
get_version
get_release

#Generate the package
if test -z "$IS_NOT_DEBUG";
then
	dump_result $PKG_NAME_DBG $PKG_NAME
	generate_pkg $PKG_NAME_DBG $PKG_NAME
else
	dump_result  $PKG_NAME $PKG_NAME_DBG
	generate_pkg $PKG_NAME $PKG_NAME_DBG
fi
