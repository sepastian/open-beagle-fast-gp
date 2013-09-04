#ifndef TrainingSetSamplingOp_hpp
#define TrainingSetSamplingOp_hpp

#include "beagle/Operator.hpp"

#include <string>
#include <vector>

namespace Beagle
{

namespace GP
{

/*!
 * \class TrainingSetSamplingOp TrainingSetSamplingOp.hpp "TrainingSetSamplingOp.hpp"
 * \brief Calculate the number of positive and negative samples to include into the training set.
 * \ingroup ICU
 *
 * Given a data set component, this operator allows for controlling the sampling of a training set.
 * The ratio of samples to include from the smaller subset can be controlled (upper/lower bound), 
 * while asserting that no more than a maximum percentage of this sample is included into the training set.
 *
 */
class TrainingSetSamplingOp : public Beagle::Operator
{

public:

	//! TrainingSetSamplingOp allocator type.
	typedef Beagle::AllocatorT<TrainingSetSamplingOp,Beagle::Operator::Alloc> Alloc;
	//!< TrainingSetSamplingOp handle type.
	typedef Beagle::PointerT<TrainingSetSamplingOp,Beagle::Operator::Handle> Handle;
	//!< TrainingSetSamplingOp bag type.
	typedef Beagle::ContainerT<TrainingSetSamplingOp,Beagle::Operator::Bag> Bag;
	
	/*!
	 * The following parameters are registered under the names specified by this operator:
	 *
	 * inTrainingsetSizeName (icu.trainingset.size)
	 *   The number of samples drawn from the data set forming the training set.
	 *
	 * inMinRatioName (icu.trainingset.min-ratio)
	 *   The minimum ratio of samples from the smaller subset to include in 
	 *   the training set. Lies in (0,1), defaults to 0.07 (7 %).
	 *
	 * inMaxRatioName (icu.trainigset.max-ratio)
	 *   The maximum ratio of samples from the smaller subset to include in 
	 *   the training set. Lies in (0,1), defaults to 0.10 (10 %).
	 *
	 */
	explicit TrainingSetSamplingOp(
		std::string inTrainingsetSizeName="icu.trainingset.size",	
		std::string inMinRatioName="icu.trainingset.min-ratio",
		std::string inMaxRatioName="icu.trainingset.max-ratio"
	);
	virtual ~TrainingSetSamplingOp()
	{
	}

	virtual void registerParams(Beagle::System& ioSystem);

	/*!
	 * Calculate sampling sizes.
	 */
	virtual void operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext);



protected:

	std::string mTrainingSetSizeName;
	std::string mMinRatioName;
	std::string mMaxRatioName;

	Int::Handle mTrainingSetSize;
	Float::Handle mMinRatio;
	Float::Handle mMaxRatio;
	
};

}

}

#endif // TrainingSetSamplingOp_hpp
