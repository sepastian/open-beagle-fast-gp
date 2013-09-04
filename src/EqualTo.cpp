#include "beagle/GP.hpp"
#include "EqualTo.hpp"

#include <cmath>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a new EqualTo primitive.
 */
EqualTo::EqualTo() :
		Beagle::GP::Primitive(2, "EQ")
{ }


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Return the Double tag needed as input for the primitive EqualTo.
 *  \param inN Index of the argument to get the tag.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info of the data type Double.
 *  \throw AssertException If the index inN given is > to 1.
 */
const std::type_info* EqualTo::getArgType(unsigned int inN, Beagle::GP::Context& ioContext) const
{
	Beagle_AssertM(inN < 2);
	return &typeid(Double);
}


/*!
 *  \brief Return the string tagging the Bool type returned but the primitive EqualTo.
 *  \param ioContext Evolutionary context.
 *  \return RTTI type_info of the Bool type returned.
 */
const std::type_info* EqualTo::getReturnType(Beagle::GP::Context& ioContext) const
{
	return &typeid(Bool);
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Test the equality between two Double, returning a Bool.
 *  \param outDatum Result of the test.
 *  \param ioContext Evolutionary context.
 */
void EqualTo::execute(GP::Datum& outDatum, GP::Context& ioContext)
{
	Bool& lResult = castObjectT<Bool&>(outDatum);
	Double lArgs[2];
	getArguments(lArgs, sizeof(Double), ioContext);
	lResult = lArgs[0].getWrappedValue() == lArgs[1].getWrappedValue();
}




