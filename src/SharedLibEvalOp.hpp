#ifndef SharedLibEvalOp_hpp
#define SharedLibEvalOp_hpp

#include "beagle/GP.hpp"
#include "FitnessMCC.hpp"
#include "StatsCalcFitnessMCCOp.hpp"

#include <string>
#include <vector>

#include "PACC/Util/Timer.hpp"

#define Spambase_TestSize 2000

namespace Beagle
{

namespace GP
{

/*!
 *  \class SharedLibEvalOp SharedLibEvalOp.hpp "SharedLibEvalOp.hpp"
 *  \brief Shared library evaluation operator.
 *  \ingroup Spambase
 */
class SharedLibEvalOp : public Beagle::GP::EvaluationOp
{

public:

	//! SharedLibEvalOp allocator type.
	typedef Beagle::AllocatorT<SharedLibEvalOp,Beagle::GP::EvaluationOp::Alloc> Alloc;
	//!< SharedLibEvalOp handle type.
	typedef Beagle::PointerT<SharedLibEvalOp,Beagle::GP::EvaluationOp::Handle> Handle;
	//!< SharedLibEvalOp bag type.
	typedef Beagle::ContainerT<SharedLibEvalOp,Beagle::GP::EvaluationOp::Bag> Bag;

	SharedLibEvalOp();
	virtual ~SharedLibEvalOp();

	virtual Beagle::Fitness::Handle evaluate(Beagle::GP::Individual& inIndividual,
	        Beagle::GP::Context& ioContext);

	/*!
	 *
	 */
	virtual void init(Beagle::System& ioSystem);

	/*!
	 *
	 */
	virtual void registerParams(Beagle::System& ioSystem);
	    
protected:

    //! PACC::Timer for profiling. The ioContext's execution timer cannot be used, as it is reset internally.
    PACC::Timer mTimer;
    
    //! A handle on the shared lib for evaluating individuals.
    void* mSharedLibHandle;
        
    //! The number of rows in the training set.    
    int mTrainingSetSize;
};

}

}

#endif // SharedLibEvalOp_hpp
