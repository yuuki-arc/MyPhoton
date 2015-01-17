//
//  PhotonLogger.h
//  MyPhoton
//
//  Created by m.yamaoka on 2015/01/17.
//
//

#ifndef __MyPhoton__PhotonLogger__
#define __MyPhoton__PhotonLogger__

#include "cocos2d.h"
#include "OutputListener.h"

class PhotonLogger : public OutputListener
{
    
public:
    PhotonLogger();
    virtual ~PhotonLogger();
    void write(const ExitGames::Common::JString& str);
    void writeLine(const ExitGames::Common::JString& str);
    void debugReturn(const ExitGames::Common::JString& str);
    
protected:
    
private:

};

#endif /* defined(__MyPhoton__PhotonLogger__) */
