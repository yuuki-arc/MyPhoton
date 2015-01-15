#include <iostream>

#include "NetworkLogic.h"

int getcharIfKbhit(void);

#ifdef _EG_WINDOWS_PLATFORM
#include <conio.h>

int getcharIfKbhit(void)
{
	int res = _kbhit();
	if(res)
		res = _getch();
	return res;
}

#else
#include <termios.h>
#include <fcntl.h>

int getcharIfKbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	return ch;
}
#endif

#if defined _EG_WINDOWS_PLATFORM
#	define SLEEP(ms) Sleep(ms)
#else
#	define SLEEP(ms) usleep(ms*1000)
#endif

class StdOutputListener: public OutputListener
{
public:
	virtual void write(const ExitGames::Common::JString& str);
	virtual void writeLine(const ExitGames::Common::JString& str);
};

void StdOutputListener::write(const ExitGames::Common::JString& str)
{
	std::wcout << str;
}

void StdOutputListener::writeLine(const ExitGames::Common::JString& str)
{
	write(str);
	write(L"\n");
}

class StdUI: public NetworkLogicListener
{
public:
	void main();

private:
	virtual void stateUpdate(State newState)
	{}

private:
	StdOutputListener mOutputListener;
};

void StdUI::main()
{
	NetworkLogic networkLogic(&mOutputListener, L"1.0");
	networkLogic.registerForStateUpdates(this);
	mOutputListener.writeLine(L"always:");
	mOutputListener.writeLine(L"0 - exit");
	mOutputListener.writeLine(L"--------------------");
	mOutputListener.writeLine(L"outside a game room:");
	mOutputListener.writeLine(L"1 - create game");
	mOutputListener.writeLine(L"2 - join random game");
	mOutputListener.writeLine(L"--------------------");
	mOutputListener.writeLine(L"inside a game room:");
	mOutputListener.writeLine(L"1 - leave game");
	mOutputListener.writeLine(L"--------------------");
	while(networkLogic.getLastInput() != INPUT_EXIT && networkLogic.getState() != STATE_DISCONNECTED)
	{
		int ch = getcharIfKbhit();
		switch(networkLogic.getState())
		{
		case STATE_CONNECTED:
			switch(ch)
			{
			case '0':
				networkLogic.setLastInput(INPUT_EXIT);
				break;
			case '1':
				networkLogic.setLastInput(INPUT_CREATE_GAME);
				break;
			case '2':
				networkLogic.setLastInput(INPUT_JOIN_RANDOM_GAME);
				break;
			default:
				networkLogic.setLastInput(INPUT_NON);
				break;
			}
			break;
		case STATE_JOINED:
			switch(ch)
			{
			case '0':
				networkLogic.setLastInput(INPUT_EXIT);
				break;
			case '1':
				networkLogic.setLastInput(INPUT_LEAVE_GAME);
				break;
			default:
				networkLogic.setLastInput(INPUT_NON);
				break;
			}
			break;
		default:
			switch(ch)
			{
			case '0':
				networkLogic.setLastInput(INPUT_EXIT);
				break;
			default:
				networkLogic.setLastInput(INPUT_NON);
				break;
			}
			break;
		}
		networkLogic.run();
		SLEEP(100);
	}
}

int main(int argc, const char* argv[])
{
	StdUI ui;
	ui.main();
	return 0;
}