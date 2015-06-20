enum eEventType {
	eEvent_None				= 0,
	eEvent_KeyDown			= 1,
	eEvent_KeyUp			= 2,
	eEvent_MouseMove		= 3,
	eEvent_MouseLeftDown	= 4,
	eEvent_MouseLeftUp		= 5,
	eEvent_MouseRightDown	= 6,
	eEvent_MouseRightUp		= 7,
	eEvent_MouseWheelUp		= 8,
	eEvent_MouseWheelDown	= 9,
	eEvent_Quit				= 10
};

class cEvent {
	public:
		eEventType			mType;

		unsigned int		mButton;
		unsigned int		mButtonCount;

		cPosition			mPosition;

	public:
							cEvent( const eEventType& pType = eEvent_None );
		virtual				~cEvent();
};
