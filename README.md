# g4-vis

Copied from geant4/examples/extendend/gdml/G01. 
Its [README](README.g4) and [History](History.g4) are
included here for reference.

While loading the GDML file(s), the Geant4 messages are
directed to `gdml_parse.out` and `gdml_parse.err` for
later inspection.

## Set Up 

### Container
The [Dockerfile](Dockerfile) packaged here is useful for creating
an image with this code and its dependencies built into it. It
takes some time to build an image (since we have to build Geant4),
but once Geant4 is built, re-building with only changes to this
source code is relatively quick.
```
docker build . -t ldmx-g4-vis
```
Running is a bit more complicated since we want to share the display
and the directory from which we are running. [ldmx-g4-vis](ldmx-g4-vis)
is a short shell script that does the correct mounting behavior. You 
could copy it to a directory in your `PATH` or just run it directly.
```
cp /path/to/ldmx-g4-vis/ldmx-g4-vis ~/.local/bin #for example
ldmx-g4-vis Detectors/data/ldmx-det-v14
```

### Container-less
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

Run from the directory where all the GDML files reside so
that any cross referencing can be deduced. The [env.sh](env.sh)
scripts offers an example of shortening this call.
```
cd ldmx-sw/Detectors/data/ldmx-det-v14
/path/to/ldmx-g4-vis/build/load_gdml detector.gdml
```

## Why
For those fluent with Geant4, you might wonder why we would have our own visualization
tool when there already exists an example program that loads and visualizes GDML. There
are a few cosmetic reasons for this.
1. We can redirect Geant4's logging and error messages into files so that they can be
   inspected more closely.
2. We can apply the `VisAttributes` as defined in our GDML auxilliary information.
3. We code in the OGL viewer instead of having the program call out to Geant4 macro file.

None of these are particularly _necessary_ to view the LDMX GDML design, but having our
own executable also gives us the ability to align it better with our workflow.
