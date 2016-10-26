// saveTreeNode.h
// this is a tree-node data type built on XML DOM, intended to be used to "save state" for nested objects

#if ((defined _MSC_VER) && (_MSC_VER > 1000)) || ((defined __GNUC__) && (__GNUC__ > 3))
#	pragma once
#else
#	error "Custom header guard needed - have fun editing all N files in this project...just HOW portable is pragma once??"
#endif

#include "standard_definitions_head.h"

// wtf, this doesn't compile in qt creator?!
//#if T_ARRAY == QVector // uh, keep this out of standard_definitions_head since not everybody needs T_ARRAY?? ALMOST defeats the purpose of centralization
//#	include <QVector>
//#elif T_ARRAY == QList
//#	include <QList>
//#else
//#	error unknown T_ARRAY type
//#endif

class SaveTreeNode; 
#if ARRAY_TYPE == ARRAY_TYPE_VECTOR
	#include <QVector>
	typedef QVector<SaveTreeNode> SaveNodeArray;
#elif ARRAY_TYPE == ARRAY_TYPE_LIST
	#include <QList>
	typedef QList<SaveTreeNode> OptionArray;
#else
	#error unknown ARRAY_TYPE
#endif // ARRAY_TYPE

#include <QDomElement>


//typedef T_ARRAY<SaveTreeNode> SaveNodeArray;
//typedef T_ARRAY<QString> StringArray;

class Option;

class SaveTreeNode : public QDomElement
{
public:
	SaveTreeNode (const QDomElement &); // overloaded constructor needed for variable initializations!
	SaveTreeNode (); // overloaded constructor requires default constructor

	SaveTreeNode& operator= (const QDomElement &);	// unnecessary?
	SaveTreeNode& operator= (const SaveTreeNode &);	

	// <object type="typeValue">
	void createChildObject (const QString &typeValue, SaveTreeNode &);
	SaveTreeNode createChildObject (const QString &);
	SaveNodeArray childObjectsByType (const QString &);
	void uniqueChildObject (const QString &, SaveTreeNode &); // overkill? can TRY using these...
	SaveTreeNode uniqueChildObject (const QString &);
	SaveTreeNode createTaggedChildObject (const QString &type, const QString &tagName, const QString &tagValue);
	SaveTreeNode uniqueTaggedChildObject (const QString &type, const QString &tagName, const QString &tagValue);
	QString objectTag() const;
	QString objectTypeAttribute() const;
	bool isObject() const;

	// <option/> - other than tag name (to avoid clash with <object>),
	// all other SaveNode/Option conversion shit currently resides in OptionsCenterBase.
	// this is a BIT of a questionable decision since this will put QDomElement code in OptionsCenterBase
	// then again i already BIT that bullet when i decided to subclass from QDomElement...
	void createChildOption (SaveTreeNode &);
	SaveTreeNode createChildOption();	
	SaveNodeArray allChildOptions();
	bool isOption() const;

	// currently exposed for access by OptionsCenterBase, but...fairly low level - candidates for protected?
	SaveNodeArray childrenByAttributeValue (const QString &name, const QString &val);
	SaveNodeArray childrenByTagName (const QString &tag);

	// stub convenience function
	bool operator==(const Option &) const;

protected:
	// dom api idiosyncracy: the great ancestor first gives birth to the child, then a (possibly different) node claims parentage!
	void createChild(const QString &, SaveTreeNode &);
	SaveTreeNode createChild(const QString &); 
	// returns by value - not too sure about this one...would a MODIFIED SaveTreeNode class necessarily use implicit sharing?!
	// i am returning QVectors of SaveTreeNode's like there is no tomorrow...but QVector is known to use implicit sharing...
	// i have a feeling this class is VERY vulnerable to breaking upon modification...probably should have just stuck with QDomElement...
		// well at least i get to write my convenience functions no sweat...
	
private:
	static QString const M_TAG_OBJECT; // static: same for all copies  of SaveTreeNode
	static QString const M_TAG_OPTION; // private: limit its scope - but unfortunately value must be declared in cpp file
	static QString const M_ATTRIB_OBJ_TYPE;
};




