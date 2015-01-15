#include "NetworkLogic.h"

#if defined _EG_MARMALADE_PLATFORM
#	if defined I3D_ARCH_X86
#		if defined _EG_MS_COMPILER
#			define PLAYER_NAME L"Marmalade X86 Windows"
#		else
#			define PLAYER_NAME L"Marmalade X86 OS X"
#		endif
#	elif defined I3D_ARCH_ARM
#		define PLAYER_NAME L"Marmalade ARM"
#	elif defined I3D_ARCH_MIPS
#		define PLAYER_NAME L"Marmalade MIPS"
#	else
#		define PLAYER_NAME L"unknown Marmalade platform"
#	endif
#elif defined _EG_WINDOWS_PLATFORM
#	define PLAYER_NAME L"Windows"
#elif defined _EG_IPHONE_PLATFORM
#	define PLAYER_NAME L"iOS"
#elif defined _EG_IMAC_PLATFORM
#	define PLAYER_NAME L"OS X"
#elif defined _EG_ANDROID_PLATFORM
#	define PLAYER_NAME L"Android"
#elif defined _EG_BLACKBERRY_PLATFORM
#	define PLAYER_NAME L"Blackberry"
#elif defined _EG_PS3_PLATFORM
#	define PLAYER_NAME L"PS3"
#elif defined _EG_LINUX_PLATFORM
#	define PLAYER_NAME L"Linux"
#else
#	define PLAYER_NAME L"unknown platform"
#endif

ExitGames::Common::JString& NetworkLogicListener::toString(ExitGames::Common::JString& retStr, bool /*withTypes*/) const
{
	return retStr;
}



State StateAccessor::getState(void) const
{
	return mState;
}

void StateAccessor::setState(State newState)
{
	mState = newState;
	for(unsigned int i=0; i<mStateUpdateListeners.getSize(); i++)
		mStateUpdateListeners[i]->stateUpdate(newState);
}

void StateAccessor::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateUpdateListeners.addElement(listener);
}



Input NetworkLogic::getLastInput(void) const
{
	return mLastInput;
}

void NetworkLogic::setLastInput(Input newInput)
{
	mLastInput = newInput;
}

State NetworkLogic::getState(void) const
{
	return mStateAccessor.getState();
}

// functions
NetworkLogic::NetworkLogic(const wchar_t* appVersion,	ExitGames::LoadBalancing::AuthenticationValues authenticationValues)
#ifdef _EG_MS_COMPILER
#	pragma warning(push)
#	pragma warning(disable:4355)
#endif
: mLoadBalancingClient(*this, Constant::PHOTON_APPLICATION_ID, appVersion, PLAYER_NAME, false, authenticationValues)
	, mLastInput(INPUT_NON)
#ifdef _EG_MS_COMPILER
#	pragma warning(pop)
#endif
{
	mStateAccessor.setState(STATE_INITIALIZED);
	mLoadBalancingClient.setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO); // that instance of LoadBalancingClient and its implementation details
//	mLogger.setListener(*this);
//	mLogger.setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO); // this class
	ExitGames::Common::Base::setListener(this);
	ExitGames::Common::Base::setDebugOutputLevel(ExitGames::Common::DebugLevel::INFO); // all classes that inherit from Base 
}

void NetworkLogic::registerForStateUpdates(NetworkLogicListener* listener)
{
	mStateAccessor.registerForStateUpdates(listener);
}

void NetworkLogic::connect(void)
{
	/* Specify the address of your Photon Server master server or the url of a Photon Cloud region here.
	If you don't specify a port, then Photons default master server ports (UDP: 5055, TCP: 4530) will be used.
	Valid cloud regions are:
	Western Europe: app-eu.exitgamescloud.com
	US: app-us.exitgamescloud.com
	Asia: app-asia.exitgamescloud.com
	Japan: app-jp.exitgamescloud.com
	Eastern Europe: app-eu-east.exitgamescloud.com
	The parameterless overload defaults to app-eu.exitgamescloud.com. */
    mLoadBalancingClient.connect(L"app-jp.exitgamescloud.com");
	mStateAccessor.setState(STATE_CONNECTING);
}

void NetworkLogic::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void NetworkLogic::opCreateRoom(void)
{
	// if last digits are always nearly the same, this is because of the timer calling this function being triggered every x ms with x being a factor of 10
	ExitGames::Common::JString tmp;
	mLoadBalancingClient.opCreateRoom(tmp=GETTIMEMS());
	mStateAccessor.setState(STATE_JOINING);
}

void NetworkLogic::opJoinRandomRoom(void)
{
	mLoadBalancingClient.opJoinRandomRoom();
}

