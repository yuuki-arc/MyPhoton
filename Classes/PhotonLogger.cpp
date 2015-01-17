//
//  PhotonLogger.cpp
//  MyPhoton
//
//  Created by m.yamaoka on 2015/01/17.
//
//

#include "PhotonLogger.h"

USING_NS_CC;
using namespace std;

PhotonLogger::PhotonLogger()
{
    
}

PhotonLogger::~PhotonLogger()
{
    
}

void PhotonLogger::write(const ExitGames::Common::JString& str)
{
    CCLOG("%s", str.UTF8Representation().cstr());
}

void PhotonLogger::writeLine(const ExitGames::Common::JString& str)
{
    CCLOG("%s", str.UTF8Representation().cstr());
}

void PhotonLogger::debugReturn(const ExitGames::Common::JString& str)
{
    CCLOG("%s", str.UTF8Representation().cstr());
}
