#include "beagle/GP.hpp"
#include "IfThenElse.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new IfThenElse primitive.
 */
IfThenElse::IfThenElse() :
		Beagle::GP::Primitive(3, "IF")
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Return the type tag needed as input for the primitive (0=Bool, {1,2}=Double).
 *  \param inN Index of the argument to get the tag.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info tagging the data type.
 *  \throw AssertException If the index inN given is > to 2.
 */
const std::type_info* IfThenElse::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
	Beagle_AssertM(inN < 3);
	if(inN == 0) return &typeid(Bool);
	return &typeid(Double);
}


/*!
 *  \brief Return the string tagging the Double type returned but the primitive IfThenElse.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info tagging the Double type returned.
 */
const std::type_info* IfThenElse::getReturnType(Beagle::GP::Context& ioContext) const
{
	return &typeid(Double);
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Depending the first argument value, return the 2nd (true) or the 3rd (false).
 *  \param outDatum Result of the comparison.
 *  \param ioContext Evolutionary context.
 */
void IfThenElse::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Bool lCondition;
	get1stArgument(lCondition, ioContext);
	if(lCondition == true) get2ndArgument(outDatum, ioContext);
	else get3rdArgument(outDatum, ioContext);
}




