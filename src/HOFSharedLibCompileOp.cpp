#include <cstring>
#include <cstdio>
#include <fstream>
#include "HOFSharedLibCompileOp.hpp"
#include "SharedLibCompiler.hpp"

using namespace Beagle;
using namespace GP;

/*!
 *  \brief Construct a milestone writer operator.
 */
HOFSharedLibCompileOp::HOFSharedLibCompileOp() :
		Operator("HOFSharedLibCompileOp")
{ }


/*!
 *  \brief Register the parameters of this operator.
 *  \param ioSystem System to use to initialize the operator.
 */
void HOFSharedLibCompileOp::registerParams(Beagle::System& ioSystem)
{
	Beagle_StackTraceBeginM();
	Operator::registerParams(ioSystem);

    // 'icu.compiler.tmp-directory', place all files generated during compiling in this directory.
    {
		std::ostringstream lOSS;
		lOSS << "Place all files generated during compilation in this directory, ";
		lOSS << "e.g. C source files (*.c) and shared libraries (*.so).";
		Register::Description lDescription(
		    "Directory for temporary files",
		    "String",
		    "./tmp",
		    lOSS.str()
		);
        ioSystem.getRegister().insertEntry("icu.compiler.tmp-directory", new String("./tmp"), lDescription);
    }

	Beagle_StackTraceEndM("void HOFSharedLibCompileOp::registerParams(Beagle::System&)");
}


/*!
 *  \brief Apply the characteristic operation.
 *  \param ioDeme
 *  \param ioContext Context of the evolution.
 */
void HOFSharedLibCompileOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
	Beagle_StackTraceBeginM();
	
    /* The abstract class Operator is defined in the namespace Beagle.
     * Thus, the Context passed in is from Beagle as well.
     * However, Beagle::GP::Individual::deparse requires a Beagle::GP::Context.
     * Cast the Beagle::Context passed in to a Beagle::GP::Context.
     */
    Beagle::GP::Context lContext= Beagle::castObjectT<Beagle::GP::Context&>(ioContext);

	// Create a SharedLibCompiler.
    int lNrColumns= castHandleT<Int>(ioContext.getSystem().getRegister()["icu.dataset.columns"])->getWrappedValue();
    std::string lTmpDirectory= castHandleT<String>(ioContext.getSystem().getRegister()["icu.compiler.tmp-directory"])->getWrappedValue();
    SharedLibCompiler lSharedLibCompiler(lNrColumns, lTmpDirectory);
    
    // Add individuals, compile.
	std::string lPathLib;
	int lIndividualIndex= 0;
//    for(Beagle::MemberMap::const_iterator lEntry=ioContext.getVivarium().getHallOfFame().begin(); lIndividual!=ioContext.getVivarium().getHallOfFame().end(); ++lEntry)
    while (lIndividualIndex < lContext.getVivarium().getHallOfFame()->size())
    {
    	Beagle::HallOfFame::Entry lEntry= (*lContext.getVivarium().getHallOfFame())[lIndividualIndex];
        Beagle::GP::Individual::Handle lGPIndividual= castHandleT<Beagle::GP::Individual>(lEntry.mIndividual);
		lSharedLibCompiler.addIndividual(*lGPIndividual, lEntry.mGeneration, lEntry.mDemeIndex, lIndividualIndex);
		lIndividualIndex++;
	}
	std::ostringstream lLibName;
	lLibName << "hof_g" << lContext.getGeneration() << "_d" << lContext.getDemeIndex();
	lPathLib= lSharedLibCompiler.compile(lLibName.str());

	Beagle_StackTraceEndM("void HOFSharedLibCompileOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)");
}

