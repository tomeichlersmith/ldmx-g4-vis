
# g4-vis/env.sh
#  simply source geant4.sh and add load_gdml to PATH

source $HOME/ldmx/geant4/install/bin/geant4.sh

# just doing an alias rather than changing PATH
#    since it is only one executable
alias load_gdml=$HOME/ldmx/g4-vis/build/load_gdml
