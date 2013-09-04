#include "beagle/GP.hpp"
#include "EphemeralPercent.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new EphemeralPercent primitive.
 *  \param inValue Value of the ephemeral.
 */
EphemeralPercent::EphemeralPercent(Beagle::Double::Handle inValue) :
		GP::EphemeralT<Double>(inValue, "EPR")
{ }


/*!
 *  \brief  Generate a new random ephemeral Double constant in [0,100].
 *  \param  inName Name of the constant.
 *  \param  ioContext Context to use to generate the value.
 *  \return Handle to the ephemeral Double constant generated.
 */
GP::Primitive::Handle EphemeralPercent::generate(std::string inName, GP::Context& ioContext)
{
	Double::Handle lValue = new Double(ioContext.getSystem().getRandomizer().rollUniform(0.,100.));
	return new EphemeralPercent(lValue);
}


/*!
 *  \brief Write EphemeralPercent as C macro.
 * 
 *  The standard implementation of deparse in Beagle::GP::Primitive
 *  writes out the element name followed by all arguments (sub-trees) of
 *  the element in braces, e.g. ADD(SUB(5,4),2).
 * 
 *  This does not work for EphemeralPercent, since no sub-trees are maintained,
 *  but the actual value is stored in mValue of EphemeralT, the base class. The 
 *  implementation of Primitive::deparse would, falsely, produce 'EPR()' for any
 *  EphemeralPercent, instead of 'EPR(VALUE)'.
 *
 *  This method overwrites Primitive::deparse to output 'EPR(EphemeralT::mValue)'
 *  instead of 'EPR()'.
 *
 */
std::string EphemeralPercent::deparse(std::vector<std::string>& lSubTrees) const
{
	std::ostringstream lOSS;
	lOSS << getName() << "(" << *(this->mValue) << ")";
	return lOSS.str();
}

