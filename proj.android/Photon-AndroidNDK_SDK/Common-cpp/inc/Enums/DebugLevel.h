/* Exit Games Common - C++ Client Lib
 * Copyright (C) 2004-2014 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

namespace ExitGames
{
	namespace Common
	{
		namespace DebugLevel
		{
			/** Amount of DebugReturn callbacks. Each debug level includes output for lower ones: OFF, ERRORS, WARNINGS, INFO, ALL. */
			enum DebugLevel
			{
				OFF = 0, /**<No debug out.*/
				ERRORS = 1,/**<Only error descriptions.*/
				WARNINGS = 2,/**<Warnings and errors.*/
				INFO = 3,/**<Information about internal workflows, warnings and errors.*/
				ALL = 4/**<Most complete workflow description (but lots of debug output), info, warnings and errors.*/
			};

			/** @file */
		}
	}
}