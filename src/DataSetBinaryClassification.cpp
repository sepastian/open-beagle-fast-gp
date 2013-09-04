#include "beagle/Beagle.hpp"
#include "DataSetBinaryClassification.hpp"

using namespace Beagle;


/*!
 *  \brief Construct data set component useful for classification problems.
 *  \param inName Name of the component.
 */
DataSetBinaryClassification::DataSetBinaryClassification(const std::string& inName) :
		DataSetClassification(inName),
		mIndexesNegatives(new std::vector<unsigned int>),
		mIndexesPositives(new std::vector<unsigned int>)
{ }


/*!
 *  \brief Construct data set component useful for classification problems.
 *  \param inName Name of the component.
 */
DataSetBinaryClassification::~DataSetBinaryClassification()
{
	delete mIndexesPositives;
	delete mIndexesNegatives;
}


/*!
 *  \brief Read data set from a CVS file.
 *  \param ioIS Input stream referring to CVS file.
 */
void DataSetBinaryClassification::readCSV(std::istream& ioIS)
{
	Beagle_StackTraceBeginM();
	DataSetClassification::readCSV(ioIS);
	createIndexes();
	Beagle_StackTraceEndM("void DataSetBinaryClassification::readCSV(std::istream&)");
}


/*!
 *  \brief Read data set for regression component.
 *  \param inIter XML iterator to use to read the set.
 *  \param ioSystem Evolutionary system.
 */
void DataSetBinaryClassification::readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem)
{
	Beagle_StackTraceBeginM();
	DataSetClassification::readWithSystem(inIter, ioSystem);
	createIndexes();
	Beagle_StackTraceEndM("void DataSetBinaryClassification::readWithSystem(PACC::XML::ConstIterator, System&)");
}

/*!
 * \brief Scan the dataset to create indexes of positive and negative samples.
 */
void DataSetBinaryClassification::createIndexes()
{
	Beagle_StackTraceBeginM();
	if (mIndexesPositives != NULL)
	{
		mIndexesPositives->resize(0);
	}
	if (mIndexesNegatives != NULL)
	{
		mIndexesNegatives->resize(0);
	}
	for (DataSetClassification::const_iterator lSample=begin(); lSample!=end(); ++lSample)
	{
		if (lSample->first == 1)
		{
			mIndexesPositives->push_back(lSample- begin());
		}
		else
		{
			mIndexesNegatives->push_back(lSample- begin());
		}
	}
	Beagle_StackTraceEndM("void DataSetBinaryClassification::createIndexes()");
}

