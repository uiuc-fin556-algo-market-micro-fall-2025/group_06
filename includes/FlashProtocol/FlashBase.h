/*===================================================================================================================================
*     Copyright (c) RCM-X, 2009 - 2011.
*     All rights reserved.
*
*     This software is part of Licensed material, which is the property of RCM-X ("Company"),
*     Unauthorized use, modification, duplication or distribution is strictly prohibited by Federal law.
*
*     No title to or ownership of this software is hereby transferred.
*
*     The software is provided "as is", and in no event shall the Company or any of its affiliates or successors be liable for any
*     damages, including any lost profits or other incidental or consequential damages relating to the use of this software.
*     The Company makes no representations or warranties, express or implied, with regards to this software.
====================================================================================================================================*/

#pragma once

#ifndef _STRATEGY_STUDIO_FLASH_PROTOCOL_FLASH_BASE_H_
#define _STRATEGY_STUDIO_FLASH_PROTOCOL_FLASH_BASE_H_

#include "FlashTypes.h"
#include <cstring>

namespace RCM {
namespace StrategyStudio {
namespace FlashProtocol {

/// This class is the abstract base for all Flash messages.  This interface also defines the interface used by the protocol/transport.  The version numbers defined here
/// are used when negotiating a connection/session within the protocol.
///
/// Message Wite Format:
///     Each message contains a header, the first 4 bytes of that header MUST be the message size.  This size includes the size of the header.
///
///     Binary Encoding notes:
///
///     byte            - direct encoding
///     integer         - direct encoding based on the size of the integer from 16-64 bits.  Wire format uses Little Endian integer encoding
///     float           - direct IEEE 754 encoding
///     double          - direct IEEE 754 encoding
///     string          - Length encoded as two bytes followed by the m_text.  Non-Unicode, UTF-7/8
///     enums           - Encoding based on size of the enum range.  If it can fit in a byte, then encoded as byte, etc.
///     container       - Length encoded as two bytes (or more if required) followed by encoding each value type by component fields.
///     keyed-container - same as container except each key occures before each value.
class FlashBase {
public:
    /// Protocol version used by client/server to negotiate a connection.  If major version is not the same, a connection will be refused.
    static const byte majorProtocolVersion = 21;

    /// Minor version specifies that while there are some changes, the client/server should operate normally.
    static const byte minorProtocolVersion = 0;

    /// Header:
    ///     Message Size    4 bytes
    ///     Message Type    2 bytes
    ///     Sequence No     4 bytes (broadcast messages have a seq no = 0)
    ///     MessageVersion  1 byte
    ///     TimeStamp       8 bytes
    static const byte headerSize = 19;

    /// Important value used by code that unpacks the message.  This must point to the first byte of the flashtype in the header
    static const StrategyStudioInt32 flashTypeOffset = 4;
    static const StrategyStudioInt32 flashSeqNoOffset = 6;
    static const StrategyStudioInt32 flashMsgVerOffset = 10;
    static const StrategyStudioInt32 flashTimeStampOffset = 11;

    // This number is a cutoff for converting wire timestamps to micros if we can't do it based on message version
    static const StrategyStudioInt64 kMillisMicrosCutoff = (StrategyStudioInt64) 2e13;
    
    // Messages that carry dates or times jump to this message version with Protocol 21, where convention shifts from millis to micros
    static const byte kFirstMicrosecondsMessageVersion = 21;

    FlashBase(StrategyStudioUInt16 type, const byte version = 1);

    /// Destructor
    virtual ~FlashBase();

    /// Unique identifier in the protocol for a message
    /// <returns>Type of the derived message</returns>
    StrategyStudioUInt16 Type() const { return m_messageType; }

    /// Binary size of the message
    StrategyStudioInt32 Size() const { return m_messageSize; }

    /// Sequence Number of the message
    StrategyStudioUInt32 SequenceNumber() const { return m_seqNum; }

    byte MessageVersion() const { return m_messageVersion; }

    StrategyStudioInt64 CreationTimeStamp() const { return m_creationTimeStamp; }

    void SequenceNumber(StrategyStudioUInt32 num) { m_seqNum = num; }

    void CreationTimeStamp(StrategyStudioInt64 time) { m_creationTimeStamp = time; }

    /// Used when the buffer sent for parsing is larger that an individual message and we want to begin parsing at a location other than zero.
    void ParseOffset(StrategyStudioInt32 offset) { m_parseOffset = offset; }

    /// This method is used to determine the size of the message in bytes, calling this will set the m_messageSize member
    virtual StrategyStudioInt32 CalculateSize() const;

    /// Serializes message fields to a byte array
    virtual void SerializeToBytes(FlashBuffer& buffer) const;

    /// Parses byte array and fills message fields
    virtual bool ParseFromBytes(FlashBuffer& buffer);

    bool operator==(const FlashBase &) const;

protected:
    /// The calculated size of the message, set by derived classes.  This is signed because of limitation of the binary unpacking code in C#.
    mutable StrategyStudioInt32 m_messageSize;

    /// Private member that is set by derived classes through the constructor only, it can never change.
    StrategyStudioUInt16 m_messageType;

    /// Sequence number of the message
    StrategyStudioUInt32 m_seqNum;

    /// Protected member used when parsing/serializing fields.  This keeps track of the buffer position.
    mutable StrategyStudioInt32 m_index;

    /// Used when parsing fields only.  This allows us to point to a different parsing location than the default zero location.
    StrategyStudioInt32 m_parseOffset;

    /// Used to designate a particular version for a message to obtain backward compatibility
    byte m_messageVersion;

    /// Time message was created on origination side of connection
    StrategyStudioInt64 m_creationTimeStamp;
};

/// Construct the base class, called by derived classes only
inline FlashBase::FlashBase(StrategyStudioUInt16 type, const byte version)
    : m_messageSize(0), m_messageType(type), m_seqNum(0), m_index(0), m_parseOffset(0), m_messageVersion(version), m_creationTimeStamp(0)
{
}

/// Destructor
inline FlashBase::~FlashBase()
{
}

} // namespace FlashProtocol
} // namespace StrategyStudio
} // namespace RCM

#endif
