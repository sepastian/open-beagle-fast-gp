#include <cmath>
#include <fstream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "SharedLibEvalOp.hpp"
#include "DataSetBinaryClassification.hpp"

using namespace Beagle;
using namespace GP;

/*!
 *  \brief Construct a new evaluation operator for shared libraries.
 */
SharedLibEvalOp::SharedLibEvalOp() :
  Beagle::GP::EvaluationOp("SharedLibEvalOp")
{
  mSharedLibHandle = NULL;
}

/*!
 *  \brief Destroy this evaluation operator for shared libraries.
 */
SharedLibEvalOp::~SharedLibEvalOp()
{
    if (this->mSharedLibHandle)
    {
        dlclose(this->mSharedLibHandle);
    }
}


/*!
 *  \brief Evaluate the individual fitness.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure.
 */
Fitness::Handle SharedLibEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
    Beagle_StackTraceBeginM();

    std::ostringstream lOSS;
    lOSS << "Evaluating individual " << ioContext.getIndividualIndex() << ", deme ";
    lOSS << ioContext.getDemeIndex() << ", generation " << ioContext.getGeneration(); 
    Beagle_LogDebugM(ioContext.getSystem().getLogger(), "evaluate", "Beagle::GP::SpamebaseEvalOp", lOSS.str());

    GP::PrimitiveSuperSet::Handle lPrimitiveSuperSet= 
        castHandleT<GP::PrimitiveSuperSet>(ioContext.getSystem().getComponent("GP-PrimitiveSuperSet"));
    GP::PrimitiveSet::Handle lPrimitiveSet= (*lPrimitiveSuperSet)[0];

    // Get a handle on the shared library used for evaluation.
    // TODO: Open shared lib only once per generation!
    this->mTimer.reset();
    if (this->mSharedLibHandle)
    {
        dlclose(this->mSharedLibHandle);
        Beagle_LogDebugM(
            ioContext.getSystem().getLogger(), "evaluate", "Beagle::GP::SpamebaseEvalOp", 
            "Closed previously opened shared lib.");
    }
    if (!ioContext.getSystem().getRegister().isRegistered("icu.compiler.lib-path"))
    {
        throw Beagle_RunTimeExceptionM("Parameter icu.compiler.lib-path not found in registry; make sure to apply SharedLibCompilerOp before applying SharedLibEvalOp.");    
    }
    std::string lLibName= castHandleT<String>(ioContext.getSystem().getRegister()["icu.compiler.lib-path"])->getWrappedValue();
    this->mSharedLibHandle= dlopen((const char*)lLibName.c_str(), RTLD_LAZY);
    if (!this->mSharedLibHandle) 
    {
        throw Beagle_RunTimeExceptionM("Cannot open shared library "+ lLibName+ ": "+ dlerror()+ ".");
    }
    dlerror();

    // Draw a sample from the data set to construct the training set.
    
    this->mTimer.reset();
	// Get a handle on D, S, and L.
    DataSetBinaryClassification::Handle lDataSet= castHandleT<DataSetBinaryClassification>(ioContext.getSystem().getComponent("DataSet"));
    std::vector<unsigned int>* lIndexesPositives= lDataSet->getIndexesPositives();
    std::vector<unsigned int>* lIndexesNegatives= lDataSet->getIndexesNegatives();
    int lNrSamplesPositive= castHandleT<Int>(ioContext.getSystem().getRegister()["icu.trainingset.size-pos"])->getWrappedValue();
    int lNrSamplesNegative= castHandleT<Int>(ioContext.getSystem().getRegister()["icu.trainingset.size-neg"])->getWrappedValue();
    
    // Shuffle both negative and positive indexes.
    std::random_shuffle(lIndexesPositives->begin(), lIndexesPositives->end(), ioContext.getSystem().getRandomizer());
    std::random_shuffle(lIndexesNegatives->begin(), lIndexesNegatives->end(), ioContext.getSystem().getRandomizer());
    
    // Nr of columns in each row in the dataset.
    int lNrColumns= castHandleT<Int>(ioContext.getSystem().getRegister()["icu.dataset.columns"])->getWrappedValue();
    
    double lTimeInit= this->mTimer.getValue();
    this->mTimer.reset();

    // Evaluate sampled test cases
    unsigned int lTruePositives = 0;
    unsigned int lTrueNegatives = 0;
    unsigned int lFalsePositives= 0;
    unsigned int lFalseNegatives= 0;
    int (*apply_individual)(float[])= 0;
    float lValues[lNrColumns];
    std::vector<unsigned int>::iterator lLastIndex= lIndexesPositives->begin()+ lNrSamplesPositive;


    // Positives.
    for(std::vector<unsigned int>::const_iterator lIndex=lIndexesPositives->begin(); lIndex!=lLastIndex; ++lIndex)
    {
        const Beagle::Vector& lData = (*lDataSet)[*lIndex].second;
        std::ostringstream lFunctionName;
        lFunctionName << "apply_individual_";
        lFunctionName << ioContext.getGeneration() << "_" << ioContext.getDemeIndex() << "_" << ioContext.getIndividualIndex();
        apply_individual= (int(*)(float*))dlsym(this->mSharedLibHandle, lFunctionName.str().c_str());
        char* lError= 0;
        if ((lError = dlerror()) != NULL) {
            throw Beagle_RunTimeExceptionM("Error loading function "+ lFunctionName.str()+ ": "+ lError+ ".");
        }
        for(unsigned int j=0; j<lData.size(); ++j) {
            lValues[j]= Float(lData[j]);
        }		
        bool lResult= apply_individual(lValues);
        (lResult == 1) ? lTruePositives++ : lFalseNegatives++;
    }
    // Negatives.
    lLastIndex= lIndexesNegatives->begin()+ lNrSamplesNegative;
    for(std::vector<unsigned int>::const_iterator lIndex=lIndexesNegatives->begin(); lIndex!=lLastIndex; ++lIndex)
    {
        const Beagle::Vector& lData = (*lDataSet)[*lIndex].second;
        std::ostringstream lFunctionName;
        lFunctionName << "apply_individual_";
        lFunctionName << ioContext.getGeneration() << "_" << ioContext.getDemeIndex() << "_" << ioContext.getIndividualIndex();
        apply_individual= (int(*)(float*))dlsym(this->mSharedLibHandle, lFunctionName.str().c_str());
        char* lError= 0;
        if ((lError = dlerror()) != NULL) {
            throw Beagle_RunTimeExceptionM("Error loading function "+ lFunctionName.str()+ ": "+ lError+ ".");
        }
        for(unsigned int j=0; j<lData.size(); ++j) {
            lValues[j]= Float(lData[j]);
        }		
        bool lResult= apply_individual(lValues);
        (lResult == 0) ? lTrueNegatives++ : lFalsePositives++;
    }


    double lTimeEvaluate= this->mTimer.getValue();

    {
        using namespace std;
        ostringstream lOSS;
        lOSS << "g" << ioContext.getGeneration();
        lOSS << " d" << ioContext.getDemeIndex();
        lOSS << " i" << ioContext.getIndividualIndex() << ", ";
        lOSS << "TP|FP|FN|TN = ";
        lOSS.width(7);
        lOSS << right << lTruePositives << "|";
        lOSS.width(7);
        lOSS << right << lFalsePositives << "|";
        lOSS.width(7);
        lOSS << right << lFalseNegatives << "|";
        lOSS.width(7);
        lOSS << right << lTrueNegatives;
        Beagle_LogDetailedM(ioContext.getSystem().getLogger(), "evaluate", "Beagle::GP::SharedLibEvalOp", lOSS.str());
    }

    GP::FitnessMCC* fitness= new GP::FitnessMCC(lTruePositives, lFalsePositives, lTrueNegatives, lFalseNegatives);

    return fitness;

    Beagle_StackTraceEndM("SharedLibEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)");
}

