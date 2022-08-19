//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file persistency/gdml/G01/load_gdml.cc
/// \brief Main program of the persistency/gdml/G01 example
//
//
//
//
// --------------------------------------------------------------
//      GEANT 4 - load_gdml
//
// --------------------------------------------------------------

#include <vector>

#include "G4Types.hh"

#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4TransportationManager.hh"

#include "G01PrimaryGeneratorAction.hh"
#include "G01DetectorConstruction.hh"
#include "G01ActionInitialization.hh"
#include "VisAttributesStore.hh"

#include "FTFP_BERT.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "G4GDMLParser.hh"

/// print how to use this program
void usage() {
  std::cout << 
    "usage: load_gdml [-h, --help] {input_file.gdml} [batch.mac]\n"
    "\n"
    "  arguments:\n"
    "    input_file.gdml : (required) The GDML file to parse and display\n"
    "                      This GDML file can link to others if necessary\n"
    "    batch.mac       : (optional) Geant4 Macro to run instead of launching display\n"
    "\n"
    "  options:\n"
    "    -h, --help      : print this help message and exit\n"
    << std::endl;
}

// --------------------------------------------------------------

int main(int argc,char **argv) {
  if (argc < 2) {
    std::cerr << "ERROR: Mandatory input GDML file not provided." << std::endl;
    return -1;
  } else if (argc > 3) {
    std::cerr << 
      "ERROR: Too many command line arguments.\n"
      "       If you want to parse a multi-file geometry, you need a central\n"
      "       file that tells Geant4 how to connect the various components."
      << std::endl;
    return -2;
  }

  std::string input_gdml{argv[1]};
  if (input_gdml == "-h" or input_gdml == "--help") {
    usage();
    return 0;
  }

  std::string batch_macro;
  if (argc > 2) {
    batch_macro = argv[2];
  }

  // command line arguments have been parsed

  G4GDMLParser parser;

  // Uncomment the following if wish to avoid names stripping
  //parser.SetStripFlag(false);

  parser.SetOverlapCheck(true);
  parser.Read(input_gdml.c_str());

  auto* runManager = G4RunManagerFactory::CreateRunManager();

  runManager->SetUserInitialization(new G01DetectorConstruction(
                                    parser.GetWorldVolume()));
  runManager->SetUserInitialization(new FTFP_BERT);
  runManager->SetUserInitialization(new G01ActionInitialization());

  runManager->Initialize();

  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  ///////////////////////////////////////////////////////////////////////
  //
  // Retrieve Auxiliary Information and Parse it into VisAttributes
  //
   
  /// create named vis attributes from global aux info
  VisAttributesStore vis_attrs(parser.GetAuxList());

  /// go through logical volumes and apply any VisAttributes found
  const G4LogicalVolumeStore* lvs = G4LogicalVolumeStore::GetInstance();
  for(auto lvciter = lvs->begin(); lvciter != lvs->end(); ++lvciter) {
    vis_attrs.apply(*lvciter, parser.GetVolumeAuxiliaryInformation(*lvciter));
  }

  //
  // Done with VisAttributes
  //
  ////////////////////////////////////////////////////////////////////////

  runManager->BeamOn(0);

  if (not batch_macro.empty()) { // user gives G4 macro, don't be interactive
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command+batch_macro);
  } else { // use default settings and then become interactive
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    /// default to OGL viewer (depends on Qt5)
    UImanager->ApplyCommand("/vis/open OGL 600x600-0+0");
    /// disable auto refresh and quiet vis messages while
    ///  firs drawing scene
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh false");
    UImanager->ApplyCommand("/vis/verbose errors");
    /// draw the geometry
    UImanager->ApplyCommand("/vis/drawVolume");
    /// specify a default zoom
    UImanager->ApplyCommand("/vis/viewer/zoom 0.4");
    /// add coordinate axes
    UImanager->ApplyCommand("/vis/scene/add/axes 0 0 0 1 m");
    
    /// re-establish auto refreshing and verbosity
    UImanager->ApplyCommand("/vis/viewer/set/autoRefresh true");
    UImanager->ApplyCommand("/vis/verbose warnings");

    /// open up GUI
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;

  return 0;
}
