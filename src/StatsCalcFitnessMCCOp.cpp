#include <cmath>
#include <sstream>

#include "beagle/GP.hpp"
#include "FitnessMCC.hpp"
#include "StatsCalcFitnessMCCOp.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a calculate stats operator.
 *  \param inName Name of the operator.
 */
GP::StatsCalcFitnessMCCOp::StatsCalcFitnessMCCOp(std::string inName) :
		StatsCalculateOp(inName)
{ }


/*!
 *  \brief Calculate MCC statistics of a given deme.
 *  \param outStats  Evaluated statistics.
 *  \param ioDeme    Deme to evalute.
 *  \param ioContext Context of the evolution.
 *
 *  The following statistics are calculated and made available in outStats:
 *    + mcc (as returned by FitnessMCC::getValue)
 *    + true-positives
 *    + false-positives
 *    + true-negatives
 *    + false-negatives
 *    + true-positives-relative
 *    + false-positives-relative
 *    + true-negatives-relative
 *    + false-negatives-relative
 *    + treedepth
 *    + treesize
 *
 */
void GP::StatsCalcFitnessMCCOp::calculateStatsDeme(Beagle::Stats& outStats,
        Beagle::Deme& ioDeme,
        Beagle::Context& ioContext) const
{
	Beagle_StackTraceBeginM();

	outStats.clear();
	outStats.clearItems();
	outStats.addItem("processed", ioContext.getProcessedDeme());
	outStats.addItem("total-processed", ioContext.getTotalProcessedDeme());

//std::cerr << ioDeme.size() << std::endl;

	if(ioDeme.size() == 0) {
	    /*
	     * The deme does not contain any individuals.
	     * Set all statistics to 0.0.
	     */
		outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 0, true);

		outStats.resize(11);
		outStats[0].mID = "mcc";
		outStats[0].mAvg = 0.0;
		outStats[0].mStd = 0.0;
		outStats[0].mMax = 0.0;
		outStats[0].mMin = 0.0;

		outStats[1].mID = "true-positives";
		outStats[1].mAvg = 0.0;
		outStats[1].mStd = 0.0;
		outStats[1].mMax = 0.0;
		outStats[1].mMin = 0.0;

		outStats[2].mID = "false-positives";
		outStats[2].mAvg = 0.0;
		outStats[2].mStd = 0.0;
		outStats[2].mMax = 0.0;
		outStats[2].mMin = 0.0;

		outStats[3].mID = "true-negatives";
		outStats[3].mAvg = 0.0;
		outStats[3].mStd = 0.0;
		outStats[3].mMax = 0.0;
		outStats[3].mMin = 0.0;

		outStats[4].mID = "false-negatives";
		outStats[4].mAvg = 0.0;
		outStats[4].mStd = 0.0;
		outStats[4].mMax = 0.0;
		outStats[4].mMin = 0.0;

		outStats[5].mID = "true-positives-relative";
		outStats[5].mAvg = 0.0;
		outStats[5].mStd = 0.0;
		outStats[5].mMax = 0.0;
		outStats[5].mMin = 0.0;

		outStats[6].mID = "false-positives-relative";
		outStats[6].mAvg = 0.0;
		outStats[6].mStd = 0.0;
		outStats[6].mMax = 0.0;
		outStats[6].mMin = 0.0;

		outStats[7].mID = "true-negatives-relative";
		outStats[7].mAvg = 0.0;
		outStats[7].mStd = 0.0;
		outStats[7].mMax = 0.0;
		outStats[7].mMin = 0.0;

		outStats[8].mID = "false-negatives-relative";
		outStats[8].mAvg = 0.0;
		outStats[8].mStd = 0.0;
		outStats[8].mMax = 0.0;
		outStats[8].mMin = 0.0;

		outStats[9].mID = "treedepth";
		outStats[9].mAvg = 0.0;
		outStats[9].mStd = 0.0;
		outStats[9].mMax = 0.0;
		outStats[9].mMin = 0.0;

		outStats[10].mID = "treesize";
		outStats[10].mAvg = 0.0;
		outStats[10].mStd = 0.0;
		outStats[10].mMax = 0.0;
		outStats[10].mMin = 0.0;
		return;
	}

	const GP::Individual::Handle lFirstIndiv= castHandleT<GP::Individual>(ioDeme[0]);
	const GP::FitnessMCC::Handle lFirstIndivFitness= castHandleT<GP::FitnessMCC>(lFirstIndiv->getFitness());

	if(ioDeme.size() == 1) {
	    /*
	     * The deme contains a single individual.
	     * No need to compute average values.
	     */
		outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
		                             ioContext.getGeneration(), 1, true);