/*!
 *
 */
void SharedLibEvalOp::init(Beagle::System& ioSystem)
{
    Beagle_StackTraceBeginM();

    Beagle::GP::EvaluationOp::init(ioSystem);
    
    std::ostringstream lOSS;
    lOSS << "Training set size: " << mTrainingSetSize;
	Beagle_LogInfoM(ioSystem.getLogger(), "init", "Beagle::GP::SharedLibEvalOp", lOSS.str());
	    
    Beagle_StackTraceEndM("void SharedLibEvalOp::init(System& ioSystem)");
}


/*!
 *
 */
void SharedLibEvalOp::registerParams(Beagle::System& ioSystem)
{
    Beagle_StackTraceBeginM();

    Beagle::GP::EvaluationOp::registerParams(ioSystem);

    if (!ioSystem.getRegister().isRegistered("icu.trainingset.size"))
    {
        // 'icu.trainingset.size', training set size.
        Register::Description lDescription(
            "Training set size",
            "Integer",
            "30000",
            "The number of rows in the training set."
        );
        ioSystem.getRegister().insertEntry("icu.trainingset.size", new Int(30000), lDescription);
    }
    mTrainingSetSize= castHandleT<Int>(ioSystem.getRegister()["icu.trainingset.size"])->getWrappedValue();
    
    Beagle_StackTraceEndM("void SharedLibEvalOp::registerParams(System&)");
}

