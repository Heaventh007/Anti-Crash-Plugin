#pragma once

enum netadrtype_t : __int32 {
	NA_BOT = 0x0,
	NA_BAD = 0x1,
	NA_LOOPBACK = 0x2,
	NA_BROADCAST = 0x3,
	NA_IP = 0x4,
};

enum netsrc_t : unsigned int {
	NS_NULL = 0xFFFFFFFF,
	NS_CLIENT1 = 0x0,
	NS_CLIENT2 = 0x1,
	NS_CLIENT3 = 0x2,
	NS_CLIENT4 = 0x3,
	NS_SERVER = 0x4,
	NS_MAXCLIENTS = 0x4,
	NS_PACKET = 0x5,
};

struct msg_t
{
	int Overflowed;
	int ReadOnly;
	unsigned __int8 *Data;
	unsigned __int8 *SplitData;
	int MaxSize;
	int CurSize;
	int SplitSize;
	int ReadCount;
	int Bit;
	int LastEntityRef;
	netsrc_t TargetLocalNetID;
};

struct netadrBO1_t
{
	netadrtype_t Type; // 0
	int IP; // 4
	short Port; // 8
	netsrc_t LocalNetID; // 12
};

struct netadrMW_t
{
	netadrtype_t Type;
	unsigned __int8 IP[4];
	unsigned __int16 Port;
	netsrc_t LocalNetID;
};

struct netadrBO2_t {
	unsigned int INAddr;
	unsigned __int16 Port;
	netadrtype_t Type;
	netsrc_t LocalNetID;
private:
	unsigned int unk0;
};

struct NetchanIncomingData_s {
	unsigned int OutgoingSequence; // 0x00
	char FragmentIndex; // 0x04
	char MaxFragmentIndex; // 0x05
	short FragmentOffset; // 0x06
	short FragmentSize; // 0x08
};