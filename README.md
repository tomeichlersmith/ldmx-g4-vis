# g4-vis

Copied from geant4/examples/extendend/gdml/G01. 
Its [README](README.g4) and [History](History.g4) are
included here for reference.

### Set Up

Install Geant4 with Qt
```
# Depedencies
sudo apt install \
  cmake \
  libxerces-c-dev \
  qtcreator \
  qtbase5-dev \
  qt5-qmake \
# Configuration
cd geant4
git fetch --tags upstream
git checkout v10.7.3
cmake -B build -S . \
  -DCMAKE_INSTALL_PREFIX=install \
  -DGEANT4_USE_GDML=ON \
  -DGEANT4_USE_QT=ON
# build
cd geant4/build
make -j2 install
```

Source the environment script
```
source /full/path/to/geant4/install/bin/geant4.sh
```

Build this library
```
cmake -B build -S .
cd build
make
```
