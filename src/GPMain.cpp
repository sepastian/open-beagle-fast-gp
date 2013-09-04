#include "beagle/GP.hpp"
#include "SharedLibCompileOp.hpp"
#include "HOFSharedLibCompileOp.hpp"
#include "TrainingSetSamplingOp.hpp"
#include "SharedLibEvalOp.hpp"
#include "DataSetBinaryClassification.hpp"
#include "LessThan.hpp"
#include "EqualTo.hpp"
#include "IfThenElse.hpp"
#include "EphemeralPercent.hpp"
#include "TokenDeparserT.hpp"

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <typeinfo>

using namespace std;
using namespace Beagle;


/*!
 *  \brief Main routine for the function spambase problem.
 *  \param argc Number of arguments on the command-line.
 *  \param argv Arguments on the command-line.
 *  \return Return value of the program.
 *  \ingroup Spambase
 */
int main(int argc, char *argv[])
{
	try {

		// Build a system.
		System::Handle lSystem = new System();

    // Register allocators and aliases for custom types.
    Factory& lFactory = lSystem->getFactory();
    lFactory.insertAllocator("Beagle::GP::FitnessMCC", new GP::FitnessMCC::Alloc);
    lFactory.insertAllocator("Beagle::GP::StatsCalcFitnessMCCOp", new GP::StatsCalcFitnessMCCOp::Alloc);
    lFactory.insertAllocator("Beagle::GP::SharedLibCompileOp", new GP::SharedLibCompileOp::Alloc);
    lFactory.insertAllocator("Beagle::GP::HOFSharedLibCompileOp", new GP::HOFSharedLibCompileOp::Alloc);
    lFactory.insertAllocator("Beagle::GP::TrainingSetSamplingOp", new GP::TrainingSetSamplingOp::Alloc);
    lFactory.aliasAllocator("Beagle::GP::FitnessMCC", "GP-FitnessMCC");
    lFactory.aliasAllocator("Beagle::GP::StatsCalcFitnessMCCOp", "GP-StatsCalcFitnessMCCOp");
    lFactory.aliasAllocator("Beagle::GP::SharedLibCompileOp", "GP-SharedLibCompileOp");
    lFactory.aliasAllocator("Beagle::GP::HOFSharedLibCompileOp", "GP-HOFSharedLibCompileOp");
    lFactory.aliasAllocator("Beagle::GP::TrainingSetSamplingOp", "GP-TrainingSetSamplingOp");

		// Register parameter "icu.dataset.path", the file holding training data.
    Register::Description lDescription(
        "Path to data file", "String", "",
        "Path to the data file used in training."
    );
		lSystem->getRegister().insertEntry(std::string("icu.dataset.path"), new String(""), lDescription);

		// Register parameter "icu.dataset.rows", the number of rows in the data file.
    lDescription.mBrief=        "Number of rows in data file";
    lDescription.mType=         "Integer";
    lDescription.mDescription=  "The number of rows in the data file.";
    lDescription.mDefaultValue= "";
		lSystem->getRegister().insertEntry(std::string("icu.dataset.rows"), new String(""), lDescription);

		// Register parameter "icu.dataset.columns", the number of columns in the data file.
    lDescription.mBrief=        "Number of columns in data file";
    lDescription.mDescription=  "The number of columns each line in the data file consists of";
		lSystem->getRegister().insertEntry(std::string("icu.dataset.columns"), new String(""), lDescription);

		// Add constrained GP package
		GP::PrimitiveSet::Handle lSet = new GP::PrimitiveSet(&typeid(Bool));
		lSystem->addPackage(new GP::PackageConstrained(lSet));

		// Set fitness evaluation operator
		lSystem->setEvaluationOp("GP-SharedLibEvalOp", new GP::SharedLibEvalOp::Alloc);

		// Initialize the evolver
		Evolver::Handle lEvolver = new Evolver;
		lEvolver->initialize(lSystem, argc, argv);

		/* Read data set.
		 * If a CSV file has been specified through 'icu.dataset.path', read that file.
		 * Otherwise, read from STDIN.
		 */
		DataSetBinaryClassification::Handle lDataSet = new DataSetBinaryClassification("DataSet");
		std::string lPath= castHandleT<String>(lSystem->getRegister().getEntry("icu.dataset.path"))->getWrappedValue();
		if (lPath.size() > 0)
		{
			// Read CSV data from the file specified through parameter 'icu.dataset.path'.
			std::ifstream lIFS(lPath.c_str());
			if(!lIFS) 
			{
				std::ostringstream lMessage;
				lMessage << "Could not open spambase data file '" << lPath.c_str() << "'!";
				throw Beagle_RunTimeExceptionM(lMessage.str());
			}

		    Beagle_LogBasicM(lSystem->getLogger(), "main", "GPMain", "Reading data from "+ lPath);

			lDataSet->readCSV(lIFS);
			lIFS.close();
		}
		else
		{
  		// Read CSV data from STDIN.
      Beagle_LogVerboseM(lSystem->getLogger(), "main", "GPMain", "Waiting for CSV data on STDIN.");
			lDataSet->readCSV(cin);
		}
		lSystem->addComponent(lDataSet);
        Beagle_LogInfoM(lSystem->getLogger(), "main", "GPMain", int2str(lDataSet->getIndexesPositives()->size())+ " positive samples.");
        Beagle_LogInfoM(lSystem->getLogger(), "main", "GPMain", int2str(lDataSet->getIndexesNegatives()->size())+ " negative samples.");

		// Build primitives
		lSet->insert(new GP::And);
		lSet->insert(new GP::Or);
		lSet->insert(new GP::Not);
		lSet->insert(new GP::Add);
		lSet->insert(new GP::Subtract);
		lSet->insert(new GP::Multiply);
		lSet->insert(new GP::Divide);
    lSet->insert(new GP::Sin);
    lSet->insert(new GP::Cos);
    lSet->insert(new GP::Exp);
    lSet->insert(new GP::Log);
		lSet->insert(new LessThan);
		lSet->insert(new EqualTo);
		lSet->insert(new IfThenElse);
		lSet->insert(new EphemeralPercent);

		lSet->insert(new GP::Nor);
		lSet->insert(new GP::Nand);
		lSet->insert(new GP::Xor);

		lSet->insert(new GP::TokenT<Bool>("FALSE", Bool(false)));
		lSet->insert(new GP::TokenT<Bool>("TRUE", Bool(true)));

		/*
		 * Dynamically add primitives representing the columns of the data file.
		 * 
		 * Determine the number of columns inspecting the
		 * first element in lDataSet created above. Each element in lDataSet represents one line
		 * and consists of a pair <int, std::vector>,
		 * where the first element contains the target value of the line, and the second
		 * element contains all fields. Thus, the size of the second element equals the 
		 * number of columns minus one (target column) -- the value sought after here.
		 *
		 * For each column c in the data file, add a TokenDeparserT<Double> named "INc",
		 * where c is the zero-based column index. TokenDeparserT is a subclass of TokenT
		 * overwritting the TokenT::deparse method provided by OpenBEAGLE; the original method
		 * outputs the value of the data column in the line currently set; TokenDeparserT::deparse
		 * outputs the name of the Token, e.g. IN0, which, in turn can be translated into in[0]
		 * by a C macro; this way, code for evaluating any float[] instead of hard-coded values
		 * can be generated.
		 */
		int lNumberOfRows= lDataSet->size();
        int lNumberOfColumns= (*lDataSet)[0].second.size();
        std::ostringstream lOSS;
        lOSS << "Data set: " << lNumberOfRows << " rows, " << lNumberOfColumns << " columns per row.";
        Beagle_LogInfoM(lSystem->getLogger(), "main", "GPMain", lOSS.str());
        for(unsigned int i=0; i<lNumberOfColumns; ++i)
        {
        	std::ostringstream lOSS;
        	lOSS << "IN" << i;
        	lSet->insert(new GP::TokenDeparserT<Double>(lOSS.str()));
        }
        // Make sure, all rows contain the same number of columns.
		for(unsigned int i=0; i<lDataSet->size(); ++i) {
			if ((*lDataSet)[i].second.size() != lNumberOfColumns)
			{
			    throw Beagle_RunTimeExceptionM(
			    	"Row "+ int2str(i+ 1)+ ": wrong number of columns, expected "+ 
			    	int2str(lNumberOfColumns)+ ", got "+ int2str((*lDataSet)[i].second.size())+ ".");
			}
		}		

        // Make number of rows/columns available in the register.
        lSystem->getRegister().modifyEntry("icu.dataset.rows", new Int(lNumberOfRows));
        lSystem->getRegister().modifyEntry("icu.dataset.columns", new Int(lNumberOfColumns));

		// Create population
		Vivarium::Handle lVivarium = new Vivarium;

		// Launch evolution
		lEvolver->evolve(lVivarium, lSystem);

	} catch(Exception& inException) {
		inException.terminate();
	} catch(exception& inException) {
		cerr << "Standard exception catched:" << endl;
		cerr << inException.what() << endl << flush;
		return 1;
	} catch(...) {
		cerr << "Unknown exception catched!" << endl << flush;
		return 1;
	}
	return 0;
}

