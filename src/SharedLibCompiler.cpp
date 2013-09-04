#include "SharedLibCompiler.hpp"
#include "beagle/FitnessSimple.hpp"
#include "FitnessMCC.hpp"

/*!
 * ioTmpDirectory  The directory in which all files generated will be placed.
 * iNrColumns      The number of columns in the dataset.
 *                 A macro for accessing each column in the dataset must be generated dynamically.
 */
SharedLibCompiler::SharedLibCompiler(int iNrColumns, std::string iTmpDirectory) :
    mTmpDirectory(iTmpDirectory)
{
    mNrColumns= iNrColumns;
}

/*!
 * Add an individual to the library to compile.
 *
 * A method representing ioIndividual will be created with the signature
 *
 *   apply_individual_GENERATEION_DEME_INDIVIDUAL(float in[])
 * 
 * where GENERATION, DEME, and INDIVIDUAL are the indexes
 * of the generation, deme, and individual, respectively.
 * The indexes are extracted from ioContext.
 * 
 * Deparsing Individuals
 * ---------------------
 * In Open Beagle, a GP::Individual is composed of a hierarchy of GP::Tree objects.
 * The root node (GP::Individual[0]) is the main function of the program represented by the individual.
 * Leaf nodes are only used when using ADFs (automatically defined functions).
 * The method GP::Individual::deparse deparses the root node and all subnodes.
 * The code generated consists is wrapped in functions, which is not desireable.
 * Only the code without function declarations is needed, thus, call the root nodes
 * deparse method GP::Individual[0].deparse, defined in GP::Tree directly, insted of calling 
 * GP::Individual.deparse.
 *
 */
int SharedLibCompiler::addIndividual(Beagle::GP::Individual& ioIndividual, int iGeneration, int iDemeIndex, int iIndividualIndex)
{
    std::ostringstream lCode;
    lCode << "// Generation " << iGeneration << ", deme " << iDemeIndex;
    lCode << ", individual " << iIndividualIndex << std::endl;
    if (ioIndividual.getFitness() != NULL && ioIndividual.getFitness()->isValid())
    {
        Beagle::FitnessSimple::Handle lFitness= Beagle::castHandleT<Beagle::FitnessSimple>(ioIndividual.getFitness());
        lCode << "// " << lFitness->getType() << ": " << lFitness->getValue() << std::endl;
// FIXME: all values (tp/fp/fn/tp) are always 0! problem with casting?
//        if (lFitness->getType() == "GP-FitnessMCC")
//        {
//            Beagle::GP::FitnessMCC::Handle lMCC= Beagle::castHandleT<Beagle::GP::FitnessMCC>(ioIndividual.getFitness());
//            lCode << "// tp/fp/fn/tn: " << lMCC->getTruePositives() << "/" << lMCC->getFalsePositives();
//            lCode << "/" << lMCC->getFalseNegatives() << "/" << lMCC->getTrueNegatives() << std::endl;
//        }
    }
    lCode << "int apply_individual_";
    lCode << iGeneration << "_" << iDemeIndex << "_" << iIndividualIndex << "(float in[])" << std::endl;
    lCode << "{" << std::endl;
    lCode << "    return " << ioIndividual[0]->deparse() << ";" << std::endl;
    lCode << "}" << std::endl;

    mIndividuals.push_back(lCode.str());
    
    return mIndividuals.size();
}

/*!
 * ioLibName       The name of the library to compile.
 *                 E.g. "g0_d0" for deme 0 in generation 0.
 *                 The source file will be named <ioLibName>.c
 *                 The library will be named lib<ioLibName>.so.
 *                 All files will be generated in ioTmpDirectory.
 *
 */
std::string SharedLibCompiler::compile(std::string iLibName)
{
    std::ostringstream lPathSource;
    std::ostringstream lPathLib;
    std::ostringstream lCompile;

    lPathSource << mTmpDirectory << "/" << iLibName << ".c";
    lPathLib << mTmpDirectory << "/lib" << iLibName << ".so";
    lCompile << "gcc -shared -nostartfiles -lm -o " << lPathLib.str() << " " << lPathSource.str();

    // Open a file for writting out the C code generated.
    std::ofstream lOFS(lPathSource.str().c_str());

    // Write macros for translating Beagle::GP::Primitives into C functions.
    // Macros defining arithmetic and logical operations do not change -- include ./macros.h.
    // Macros accessing fields in the dataset must be generated dynamically.
    // FIXME: this assumes that tmpDirectory is ./tmp!!
    lOFS << "#include \"math.h\"" << std::endl;
    lOFS << "#include \"../lib/macros.h\"" << std::endl << std::endl;
    int lColumnIndex= 0;
    while (lColumnIndex < mNrColumns)
    {
        lOFS << "#define IN" << lColumnIndex << " in[" << lColumnIndex << "]" << std::endl;
        lColumnIndex++;
    }
    lOFS << std::endl;
    
    // Append all individuals to the library.
    for(std::vector<std::string>::const_iterator lIndividual=mIndividuals.begin(); lIndividual!=mIndividuals.end(); ++lIndividual)
    {
        lOFS << *lIndividual << std::endl;
    }
        
    // Compile a shared library.
    //
    // gcc -shared -nostartfiles -o libFILE FILE.c
    // 
    // Added '-lm' to include math in linking (sin, cos, exp, log).
    // See: http://linux.die.net/man/3/dlopen
    Beagle_LogDebugM(lContext.getSystem().getLogger(), "makeSharedLib", "Beagle::GP::SpamebaseEvalOp", lCompile.str());
    if (system(lCompile.str().c_str()) != 0)
    {
        throw Beagle_RunTimeExceptionM("Error compiling shared library.");
    }
    
    // Remove all individuals.
    mIndividuals.clear();

    return lPathLib.str();
}

