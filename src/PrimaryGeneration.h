#ifndef PRIMARY_GENERATION_H
#define PRIMARY_GENERATION_H

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4String.hh>
#include "G4GenericMessenger.hh"


class G4VPrimaryGenerator;
class G4GenericMessenger;


class PrimaryGeneration: public G4VUserPrimaryGeneratorAction
{
public:
  /// Constructor
  PrimaryGeneration();
  /// Destructor
  ~PrimaryGeneration();

  ///
  void GeneratePrimaries(G4Event*);

  /// Sets the primary generator
  void SetGenerator(G4VPrimaryGenerator*);
  /// Returns a pointer to the primary generator
  const G4VPrimaryGenerator* GetGenerator() const;

private:
  G4VPrimaryGenerator* generator_; ///< Pointer to the primary generator
  G4GenericMessenger* msg_; ///< Pointer to the messenger
  G4String name_; ///< Name of the chosen primary generator
};

// INLINE DEFINITIONS //////////////////////////////////////////////

inline void PrimaryGeneration::SetGenerator(G4VPrimaryGenerator* pg)
{ generator_ = pg; }

inline const G4VPrimaryGenerator* PrimaryGeneration::GetGenerator() const
{ return generator_; }


#endif



// // -----------------------------------------------------------------------------
// //  G4Basic | PrimaryGeneration.h
// //
// //  Class for the definition of the primary generation action.
// //   * Author: Justo Martin-Albo
// //   * Creation date: 14 Aug 2019
// // -----------------------------------------------------------------------------

// #ifndef PRIMARY_GENERATION_H
// #define PRIMARY_GENERATION_H

// // GEANT4 includes
// #include "G4VUserPrimaryGeneratorAction.hh"
// #include "G4ParticleTable.hh"
// #include "G4String.hh"
// #include "G4Box.hh"

// #include "G4ParticleGun.hh"


// class G4ParticleDefinition;
// class G4GenericMessenger;
// class G4ParticleGun;

// class PrimaryGeneration : public G4VUserPrimaryGeneratorAction
// {

//   public:

//     PrimaryGeneration();
//     virtual ~PrimaryGeneration();
//     virtual void GeneratePrimaries(G4Event*);

//   protected:

//     // GEANT4 dictionary of particles
//     G4ParticleTable* particle_table_;

//   private:

//     G4GenericMessenger* msg_; // Messenger for configuration parameters
//     G4String Particle_Type_;
//     G4String Event_Type_;
//     //double Particle_Energy_;

//     double detector_length_x_;
//     double detector_length_y_;
//     double detector_length_z_;

//     G4Box* detector_solid_vol_;

//     G4ParticleGun* particleGun;

// };

// #endif
