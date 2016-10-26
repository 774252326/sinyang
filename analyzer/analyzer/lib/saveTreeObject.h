// saveTreeObject.h
// a pure abstract class to be used for multiple inheritance (for Foo, not FooOptions)
// basically just shorthand to remember what i have to implement to get Save working...
// but still leave SFINAE in as an extra check.

// i.e.
// SFINAE will CHECK when OptionsCenter has been added as a member variable but Save/Load haven't been written yet
	// this is more of a failsafe, passive check, OptionCenter child to object parent
// WHEN YOU REMEMBER, SaveTreeObject inheritance will TELL you what functions you need
	// so the multiple inheritance is more of a lazy reminder? + rtti check parent to child?

#if ((defined _MSC_VER) && (_MSC_VER > 1000)) || ((defined __GNUC__) && (__GNUC__ > 3))
#	pragma once
#else
#	error "Custom header guard needed - have fun editing all N files in this project...just HOW portable is pragma once??"
#endif

#include "standard_definitions_head.h"





// design decision: a SCAFFOLDING CLASS that MUST be removed for release builds
#ifndef QT_NO_DEBUG

class SaveTreeNode;

class SaveTreeObject
{
public:
	// arguments are unique enough to avoid future nameclashes via multiple inheritance
	virtual void Save(SaveTreeNode &currentNode) = 0;
	virtual void Load(SaveTreeNode &currentNode) = 0;

	// decided to make name more unique to avoid future nameclashes
	virtual QString className() const = 0;

// sample implementation
	//void PlotArea::Save(SaveTreeNode &currentNode)
	//{
	//	// more robust error checking? scary scary pointers
	//	if (m_plot == NULL)
	//	{
	//		qWarning("%s null plot...terminating tree!", __FUNCTION__);
	//		return;
	//	}
	//	
	//	// birth and name its node, numbering IF DESIRED (just needs to match in Load). easy enough NOT to go into SaveTreeObject!!
	//	SaveTreeNode childNode = currentNode.createChildObject( m_plot->className() );
	//	m_plot->Save( childNode );
	//}

	//void PlotArea::Load(SaveTreeNode &currentNode) 
	//{
	//	// more robust error checking? scary scary pointers
	//	if (m_plot == NULL)
	//	{
	//		qWarning("%s null plot...terminating tree!", __FUNCTION__);
	//		return;
	//	}

	//	// uniqueChildObject is usable until you have multiple child objects that need IDs - handle class by class?
	//	SaveTreeNode childNode = currentNode.uniqueChildObject( m_plot->className() ); 
	//	if ( !childNode.isNull() )
	//		m_plot->Load(childNode);	
	//}


};
#endif // !defined QT_NO_DEBUG

