

#define BEGIN_MESSAGE_MAP_HPP(theClass, baseClass) \
	protected: \
	PTM_WARNING_DISABLE \
	static const AFX_MSGMAP* PASCAL theClass::GetThisMessageMap() \
	{ \
		typedef theClass ThisClass;						   \
		typedef baseClass TheBaseClass;					   \
		static const AFX_MSGMAP_ENTRY _messageEntries[] =  \
		{

#define END_MESSAGE_MAP_HPP() \
		{0, 0, 0, 0, AfxSig_end, (AFX_PMSG)0 } \
	}; \
		static const AFX_MSGMAP messageMap = \
		{ &TheBaseClass::GetThisMessageMap, &_messageEntries[0] }; \
		return &messageMap; \
	}								  \
	virtual const AFX_MSGMAP* GetMessageMap() const \
		{ return GetThisMessageMap(); } \
	PTM_WARNING_RESTORE