std::cerr << lFirstIndivFitness->getTruePositives() << "/" << lFirstIndivFitness->getFalsePositives() << "/";
std::cerr << lFirstIndivFitness->getFalseNegatives() << "/" << lFirstIndivFitness->getTrueNegatives() << std::endl;
std::cerr << lFirstIndivFitness->getTruePositivesRel() << "/" << lFirstIndivFitness->getFalsePositivesRel() << "/";
std::cerr << lFirstIndivFitness->getFalseNegativesRel() << "/" << lFirstIndivFitness->getTrueNegativesRel() << std::endl;
std::cerr << std::endl;

		outStats.resize(11);
		outStats[0].mID = "mcc";
		outStats[0].mAvg = lFirstIndivFitness->getValue();
		outStats[0].mStd = 0.0;
		outStats[0].mMax = lFirstIndivFitness->getValue();
		outStats[0].mMin = lFirstIndivFitness->getValue();

		outStats[1].mID = "true-positives";
		outStats[1].mAvg = lFirstIndivFitness->getTruePositives();
		outStats[1].mStd = 0.0;
		outStats[1].mMax = lFirstIndivFitness->getTruePositives();
		outStats[1].mMin = lFirstIndivFitness->getTruePositives();

		outStats[2].mID = "false-positives";
		outStats[2].mAvg = lFirstIndivFitness->getFalsePositives();
		outStats[2].mStd = 0.0;
		outStats[2].mMax = lFirstIndivFitness->getFalsePositives();
		outStats[2].mMin = lFirstIndivFitness->getFalsePositives();

		outStats[3].mID = "true-negatives";
		outStats[3].mAvg = lFirstIndivFitness->getTrueNegatives();
		outStats[3].mStd = 0.0;
		outStats[3].mMax = lFirstIndivFitness->getTrueNegatives();
		outStats[3].mMin = lFirstIndivFitness->getTrueNegatives();

		outStats[4].mID = "false-negatives";
		outStats[4].mAvg = lFirstIndivFitness->getFalseNegatives();
		outStats[4].mStd = 0.0;
		outStats[4].mMax = lFirstIndivFitness->getFalseNegatives();
		outStats[4].mMin = lFirstIndivFitness->getFalseNegatives();

		outStats[5].mID = "true-positives-relative";
		outStats[5].mAvg = lFirstIndivFitness->getTruePositivesRel();
		outStats[5].mStd = 0.0;
		outStats[5].mMax = lFirstIndivFitness->getTruePositivesRel();
		outStats[5].mMin = lFirstIndivFitness->getTruePositivesRel();

		outStats[6].mID = "false-positives-relative";
		outStats[6].mAvg = lFirstIndivFitness->getFalsePositivesRel();
		outStats[6].mStd = 0.0;
		outStats[6].mMax = lFirstIndivFitness->getFalsePositivesRel();
		outStats[6].mMin = lFirstIndivFitness->getFalsePositivesRel();

		outStats[7].mID = "true-negatives-relative";
		outStats[7].mAvg = lFirstIndivFitness->getTrueNegativesRel();
		outStats[7].mStd = 0.0;
		outStats[7].mMax = lFirstIndivFitness->getTrueNegativesRel();
		outStats[7].mMin = lFirstIndivFitness->getTrueNegativesRel();

		outStats[8].mID = "false-negatives-relative";
		outStats[8].mAvg = lFirstIndivFitness->getFalseNegativesRel();
		outStats[8].mStd = 0.0;
		outStats[8].mMax = lFirstIndivFitness->getFalseNegativesRel();
		outStats[8].mMin = lFirstIndivFitness->getFalseNegativesRel();

		outStats[9].mID = "treedepth";
		outStats[9].mAvg = lFirstIndiv->getMaxTreeDepth();
		outStats[9].mStd = 0.0;
		outStats[9].mMax = outStats[5].mAvg;
		outStats[9].mMin = outStats[5].mAvg;

		outStats[10].mID = "treesize";
		outStats[10].mAvg = lFirstIndiv->getTotalNodes();
		outStats[10].mStd = 0.0;
		outStats[10].mMax = outStats[6].mAvg;
		outStats[10].mMin = outStats[6].mAvg;
		return;
	}
	
	/*
	 * Compute AVG, STD, MIN, and MAX over all individuals in the deme 
	 * for each of the 11 measures (mcc, tp, fp, tn, fn, tpr, fpr, 
	 * tnr, fnr, treesize, treedepth).
	 */
	// MCC
	double lMCCSum=         (double)lFirstIndivFitness->getValue();
	double lMCCPow2Sum=     pow2Of<double>(lMCCSum);
	double lMCCMax=         lMCCSum;
	double lMCCMin=         lMCCSum;
	// True positives
	double lTPSum=          (double)lFirstIndivFitness->getTruePositives();
	double lTPPow2Sum=      pow2Of<double>(lTPSum);
	double lTPMax=          lTPSum;
	double lTPMin=          lTPSum;
	// False positives
	double lFPSum=          (double)lFirstIndivFitness->getFalsePositives();
	double lFPPow2Sum=      pow2Of<double>(lFPSum);
	double lFPMax=          lFPSum;
	double lFPMin=          lFPSum;
	// True negatives
	double lTNSum=          (double)lFirstIndivFitness->getTrueNegatives();
	double lTNPow2Sum=      pow2Of<double>(lTNSum);
	double lTNMax=          lTNSum;
	double lTNMin=          lTNSum;
	// False negatives
	double lFNSum=          (double)lFirstIndivFitness->getFalseNegatives();
	double lFNPow2Sum=      pow2Of<double>(lFNSum);
	double lFNMax=          lFNSum;
	double lFNMin=          lFNSum;
	// True positives, relative
	double lTPRSum=         (double)lFirstIndivFitness->getTruePositivesRel();
	double lTPRPow2Sum=     pow2Of<double>(lTPRSum);
	double lTPRMax=         lTPRSum;
	double lTPRMin=         lTPRSum;
	// False positives, relative
	double lFPRSum=         (double)lFirstIndivFitness->getFalsePositivesRel();
	double lFPRPow2Sum=     pow2Of<double>(lFPRSum);
	double lFPRMax=         lFPRSum;
	double lFPRMin=         lFPRSum;
	// True negatives, relative
	double lTNRSum=         (double)lFirstIndivFitness->getTrueNegativesRel();
	double lTNRPow2Sum=     pow2Of<double>(lTNRSum);
	double lTNRMax=         lTNRSum;
	double lTNRMin=         lTNRSum;
	// False negatives, relative
	double lFNRSum=         (double)lFirstIndivFitness->getFalseNegativesRel();
	double lFNRPow2Sum=     pow2Of<double>(lFNRSum);
	double lFNRMax=         lFNRSum;
	double lFNRMin=         lFNRSum;
	// Tree depth
	double lDepthMax=       lFirstIndiv->getMaxTreeDepth();
	double lDepthMin=       lDepthMax;
	double lDepthSum=       (double)lDepthMax;
	double lDepthPow2Sum=   pow2Of<double>(lDepthSum);
	// Tree size
	double lSizeMax=        lFirstIndiv->getTotalNodes();
	double lSizeMin=        lSizeMax;
	double lSizeSum=        (double)lSizeMax;
	double lSizePow2Sum=    pow2Of<double>(lSizeSum);
    
    // Cummulate values.
