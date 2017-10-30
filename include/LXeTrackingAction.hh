//
// Created by keavan on 24/10/17.
//

#ifndef LXe_LXeTrackingAction_h
#define LXe_LXeTrackingAction_h


#include "G4UserTrackingAction.hh"


class LXeTrackingAction : public G4UserTrackingAction {


public:

    virtual void PreUserTrackingAction(const G4Track *);

    virtual void PostUserTrackingAction(const G4Track *);

};

#endif //LXe_LXeTrackingAction_h