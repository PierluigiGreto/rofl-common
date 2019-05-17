rofl-common deps:
```
apt install autoconf libtool pkg-config linux-libc-dev libc6-dev libssl-dev libgoogle-glog-dev g++ libcunit1-dev libcppunit-dev
```
packaging deps
```
apt install build-essential devscripts debhelper dh-autorecon dh-autoreconf autogen
cd rofl-common
mkdir debian
dch --create -v 0.13.2-1 --package rofl-common
```
create debian/control
```
Source: rofl-common
Section: rofl-common
Priority: optional
Maintainer: Pierluigi Greto <pierluigi.greto@bisdn.de>

Package: rofl-common
Version: 0.13.2
Depends: autoconf, libtool, pkg-config, linux-libc-dev, libc6-dev, libssl-dev, libgoogle-glog-dev, g++, libcunit1-dev, libcppunit-dev
Architecture: any
Description: rofl-common
 When you need some sunshine, just run this
 small program!
```
create debian/rules:
```
#!/usr/bin/make -f
export DEB_BUILD_OPTIONS=nocheck

%:
        dh $@ --with autoreconf
```
to build
```
mv rofl-common rofl-common_0.13.2-1
tar -zcvf rofl-common_0.13.2.orig.tar.gz rofl-common_0.13.2-1
cd rofl-common_0.13.2-1/
debuild -us -uc
```
