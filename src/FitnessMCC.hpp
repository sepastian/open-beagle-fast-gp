#ifndef Beagle_GP_FitnessMCC_hpp
#define Beagle_GP_FitnessMCC_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Stats.hpp"
#include "beagle/FitnessSimple.hpp"


namespace Beagle
{
namespace GP
{

/*!
 *  \class FitnessMCC beagle/GP/FitnessMCC.hpp "beagle/GP/FitnessMCC.hpp"
 *  \brief MCC's fitness class.
 *  \ingroup GPF
 *  \ingroup GPFit
 *
 *  \par (Matthews Correlation Coefficient):
 *  From http://en.wikipedia.org/wiki/Matthews_Correlation_Coefficient:
 *  [Matthews correlation coefficient (MCC)] takes into account true and false 
 *  positives and negatives and is generally regarded as a balanced measure which 
 *  can be used even if the classes are of very different sizes. The MCC is in 
 *  essence a correlation coefficient between the observed and predicted binary 
 *  classifications; it returns a value between −1 and +1. A coefficient of +1 
 *  represents a perfect prediction, 0 an average random prediction and −1 an 
 *  inverse prediction. [...] While there is no perfect way of describing the 
 *  confusion matrix of true and false positives and negatives by a single number, 
 *  the Matthews correlation coefficient is generally regarded as being one of the 
 *  best such measures. Other measures, such as the proportion of correct 
 *  predictions (also termed accuracy), are not useful when the two classes are 
 *  of very different sizes.
 *
 */
class FitnessMCC : public FitnessSimple
{

public:

	//! GP::FitnessMCC allocator type.
	typedef AllocatorT<FitnessMCC,FitnessSimple::Alloc>
	Alloc;
	//! GP::FitnessMCC handle type.
	typedef PointerT<FitnessMCC,FitnessSimple::Handle>
	Handle;
	//! GP::FitnessMCC bag type.
	typedef ContainerT<FitnessMCC,FitnessSimple::Bag>
	Bag;

	FitnessMCC();
	explicit FitnessMCC(unsigned int inTruePositives,
						unsigned int inFalsePositives,
						unsigned int inTrueNegatives,
						unsigned int inFalseNegatives);

	virtual const std::string&  getType() const;
	virtual void                read(PACC::XML::ConstIterator inIter);
	virtual void                setFitness(unsigned int inTruePositives,
                                           unsigned int inFalsePositives,
                                           unsigned int inTrueNegatives,
                                           unsigned int inFalseNegatives);
	virtual void                writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

	/*!
	 *  \brief  Return the number of true positives 
	 *          (TP, positive samples classified as positive).
	 *  \return Number of true positives (TP).
	 */
	inline float getTruePositives() const
	{
		Beagle_StackTraceBeginM();
		return mTruePositives;
		Beagle_StackTraceEndM("float GP::FitnessMCC::getTruePositives() const");
	}

	/*!
	 *  \brief  Return the number of false positives 
	 *          (FP, negative samples classified as positive).
	 *  \return Number of false positives (FP).
	 */
	inline float getFalsePositives() const
	{
		Beagle_StackTraceBeginM();
		return mFalsePositives;
		Beagle_StackTraceEndM("float GP::FitnessMCC::getFalsePositives() const");
	}

	/*!
	 *  \brief  Return the number of true negatives 
	 *          (TN, negative samples classified as negative).
	 *  \return Number of true negatives (TN).
	 */
	inline float getTrueNegatives() const
	{
		Beagle_StackTraceBeginM();
		return mTrueNegatives;
		Beagle_StackTraceEndM("float GP::FitnessMCC::getTrueNegatives() const");
	}

	/*!
	 *  \brief  Return the number of false negatives 
	 *          (FN, positive samples classified as negative).
	 *  \return Number of false negatives (FN).
	 */
	inline float getFalseNegatives() const
	{
		Beagle_StackTraceBeginM();
		return mFalseNegatives;
		Beagle_StackTraceEndM("float GP::FitnessMCC::getFalseNegatives() const");
	}

	/*!
	 *  \brief  Return the relative number of true positives 
	 *          (TP, positive samples classified as positive).
	 *  \return Relative number of true positives (TP), e.g. TP/ (TP+ FN), 
	 *          or -1.0, if isValid() returns false.
	 */
	inline float getTruePositivesRel() const
	{
		Beagle_StackTraceBeginM();
		return mTruePositivesRel;
		Beagle_StackTraceEndM("float GP::FitnessMCC::getTruePositivesRel() const");
	}

	/*!
	 *  \brief  Return the relative number of false positives 
	 *          (FP, negative samples classified as positive).
	 *  \return Relative number of false positives (FP), e.g. FP/ (TN+ FP), 
	 *          or -1.0, if isValid() returns false.
	 */
	inline float getFalsePositivesRel() const
	{
		Beagle_StackTraceBeginM();
		return mFalsePositivesRel;		
		Beagle_StackTraceEndM("float GP::FitnessMCC::getFalsePositivesRel() const");
	}

	/*!
	 *  \brief  Return the relative number of true negatives 
	 *          (TN, negative samples classified as negative).
	 *  \return Relative number of true negatives (TN), e.g. TN/ (TN+ FP), 
	 *          or -1.0, if isValid() returns false.
	 */
	inline float getTrueNegativesRel() const
	{
		Beagle_StackTraceBeginM();
		return mTrueNegativesRel;
		Beagle_StackTraceEndM("float GP::FitnessMCC::getTrueNegativesRel() const");
	}

	/*!
	 *  \brief  Return the relative number of false negatives 
	 *          (FN, positive samples classified as negative).
	 *  \return Relative number of false negatives (FN), e.g. FN/ (TP+ FN), 
	 *          or -1.0, if isValid() returns false.
	 */
	inline float getFalseNegativesRel() const
	{
		Beagle_StackTraceBeginM();
		return mFalseNegativesRel;
		Beagle_StackTraceEndM("float GP::FitnessMCC::getFalseNegativesRel() const");
	}

private:

	unsigned int mTruePositives;	//!< Number of true positives.
	unsigned int mFalsePositives;	//!< Number of false positives.
	unsigned int mTrueNegatives;	//!< Number of true negatives.
	unsigned int mFalseNegatives;	//!< Number of false negatives.

	float mTruePositivesRel;	//!< Relative number of true positives, TP/ (TP+ FN).
	float mFalsePositivesRel;	//!< Relative number of false positives, FP/ (TN+ FP).
	float mTrueNegativesRel;	//!< Relative number of true negatives, TN/ (TN+ FP).
	float mFalseNegativesRel;	//!< Relative number of false negatives, FN/ (TP+ FN).
	
};

}
}

#endif // Beagle_GP_FitnessMCC_hpp

