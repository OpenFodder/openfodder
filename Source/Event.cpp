#include "stdafx.hpp"

cEvent::cEvent( const eEventType& pType ) {

	mType = pType;
	mButton = 0;
	mButtonCount = 0;
}

cEvent::~cEvent() {

}
