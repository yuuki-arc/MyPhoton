/* Exit Games Photon - C++ Client Lib
 * Copyright (C) 2004-2014 by Exit Games GmbH. All rights reserved.
 * http://www.exitgames.com
 * mailto:developer@exitgames.com
 */

#pragma once

#include "Photon-cpp/inc/Internal/PeerBase.h"
#include "Photon-cpp/inc/Internal/EnetChannel.h"

namespace ExitGames
{
	namespace Photon
	{
		namespace Internal
		{
			class EnetPeer: public PeerBase
			{
			public:
				EnetPeer(PhotonListener& listener, const Common::Logger& logger);
				~EnetPeer(void);
			private:
				typedef PeerBase super;

				void cleanupNonHierarchical(void);
				void cleanup(void);
				void reset(void);
				bool connect(const EG_CHAR* ipAddr, const nByte appID[32]);
				bool startConnection(const char* ipAddr);
				void disconnect(void);
				void stopConnection(void);
				void sendOutgoingCommands(void);
				void copyCommandToBuffer(nByte* pDst, const nByte* pSrc, int& size);
				bool dispatchIncomingCommands(void);
				void fetchServerTimestamp(void);
				void onConnect(int nError);
				void onReceiveData(nByte* pBuf, int iCount, int nError);

				int getIncomingReliableCommandsCount(void);
				int getQueuedIncomingCommands(void);
				int getQueuedOutgoingCommands(void);
				int getSentCountAllowance(void);
				void setSentCountAllowance(int sentCountAllowance);

				void send(nByte cType, nByte* payload, unsigned int payloadSize, nByte channelId);
				bool sendInFragments(nByte* payload, unsigned int payloadSize, nByte channelId);
				void queueOutgoingReliableCommand(EnetCommand& command);
				void queueOutgoingUnreliableCommand(EnetCommand& command);
				void queueOutgoingAcknowledgement(const EnetCommand& command);
				bool queueIncomingCommand(const EnetCommand& command);
				void queueSentReliableCommand(const EnetCommand& command);
				int serializeToBuffer(ExitGames::Common::JVector<EnetCommand>& ev);
				void execute(const EnetCommand& command);
				bool removeSentReliableCommand(int ackReceivedReliableSequenceNumber,  int ackReceivedChannel, EnetCommand& pCommand);
				void sortLastElementInQueue(ExitGames::Common::JVector<EnetCommand>& argQueue, bool sortByReliableSequenceNumber);
				void clearAllQueues(void);

				ExitGames::Common::JVector<EnetCommand> outgoingAcknowledgements;
				ExitGames::Common::JVector<EnetCommand> sentReliableCommands;
				EnetChannel** channels;						
				int windowSize;  
				int* unsequencedWindow;
				int outgoingUnsequencedGroupNumber;
				int incomingUnsequencedGroupNumber;
				nByte commandCount;
				nByte* buffer;
				int bufferIndex;
				int commandSize;
				int mPacketsLost;
				int mPacketsSent;
				int mTimeInt;
				int mTimeoutInt;
				int mServerSentTime;
				int mSentCountAllowance;

				friend void EnetCommand::init(EnetPeer* const  pEnetPeer, nByte cType, const nByte* const payload, int payloadLen);
			};
		}
	}
}