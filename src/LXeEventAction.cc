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
// $Id: LXeEventAction.cc 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file LXeEventAction.cc
/// \brief Implementation of the LXeEventAction class

//#include <mutex>

#include "LXeTrackingAction.hh"
#include "LXeEventAction.hh"
#include "LXeRunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

static std::vector<G4double> scintEnergies;
static std::vector<G4double> cerenEnergies;


static std::ofstream cerenOut;
static std::ofstream scintOut;

//static std::mutex cerenLock;
//static std::mutex scintLock;

LXeEventAction::LXeEventAction(LXeRunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fEdep(0.)
{} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

LXeEventAction::~LXeEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeEventAction::BeginOfEventAction(const G4Event*)
{
  fEdep = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void LXeEventAction::EndOfEventAction(const G4Event*)
{

  unsigned long nC = cerenEnergies.size();
  unsigned long nS = scintEnergies.size();
  // accumulate statistics in run action
  //G4cout << "Cerenkov photons created = " << nC << G4endl;
  //G4cout << "Scintillation photons created = " << nS << G4endl;

  //cerenLock.lock();
    cerenOut.open("cerenkovEnergies.csv", std::ios::app);
    for(unsigned long i=0;i<nC;i++){
      cerenOut << cerenEnergies[i] << ",";
    }
    resetCeren();
    cerenOut.close();
//  cerenLock.unlock();

  //scintLock.lock();
    scintOut.open("scintillationEnergies.csv", std::ios::app);
    for(unsigned long i=0;i<nS;i++){
      scintOut << scintEnergies[i] << ",";
    }
    resetScint();
    scintOut.close();
  //scintLock.unlock();
  fRunAction->AddEdep(fEdep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
std::vector <G4double> LXeEventAction::getCerenEnergies(){
  return cerenEnergies;
}
unsigned long LXeEventAction::getNumCerenEnergies(){
  return cerenEnergies.size();
}
void LXeEventAction::addCerenEnergy(G4double NRG){
  cerenEnergies.push_back(NRG);
}

std::vector <G4double> LXeEventAction::getScintEnergies(){
  return scintEnergies;
}
unsigned long LXeEventAction::getNumScintEnergies(){
  unsigned long s = scintEnergies.size();
  //std::vector<G4double> scintEnergiesTest = scintEnergies;
  return s;
}
void LXeEventAction::addScintEnergy(G4double NRG){
  scintEnergies.push_back(NRG);
  //unsigned long s = scintEnergies.size();
  //std::vector<G4double> scintEnergiesTest = scintEnergies;
}

void LXeEventAction::resetScint() {
  scintEnergies.clear();
}

void LXeEventAction::resetCeren() {
  cerenEnergies.clear();
}