std::cerr << ioDeme.size() << " individuals in deme." << std::endl;

	for(unsigned int i=1; i<ioDeme.size(); i++) 
	{
		const GP::Individual::Handle lIndiv= castHandleT<GP::Individual>(ioDeme[i]);
		const GP::FitnessMCC::Handle lIndivFitness= castHandleT<GP::FitnessMCC>(lIndiv->getFitness());
		double lValue= 0.0;

//if (lIndivFitness->getTruePositives()+ lIndivFitness->getFalsePositives()+ lIndivFitness->getFalseNegatives()+ lIndivFitness->getTrueNegativesRel() == 0)
//{
//std::cerr << i << " / " << ioDeme.size() << ", ";
//std::cerr << lIndivFitness->getTruePositives() << "/" << lIndivFitness->getFalsePositives() << "/";
//std::cerr << lIndivFitness->getFalseNegatives() << "/" << lIndivFitness->getTrueNegatives() << std::endl;
//std::cerr << lIndivFitness->getTruePositivesRel() << "/" << lIndivFitness->getFalsePositivesRel() << "/";
//std::cerr << lIndivFitness->getFalseNegativesRel() << "/" << lIndivFitness->getTrueNegativesRel() << std::endl;
//std::cerr << std::endl;
//}

		// MCC
		lValue=         lIndivFitness->getValue();
		lMCCSum+=       lValue;
		lMCCPow2Sum+=   pow2Of<double>(lValue);
		lMCCMax=        maxOf(lMCCMax, lValue);
		lMCCMin=        minOf(lMCCMin, lValue);
		// True positives
		lValue=         lIndivFitness->getTruePositives();
		lTPSum+=        lValue;
		lTPPow2Sum+=    pow2Of<double>(lValue);
		lTPMax=         maxOf(lTPMax, lValue);
		lTPMin=         minOf(lTPMin, lValue);
		// False positives
		lValue=         lIndivFitness->getFalsePositives();
		lFPSum+=        lValue;
		lFPPow2Sum+=    pow2Of<double>(lValue);
		lFPMax=         maxOf(lFPMax, lValue);
		lFPMin=         minOf(lFPMin, lValue);
		// True negatives
		lValue=         lIndivFitness->getTrueNegatives();
		lTNSum+=        lValue;
		lTNPow2Sum+=    pow2Of<double>(lValue);
		lTNMax=         maxOf(lTNMax, lValue);
		lTNMin=         minOf(lTNMin, lValue);
		// False negatives
		lValue=         lIndivFitness->getFalseNegatives();
		lFNSum+=        lValue;
		lFNPow2Sum+=    pow2Of<double>(lValue);
		lFNMax=         maxOf(lFNMax, lValue);
		lFNMin=         minOf(lFNMin, lValue);
		// True positives, relative
		lValue=         lIndivFitness->getTruePositivesRel();
		lTPRSum+=       lValue;
		lTPRPow2Sum+=   pow2Of<double>(lValue);
		lTPRMax=        maxOf(lTPRMax, lValue);
		lTPRMin=        minOf(lTPRMin, lValue);
		// False positives, relative
		lValue=         (double)lIndivFitness->getFalsePositivesRel();
		lFPRSum+=       lValue;
		lFPRPow2Sum+=   pow2Of<double>(lValue);
		lFPRMax=        maxOf(lFPRMax, lValue);
		lFPRMin=        minOf(lFPRMin, lValue);
		// True negatives, relative
		lValue+=        (double)lIndivFitness->getTrueNegativesRel();
		lTNRSum+=       lValue;
		lTNRPow2Sum+=   pow2Of<double>(lValue);
		lTNRMax=        maxOf(lTNRMax, lValue);
		lTNRMin=        minOf(lTNRMin, lValue);
		// False negatives, relative
		lValue+=        (double)lIndivFitness->getFalseNegativesRel();
		lFNRSum+=       lValue;
		lFNRPow2Sum+=   pow2Of<double>(lValue);
		lFNRMax=        maxOf(lFNRMax, lValue);
		lFNRMin=        minOf(lFNRMin, lValue);
        // Tree depth		
        lValue=         lIndiv->getMaxTreeDepth();
		lDepthSum+=     lValue;
		lDepthPow2Sum+= pow2Of<double>(lValue);
		lDepthMax=      maxOf(lDepthMax, lValue);
		lDepthMin=      minOf(lDepthMin, lValue);
		// Tree size
        lValue=         lIndiv->getTotalNodes();
		lSizeSum+=      lValue;
		lSizePow2Sum+=  pow2Of<double>(lValue);
		lSizeMax=       maxOf(lSizeMax, lValue);
		lSizeMin=       minOf(lSizeMin, lValue);
	}

	// Compute average and standard error.
	double lDemeSize= ioDeme.size();
	// MCC
	float lMCCAverage= lMCCSum/ lDemeSize;
	float lMCCStdError= sqrt((lMCCPow2Sum- (pow2Of<double>(lMCCSum)/ lDemeSize))/ (lDemeSize- 1));
	// True positives
	float lTPAverage= lTPSum/ lDemeSize;
	float lTPStdError= sqrt((lTPPow2Sum- (pow2Of<double>(lTPSum)/ lDemeSize))/ (lDemeSize- 1));
	// False positives
	float lFPAverage= lFPSum/ lDemeSize;
	float lFPStdError= sqrt((lFPPow2Sum- (pow2Of<double>(lFPSum)/ lDemeSize))/ (lDemeSize- 1));
	// True negatives
	float lTNAverage= lTNSum/ lDemeSize;
	float lTNStdError= sqrt((lTNPow2Sum- (pow2Of<double>(lTNSum)/ lDemeSize))/ (lDemeSize- 1));
	// False negatives
	float lFNAverage= lFNSum/ lDemeSize;
	float lFNStdError= sqrt((lFNPow2Sum- (pow2Of<double>(lFNSum)/ lDemeSize))/ (lDemeSize- 1));
	// True positives, relative
	float lTPRAverage= lTPRSum/ lDemeSize;
	float lTPRStdError= sqrt((lTPRPow2Sum- (pow2Of<double>(lTPRSum)/ lDemeSize))/ (lDemeSize- 1));
	// False positives, relative
	float lFPRAverage= lFPRSum/ lDemeSize;
	float lFPRStdError= sqrt((lFPRPow2Sum- (pow2Of<double>(lFPRSum)/ lDemeSize))/ (lDemeSize- 1));
	// True negatives, relative
	float lTNRAverage= lTNRSum/ lDemeSize;
	float lTNRStdError= sqrt((lTNRPow2Sum- (pow2Of<double>(lTNRSum)/ lDemeSize))/ (lDemeSize- 1));
	// False negatives, relative
	float lFNRAverage= lFNRSum/ lDemeSize;
	float lFNRStdError= sqrt((lFNRPow2Sum- (pow2Of<double>(lFNRSum)/ lDemeSize))/ (lDemeSize- 1));
	// Tree size
	float lSizeAverage= lSizeSum/ lDemeSize;
	float lSizeStdError= sqrt((lSizePow2Sum- (pow2Of<double>(lSizeSum)/ lDemeSize))/ (lDemeSize- 1));
	// Tree depth
	float lDepthAverage= lDepthSum/ lDemeSize;
	float lDepthStdError= sqrt((lDepthPow2Sum- (pow2Of<double>(lDepthSum)/ lDemeSize))/ (lDemeSize- 1));
	
	outStats.setGenerationValues(std::string("deme")+uint2str(ioContext.getDemeIndex()),
	                             ioContext.getGeneration(), ioDeme.size(), true);

	outStats.resize(11);
	outStats[0].mID = "mcc";
	outStats[0].mAvg = lMCCAverage;
	outStats[0].mStd = lMCCStdError;
	outStats[0].mMax = lMCCMax;
	outStats[0].mMin = lMCCMin;

	outStats[1].mID = "true-positives";
	outStats[1].mAvg = lTPAverage;
	outStats[1].mStd = lTPStdError;
	outStats[1].mMax = (unsigned int)lTPMax;
	outStats[1].mMin = (unsigned int)lTPMin;

	outStats[2].mID = "false-positives";
	outStats[2].mAvg = lFPAverage;
	outStats[2].mStd = lFPStdError;
	outStats[2].mMax = (unsigned int)lFPMax;
	outStats[2].mMin = (unsigned int)lFPMin;

	outStats[3].mID = "true-negatives";
	outStats[3].mAvg = lTNAverage;
	outStats[3].mStd = lTNStdError;
	outStats[3].mMax = (unsigned int)lTNMax;
	outStats[3].mMin = (unsigned int)lTNMin;

	outStats[4].mID = "false-negatives";
	outStats[4].mAvg = lFNAverage;
	outStats[4].mStd = lFNStdError;
	outStats[4].mMax = (unsigned int)lFNMax;
	outStats[4].mMin = (unsigned int)lFNMin;

	outStats[5].mID = "true-positives-relative";
	outStats[5].mAvg = lTPRAverage;
	outStats[5].mStd = lTPRStdError;
	outStats[5].mMax = lTPRMax;
	outStats[5].mMin = lTPRMin;

	outStats[6].mID = "false-positives-relative";
	outStats[6].mAvg = lFPRAverage;
	outStats[6].mStd = lFPRStdError;
	outStats[6].mMax = lFPRMax;
	outStats[6].mMin = lFPRMin;

	outStats[7].mID = "true-negatives-relative";
	outStats[7].mAvg = lTNRAverage;
	outStats[7].mStd = lTNRStdError;
	outStats[7].mMax = lTNRMax;
	outStats[7].mMin = lTNRMin;

	outStats[8].mID = "false-negatives-relative";
	outStats[8].mAvg = lFNRAverage;
	outStats[8].mStd = lFNRStdError;
	outStats[8].mMax = lFNRMax;
	outStats[8].mMin = lFNRMin;

	outStats[9].mID = "treedepth";
	outStats[9].mAvg = lDepthAverage;
	outStats[9].mStd = lDepthStdError;
	outStats[9].mMax = (unsigned int)lDepthMax;
	outStats[9].mMin = (unsigned int)lDepthMin;

	outStats[10].mID = "treesize";
	outStats[10].mAvg = lSizeAverage;
	outStats[10].mStd = lSizeStdError;
	outStats[10].mMax =(unsigned int) lSizeMax;
	outStats[10].mMin =(unsigned int) lSizeMin;
	
	Beagle_StackTraceEndM("void GP::StatsCalcFitnessMCCOp::calculateStatsDeme(Beagle::Stats& outStats, Beagle::Deme& ioDeme, Beagle::Context& ioContext) const");
}

