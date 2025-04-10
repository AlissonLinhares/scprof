#!/bin/sh

SYSTEMC_HTTP_ADDR="https://accellera.org/images/downloads/standards/systemc/systemc-2.3.3.tar.gz"
SYSTEMC_INSTALL_PATH="/usr/local/systemc"

# abort immediately if a command exits with a nonzero status.
set -e

echo " -- Downloading systemc (Default: systemc-2.3.3.tar.gz)."
wget -c $SYSTEMC_HTTP_ADDR -O systemc.tar.gz

echo " -- Extracting files."
tar -xzf systemc.tar.gz
mv systemc\-* systemc

echo " -- Applying patchs."
cp -rf ext/aclocal.m4           systemc/aclocal.m4
cp -rf ext/autom4te.cache       systemc/autom4te.cache
cp -rf ext/configure            systemc/configure
cp -rf ext/docs/Makefile.in     systemc/docs/Makefile.in
cp -rf ext/examples/Makefile.in systemc/examples/Makefile.in

cp -rf ext/examples/Makefile.in            systemc/examples/Makefile.in
cp -rf ext/examples/sysc/Makefile.in       systemc/examples/sysc/Makefile.in
cp -rf ext/examples/tlm/common/Makefile.in systemc/examples/tlm/common/Makefile.in
cp -rf ext/examples/tlm/Makefile.in        systemc/examples/tlm/Makefile.in

cp -rf ext/Makefile.in                               systemc/Makefile.in
cp -rf ext/src/Makefile.in                           systemc/src/Makefile.in
cp -rf ext/src/sysc/datatypes/fx/sc_fixed.h          systemc/src/sysc/datatypes/fx/sc_fixed.h
cp -rf ext/src/sysc/datatypes/fx/sc_fxval_observer.h systemc/src/sysc/datatypes/fx/sc_fxval_observer.h
cp -rf ext/src/sysc/datatypes/fx/sc_ufixed.h         systemc/src/sysc/datatypes/fx/sc_ufixed.h
cp -rf ext/src/sysc/datatypes/fx/sc_ufix.h           systemc/src/sysc/datatypes/fx/sc_ufix.h
cp -rf ext/src/sysc/Makefile.am                      systemc/src/sysc/Makefile.am
cp -rf ext/src/sysc/Makefile.in                      systemc/src/sysc/Makefile.in
cp -rf ext/src/sysc/packages/boost/Makefile.in       systemc/src/sysc/packages/boost/Makefile.in
cp -rf ext/src/sysc/packages/qt/Makefile.in          systemc/src/sysc/packages/qt/Makefile.in

cp -rf ext/src/systemc.h             systemc/src/systemc.h
cp -rf ext/src/tlm_core/Makefile.in  systemc/src/tlm_core/Makefile.in
cp -rf ext/src/tlm_utils/Makefile.in systemc/src/tlm_utils/Makefile.in

cp -rf src systemc/src/sysc/scprof

echo " -- Configuring Systemc + scprof."
mkdir bin
cd bin
../systemc/configure --prefix=$SYSTEMC_INSTALL_PATH

echo " -- Building Systemc + scprof."
make

echo " -- Installing Systemc + scprof."
sudo make install
