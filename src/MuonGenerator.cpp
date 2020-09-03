#include "MuonGenerator.h"
// #include "DetectorConstruction.h"
// #include "BaseGeometry.h"
#include <G4GenericMessenger.hh>
#include <G4ParticleDefinition.hh>
#include <G4RunManager.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryVertex.hh>
#include <G4Event.hh>
#include <G4RandomDirection.hh>
#include <Randomize.hh>
#include <G4OpticalPhoton.hh>
// #include "MuonsPointSampler.h"
// #include "AddUserInfoToPV.h"
#include "G4LogicalVolumeStore.hh"
#include "TF1.h"

#include <TMath.h>
#include "CLHEP/Units/SystemOfUnits.h"

#include "UserInfo.h"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <cstdlib>
#include "G4GenericMessenger.hh"


using namespace CLHEP;


MuonGenerator::MuonGenerator(UserInfo * User_Info):
    G4VPrimaryGenerator()
{
    // energy_min_ = User_Info->User_energy_min_ ;
    // energy_max_ = User_Info->User_energy_max_ ;

    // momentum_X_ = User_Info->User_momentum_X_ ;
    // momentum_Y_ = User_Info->User_momentum_Y_ ;
    // momentum_Z_ = User_Info->User_momentum_Z_ ;

    // position_X_ = User_Info->User_position_X_;
    // position_Y_ = User_Info->User_position_Y_ ;
    // position_Z_ = User_Info->User_position_Z_ ;

    // if ( abs( energy_min_- 200.*MeV) >0.1)
    // G4Exception("[FUCK]", "FUCK()",
    //             FatalException, "FUCK FUCK FUCK!");

    // energy_min_ =  1.;
    // energy_max_ =  1.;

    // momentum_X_ =  10.;
    // momentum_Y_ =  10.;
    // momentum_Z_ =  10.;

    // position_X_ = 10.;
    // position_Y_ =  10.;
    // position_Z_ =  10.;

    // msg_ = new G4GenericMessenger(this, "/Generator/MuonGenerator/",
	// 			"Control commands of muongenerator.");
    // msg_->DeclareProperty("min_energy", energy_min_, "Set minimum kinetic energy of the particle.");
    // msg_->DeclareProperty("max_energy", energy_max_, "Set maximum kinetic energy of the particle");

    // msg_->DeclareProperty("momentum_X", momentum_X_,"x coord of momentum");
    // msg_->DeclareProperty("momentum_Y", momentum_Y_,"y coord of momentum");
    // msg_->DeclareProperty("momentum_Z", momentum_Z_,"z coord of momentum");

    // msg_->DeclareProperty("position_X", position_X_,"x coord of position");
    // msg_->DeclareProperty("position_Y", position_Y_,"y coord of position");
    // msg_->DeclareProperty("position_Z", position_Z_,"z coord of position");


}



MuonGenerator::~MuonGenerator()
{

}

void MuonGenerator::GeneratePrimaryVertex(G4Event* event)
{

  particle_definition_ = G4ParticleTable::GetParticleTable()->FindParticle("e-");
//   particle_definition_ = G4ParticleTable::GetParticleTable()->FindParticle(MuonCharge());
  if (!particle_definition_)
    G4Exception("[MuonGenerator]", "SetParticleDefinition()",
                FatalException, " can not create a muon ");


  G4double time = 0.;
  G4ThreeVector position = G4ThreeVector(position_X_, position_Y_, position_Z_);
  // Create a new vertex
  G4PrimaryVertex* vertex = new G4PrimaryVertex(position, time);
  // Generate uniform random energy in [E_min, E_max]
  G4double kinetic_energy = RandomEnergy();

  // Particle propierties
  G4double mass   = particle_definition_->GetPDGMass();
  G4double energy = kinetic_energy + mass;
  G4double pmod = std::sqrt(energy*energy - mass*mass);

  // Generate momentum direction in spherical coordinates
  G4double theta = GetTheta();
  G4double phi   = GetPhi();

  G4double x, y, z;

  // NEXT axis convention (z<->y) and generate with -y! towards the detector
  x = sin(theta) * cos(phi);
  y = -cos(theta);
  z = sin(theta) * sin(phi);

  G4ThreeVector _p_dir(x,y,z);

  G4double px = pmod * _p_dir.x();
  G4double py = pmod * _p_dir.y();
  G4double pz = pmod * _p_dir.z();


  // If user provides a momentum direction, this one is used
  if (momentum_X_ != 0. || momentum_Y_ != 0. || momentum_Z_ != 0.) {
    // Normalize if needed
    G4double mom_mod = std::sqrt(momentum_X_ * momentum_X_ +
				 momentum_Y_ * momentum_Y_ +
				 momentum_Z_ * momentum_Z_);
    px = pmod * momentum_X_/mom_mod;
    py = pmod * momentum_Y_/mom_mod;
    pz = pmod * momentum_Z_/mom_mod;
  }

  // Create the new primary particle and set it some properties
  G4PrimaryParticle* particle =
    new G4PrimaryParticle(particle_definition_, px, py, pz);

  // Add info to PrimaryVertex to be accessed from EventAction type class to make histos of variables generated here.
//   AddUserInfoToPV *info = new AddUserInfoToPV(theta, phi);

//   vertex->SetUserInformation(info);

  // Add particle to the vertex and this to the event
  vertex->SetPrimary(particle);
  event->AddPrimaryVertex(vertex);

}

G4double MuonGenerator::RandomEnergy() const
{
  if (energy_max_ == energy_min_)
    return energy_min_;
  else
    return G4UniformRand()*(energy_max_ - energy_min_) + energy_min_;
}

G4String MuonGenerator::MuonCharge() const
{
  G4double rndCh = 2.3 *G4UniformRand(); //From PDG cosmic muons  mu+/mu- = 1.3
  if (rndCh <1.3)
    return "mu+";
  else
    return "mu-";
}


G4double MuonGenerator::GetTheta() const
{
  TF1 *f1 = new TF1("f1","pow(cos(x),2)",0,pi/2);
  G4double theta = f1->GetRandom();
  return theta;
}


G4double MuonGenerator::GetPhi() const
{
  return twopi*G4UniformRand();
}
