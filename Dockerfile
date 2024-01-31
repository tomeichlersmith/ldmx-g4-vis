FROM ubuntu:22.04
RUN apt-get update &&\
    DEBIAN_FRONTEND=noninteractive \
    apt-get install --yes \
      wget \
      cmake \
      expat \
      gcc \
      g++ \
      libxerces-c-dev \
      qtcreator \
      qtbase5-dev \
      qt5-qmake \
    && rm -rf /var/lib/apt/lists/* &&\
    apt-get autoremove --purge &&\
    apt-get clean all
RUN mkdir g4-src &&\
    wget -q -O - https://github.com/geant4/geant4/archive/v10.7.3.tar.gz |\
      tar -xz --strip-components=1 --directory g4-src &&\
    cmake \
      -DCMAKE_INSTALL_PREFIX=/usr/local \
      -DGEANT4_USE_SYSTEM_EXPAT=OFF \
      -DGEANT4_USE_GDML=ON \
      -DGEANT4_USE_QT=ON \
      -DGEANT4_INSTALL_EXAMPLES=ON \
      -DGEANT4_INSTALL_DATA=ON \
      -B g4-src/build \
      -S g4-src \
      &&\
    cmake --build g4-src/build --target install -- -j4 &&\
    rm -rf g4-src &&\
    ldconfig
COPY ./CMakeLists.txt /vis-src/
COPY ./src /vis-src/src
RUN cmake -B /vis-src/build -S /vis-src &&\
    cmake --build /vis-src/build
ENV G4DATADIR="/usr/local/share/Geant4-10.7.3/data"
ENV G4NEUTRONHPDATA="${G4DATADIR}/G4NDL4.6"
ENV G4LEDATA="${G4DATADIR}/G4EMLOW7.13"
ENV G4LEVELGAMMADATA="${G4DATADIR}/PhotonEvaporation5.7"
ENV G4RADIOACTIVEDATA="${G4DATADIR}/RadioactiveDecay5.6"
ENV G4PARTICLEXSDATA="${G4DATADIR}/G4PARTICLEXS3.1.1"
ENV G4PIIDATA="${G4DATADIR}/G4PII1.3"
ENV G4REALSURFACEDATA="${G4DATADIR}/RealSurface2.2"
ENV G4SAIDXSDATA="${G4DATADIR}/G4SAIDDATA2.0"
ENV G4ABLADATA="${G4DATADIR}/G4ABLA3.1"
ENV G4INCLDATA="${G4DATADIR}/G4INCL1.0"
ENV G4ENSDFSTATEDATA="${G4DATADIR}/G4ENSDFSTATE2.3"
ENTRYPOINT [ "/vis-src/build/load_gdml" ]
