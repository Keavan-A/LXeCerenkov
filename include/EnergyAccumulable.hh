//
// Created by Keavan on 01/11/17.
//

#ifndef LXE_ENERGYACCUMULABLE_H
#define LXE_ENERGYACCUMULABLE_H

#include "G4VAccumulable.hh"
#include "globals.hh"
#include <map>
class EnergyAccumulable : public G4VAccumulable
{
public:
    EnergyAccumulable(const G4String& name)
            : G4VAccumulable(name), Energies() {}
    virtual ~EnergyAccumulable() {}

    virtual std::vector<G4double> getEnergies();
    virtual unsigned long getLength();
    virtual void AddEnergy(G4double NRG);

    virtual void Merge(const G4VAccumulable& other);
    virtual void Reset();

private:
    std::vector<G4double> Energies=std::vector<G4double >(500000);
};

#endif //LXE_ENERGYACCUMULABLE_H
