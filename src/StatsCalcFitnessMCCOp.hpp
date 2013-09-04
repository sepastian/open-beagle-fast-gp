#ifndef Beagle_GP_StatsCalcFitnessMCCOp_hpp
#define Beagle_GP_StatsCalcFitnessMCCOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/Context.hpp"
#include "beagle/StatsCalculateOp.hpp"

namespace Beagle
{

namespace GP
{

/*!
 *  \class StatsCalcFitnessMCCOp StatsCalcFitnessMCCOp.hpp
 *     "beagle/GP/StatsCalcFitnessMCCOp.hpp"
 *  \brief Calculate MCC statistics of a GP deme, for a generation, operator class.
 *  \ingroup GPF
 */
class StatsCalcFitnessMCCOp : public StatsCalculateOp
{

public:

	//! StatsCalcFitnessMCCOp allocator type
	typedef AllocatorT<StatsCalcFitnessMCCOp,StatsCalculateOp::Alloc>
	Alloc;
	//! StatsCalcFitnessMCCOp handle type.
	typedef PointerT<StatsCalcFitnessMCCOp,StatsCalculateOp::Handle>
	Handle;
	//! StatsCalcFitnessMCCOp bag type.
	typedef ContainerT<StatsCalcFitnessMCCOp,StatsCalculateOp::Bag>
	Bag;

	explicit StatsCalcFitnessMCCOp(std::string inName="GP-StatsCalcFitnessMCCOp");
	virtual ~StatsCalcFitnessMCCOp()
	{ }

	virtual void calculateStatsDeme(Beagle::Stats& outStats,
	                                Beagle::Deme& ioDeme,
	                                Beagle::Context& ioContext) const;

};

}

}

#endif // Beagle_StatsCalcFitnessMCCOp_hpp
