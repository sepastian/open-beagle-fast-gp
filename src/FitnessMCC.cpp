#include "beagle/GP.hpp"
#include "FitnessMCC.hpp"

#include <cmath>
#include <cfloat>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Default construct a MCC's fitness object.
 *  \par   The fitness object constructed will be invalid, 
 *         as no fitness values have been specified.
 */
GP::FitnessMCC::FitnessMCC() :
		FitnessSimple(),
		mTruePositives(0),
		mFalsePositives(0),
		mTrueNegatives(0),
		mFalseNegatives(0)
{ }


/*!
 *  \brief Construct a valid MCC fitness object.
 *  \param inTruePositives  Number of true positives (positive samples classified positive).
 *  \param inFalsePositives Number of false positives (negative samples classified negative).
 *  \param inTrueNegatives  Number of true negatives (negative samples classified negative).
 *  \param inFalseNegatives Number of false negatives (positive samples classified positive).
 */
GP::FitnessMCC::FitnessMCC(unsigned int inTruePositives,
						unsigned int inFalsePositives,
						unsigned int inTrueNegatives,
						unsigned int inFalseNegatives)
{
	Beagle_StackTraceBeginM();
	setFitness(inTruePositives,
	           inFalsePositives,
	           inTrueNegatives,
	           inFalseNegatives);
	Beagle_StackTraceEndM("GP::FitnessMCC::FitnessMCC(unsigned int,unsigned int,unsigned int,unsigned int)");
}


/*!
 *  \brief Get exact type of the fitness.
 *  \return Exact type of the fitness.
 */
const std::string& GP::FitnessMCC::getType() const
{
	Beagle_StackTraceBeginM();
	const static std::string lType("GP-FitnessMCC");
	return lType;
	Beagle_StackTraceEndM("const std::string& GP::FitnessMCC::getType() const")
}


/*!
 *  \brief Read a MCC's fitness from a XML subtree.
 *  \param inIter XML iterator to use to read the fitness values.
 */
