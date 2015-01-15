/* Exit Games Common - objC Client Lib
 * Copyright (C) 2004-2014 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#import <Foundation/NSString.h>

@protocol EGBaseListener

- (void) debugReturn:(int)debugLevel :(NSString*)string;

@end

/** @file */