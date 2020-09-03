#ifndef GENERATOR_FACTORY_H
#define GENERATOR_FACTORY_H

#include <G4String.hh>
#include "G4GenericMessenger.hh"


class G4VPrimaryGenerator;
class G4GenericMessenger;

/// Factory class for the construction of primary generators
/// chosen by the user via messenger.

class GeneratorFactory
{
public:
/// Constructor. Defines the messenger commands.
GeneratorFactory();
/// Destructor
~GeneratorFactory();
/// Returns an instance of the chosen generator
G4VPrimaryGenerator* CreateGenerator() const;

private:
G4GenericMessenger* msg_; ///< Pointer to the messenger
G4String name_; ///< Name of the chosen primary generator
};


#endif