void GP::FitnessMCC::read(PACC::XML::ConstIterator inIter)
{
	Beagle_StackTraceBeginM();
	if((inIter->getType() != PACC::XML::eData) || (inIter->getValue() != "Fitness")) {
		throw Beagle_IOExceptionNodeM(*inIter, "tag <Fitness> expected!");
	}

	std::string lValid = inIter->getAttribute("valid");
	if(lValid.empty() || (lValid == "yes")) {
	
		// Check type of fitness read
		std::string lType = inIter->getAttribute("type");
		if((lType.empty() == false) && (lType != getType())) {
			std::ostringstream lOSS;
			lOSS << "type given '" << lType << "' mismatch type of the fitness '";
			lOSS << getType() << "'!";
			throw Beagle_IOExceptionNodeM(*inIter, lOSS.str());
		}

		// Read values of MCC's fitness
		for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
			if(lChild->getType() == PACC::XML::eData) {
				if(lChild->getValue() == "MCC") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no MCC fitness value present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no MCC fitness value present!");
					mValue= str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "TruePositives") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for true positives present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for true positives present!");
					mTruePositives= str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "FalsePositives") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for false positives present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for false positives present!");
					mFalsePositives= str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "TrueNegatives") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for true negatives present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for true negatives present!");
					mTrueNegatives= str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "FalseNegatives") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for false negatives present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for false negatives present!");
					mFalseNegatives= str2uint(lChild2->getValue());
				} else if(lChild->getValue() == "TruePositivesRel") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for true positives rel present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for true positives rel present!");
					mTruePositivesRel= str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "FalsePositivesRel") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for false positives rel present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for false positives rel present!");
					mFalsePositivesRel= str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "TrueNegativesRel") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for true negatives rel present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for true negatives rel present!");
					mTrueNegativesRel= str2dbl(lChild2->getValue());
				} else if(lChild->getValue() == "FalseNegativesRel") {
					PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
					if(!lChild2) throw Beagle_IOExceptionNodeM(*lChild, "no value for false negatives Rel present!");
					if(lChild2->getType() != PACC::XML::eString)
						throw Beagle_IOExceptionNodeM(*lChild2, "no value for false negatives Rel present!");
					mFalseNegativesRel= str2uint(lChild2->getValue());
				}
			}
		}

	} else if(lValid == "no") {
		setInvalid();
	}
	else {
		throw Beagle_IOExceptionNodeM(*inIter, "bad fitness validity value!");
	}

	Beagle_StackTraceEndM("void GP::FitnessMCC::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Set the fitness value computing MCC from TP, FP, TN, and FN; set validity.
 *  \param inTruePositives  Number of true positives (positive samples classified positive).
 *  \param inFalsePositives Number of false positives (negative samples classified negative).
 *  \param inTrueNegatives  Number of true negatives (negative samples classified negative).
 *  \param inFalseNegatives Number of false negatives (positive samples classified positive).
 */
void GP::FitnessMCC::setFitness(unsigned int inTruePositives,
                                unsigned int inFalsePositives,
                                unsigned int inTrueNegatives,
                                unsigned int inFalseNegatives)
{
	Beagle_StackTraceBeginM();
    mTruePositives=  inTruePositives;
    mFalsePositives= inFalsePositives;
    mTrueNegatives=  inTrueNegatives;
    mFalseNegatives= inFalseNegatives;
    
	// Set default values for all members; these values will be returned if this fitness object is invalid.
	mValue= -FLT_MAX;
	// The fitness is valid, if no member m[True|False][Positives|Negatives] < 0.
	if (mTruePositives < 0 || mFalsePositives < 0 || mTrueNegatives < 0 || mFalseNegatives < 0) {
        setInvalid();
        return;
	}
    // Compute relative values for TP, FP, TN, FP.
    mTruePositivesRel=  (float)mTruePositives/ (mTruePositives+ mFalseNegatives);
    mFalsePositivesRel= (float)mFalsePositives/ (mTrueNegatives+ mFalsePositives);
	mTrueNegativesRel=  (float)mTrueNegatives/ (mTrueNegatives+ mFalsePositives);
	mFalseNegativesRel= (float)mFalseNegatives/ (mTruePositives+ mFalseNegatives);

    // Compute MCC.
    double lNumerator= ((double)mTruePositives* mTrueNegatives)- ((double)mFalsePositives* mFalseNegatives);
    double lDenominator= sqrt(
        ((double)mTruePositives+ mFalsePositives)* 
        ((double)mTruePositives+ mFalseNegatives)* 
        ((double)mTrueNegatives+ mFalsePositives)* 
        ((double)mTrueNegatives+ mFalseNegatives)
    );
    if (lDenominator == 0) lDenominator= 1;
    setValue(lNumerator/ lDenominator);

    // Set the fitness object valid.
	Beagle_StackTraceEndM("void GP::FitnessMCC::setFitness(unsigned int,unsigned int,unsigned int,unsigned int)");
}


/*!
 *  \brief Write an MCC fitness object into a Beagle XML streamer.
 *  \param ioStreamer XML streamer to use to write the fitness values.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::FitnessMCC::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
	Beagle_StackTraceBeginM();
	ioStreamer.openTag("MCC", false);
	ioStreamer.insertStringContent(dbl2str(mValue));
	ioStreamer.closeTag();
	ioStreamer.openTag("TruePositives", false);
	ioStreamer.insertStringContent(dbl2str(mTruePositives));
	ioStreamer.closeTag();
	ioStreamer.openTag("FalsePositives", false);
	ioStreamer.insertStringContent(dbl2str(mFalsePositives));
	ioStreamer.closeTag();
	ioStreamer.openTag("TrueNegatives", false);
	ioStreamer.insertStringContent(dbl2str(mTrueNegatives));
	ioStreamer.closeTag();
	ioStreamer.openTag("FalseNegatives", false);
	ioStreamer.insertStringContent(uint2str(mFalseNegatives));
	ioStreamer.closeTag();
	ioStreamer.openTag("TruePositivesRel", false);
	ioStreamer.insertStringContent(dbl2str(mTruePositivesRel));
	ioStreamer.closeTag();
	ioStreamer.openTag("FalsePositivesRel", false);
	ioStreamer.insertStringContent(dbl2str(mFalsePositivesRel));
	ioStreamer.closeTag();
	ioStreamer.openTag("TrueNegativesRel", false);
	ioStreamer.insertStringContent(dbl2str(mTrueNegativesRel));
	ioStreamer.closeTag();
	ioStreamer.openTag("FalseNegativesRel", false);
	ioStreamer.insertStringContent(dbl2str(mFalseNegativesRel));
	ioStreamer.closeTag();
	Beagle_StackTraceEndM("void GP::FitnessMCC::writeContent(PACC::XML::Streamer&, bool) const");
}

