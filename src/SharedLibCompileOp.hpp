#ifndef Beagle_GP_SharedLibCompileOp_hpp
#define Beagle_GP_SharedLibCompileOp_hpp

#include <string>

#include "beagle/GP.hpp"
#include "beagle/Operator.hpp"
#include "SharedLibCompiler.hpp"

namespace Beagle
{

namespace GP
{

/*!
 *  \class SharedLibCompiler beagle/GP/SharedLibCompiler.hpp "beagle/GP/SharedLibCompiler.hpp"
 *  \brief Compile a shared library for evaluating the fitness of each individual in deme specified.
 *  \ingroup Op
 */
class SharedLibCompileOp : public Beagle::Operator
{

public:

	//! Operator allocator type.
	typedef Beagle::AllocatorT<SharedLibCompileOp, Beagle::Operator::Alloc> Alloc;
	//! Operator handle type.
	typedef Beagle::PointerT<SharedLibCompileOp, Beagle::Operator::Handle> Handle;
	//! Operator bag type.
	typedef Beagle::ContainerT<SharedLibCompileOp, Beagle::Operator::Bag> Bag;

	SharedLibCompileOp();
	virtual ~SharedLibCompileOp();

	/*!
	 *  \brief Compile a shared library for evaluating the fitness of each individual in deme specified.
	 *  \param ioDeme Reference to the deme for which to compile a shared library.
	 *  \param ioContext Evolutionary context of the operation.
	 *
	 */
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);
	
	/*!
	 *
	 */
	virtual void init(Beagle::System& ioSystem);
	
	/*!
	 *
	 */
	virtual void registerParams(Beagle::System& ioSystem);

	/*
	 * TODO: implement read/write to make mCompilerPath persistent.
	 */
//	virtual void read(PACC::XML::ConstIterator);
//	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
//	virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

}

#endif // Beagle_GP_SharedLibCompileOp_hpp
