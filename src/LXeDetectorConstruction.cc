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
// $Id: LXeDetectorConstruction.cc 94307 2015-11-11 13:42:46Z gcosmo $
//
/// \file LXeDetectorConstruction.cc
/// \brief Implementation of the LXeDetectorConstruction class

#include "LXeDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tubs.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorConstruction::LXeDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeDetectorConstruction::~LXeDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* LXeDetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  // Envelope parameters
  //
  G4double env_sizeXY = 29*mm, env_sizeZ = 48*mm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_TEFLON");
  G4Material* LXe = nist->FindOrBuildMaterial("G4_lXe");

  const G4int NUMENTRIES = 20;
  //const G4int NUMENTRIES = 9;
  
  //G4double LXe_PP[NUMENTRIES] = {6.6*eV,6.7*eV,6.8*eV,6.9*eV,7.0*eV, 7.1*eV,7.2*eV,7.3*eV,7.4*eV};
  G4double LXe_PP[NUMENTRIES] = { 2.0*eV, 2.5*eV, 3.0*eV, 3.5*eV,
                                  4.0*eV, 4.5*eV, 5.0*eV, 5.5*eV, 6.0*eV,
                                  6.6*eV,6.7*eV,6.8*eV,6.9*eV,7.0*eV, 7.1*eV,7.2*eV,7.3*eV,7.4*eV,
                                  7.7*eV,8.0*eV};
  G4double LXe_SCINT[NUMENTRIES] = {0,0,0,0,0,0,0,0,0,
                  0.000134, 0.004432, 0.053991, 0.241971, 0.398942, 0.37134, 0.32432, 0.283991,0.241971,
                                    0.005,0.0000};// This distribution needs to be reevaluated.
  //0.000134, 0.004432, 0.053991, 0.241971, 0.398942, 0.000134, 0.004432, 0.053991,0.241971,
  G4double LXe_RIND[NUMENTRIES];for(int i=0;i<NUMENTRIES;i++) LXe_RIND[i] = 1.57;
  
  G4double LXe_ABSL[NUMENTRIES];for(int i=0;i<NUMENTRIES;i++) LXe_ABSL[i] = 35.*cm;
  
  G4MaterialPropertiesTable* LXe_MPT = new G4MaterialPropertiesTable();
  
  LXe_MPT -> AddProperty("FASTCOMPONENT",LXe_PP, LXe_SCINT, NUMENTRIES);
  LXe_MPT -> AddProperty("SLOWCOMPONENT",LXe_PP, LXe_SCINT, NUMENTRIES);
  LXe_MPT -> AddProperty("RINDEX", LXe_PP, LXe_RIND, NUMENTRIES);
  LXe_MPT -> AddProperty("ABSLENGTH",LXe_PP, LXe_ABSL, NUMENTRIES);
  LXe_MPT -> AddConstProperty("RESOLUTIONSCALE", 1.0);
 
  LXe_MPT -> AddConstProperty ("SCINTILLATIONYIELD", 68000/MeV);//  was 100 before
  LXe_MPT -> AddConstProperty("FASTTIMECONSTANT",20.*ns);
  LXe_MPT -> AddConstProperty("SLOWTIMECONSTANT",45.*ns);
  LXe_MPT -> AddConstProperty("YIELDRATIO",1.0);
  
  LXe -> SetMaterialPropertiesTable(LXe_MPT);
  //LXe->GetIonisation()->SetBirksConstant(0.126*mm/MeV);

  G4double xenon_sizeXY = 19*mm;
  G4double xenon_sizeZ  = 38*mm;
  G4Material* xenon_mat = LXe;
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 39*mm;
  G4double world_sizeZ  = 58*mm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
  
  G4Box* solidWorld =    
    new G4Box("World",              //its name
              0.5*world_sizeXY,
              0.5*world_sizeXY,
              0.5*world_sizeZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking
                     
  //     
  // Envelope
  //  
  G4Box* solidEnv =    
    new G4Box("Envelope",     //its name
              0.5*env_sizeXY,
              0.5*env_sizeXY,
              0.5*env_sizeZ); //its size
      
  G4LogicalVolume* logicEnv =                         
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking

  G4Box* solidXenon =
          new G4Box("Xenon",          //its name
                    0.5*xenon_sizeXY,
                    0.5*xenon_sizeXY,
                    0.5*xenon_sizeZ); //its size
  G4LogicalVolume* logicXenon =
          new G4LogicalVolume(solidXenon, //its solid
                   xenon_mat,             //its material
                   "Xenon");              //its name

  new G4PVPlacement(0,              //no rotation
           G4ThreeVector(),         //at (0,0,0)
           logicXenon,              //its logical volume
           "Xenon",                 //its name
           logicEnv,                //its mother  volume
           false,                   //no boolean operation
           0,                       //copy number
           checkOverlaps);          //overlaps checking
  //     
  // SiPM Definitions
  //  
  G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");
  G4double Package_sizeXY = 15*mm;
  G4double Package_sizeZ = 3*mm;
  G4ThreeVector Package_pos = G4ThreeVector(0, 0, Package_sizeZ/2);
  G4Box* SolidPackage = new G4Box("Package", Package_sizeXY/2, Package_sizeXY/2, Package_sizeZ/2);
  G4LogicalVolume* Packagelogic = new G4LogicalVolume(SolidPackage, silicon, "Package");

  G4double MPPC_sizeXY = 5.9*mm;
  G4double MPPC_sizeZ = 1*mm;
  G4ThreeVector MPPC1_pos = G4ThreeVector(   (MPPC_sizeXY/2+0.5*mm),    (MPPC_sizeXY/2+0.5*mm), Package_sizeZ/2 - MPPC_sizeZ/2);
  //G4ThreeVector MPPC2_pos = G4ThreeVector(0,0,0);
  G4ThreeVector MPPC2_pos = G4ThreeVector(   (MPPC_sizeXY/2+0.5*mm), -1*(MPPC_sizeXY/2+0.5*mm), Package_sizeZ/2 - MPPC_sizeZ/2);
  G4ThreeVector MPPC3_pos = G4ThreeVector(-1*(MPPC_sizeXY/2+0.5*mm), -1*(MPPC_sizeXY/2+0.5*mm), Package_sizeZ/2 - MPPC_sizeZ/2);
  G4ThreeVector MPPC4_pos = G4ThreeVector(-1*(MPPC_sizeXY/2+0.5*mm),    (MPPC_sizeXY/2+0.5*mm), Package_sizeZ/2 - MPPC_sizeZ/2);
  G4Box* SolidMPPC = new G4Box("MPPC", MPPC_sizeXY/2, MPPC_sizeXY/2, MPPC_sizeZ/2);
  G4LogicalVolume* MPPClogic = new G4LogicalVolume(SolidMPPC, silicon, "MPPC");

  //
  // SiPM Pacements
  //

  G4ThreeVector SiPM_Pos[10];
  SiPM_Pos[0] = G4ThreeVector(                                  0,                                   0,    xenon_sizeZ/2 + Package_sizeZ/2);
  SiPM_Pos[1] = G4ThreeVector(                                  0,                                   0, -1*xenon_sizeZ/2 - Package_sizeZ/2);
  SiPM_Pos[2] = G4ThreeVector(   xenon_sizeXY/2 + Package_sizeZ/2,                                   0,    xenon_sizeZ/4);
  SiPM_Pos[3] = G4ThreeVector(   xenon_sizeXY/2 + Package_sizeZ/2,                                   0, -1*xenon_sizeZ/4);
  SiPM_Pos[4] = G4ThreeVector(-1*xenon_sizeXY/2 - Package_sizeZ/2,                                   0,    xenon_sizeZ/4);
  SiPM_Pos[5] = G4ThreeVector(-1*xenon_sizeXY/2 - Package_sizeZ/2,                                   0, -1*xenon_sizeZ/4);
  SiPM_Pos[6] = G4ThreeVector(                                  0,    xenon_sizeXY/2 + Package_sizeZ/2,    xenon_sizeZ/4);
  SiPM_Pos[7] = G4ThreeVector(                                  0,    xenon_sizeXY/2 + Package_sizeZ/2, -1*xenon_sizeZ/4);
  SiPM_Pos[8] = G4ThreeVector(                                  0, -1*xenon_sizeXY/2 - Package_sizeZ/2,    xenon_sizeZ/4);
  SiPM_Pos[9] = G4ThreeVector(                                  0, -1*xenon_sizeXY/2 - Package_sizeZ/2, -1*xenon_sizeZ/4);

  G4RotationMatrix* SiPM_Rot[10];
  SiPM_Rot[0] = new G4RotationMatrix();
  SiPM_Rot[0]->rotateX(180*degree);
  SiPM_Rot[1] = new G4RotationMatrix();
  SiPM_Rot[1]->rotateX(0*degree);
  SiPM_Rot[2] = new G4RotationMatrix();
  SiPM_Rot[2]->rotateY(90*degree);
  SiPM_Rot[3] = new G4RotationMatrix();
  SiPM_Rot[3]->rotateY(90*degree);
  SiPM_Rot[4] = new G4RotationMatrix();
  SiPM_Rot[4]->rotateY(-90*degree);
  SiPM_Rot[5] = new G4RotationMatrix();
  SiPM_Rot[5]->rotateY(-90*degree);
  SiPM_Rot[6] = new G4RotationMatrix();
  SiPM_Rot[6]->rotateX(-90*degree);
  SiPM_Rot[7] = new G4RotationMatrix();
  SiPM_Rot[7]->rotateX(-90*degree);
  SiPM_Rot[8] = new G4RotationMatrix();
  SiPM_Rot[8]->rotateX(90*degree);
  SiPM_Rot[9] = new G4RotationMatrix();
  SiPM_Rot[9]->rotateX(90*degree);

  new G4PVPlacement(0, MPPC1_pos, MPPClogic, "MPPC", Packagelogic, 0, checkOverlaps);
  new G4PVPlacement(0, MPPC2_pos, MPPClogic, "MPPC", Packagelogic, 0, checkOverlaps);
  new G4PVPlacement(0, MPPC3_pos, MPPClogic, "MPPC", Packagelogic, 0, checkOverlaps);
  new G4PVPlacement(0, MPPC4_pos, MPPClogic, "MPPC", Packagelogic, 0, checkOverlaps);

  for (int i = 0; i < 10; i++){
  new G4PVPlacement(SiPM_Rot[i], SiPM_Pos[i]/* + Package_pos.transform(*SiPM_Rot[i])*/, Packagelogic, "Package", logicEnv, 0, checkOverlaps);
  }

  //
  fScoringVolume = MPPClogic;

  //
  // Needle
  //

  G4double innerRadius = 0.5*mm;
  G4double outerRadius = 1*mm;
  G4double hz = xenon_sizeXY/2;
  G4double startAngle = 0.*deg;
  G4double spanningAngle = 360.*deg;

  G4Tubs* SolidNeedle = new G4Tubs("Needle", innerRadius/2, outerRadius/2, hz/2, startAngle, spanningAngle);
  G4RotationMatrix* Needle_Rot = new G4RotationMatrix();
  Needle_Rot->rotateX(90*degree);
  G4Material* steel = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
  G4LogicalVolume* Needlelogic = new G4LogicalVolume(SolidNeedle, steel, "Needle");
  G4ThreeVector Needle_pos = G4ThreeVector(0, hz/2, 0);
  new G4PVPlacement(Needle_Rot, Needle_pos, Needlelogic, "Needle", logicXenon, 0, checkOverlaps);


  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