void NetworkLogic::run(void)
{
	if(mLastInput == INPUT_EXIT && mStateAccessor.getState() != STATE_DISCONNECTING && mStateAccessor.getState() != STATE_DISCONNECTED)
	{
		disconnect();
		mStateAccessor.setState(STATE_DISCONNECTING);
	}
	else
	{
		State state = mStateAccessor.getState();
		switch(state)
		{
		case STATE_INITIALIZED:
			connect();
			mStateAccessor.setState(STATE_CONNECTING);
			break;
		case STATE_CONNECTING:
			break; // wait for callback
		case STATE_CONNECTED:
			switch(mLastInput)
			{
			case INPUT_CREATE_GAME: // create Game
				opCreateRoom();
				break;
			case INPUT_JOIN_RANDOM_GAME: // join Game
				opJoinRandomRoom();
				mStateAccessor.setState(STATE_JOINING);
				break;
			default: // no or illegal input -> stay waiting for legal input
				break;
			}
			break;
		case STATE_JOINING:
			break; // wait for callback
		case STATE_JOINED:
			sendEvent();
			switch(mLastInput)
			{
			case INPUT_LEAVE_GAME: // leave Game
				mLoadBalancingClient.opLeaveRoom();
				mStateAccessor.setState(STATE_LEAVING);
				break;
			default: // no or illegal input -> stay waiting for legal input
				break;
			}
			break;
		case STATE_LEAVING:
			break; // wait for callback
		case STATE_LEFT:
			mStateAccessor.setState(STATE_CONNECTED);
			break;
		case STATE_DISCONNECTING:
			break; // wait for callback
		default:
			break;
		}
	}
	mLastInput = INPUT_NON;
	mLoadBalancingClient.service();
}

void NetworkLogic::sendEvent(void)
{
	static int64 count = 0;
	mLoadBalancingClient.opRaiseEvent(false, ++count, 0);
}

// protocol implementations

void NetworkLogic::debugReturn(ExitGames::Common::DebugLevel::DebugLevel /*debugLevel*/, const ExitGames::Common::JString& string)
{
}

void NetworkLogic::connectionErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
	mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::clientErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
}

void NetworkLogic::warningReturn(int warningCode)
{
	EGLOG(ExitGames::Common::DebugLevel::WARNINGS, L"code: %d", warningCode);
}

void NetworkLogic::serverErrorReturn(int errorCode)
{
	EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"code: %d", errorCode);
}

void NetworkLogic::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int>& /*playernrs*/, const ExitGames::LoadBalancing::Player& player)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"%ls joined the game", player.getName().cstr());
}

void NetworkLogic::leaveRoomEventAction(int playerNr)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkLogic::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
    ExitGames::Common::Hashtable* event;
    
    switch (eventCode) {
        case 1:
            event = ExitGames::Common::ValueObject<ExitGames::Common::Hashtable*>(eventContent).getDataCopy();
            float x = ExitGames::Common::ValueObject<float>(event->getValue(1)).getDataCopy();
            float y = ExitGames::Common::ValueObject<float>(event->getValue(2)).getDataCopy();
            eventQueue.push({static_cast<float>(playerNr), x, y});
            break;
    }
}

void NetworkLogic::connectReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mStateAccessor.setState(STATE_CONNECTED);
}

void NetworkLogic::disconnectReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	mStateAccessor.setState(STATE_DISCONNECTED);
}

void NetworkLogic::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mStateAccessor.setState(STATE_JOINED);

    
    // ルーム内で割り当てられたプレイヤー番号を取得する
    playerNr = localPlayerNr;
}

void NetworkLogic::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
    EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mStateAccessor.setState(STATE_JOINED);

    
    // ルーム内で割り当てられたプレイヤー番号を取得する
    playerNr = localPlayerNr;
}

void NetworkLogic::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_CONNECTED);
		return;
	}
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"localPlayerNr: %d", localPlayerNr);
	mStateAccessor.setState(STATE_JOINED);

    
    // ルーム内で割り当てられたプレイヤー番号を取得する
    playerNr = localPlayerNr;    
}

void NetworkLogic::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
	if(errorCode)
	{
		EGLOG(ExitGames::Common::DebugLevel::ERRORS, L"%ls", errorString.cstr());
		mStateAccessor.setState(STATE_DISCONNECTING);
		return;
	}
	mStateAccessor.setState(STATE_LEFT);
}

void NetworkLogic::joinLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

void NetworkLogic::leaveLobbyReturn(void)
{
	EGLOG(ExitGames::Common::DebugLevel::INFO, L"");
}

bool NetworkLogic::isRoomExists(void)
{
    if (mLoadBalancingClient.getRoomList().getIsEmpty()) {
        return false;
    }
    
    return true;
}

void NetworkLogic::sendEvent(nByte code, ExitGames::Common::Hashtable* eventContent)
{
    mLoadBalancingClient.opRaiseEvent(true, eventContent, 1, code);
}