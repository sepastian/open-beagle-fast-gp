#ifndef EqualTo_hpp
#define EqualTo_hpp

#include "beagle/GP.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


/*!
 *  \class EqualTo EqualTo.hpp "EqualTo.hpp"
 *  \brief EqualTo primitive that compare equality of two Double and return a Bool.
 *  \ingroup Spambase
 */
class EqualTo : public Beagle::GP::Primitive
{

public:

	//! EqualTo allocator type.
	typedef Beagle::AllocatorT<EqualTo,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< EqualTo handle type.
	typedef Beagle::PointerT<EqualTo,Beagle::GP::Primitive::Handle>
	Handle;
	//!< EqualTo bag type.
	typedef Beagle::ContainerT<EqualTo,Beagle::GP::Primitive::Bag>
	Bag;

	explicit EqualTo();
	virtual ~EqualTo()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // EqualTo_hpp
