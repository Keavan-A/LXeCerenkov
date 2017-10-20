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
//
/// \file B3PhysicsList.cc
/// \brief Implementation of the B3PhysicsList class
//
//  create a csv using verbose tracking?
//  us iostream or some other package to write the file.
//  look at LXe example from geant package for insight
//
//



#include "LXePhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"


#include "G4EmLowEPPhysics.hh"
#include "G4Cerenkov.hh"
#include "G4ProcessManager.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
 
#include "G4Gamma.hh"
 
#include "G4Electron.hh"
#include "G4Positron.hh"
 
#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePhysicsList::LXePhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(1);

  defaultCutValue=1.0*mm;
  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmLowEPPhysics());

  // Optical Physics
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  RegisterPhysics( opticalPhysics );
  
  //opticalPhysics->SetWLSTimeProfile("delta");
  opticalPhysics->SetScintillationYieldFactor(1.0);
  opticalPhysics->SetScintillationExcitationRatio(0.0);
  opticalPhysics->SetMaxNumPhotonsPerStep(100);
  opticalPhysics->SetMaxBetaChangePerStep(1.0);
  opticalPhysics->SetTrackSecondariesFirst(kCerenkov,true);
  opticalPhysics->SetTrackSecondariesFirst(kScintillation,true);
  
  /* 
  G4Cerenkov* theCerenkovProcess = new G4Cerenkov("Cerenkov");
  theCerenkovProcess->SetTrackSecondariesFirst(true);
  G4int MaxNumPhotons = 30;
  theCerenkovProcess->SetMaxNumPhotonsPerStep(MaxNumPhotons);
  G4ProcessManager* eMan = G4Electron::Electron()->GetProcessManager();
  //eMan->AddProcess(theCerenkovProcess);
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXePhysicsList::~LXePhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXePhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCutsWithDefault();
}  





/*void LXePhysicsList::ConstructParticle()
{
// gamma
G4Gamma::GammaDefinition();
  
// electron
G4Electron::ElectronDefinition();
G4Positron::PositronDefinition();
G4NeutrinoE::NeutrinoEDefinition();
G4AntiNeutrinoE::AntiNeutrinoEDefinition();

}

void LXePhysicsList::ConstructProcess()
{
G4PhotoElectricEffect* fPhotoEffect =
  new G4PhotoElectricEffect();
G4ComptonScattering* fComptonEffect =
  new G4ComptonScattering();
G4GammaConversion* fPairProduction =
  new G4GammaConversion();
  
// Electron physics
G4eMultipleScattering* fElectronMultipleScattering =
new G4eMultipleScattering();
G4eIonisation* fElectronIonisation =
new G4eIonisation();
G4eBremsstrahlung* fElectronBremsStrahlung =
new G4eBremsstrahlung();

//Positron physics
G4eMultipleScattering* fPositronMultipleScattering =
new G4eMultipleScattering();
G4eIonisation* fPositronIonisation =
new G4eIonisation();
G4eBremsstrahlung* fPositronBremsStrahlung =
new G4eBremsstrahlung();
G4eplusAnnihilation* fAnnihilation =
new G4eplusAnnihilation();

G4ProcessManager* pManager = 0;
  
// Gamma Physics
pManager = G4Gamma::Gamma()->GetProcessManager();
pManager->AddDiscreteProcess(fPhotoEffect);
pManager->AddDiscreteProcess(fComptonEffect);
pManager->AddDiscreteProcess(fPairProduction);
 
// Electron Physics
pManager = G4Electron::Electron()->GetProcessManager();

pManager->AddProcess(fElectronMultipleScattering, -1, 1, 1);
pManager->AddProcess(fElectronIonisation,         -1, 2, 2);
pManager->AddProcess(fElectronBremsStrahlung,     -1, 3, 3);
 
//Positron Physics
pManager = G4Positron::Positron()->GetProcessManager();

pManager->AddProcess(fPositronMultipleScattering, -1, 1, 1);
pManager->AddProcess(fPositronIonisation,         -1, 2, 2);
pManager->AddProcess(fPositronBremsStrahlung,     -1, 3, 3);  
pManager->AddProcess(fAnnihilation,                0,-1, 4);  

}
*/

