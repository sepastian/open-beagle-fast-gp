#ifndef EphemeralPercent_hpp
#define EphemeralPercent_hpp

#include "beagle/GP.hpp"
#include <string>


/*!
 *  \class EphemeralPercent EphemeralPercent.hpp "EphemeralPercent.hpp"
 *  \brief Ephemeral constant of Double randomly generated in [0,100].
 *  \ingroup Spambase
 */
class EphemeralPercent : public Beagle::GP::EphemeralT<Beagle::Double>
{

public:

	//! EphemeralPercent allocator type.
	typedef Beagle::AllocatorT<EphemeralPercent,Beagle::GP::EphemeralT<Beagle::Double>::Alloc>
	Alloc;
	//!< EphemeralPercent handle type.
	typedef Beagle::PointerT<EphemeralPercent,Beagle::GP::EphemeralT<Beagle::Double>::Handle>
	Handle;
	//!< EphemeralPercent bag type.
	typedef Beagle::ContainerT<EphemeralPercent,Beagle::GP::EphemeralT<Beagle::Double>::Bag>
	Bag;

	explicit EphemeralPercent(Beagle::Double::Handle inValue=NULL);
	virtual ~EphemeralPercent()
	{ }

	virtual Beagle::GP::Primitive::Handle generate(std::string inName, Beagle::GP::Context& ioContext);
	
	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;

};


#endif // EphemeralPercent_hpp
