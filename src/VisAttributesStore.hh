#ifndef VISATTRIBUTESSTORE_HH
#define VISATTRIBUTESSTORE_HH

#include "G4LogicalVolume.hh"
#include "G4VisAttributes.hh"
#include "G4GDMLParser.hh"

/**
 * Create vis attributes so that they can be used
 * in multiple volumes within the GDML
 */
class VisAttributesStore {
  /// the store of attributes already defined
  std::map<G4String,G4VisAttributes*> defined_vis_attributes_;

  /// the function that creates a single VisAttribute
  void createVisAttributes(G4String name, const G4GDMLAuxListType* aux_info);
 public:
  /// construct the set of defined vis attributes from the global aux info
  VisAttributesStore(const G4GDMLAuxListType* globa_aux_info);

  /// apply any vis attributes that are found to this logical volume
  void apply(G4LogicalVolume* lv, const G4GDMLAuxListType& aux_info) const;
};

#endif
