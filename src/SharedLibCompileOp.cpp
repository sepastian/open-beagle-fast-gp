#include "SharedLibCompileOp.hpp"

using namespace Beagle;
using namespace GP;

/*!
 *
 */
SharedLibCompileOp::SharedLibCompileOp() : 
    Operator("SharedLibCompileOp")
{
}

/*!
 *
 */
SharedLibCompileOp::~SharedLibCompileOp()
{
}

/*!
 *  \brief Compile a shared library for evaluating the fitness of each individual in deme specified.
 *  \param ioDeme Reference to the deme for which to compile a shared library.
 *  \param ioContext Evolutionary context of the operation.
 *
 */
void SharedLibCompileOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{  
    Beagle_StackTraceBeginM();

	std::string lPathLib;

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
    for(Beagle::Deme::const_iterator lIndividual=ioDeme.begin(); lIndividual!=ioDeme.end(); ++lIndividual)
    {
        Beagle::GP::Individual::Handle lGPIndividual= castHandleT<Beagle::GP::Individual>(*lIndividual);
		lSharedLibCompiler.addIndividual(*lGPIndividual, lContext.getGeneration(), lContext.getDemeIndex(), lIndividual- ioDeme.begin());
	}
	std::ostringstream lLibName;
	lLibName << "g" << lContext.getGeneration() << "_d" << lContext.getDemeIndex();
	lPathLib= lSharedLibCompiler.compile(lLibName.str());

    // Update register with the path of the newly compiled library.
    lContext.getSystem().getRegister().modifyEntry("icu.compiler.lib-path", new String(lPathLib));

    Beagle_StackTraceEndM("void SharedLibCompileOp::operate(Deme& ioDeme, Context& ioContext)");
}

/*!
 *
 */
void SharedLibCompileOp::init(Beagle::System& ioSystem)
{
    Beagle_StackTraceBeginM();

    Beagle::Operator::init(ioSystem);
	
    Beagle_StackTraceEndM("void SharedLibCompileOp::init(System& ioSystem)");
}

/*!
 *
 */
void SharedLibCompileOp::registerParams(Beagle::System& ioSystem)
{
    Beagle_StackTraceBeginM();

    Beagle::Operator::registerParams(ioSystem);

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

    // 'icu.compiler.lib-path', register an entry updated with the path of the shared library after each compilation.
    {
		std::ostringstream lOSS;
		lOSS << "Path to the shared library to use for fitness evaluation. ";
		lOSS << "After each compilation step, this paramter will contain the ";
		lOSS << "path to the shared library for the current generation and deme.";
		Register::Description lDescription(
		    "Path to shared library for current generation and deme",
		    "String",
		    "",
		    lOSS.str()
		);
        ioSystem.getRegister().insertEntry("icu.compiler.lib-path", new String(""), lDescription);
    }
        
    Beagle_StackTraceEndM("void SharedLibCompileOp::registerParams(System&)");
}

