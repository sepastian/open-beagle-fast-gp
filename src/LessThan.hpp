#ifndef LessThan_hpp
#define LessThan_hpp

#include "beagle/GP.hpp"
#include <string>

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


/*!
 *  \class LessThan LessThan.hpp "LessThan.hpp"
 *  \brief LessThan primitive that compare the order of two Double and return a Bool.
 *  \ingroup Spambase
 */
class LessThan : public Beagle::GP::Primitive
{

public:

	//! LessThan allocator type.
	typedef Beagle::AllocatorT<LessThan,Beagle::GP::Primitive::Alloc>
	Alloc;
	//!< LessThan handle type.
	typedef Beagle::PointerT<LessThan,Beagle::GP::Primitive::Handle>
	Handle;
	//!< LessThan bag type.
	typedef Beagle::ContainerT<LessThan,Beagle::GP::Primitive::Bag>
	Bag;

	explicit LessThan();
	virtual ~LessThan()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const;
	virtual const std::type_info* getReturnType(Beagle::GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual void                  execute(Beagle::GP::Datum& outDatum, Beagle::GP::Context& ioContext);

};


#endif // LessThan_hpp
