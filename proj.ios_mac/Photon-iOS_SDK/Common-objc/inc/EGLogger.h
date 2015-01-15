/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2014 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Common-cpp/inc/Logger.h"
#import "Common-objc/inc/EGBaseListener.h"
#import "Common-objc/inc/NSString+UTF32.h"

@interface EGLogger : NSObject
{
@private
	ExitGames::Common::Logger mLogger;
	ExitGames::Common::BaseListener* mpListener;
}

@property (readwrite) int DebugOutputLevel;
- (void) setListener:(id<EGBaseListener>)listener;

- (instancetype) initWithDebugOutputLevel:(int)debugLevel;
+ (instancetype) logger;
+ (instancetype) loggerWithDebugOutputLevel:(int)debugLevel;
- (void) log:(int)debugLevel :(const EG_CHAR* const)file :(const EG_CHAR* const)function :(unsigned int)line :(const EG_CHAR* const)dbgMsg, ...;
- (void) vlog:(int)debugLevel :(const EG_CHAR* const)file :(const EG_CHAR* const)function :(unsigned int)line :(const EG_CHAR* const)dbgMsg :(va_list)args;

@end

#ifdef EG_DEBUGGER
#	undef EGLOG
#	define EGLOG(debugLevel, ...) [self.Logger log:debugLevel :__WFILE__ :__WFUNCTION__ :__LINE__ :__VA_ARGS__]
#	define EGVLOG(debugLevel, dbgMsg, args) [self.Logger vlog:debugLevel :__WFILE__ :__WFUNCTION__ :__LINE__ :dbgMsg :args]
#endif