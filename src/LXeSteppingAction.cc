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
// $Id: LXeSteppingAction.cc 74483 2013-10-09 13:37:06Z gcosmo $
//
/// \file LXeSteppingAction.cc
/// \brief Implementation of the LXeSteppingAction class

#include "LXeSteppingAction.hh"
#include "LXeEventAction.hh"
#include "LXeDetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeSteppingAction::LXeSteppingAction(LXeEventAction* eventAction)
: G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeSteppingAction::~LXeSteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeSteppingAction::UserSteppingAction(const G4Step* step)
{
  /*
  G4cout<<step->GetTrack()->GetDefinition()->GetParticleName()<<G4endl;
  G4cout<<step->GetNumberOfSecondariesInCurrentStep()<<G4endl;
  G4cout<<step->IsFirstStepInVolume()<<G4endl;
  */

  if(step->IsFirstStepInVolume() && step->GetTrack()->GetDefinition()->GetParticleName()=="opticalphoton"){
    G4Track* curTrack = step->GetTrack();
    const G4VProcess* proc = curTrack->GetCreatorProcess();
    //const char* pName = curTrack->GetDefinition()->GetParticleName();
    const G4double dpEnergy = curTrack->GetDynamicParticle()->GetKineticEnergy();
      if(proc->GetProcessName()=="Scintillation"){
          LXeEventAction::addScintEnergy(dpEnergy);
      }else if(proc->GetProcessName()=="Cerenkov"){
          LXeEventAction::addCerenEnergy(dpEnergy);
      }

  }
  if (!fScoringVolume) { 
    const LXeDetectorConstruction* detectorConstruction
      = static_cast<const LXeDetectorConstruction*>
        (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }

  // get volume of the current step
  G4LogicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()
      ->GetVolume()->GetLogicalVolume();
      
  // check if we are in scoring volume
  if (volume != fScoringVolume) return;

  // collect energy deposited in this step
  G4double edepStep = step->GetTotalEnergyDeposit();
  fEventAction->AddEdep(edepStep);  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

