
#include "VisAttributesStore.hh"

void VisAttributesStore::createVisAttributes(G4String name,
                         const G4GDMLAuxListType* auxInfoList) {
  G4double rgba[4] = {1., 1., 1., 1.};
  G4bool visible = true;
  G4bool dauInvisible = false;
  G4bool forceWireframe = false;
  G4bool forceSolid = false;
  G4double lineWidth = 1.0;
  G4VisAttributes::LineStyle lineStyle = G4VisAttributes::unbroken;

  for (std::vector<G4GDMLAuxStructType>::const_iterator iaux =
           auxInfoList->begin();
       iaux != auxInfoList->end(); iaux++) {
    G4String auxType = iaux->type;
    G4String auxVal = iaux->value;
    G4String auxUnit = iaux->unit;

    if (auxType == "R") {
      rgba[0] = atof(auxVal.c_str());
    } else if (auxType == "G") {
      rgba[1] = atof(auxVal.c_str());
    } else if (auxType == "B") {
      rgba[2] = atof(auxVal.c_str());
    } else if (auxType == "A") {
      rgba[3] = atof(auxVal.c_str());
    } else if (auxType == "Style") {
      if (auxVal == "wireframe") {
        forceWireframe = true;
      } else if (auxVal == "solid") {
        forceSolid = true;
      }
    } else if (auxType == "DaughtersInvisible") {
      if (auxVal == "true") {
        dauInvisible = true;
      } else if (auxVal == "false") {
        dauInvisible = false;
      }
    } else if (auxType == "Visible") {
      if (auxVal == "true") {
        visible = true;
      } else if (auxVal == "false") {
        visible = false;
      }
    } else if (auxType == "LineStyle") {
      if (auxVal == "unbroken") {
        lineStyle = G4VisAttributes::unbroken;
      } else if (auxVal == "dashed") {
        lineStyle = G4VisAttributes::dashed;
      } else if (auxVal == "dotted") {
        lineStyle = G4VisAttributes::dotted;
      }
    } else if (auxType == "LineWidth") {
      lineWidth = atof(auxVal.c_str());
    }
  }

  G4VisAttributes* visAttributes = new G4VisAttributes();
  visAttributes->SetColor(rgba[0], rgba[1], rgba[2], rgba[3]);
  visAttributes->SetVisibility(visible);
  visAttributes->SetDaughtersInvisible(dauInvisible);
  visAttributes->SetForceWireframe(forceWireframe);
  visAttributes->SetForceSolid(forceSolid);
  visAttributes->SetLineWidth(lineWidth);
  visAttributes->SetLineStyle(lineStyle);
  defined_vis_attributes_[name] = visAttributes;
}

VisAttributesStore::VisAttributesStore(const G4GDMLAuxListType* global_aux_info) {
  for (auto iaux{global_aux_info->begin()}; iaux != global_aux_info->end(); ++iaux) {
    if (iaux->type == "VisAttributes") {
      createVisAttributes(iaux->value, iaux->auxList);
    }
  }
}

void VisAttributesStore::apply(G4LogicalVolume* lv, const G4GDMLAuxListType& aux_info) const {
  if (aux_info.size()>0) {
    for (auto iaux{aux_info.begin()}; iaux != aux_info.end(); iaux++) {
      if (iaux->type=="VisAttributes") {
         if (defined_vis_attributes_.find(iaux->value)!=defined_vis_attributes_.end()) {
           lv->SetVisAttributes(defined_vis_attributes_.at(iaux->value));
         } else {
           std::cerr << "WARNING: Referenced VisAttributes '" 
             << iaux->value << "' was not found."
             << std::endl;
         }
      }
    }
  }
}
