//
// Created by keavan on 24/10/17.
//

#include "LXeEventAction.hh"
#include "G4TrackingManager.hh"
#include "LXeTrackingAction.hh"



void LXeTrackingAction::PreUserTrackingAction(const G4Track *) {
    /*G4Track* curTrack = fpTrackingManager->GetTrack();
    G4cout << curTrack->GetDynamicParticle()->GetDefinition()->GetParticleName()<<G4endl;
    const G4VProcess* proc = curTrack->GetCreatorProcess();
    const char* c = curTrack->GetDefinition()->GetParticleName();
    if(proc)G4cout << curTrack->GetCreatorProcess()->GetProcessName()<<G4endl;
    G4cout << curTrack->GetDynamicParticle()->GetKineticEnergy()<<G4endl;
    */
    G4Track* curTrack = fpTrackingManager->GetTrack();
    const G4VProcess* proc = curTrack->GetCreatorProcess();
    //const char* pName = curTrack->GetDefinition()->GetParticleName();
    const G4double dpEnergy = curTrack->GetDynamicParticle()->GetKineticEnergy();
    if(proc){//} && pName == "opticalphoton"){
        if(proc->GetProcessName()=="Scintillation"){
            LXeEventAction::addScintEnergy(dpEnergy);
        }else if(proc->GetProcessName()=="Cerenkov"){
            LXeEventAction::addCerenEnergy(dpEnergy);
        }
    }
}

void LXeTrackingAction::PostUserTrackingAction(const G4Track *) {

}

