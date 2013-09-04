#ifndef Beagle_DataSetBinaryClassification_hpp
#define Beagle_DataSetBinaryClassification_hpp

#include "beagle/Beagle.hpp"
#include <vector>


namespace Beagle {

/*!
 *  \class DataSetBinaryClassification DataSetBinaryClassification.hpp "DataSetBinaryClassification.hpp"
 *  \brief Component of a data set useful for classification problems.
 *  \ingroup OOF
 */
class DataSetBinaryClassification : public DataSetClassification
{

public:

	//! DataSetBinaryClassification allocator type.
	typedef AllocatorT< DataSetBinaryClassification, DataSetClassification::Alloc > Alloc;
	//!< DataSetBinaryClassification handle type.
	typedef PointerT< DataSetBinaryClassification, DataSetClassification::Handle > Handle;
	//!< DataSetBinaryClassification bag type.
	typedef ContainerT< DataSetBinaryClassification, DataSetClassification::Bag > Bag;

	explicit DataSetBinaryClassification(const std::string& inName=std::string("DataSetBinaryClassification"));
	virtual ~DataSetBinaryClassification();

	void readCSV(std::istream& ioIS);
	virtual void readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
	
	virtual inline std::vector<unsigned int>* getIndexesPositives() const
	{
		Beagle_StackTraceBeginM();
		return mIndexesPositives;
		Beagle_StackTraceEndM("void DataSetBinaryClassification::getIndexesPositives()");
	}
	virtual inline std::vector<unsigned int>* getIndexesNegatives() const
	{
		Beagle_StackTraceBeginM();
		return mIndexesNegatives;
		Beagle_StackTraceEndM("void DataSetBinaryClassification::getIndexesNegatives()");
	}
	
protected:

	std::vector<unsigned int>* mIndexesPositives;
	std::vector<unsigned int>* mIndexesNegatives;

private:

	virtual void createIndexes();
};

}

#endif // Beagle_DataSetBinaryClassification_hpp
