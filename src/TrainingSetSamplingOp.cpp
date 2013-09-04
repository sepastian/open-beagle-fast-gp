#include "beagle/GP.hpp"
#include "TrainingSetSamplingOp.hpp"
#include "DataSetBinaryClassification.hpp"

using namespace Beagle;
using namespace GP;

/*!
 * The following parameters are registered by this operator:
 *
 * inMinRatioName (icu.trainingset.min-ratio)
 *
 * inMaxRatioName (icu.trainigset.max-ratio)
 *
 * inMaxRatioDatasetName (icu.trainigset.max-ratio-dataset)
 *
 */
TrainingSetSamplingOp::TrainingSetSamplingOp(
	std::string inTrainingsetSizeName, 
	std::string inMinRatioName, 
	std::string inMaxRatioName) 
	:
	Operator("TrainingSetSamplingOp"),
	mTrainingSetSizeName(inTrainingsetSizeName),
	mMinRatioName(inMinRatioName),
	mMaxRatioName(inMaxRatioName)
{
}

/*!
 *
 */
void TrainingSetSamplingOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	
	Operator::registerParams(ioSystem);
	
	Beagle_LogTraceM(
	    ioSystem.getLogger(),
	    "registerParams", "Beagle::GP::TrainingSetSamplingOp",
		"TrainingSetSamplingOp"
	);
	
	{
		Register::Description lDescription(
		    "Training Set size",
		    "Int",
		    "10000",
		    "The size of the training set used to evalutate the fitness of individuals."
		);
		mTrainingSetSize= castHandleT<Int>(
			ioSystem.getRegister().insertEntry(mTrainingSetSizeName, new Int(10000), lDescription));
	}
	
	{
		Register::Description lDescription(
		    "Minimum ratio of smaples from smaller subset in training set",
		    "Float",
		    "0.07",
		    "The minimum ratio of samples from the smaller subset to be included in the training set."
		);
		mMinRatio= castHandleT<Float>(
			ioSystem.getRegister().insertEntry(mMinRatioName, new Float(0.07), lDescription));
	}
	
	{
		Register::Description lDescription(
		    "Maximum ratio of samples from smaller subset in training set",
		    "Float",
		    "0.10",
		    "The maximum ratio of samples from the smaller subset to be included in the training set."
		);
		mMaxRatio= castHandleT<Float>(
			ioSystem.getRegister().insertEntry(mMaxRatioName, new Float(0.10), lDescription));
	}
	
	Beagle_StackTraceEndM("void TrainingSetSamplingOp::registerParams(Beagle::Systeme& ioSystem)");
}

/*!
 * Calculate sampling sizes.
 */
void TrainingSetSamplingOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
	// Build the training set T from the data set D.
    // The data set D can be divided into positive and negative (binary) subsets.
    // Let the smaller subset be DS and the larger subset be DL, with |DS| <= |DL|.
    // Likewise, T can be divided into smaller and larger subsets, TS and TL, with |TS| <= |TL|.
    
	// Validate parameters.
	int lSizeT= mTrainingSetSize->getWrappedValue();
    float lMinRatioTS= mMinRatio->getWrappedValue();
    float lMaxRatioTS= mMaxRatio->getWrappedValue();
	Beagle_ValidateParameterM(lMinRatioTS >= 0.0 && lMinRatioTS <= lMaxRatioTS, mMinRatioName, "0.0 <= mMinRatio <= "+ dbl2str(lMaxRatioTS));
	Beagle_ValidateParameterM(lMaxRatioTS <= 1.0, mMaxRatioName, dbl2str(lMinRatioTS)+ " <= mMaxRatio <= 1.0");
	
	// Get a handle on D, S, and L.
    DataSetBinaryClassification::Handle lD= castHandleT<DataSetBinaryClassification>(ioContext.getSystem().getComponent("DataSet"));
    std::vector<unsigned int>* lIndexesPos= lD->getIndexesPositives();
    std::vector<unsigned int>* lIndexesNeg= lD->getIndexesNegatives();
    Beagle_LogInfoM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
        "Data set: "+ int2str(lD->size())+ " rows, training set: "+ int2str(lSizeT)+ " rows.");

    // Abort, if |D| < |T|.
    if (lD->size() < lSizeT)
    {
        throw Beagle_RunTimeExceptionM("The size of the training set is set to "+ 
            int2str(lSizeT)+ ", but the data set contains only "+ 
            int2str(lD->size())+ " samples.");
    }
    
    // Stratified sampling.
    //
    // Select mTrainingSetSize samples from D to build the training set T.
    // Thereby, assure that at least |T|* mMinRatio, but no more than |T|* mMaxRatio be selected from S.
    // Further, do not allow to select more than |S|* mMaxRatioDataset from S.
    //
    // Building T will fail, if S is too small compared to L, or mMinRatio is too high, or mMaxRatioDataset is too low.
    
    // Determine the ratio of positives/negatives in D.
    float lRatioPosD= (float)lIndexesPos->size()/ lD->size();
    float lRatioNegD= (float)lIndexesNeg->size()/ lD->size();
    int lSizePosD= round(lD->size()* lRatioPosD);
    int lSizeNegD= round(lD->size()* lRatioNegD);
    Beagle_LogInfoM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
        "Data set: "+ int2str(lSizePosD)+ "("+ dbl2str(lRatioPosD*100, 2)+ " %) positive, "+ 
        int2str(lSizeNegD)+ "("+ dbl2str(lRatioNegD*100, 2)+ " %) negative.");

    // Set the ratio of positives/negatives in T to the ratio found in D.
    // Correct, such that minRatioS <= S <= maxRatioS. 
    float lRatioPosT= lRatioPosD;
    float lRatioNegT= lRatioNegD;
    if (lRatioPosD <= lRatioNegD)
    {
    	// More negatives than positives.
    	if (lRatioPosT < lMinRatioTS)
    	{
    		// The ratio DS is smaller than the min ratio TS.
		    Beagle_LogDetailedM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
		        "Training set: "+ dbl2str(lRatioPosT, 2)+ " -> "+ dbl2str(lMinRatioTS, 2)+ ", "+
		        dbl2str(lRatioNegT, 2)+ " -> "+ dbl2str(1.0- lMinRatioTS, 2)+ ".");
    		lRatioPosT= lMinRatioTS;
    		lRatioNegT= 1.0- lMinRatioTS;
    	}
    	else if (lRatioPosT > lMaxRatioTS)
    	{
    		// The ratio DS is larger than the max ratio TS.
		    Beagle_LogDetailedM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
		        "Training set: "+ dbl2str(lRatioPosT, 2)+ " -> "+ dbl2str(lMaxRatioTS, 2)+ ", "+
		        dbl2str(lRatioNegT, 2)+ " -> "+ dbl2str(1.0- lMaxRatioTS, 2)+ ".");
    		lRatioPosT= lMaxRatioTS;
    		lRatioNegT= 1.0- lMaxRatioTS;    	
    	}
    } 
    else
    {
    	// More positives than negatives.
    	if (lRatioNegT < lMinRatioTS)
    	{
    		// The ratio DS is smaller than the min ratio TS.
		    Beagle_LogDetailedM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
		        "Training set: "+ dbl2str(lRatioNegT, 2)+ " -> "+ dbl2str(lMinRatioTS, 2)+ ", "+
		        dbl2str(lRatioPosT, 2)+ " -> "+ dbl2str(1.0- lMinRatioTS, 2)+ ".");
    		lRatioNegT= lMinRatioTS;
    		lRatioPosT= 1.0- lMinRatioTS;
    	}
    	else if (lRatioNegT > lMaxRatioTS)
    	{
    		// The ratio DS is larger than the max ratio TS.
		    Beagle_LogDetailedM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
		        "Training set: "+ dbl2str(lRatioNegT, 2)+ " -> "+ dbl2str(lMaxRatioTS, 2)+ ", "+
		        dbl2str(lRatioPosT, 2)+ " -> "+ dbl2str(1.0- lMaxRatioTS, 2)+ ".");
    		lRatioNegT= lMaxRatioTS;
    		lRatioPosT= 1.0- lMaxRatioTS;    	
    	}    	
    }
    int lSizePosT= round(lSizeT* lRatioPosT);
    int lSizeNegT= round(lSizeT* lRatioNegT);
    Beagle_LogInfoM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
        "Training set: "+ int2str(lSizePosT)+ "("+ dbl2str(lRatioPosT*100, 2)+ " %) positive, "+ 
        int2str(lSizeNegT)+ "("+ dbl2str(lRatioNegT*100, 2)+ " %) negative.");
	
    // Abort, if (|TS| + |TL|) < |T|.
    if (lSizePosT+ lSizeNegT < lSizeT)
    {
        throw Beagle_RunTimeExceptionM(
            "The data set contains too few samples to construct a training set of size "+ int2str(lSizeT));
    }
    
    //the training set is too small or too many samples of one class have been selected.
    // Do not allow to put more than lMaxPercentageSmallerSample % of the smaller sample into the training set.
//    float lMaxPercentageSmallerSample= 0.7;
//    if (lNrSamples < mTrainingSetSize || 
//        lNrSamplesPositive > (lIndexesPositives->size()* lMaxPercentageSmallerSample) || 
//        lNrSamplesPositive > (lIndexesNegatives->size()* lMaxPercentageSmallerSample))
//    {
//        std::string lStringSmaller= "positives";
//        int l10Percent= lNrSamplesPositive;
//        int lMaxOfSmallerSamples= lIndexesPositives->size()* lMaxPercentageSmallerSample;
//        if (lPercentageNegativesTrainingSet < lPercentagePositivesTrainingSet)
//        {
//            lStringSmaller= "negatives";
//            l10Percent= lNrSamplesNegative;
//            lMaxOfSmallerSamples= lIndexesNegatives->size()* lMaxPercentageSmallerSample;
//        }
//        throw Beagle_RunTimeExceptionM(
//            "The data set contains too few samples to construct a training set of size "+
//            int2str(mTrainingSetSize)+ ", containing "+ dbl2str(lPercentageSmallerSample* 100)+ 
//            "% ("+ int2str(l10Percent)+ " samples) "+ lStringSmaller+ " and no more than "+ 
//            dbl2str(lMaxPercentageSmallerSample)+ " % of all "+ lStringSmaller);
//    }

	{
	    lSizePosT= lSizeT* lRatioPosD;
		Register::Description lDescription(
		    "Number of positive samples in training set",
		    "Int",
		    "0",
		    "The number of positive smaples from the data set to go into the training set."
		);
		ioContext.getSystem().getRegister().insertEntry("icu.trainingset.size-pos", new Int(lSizePosT), lDescription);
	}

	{
	    lSizeNegT= lSizeT* lRatioNegD;
		Register::Description lDescription(
		    "Number of negative samples in training set",
		    "Int",
		    "0",
		    "The number of negative smaples from the data set to go into the training set."
		);
		ioContext.getSystem().getRegister().insertEntry("icu.trainingset.size-neg", new Int(lSizeNegT), lDescription);
	}

    Beagle_LogInfoM(ioContext.getSystem().getLogger(), "operate", "Beagle::GP::TrainingSetSamplingOp", 
        "Training set: "+ int2str(lSizePosT)+ " positive, "+ int2str(lSizeNegT)+ " negative.");

}

