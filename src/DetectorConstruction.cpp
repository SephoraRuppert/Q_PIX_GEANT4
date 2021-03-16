// -----------------------------------------------------------------------------
//  G4Basic | DetectorConstruction.cpp
//
//  Definition of detector geometry and materials.
//   * Author: Justo Martin-Albo
//   * Creation date: 14 Aug 2019
// -----------------------------------------------------------------------------

#include "DetectorConstruction.h"
#include "TrackingSD.h"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4LogicalVolumeStore.hh"


DetectorConstruction::DetectorConstruction(): G4VUserDetectorConstruction()
{}

DetectorConstruction::~DetectorConstruction()
{}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // WORLD /////////////////////////////////////////////////

  G4double world_size = 15.*m;
  G4Material* world_mat = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  G4Box* world_solid_vol =
    new G4Box("world.solid", world_size/2., world_size/2., world_size/2.);

  G4LogicalVolume* world_logic_vol =
    new G4LogicalVolume(world_solid_vol, world_mat, "world.logical");
  world_logic_vol->SetVisAttributes(G4VisAttributes::Invisible);

  G4VPhysicalVolume* world_phys_vol =
    new G4PVPlacement(0, G4ThreeVector(0.,0.,0.),
                      world_logic_vol, "world.physical", 0, false, 0, true);

  // DETECTOR //////////////////////////////////////////////
  // resemble an APA size
  G4double detector_width   = 2.3*m;
  G4double detector_height  = 6.0*m;
  G4double detector_length  = 3.6*m;
  G4Material* xenon_mat = DefineXenon();

  G4Box* detector_solid_vol =
    new G4Box("detector.solid", detector_width/2., detector_height/2., detector_length/2.);

  G4LogicalVolume* detector_logic_vol =
    new G4LogicalVolume(detector_solid_vol, detector_mat, "detector.logical");

  G4ThreeVector offset(detector_width/2., detector_height/2., detector_length/2.);

  new G4PVPlacement(0, offset,
                    detector_logic_vol, "detector.physical", world_logic_vol, false, 0, true);

  //////////////////////////////////////////////////////////

  return world_phys_vol;
}

void DetectorConstruction::ConstructSDandField()
{
  // SENSITIVE DETECTOR ////////////////////////////////////

  TrackingSD* tracking_sd = new TrackingSD("/G4BASIC/TRACKING", "TrackingHitsCollection");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracking_sd);

  G4LogicalVolume* detector_logic_vol =
    G4LogicalVolumeStore::GetInstance()->GetVolume("detector.logical");

  SetSensitiveDetector(detector_logic_vol, tracking_sd);

  //////////////////////////////////////////////////////////
}


G4Material* DetectorConstruction::DefineXenon() const{
  // Defines the material and optical properties of gaseous xenon

  G4String material_name = "GXe";
  G4double density = 88.56 * kg/m3;
  //G4double pressure = 15.0 * bar;
  G4double temperature = 300. * kelvin;
  G4double sc_yield = 20000*1/MeV; // Estimated ~50 photons/eV

  G4Material* material = new G4Material(material_name, density, 1,
			    kStateGas, temperature, fpressure);
  G4Element* Xe = G4NistManager::Instance()->FindOrBuildElement("Xe");
  material->AddElement(Xe,1);

  // Optical Properties of Xenon
  // From Geant4 Liquid Xenon example
  // FIXME: what should these number be for this gasous xenon detector???
  const G4int GXe_NUMENTRIES = 3;
  G4double GXe_Energy[GXe_NUMENTRIES]    = { 7.0*eV , 7.07*eV, 7.14*eV };
  G4double GXe_SCINT[GXe_NUMENTRIES] = { 0.1, 1.0, 0.1 };
  G4double GXe_RIND[GXe_NUMENTRIES]  = {1.0, 1.0, 1.0};
  G4double GXe_ABSL[GXe_NUMENTRIES]  = {1.e8*m, 1.e8*m, 1.e8*m};

  G4MaterialPropertiesTable* GXe_mt = new G4MaterialPropertiesTable();

  GXe_mt->AddProperty("FASTCOMPONENT", GXe_Energy, GXe_SCINT, GXe_NUMENTRIES);
  GXe_mt->AddProperty("RINDEX",        GXe_Energy, GXe_RIND,  GXe_NUMENTRIES);
  GXe_mt->AddProperty("ABSLENGTH",     GXe_Energy, GXe_ABSL,  GXe_NUMENTRIES);
  GXe_mt->AddConstProperty("SCINTILLATIONYIELD",sc_yield);
  GXe_mt->AddConstProperty("RESOLUTIONSCALE",1.0);
  GXe_mt->AddConstProperty("FASTTIMECONSTANT",1.*ns);
  GXe_mt->AddConstProperty("YIELDRATIO",1.0);
  GXe_mt->AddProperty("ELSPECTRUM", GXe_Energy, GXe_SCINT, GXe_NUMENTRIES);
  GXe_mt->AddConstProperty("ELTIMECONSTANT", 50.*ns);

  material->SetMaterialPropertiesTable(GXe_mt);

  return material;
}
