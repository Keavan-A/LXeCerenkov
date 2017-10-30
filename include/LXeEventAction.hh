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
// $Id: LXeEventAction.hh 93886 2015-11-03 08:28:26Z gcosmo $
//
/// \file LXeEventAction.hh
/// \brief Definition of the LXeEventAction class

#ifndef LXeEventAction_h
#define LXeEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "G4Step.hh"
class LXeRunAction;
//class G4Step;

/// Event action class
///




class LXeEventAction : public G4UserEventAction
{
  public:
    LXeEventAction(LXeRunAction* runAction);
    virtual ~LXeEventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void AddEdep(G4double edep) { fEdep += edep; }
    static std::vector <G4double> getCerenEnergies();
    static unsigned long getNumCerenEnergies();

    static void addCerenEnergy(G4double NRG);

    static std::vector <G4double> getScintEnergies();
    static unsigned long getNumScintEnergies();
    static void addScintEnergy(G4double NRG);

    virtual void resetCeren();
    virtual void resetScint();
private:
    LXeRunAction* fRunAction;
    //G4Step* fStep;
    G4double     fEdep;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
