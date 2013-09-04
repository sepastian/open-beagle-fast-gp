#ifndef SharedLibCompiler_hpp
#define SharedLibCompiler_hpp

#include "beagle/GP.hpp"

/*!
 *  \class SharedLibCompiler beagle/GP/SharedLibCompiler.hpp "beagle/GP/SharedLibCompiler.hpp"
 *  \brief Compile a shared library for evaluating the fitness of each individual in deme specified.
 *
 */
class SharedLibCompiler
{

public:
    
    /*!
     * iTmpDirectory  The directory in which all files generated will be placed.
     * iNrColumns     The number of columns in the dataset.
     */
    SharedLibCompiler(int iNrColumns, std::string iTmpDirectory=".");

    /*!
     *
     */
    virtual ~SharedLibCompiler()
    { }
    
    /*!
     * Add an individual to the library to compile.
     *
     * A method representing ioIndividual will be created with the signature
     *
     *   apply_individual_GENERATEION_DEME_INDEX(float v[])
     * 
     * where GENERATION, DEME, and INDIVIDUAL are the indexes
     * of the generation and deme, as extracted from the ioContext.
     * Individuals are indexed in ascending order, starting from 0.
     * 
     * ioIndividual      The individual to add.
     * iGeneration       The generation in which the individual was born.
     * iDemeIndex        The index of the deme to which the individual belongs.
     * iIndividualIndex  The index of the individual in the deme.
     *
     * Returns the number of individuals added.
     *
     */
    virtual int addIndividual(Beagle::GP::Individual& ioIndividual, int iGeneration, int iDeme, int iIndividual);

    /*!
     * iLibName        The name of the library to compile.
     *                 E.g. "g0_d0" for deme 0 in generation 0.
     *                 The source file will be named <ioLibName>.c
     *                 The library will be named lib<ioLibName>.so.
     *                 All files will be generated in ioTmpDirectory.
     *
     * Returns the path to the newly compiled library.
     *
     */
    virtual std::string compile(std::string iLibName);
    
protected:

    std::string mTmpDirectory;
    std::vector<std::string> mIndividuals;
    int mNrColumns;
    
};

#endif // SharedLibCompiler_hpp

