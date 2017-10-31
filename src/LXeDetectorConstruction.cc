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
  G4double env_sizeXY = 3*cm, env_sizeZ = 4.5*cm;
  G4Material* LXe = nist->FindOrBuildMaterial("G4_lXe");

  const G4int NUMENTRIES = 20;
  //const G4int NUMENTRIES = 9;
  
  //G4double LXe_PP[NUMENTRIES] = {6.6*eV,6.7*eV,6.8*eV,6.9*eV,7.0*eV, 7.1*eV,7.2*eV,7.3*eV,7.4*eV};
  G4double LXe_PP[NUMENTRIES] = { 2.0*eV, 2.5*eV, 3.0*eV, 3.5*eV,
                                  4.0*eV, 4.5*eV, 5.0*eV, 5.5*eV, 6.0*eV,
                                  6.6*eV,6.7*eV,6.8*eV,6.9*eV,7.0*eV, 7.1*eV,7.2*eV,7.3*eV,7.4*eV,
                                  7.7*eV,8.0*eV};
  G4double LXe_SCINT[NUMENTRIES] = {0,0,0,0,0,0,0,0,0,
                  0.000134, 0.004432, 0.053991, 0.241971, 0.398942, 0.38134, 0.32432, 0.283991,0.241971,
                                    0.045,0.0005};
  //0.000134, 0.004432, 0.053991, 0.241971, 0.398942, 0.000134, 0.004432, 0.053991,0.241971,
  G4double LXe_RIND[NUMENTRIES] = {1.57,1.57,1.57,1.57,1.57,1.57,1.57,1.57,1.57,
          1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57, 1.57
          ,1.57, 1.57};
  
  G4double LXe_ABSL[NUMENTRIES] = { 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm,
          35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm, 35.*cm
          , 35.*cm, 35.*cm};
  
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

  G4Material* env_mat = LXe;
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 1.2*env_sizeXY;
  G4double world_sizeZ  = 1.2*env_sizeZ;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Fe");
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size
      
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
    new G4Box("Envelope",                    //its name
        0.5*env_sizeXY, 0.5*env_sizeXY, 0.5*env_sizeZ); //its size
      
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
 
  
  //     
  // Shape 1
  //  
  G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");
  G4ThreeVector pos1 = G4ThreeVector(0, 0, (15+1.5+2)*mm);
  G4ThreeVector pos2 = G4ThreeVector(0, 0, -(15+1.5+2)*mm);
  G4ThreeVector pos3 = G4ThreeVector(10*mm, 0, 9*mm);
  G4ThreeVector pos4 = G4ThreeVector(-10*mm, 0, 9*mm);
  G4ThreeVector pos5 = G4ThreeVector(10*mm, 0, -9*mm);  
  G4ThreeVector pos6 = G4ThreeVector(-10*mm, 0, -9*mm);
      
  G4Box* vuv4shape = new G4Box("vuv4", 7.5*mm, 7.5*mm, 1*mm);
  G4LogicalVolume* vuv4logic = new G4LogicalVolume(vuv4shape, silicon, "vuv4");
/*
  G4RotationMatrix* rot1 = new G4RotationMatrix(); 
  rot1->rotateY(90*degree);              
  new G4PVPlacement(0, pos1, vuv4logic, "vuv4", logicEnv, 0, checkOverlaps);
  new G4PVPlacement(0, pos2, vuv4logic, "vuv4", logicEnv, 0, checkOverlaps);
  new G4PVPlacement(rot1, pos3, vuv4logic, "vuv4", logicEnv, 0, checkOverlaps);
  new G4PVPlacement(rot1, pos4, vuv4logic, "vuv4", logicEnv, 0, checkOverlaps);
  new G4PVPlacement(rot1, pos5, vuv4logic, "vuv4", logicEnv, 0, checkOverlaps);
  new G4PVPlacement(rot1, pos6, vuv4logic, "vuv4", logicEnv, 0, checkOverlaps);
*/

  //
  fScoringVolume = vuv4logic;

  //
  //always return the physical World
  //
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
