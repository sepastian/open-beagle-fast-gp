#ifndef Beagle_GP_TokenDeparserT_hpp
#define Beagle_GP_TokenDeparserT_hpp

#include "beagle/GP.hpp"

#ifdef BEAGLE_HAVE_RTTI
#include <typeinfo>
#endif // BEAGLE_HAVE_RTTI


namespace Beagle
{
namespace GP
{

/*!
 *  \class TokenDeparserT beagle/GP/TokenDeparserT.hpp "beagle/GP/TokenDeparserT.hpp"
 *  \brief GP simple token terminal primitive class.
 *  \param T Type of the token contained.
 *  \ingroup GPF
 *  \ingroup Primit
 *  \ingroup FunctPrimit
 */
template <class T>
class TokenDeparserT : public TokenT<T>
{

public:

	//! GP::TokenDeparserT allocator type.
	typedef AllocatorT<TokenDeparserT<T>, typename TokenT<T>::Alloc>
	Alloc;
	//! GP::TokenDeparserT handle type.
	typedef PointerT<TokenDeparserT<T>, typename TokenT<T>::Handle>
	Handle;
	//! GP::TokenDeparserT bag type.
	typedef ContainerT<TokenDeparserT<T>, typename TokenT<T>::Bag>
	Bag;

	explicit TokenDeparserT(std::string inName="TOKEN");
	explicit TokenDeparserT(std::string inName, const T& inToken);
	virtual ~TokenDeparserT()
	{ }

#ifdef BEAGLE_HAVE_RTTI
	virtual const std::type_info* getReturnType(GP::Context& ioContext) const;
#endif // BEAGLE_HAVE_RTTI

	virtual std::string deparse(std::vector<std::string>& lSubTrees) const;

};

}
}

/*!
 *  \brief Construct a new token terminal primitive.
 *  \param inName Name of the token terminal primitive.
 */
template <class T>
Beagle::GP::TokenDeparserT<T>::TokenDeparserT(std::string inName) :
		Beagle::GP::TokenT<T>(inName)
{ }


/*!
 *  \brief Construct a new token terminal primitive.
 *  \param inName Name of the token terminal primitive.
 *  \param inToken Initial value of the token terminal.
 */
template <class T>
Beagle::GP::TokenDeparserT<T>::TokenDeparserT(std::string inName, const T& inToken) :
    Beagle::GP::TokenT<T>(inName)
{
    this.mToken= inToken;
}

/*!
 *  \brief Write this primitive as a C function
 */
template <class T>
std::string Beagle::GP::TokenDeparserT<T>::deparse(std::vector<std::string>& lSubTrees) const
{
//	return mToken.serialize();
    return this->getName();
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief  Return the tag of the type of data return by primitive TokenDeparserT.
 *  \param  ioContext Evolutionary context.
 *  \return Type_info (RTTI) tagging the data type returned.
 */
template <class T>
const std::type_info* Beagle::GP::TokenDeparserT<T>::getReturnType(Beagle::GP::Context& ioContext) const
{
	Beagle_StackTraceBeginM();
	return &typeid(T);
	Beagle_StackTraceEndM("const std::type_info* GP::TokenDeparserT<T>::getReturnType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI

#endif // Beagle_GP_TokenDeparserT_hpp
