#include "beagle/GP.hpp"
#include "LessThan.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new LessThan primitive.
 */
LessThan::LessThan() :
		Beagle::GP::Primitive(2, "LT")
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Return the Double tag needed as input for the primitive LessThan.
 *  \param inN Index of the argument to get the tag.
 *  \param ioContext Evolutionary context.
 *  \return RTTO type_info tagging the data type Double.
 *  \throw AssertException If the index inN given is > to 1.
 */
const std::type_info* LessThan::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
	Beagle_AssertM(inN < 2);
	return &typeid(Double);
}


/*!
 *  \brief Return the string tagging the Bool type returned but the primitive LessThan.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info tagging the Bool type returned.
 */
const std::type_info* LessThan::getReturnType(Beagle::GP::Context& ioContext) const
{

	return &typeid(Bool);
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Execute the comparison operation between two Double, returning a Bool.
 *  \param outDatum Result of the comparison.
 *  \param ioContext Evolutionary context.
 */
void LessThan::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Bool& lResult = castObjectT<Bool&>(outDatum);
	Double lArgs[2];
	getArguments(lArgs, sizeof(Double), ioContext);
	lResult = lArgs[0].getWrappedValue() < lArgs[1].getWrappedValue();
}




