//
// Created by Keavan on 01/11/17.
//

#include "EnergyAccumulable.hh"

void EnergyAccumulable::AddEnergy(G4double NRG){
  Energies.push_back(NRG);
}

std::vector<G4double > EnergyAccumulable::getEnergies(){
  return Energies;
}
void EnergyAccumulable::Merge(const G4VAccumulable& other)
{
  const EnergyAccumulable& otherEnergyAccumulable = static_cast<const EnergyAccumulable&>(other);
  std::vector<G4double>::const_iterator it;
  for(it = otherEnergyAccumulable.Energies.begin();
      it != otherEnergyAccumulable.Energies.end();
      ++it){
   Energies.push_back(*it);
  }
}

void EnergyAccumulable::Reset()
{
  Energies.clear();
}