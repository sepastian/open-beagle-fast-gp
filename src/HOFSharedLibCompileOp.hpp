#ifndef Beagle_HOFSharedLibCompileOp_hpp
#define Beagle_HOFSharedLibCompileOp_hpp

#include "beagle/GP.hpp"
#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/UInt.hpp"
#include "beagle/String.hpp"
#include "beagle/Bool.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/Context.hpp"

namespace Beagle
{

namespace GP
{

/*!
 *  \class HOFSharedLibCompileOp beagle/HOFSharedLibCompileOp.hpp "beagle/HOFSharedLibCompileOp.hpp"
 *  \brief Compile a shared lib containing all individuals in the Hall of Fame.
 *  \ingroup Op
 */
class HOFSharedLibCompileOp : public Operator
{

public:

	//! HOFSharedLibCompileOp allocator type
	typedef AllocatorT<HOFSharedLibCompileOp,Beagle::Operator::Alloc>
	Alloc;
	//! HOFSharedLibCompileOp handle type.
	typedef PointerT<HOFSharedLibCompileOp,Beagle::Operator::Handle>
	Handle;
	//! HOFSharedLibCompileOp bag type.
	typedef ContainerT<HOFSharedLibCompileOp,Beagle::Operator::Bag>
	Bag;

	explicit HOFSharedLibCompileOp();
	virtual ~HOFSharedLibCompileOp()
	{ }

	virtual void registerParams(Beagle::System& ioSystem);
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);

};

}

}

#endif // Beagle_HOFSharedLibCompileOp_hpp
