// -*- mode: c -*-

#define BINDING(name) static int lua__##name(lua_State* L)

//USHORT CTOS_GetSerialNumber(BYTE baBuf[16]);
BINDING(CTOS_GetSerialNumber)
{
    BYTE serial[16];
    char serial_as_string[sizeof(serial) * 2 + 1];
    CTOS_CALL(GetSerialNumber(serial));

    bytes_to_hexa_string(serial, sizeof(serial), serial_as_string);

    lua_pushstring(L, serial_as_string);
    return 1;
}

/*

USHORT CTOS_GetSystemInfo(BYTE bID, BYTE baBuf[17]);
USHORT CTOS_GetKeyHash(BYTE bKeyIndex, BYTE* baHash);

USHORT CTOS_LEDSet(BYTE bLED, BYTE bOnOff);
USHORT CTOS_BackLightSet (BYTE bDevice, BYTE bOnOff);
USHORT CTOS_BackLightSetEx(BYTE bDevice, BYTE bOnOff, DWORD dwDuration);

//=============================================================================================================================
//
// Clock and Time Functions
//
//=============================================================================================================================

// Real Time Clock define for RTC functions
typedef struct
{
	BYTE    bSecond;
	BYTE    bMinute;
	BYTE    bHour;
	BYTE    bDay;
	BYTE    bMonth;
	BYTE    bYear;
	BYTE    bDoW;	
} CTOS_RTC;


USHORT CTOS_RTCGet (CTOS_RTC *pstRTC);
USHORT CTOS_RTCSet (CTOS_RTC *pstRTC);
void CTOS_Delay(ULONG ulMSec); // in 1 ms
*/

//ULONG CTOS_TickGet(void); // in 10 ms
BINDING(CTOS_TickGet)
{
    ULONG ticksIn10msecs = CTOS_TickGet();

    lua_pushnumber(L, ((double)ticksIn10msecs) / 100);
    return 1;
}

/*
USHORT CTOS_TimeOutSet(BYTE bTID, ULONG ulMSec); // in 10 ms
USHORT CTOS_TimeOutCheck(BYTE bTID);

//=============================================================================================================================
//
// Buzzer Functions
//
//=============================================================================================================================

USHORT CTOS_Beep(void);
USHORT CTOS_Sound(USHORT usFreq, USHORT usDuration); // in 10 ms

//=============================================================================================================================
//
// Encryption/Decryption Functions
//
//=============================================================================================================================

typedef struct
{
	ULONG digest[5];            // Message digest 
	ULONG countLo, countHi;     // 64-bit bit count 
	ULONG data[16];             // SHS data buffer 
	int Endianness;
}SHA_CTX;

USHORT CTOS_RSA(BYTE *baModulus, USHORT usModulousLen, BYTE *baExponent, USHORT usExponentLen, BYTE *baData, BYTE *baResult);
USHORT CTOS_RNG(BYTE *baResult);
USHORT CTOS_DES (BYTE bEncDec, BYTE *baKey, BYTE bKeyLen, BYTE *baData, USHORT usDataLen, BYTE *baResult);
void CTOS_SHA1Init (SHA_CTX *pstInfo);
void CTOS_SHA1Update (SHA_CTX *pstInfo, BYTE *baBuffer, USHORT usCount);
void CTOS_SHA1Final (BYTE *baOutput, SHA_CTX *pstInfo);
USHORT CTOS_AES (BYTE bEncDec, BYTE *baKey, BYTE *baData, USHORT usDataLen, BYTE *baResult);
USHORT CTOS_MAC (BYTE *baKey, BYTE bKeyLen, BYTE *baICV, BYTE *baData, USHORT usDataLen, BYTE *baMAC);

//=============================================================================================================================
//
// KMS Functions
//
//=============================================================================================================================

typedef struct{
	BYTE EncMode;
	WORD TimeOut;
	BYTE MaxLEN;
	BYTE MinLEN;
	BYTE MK_Index;
	BYTE LenPAN;
	BYTE *sPAN;
	BYTE LenSK;
	BYTE *sSK;
	BYTE *sLine1Msg;
	BYTE *sLine2Msg;
	BYTE *sProcMsg;
	BYTE NULLPIN;
	BYTE LenPIN;
	BYTE *sEPB;
	BYTE *sKSN;
	BYTE PBType;
	int (*piTestCancel)(void);
} CTOS_stDefEncPINStruc;

typedef struct{
	BYTE *baData;
	BYTE bMK_Index;
	BYTE bLenSK;
	BYTE *baSK;
	BYTE *baEDB;
} CTOS_stDefEncDataStruc;

typedef struct{
	WORD iMLen;
	BYTE bELen;
	BYTE *baM;
	BYTE *baD;
	BYTE *baE;
} CTOS_stDefRSACertKey;

typedef struct{
	BYTE bMode;
	BYTE bMK_Index;
	BYTE bLenSK;
	BYTE *baSK;
	BYTE *baICV;
	USHORT usDataLen;
	BYTE *baData;
	BYTE *baMAC;
} CTOS_stDefMACStruc;

typedef struct{
    BYTE bKBPK_Index;
    USHORT usDataLen;
    BYTE *baData;
} CTOS_stDefTR31Struc;

typedef struct
{
    BYTE bMK_Index;
    BYTE bKey_Flag;
    BYTE *sComp1;
    BYTE *sComp2;
    BYTE *sComp3;
    BYTE *sCV;
} CTOS_stSaveDESMK16;

typedef struct
{
    BYTE bMK_Index;
    BYTE bKey_Flag;
    BYTE *sComp1;
    BYTE *sComp2;
    BYTE *sCV;
} CTOS_stSaveDESMK32;

typedef struct
{
    BYTE bMK_Index;
    BYTE bKey_Flag;
    BYTE *sComp1;
    BYTE *sComp2;
    BYTE *sCV;
} CTOS_stSaveDESMK48;

typedef struct
{
	BYTE bMK_Index;
	BYTE bKey_Flag;
    BYTE *sIK;
    BYTE *sKSN;
} CTOS_stSaveDUKPTIK;

typedef struct {
   BYTE bKeyType;
   BYTE bMKIndex;
   BYTE bC1Len;
   BYTE *baC1;
   BYTE bC2Len;
   BYTE *baC2;
   BYTE bC3Len;
   BYTE *baC3;
   BYTE bC4Len;
   BYTE *baC4;
}CTOS_stGenKeyCertifyed;

typedef struct
{	
	BYTE Index;
	BYTE RFU;
	BYTE KeyLen;    
	BYTE CVLen;
    BYTE *sComp1;
    BYTE *sComp2;
    BYTE *sComp3;
    BYTE *sCV;
}SAVE_KPK;

typedef struct
{	
	BYTE MK_Index;
	BYTE KeyFlag;
    BYTE MK_KeyLen;
    BYTE KPK_Index;
    BYTE *EMK;    
    BYTE *sCV;
    BYTE CVLen;
    BYTE RFU[3]; 
}SAVE_MK_BY_KPK;


void CTOS_KMSInitializeCryptoLib(void);
USHORT CTOS_KMSbEncryptData(CTOS_stDefEncDataStruc *para);
USHORT CTOS_KMSbCheckMK(BYTE bMKIndex);
USHORT CTOS_KMSbGetDUKPTKSN(BYTE bMKIndex, BYTE *pbKSN);
USHORT CTOS_KMSbSaveKeyCertifyed(BYTE *baCert);
USHORT CTOS_KMSbSaveRSACertKey(CTOS_stDefRSACertKey *para);
void CTOS_KMSSelfTest(BYTE *baPVR);
USHORT CTOS_SelfTestSetFlag(BYTE bFlag);
USHORT CTOS_KMSbGetEncPIN(CTOS_stDefEncPINStruc *para);
USHORT CTOS_KMSbDeleteKey(BYTE bMKIndex);
USHORT CTOS_KMSbMAC(CTOS_stDefMACStruc *para);
USHORT CTOS_KMSbSaveDESMK16(CTOS_stSaveDESMK16 *para);
USHORT CTOS_KMSbSaveDESMK32(CTOS_stSaveDESMK32 *para);
USHORT CTOS_KMSbSaveDESMK48(CTOS_stSaveDESMK48 *para);
USHORT CTOS_KMSbSaveDUKPTIK(CTOS_stSaveDUKPTIK *para);
USHORT CTOS_KMSbSaveDUKPTIK_T(CTOS_stSaveDUKPTIK *para);
USHORT CTOS_KMSGenKeyCertificate(CTOS_stGenKeyCertifyed *para, BYTE *baCertifyedKey);
USHORT CTOS_KMSSaveKPK(SAVE_KPK *pKPK);
USHORT CTOS_KMSSaveMKbyKPK(SAVE_MK_BY_KPK *pMKbyKPK);
USHORT CTOS_KMSUserDataWrite(ULONG Offset, BYTE *pData, USHORT Len);
USHORT CTOS_KMSUserDataRead(ULONG Offset, BYTE *pData, USHORT Len);
void CTOS_KMSReset(void);
void CTOS_KMSSetCurrAPID(USHORT apid);
void CTOS_KMSbKillAPKeys(BYTE bAPID);

//=============================================================================================================================
//
// File System Functions
//
//=============================================================================================================================

typedef struct
{
	BYTE Filename[15];
	ULONG Filesize;
	BYTE FileAttrib;
	ULONG UID;
} CTOS_FILE_ATTRIB;

typedef struct
{
	BYTE  Filename[15];
	ULONG Filesize;
	BYTE  FileStorageType;
	BYTE  RFU[4];
} CTOS_FILE_INFO;

USHORT CTOS_FileOpen(BYTE* caFileName, BYTE bStorageType, ULONG* pulFileHandle);
USHORT CTOS_FileOpenAttrib(BYTE* caFileName, BYTE bStorageType, ULONG* pulFileHandle ,BYTE bAttrib); //<VK>2011.5.3
USHORT CTOS_FileClose(ULONG ulFileHandle);
USHORT CTOS_FileDelete(BYTE* caFileName);
USHORT CTOS_FileGetSize(BYTE* caFileName, ULONG* pulFileSize);
USHORT CTOS_FileSeek(ULONG ulFileHandle, ULONG ulOffset, BYTE bOrigin);
USHORT CTOS_FileRead(ULONG ulFileHandle, BYTE* baBuffer, ULONG* pulActualLength);
USHORT CTOS_FileWrite(ULONG ulFileHandle, BYTE* baBuffer, ULONG ulBufferLength);
USHORT CTOS_FileDir(BYTE *baFileBuf, ULONG *pulFileSize, USHORT *pusLen);
USHORT CTOS_FileCut (BYTE* caFileName, ULONG ulFileLen);
USHORT CTOS_FileRename (BYTE* caFileName, BYTE* caFileNewName);
USHORT CTOS_FileGetPosition(ULONG ulFileHandle, ULONG *pulPosition);
USHORT CTOS_FileReopen(BYTE* caFileName, BYTE bStorageType, ULONG* pulFileHandle);
USHORT CTOS_FileSetAttrib(BYTE* caFileName, BYTE bAttrib);
USHORT CTOS_FileDirAttrib(CTOS_FILE_ATTRIB *pstFA, USHORT *pusFileNumber);
USHORT CTOS_FileDirA(CTOS_FILE_INFO *pstInfo , USHORT *pusLen);
USHORT CTOS_FileGetAttrib(BYTE* caFileName, CTOS_FILE_ATTRIB *pstFA);
USHORT CTOS_FileFormat(BYTE bType);
USHORT CTOS_FileGetFreeSpace(BYTE bStorgeType, ULONG* pulFreeSize);


//=============================================================================================================================
//
// RS232 Functions
//
//=============================================================================================================================

*/

//USHORT CTOS_RS232Open(BYTE bPort, ULONG ulBaudRate, BYTE bParity, BYTE bDataBits, BYTE bStopBits);
BINDING(CTOS_RS232Open)
{
    BYTE port          = luaL_checknumber(L, 1);
    ULONG baud         = luaL_checknumber(L, 2);
    const char *parity = luaL_checkstring(L, 3);
    BYTE dataBits      = luaL_checknumber(L, 4);
    BYTE stopBits      = luaL_checknumber(L, 5);

    // TODO this does not show up propery?! luaL_error(L, "CTOS_RS232Open auto errororororororororo delme")

    if (strlen(parity) > 1)
        return luaL_error(L, "CTOS_RS232Open: parity argument must be a string of length 1");

    switch (parity[0])
    {
      case 'E':
      case 'O':
      case 'N':
        break;
      default:
        return luaL_error(L, "CTOS_RS232Open: parity argument must be either 'E', 'O', or 'N'");
    }

    CTOS_CALL(RS232Open(port, baud, parity[0], dataBits, stopBits));
    return 0;
}

//USHORT CTOS_RS232TxReady(BYTE bPort);
BINDING(CTOS_RS232TxReady)
{
    BYTE port = luaL_checknumber(L, 1);

    USHORT result = CTOS_RS232TxReady(port);

    if (result == d_RS232_BUSY)
        lua_pushboolean(L, 0);
    else if (result == d_OK)
        lua_pushboolean(L, 1);
    else
        return luaL_error(L, "error from CTOS call CTOS_RS232TxReady, %d", (int)result);

    return 1;
}

//USHORT CTOS_RS232TxData(BYTE bPort, BYTE* baBuf, USHORT usLen);
BINDING(CTOS_RS232TxData)
{
    BYTE port = luaL_checknumber(L, 1);
    BYTE *message = (BYTE *)luaL_checkstring(L, 2);

    CTOS_CALL(RS232TxData(port, message, strlen(message)));
    return 0;
}

//USHORT CTOS_RS232RxReady(BYTE bPort, USHORT* pusLen);
BINDING(CTOS_RS232RxReady)
{
    BYTE port = luaL_checknumber(L, 1);
    USHORT length = 0;

    CTOS_CALL(RS232RxReady(port, &length));

    lua_pushnumber(L, length);

    return 1;
}

//USHORT CTOS_RS232RxData(BYTE bPort, BYTE* baBuf, USHORT* pusLen);
BINDING(CTOS_RS232RxData)
{
    BYTE port           = luaL_checknumber(L, 1);
    int requestedLength = luaL_optnumber(L, 2, -1);

    if (requestedLength <= 0)
    {
        USHORT rxReady = 0;
        CTOS_CALL(RS232RxReady(port, &rxReady));
        requestedLength = rxReady;
    }

    luaL_Buffer luaBuffer;
    luaL_buffinit(L, &luaBuffer);

    if (requestedLength < 0)
        return luaL_error(L, "buffer size %d < 0 for CTOS_RS232RxData", requestedLength);

    {
        void *cBuffer = luaL_prepbuffsize(&luaBuffer, requestedLength);
        USHORT receivedLength = requestedLength;

        CTOS_CALL(RS232RxData(port, cBuffer, &receivedLength));

        luaL_addsize(&luaBuffer, receivedLength);
    }

    luaL_pushresult(&luaBuffer);
    return 1;
}

//USHORT CTOS_RS232SetRTS(BYTE bPort, BYTE bOnOff);
BINDING(CTOS_RS232SetRTS)
{
    BYTE port = luaL_checknumber(L, 1);
    BYTE ready = lua_checkBoolean(L, 2);

    CTOS_CALL(RS232SetRTS(port, ready));

    return 0;
}

//USHORT CTOS_RS232GetCTS(BYTE bPort);
BINDING(CTOS_RS232GetCTS)
{
    BYTE port = luaL_checknumber(L, 1);

    USHORT result = CTOS_RS232GetCTS(port);

    if (result == d_ON)
        lua_pushboolean(L, 1);
    else if (result == d_OFF)
        lua_pushboolean(L, 0);
    else
        return luaL_error(L, "error from CTOS call CTOS_RS232GetCTS, %x", (int)result);

    return 0;
}

//USHORT CTOS_RS232FlushRxBuffer(BYTE bPort);
BINDING(CTOS_RS232FlushRxBuffer)
{
    BYTE port = luaL_checknumber(L, 1);

    CTOS_CALL(RS232FlushRxBuffer(port));

    return 0;
}

//USHORT CTOS_RS232FlushTxBuffer(BYTE bPort);
BINDING(CTOS_RS232FlushTxBuffer)
{
    BYTE port = luaL_checknumber(L, 1);

    CTOS_CALL(RS232FlushTxBuffer(port));

    return 0;
}

//USHORT CTOS_RS232Close(BYTE bPort);
BINDING(CTOS_RS232Close)
{
    BYTE port = luaL_checknumber(L, 1);

    CTOS_CALL(RS232Close(port));

    return 0;
}

/*

//=============================================================================================================================
//
// Modem API Functions
//
//=============================================================================================================================
USHORT CTOS_ModemOpen(BYTE bMode, BYTE bHandShake, BYTE bCountryCode);
USHORT CTOS_ModemClose(void);
USHORT CTOS_ModemDialup(BYTE *baNumber, USHORT usLen);
USHORT CTOS_ModemTxReady(void);
USHORT CTOS_ModemTxData(BYTE *baData, USHORT usLen);
USHORT CTOS_ModemRxReady(USHORT* pusLen);
USHORT CTOS_ModemRxData(BYTE* baData, USHORT* pusLen);
USHORT CTOS_ModemATCommand(BYTE* baCmd, USHORT usCmdLen, BYTE* pbResponse, USHORT* pusRespLen);
USHORT CTOS_ModemStatus(DWORD* pdwStatus);
USHORT CTOS_ModemHookOff(void);
USHORT CTOS_ModemHookOn(void);
USHORT CTOS_ModemSetDialPrecheck(BYTE bMode);
USHORT CTOS_ModemSetConfig(BYTE bTag, USHORT usValue);
USHORT CTOS_ModemReadConfig(BYTE bTag, USHORT* pusValue);
USHORT CTOS_ModemFlushRxData(void);
USHORT CTOS_ModemListen (BYTE bNumOfRing);


//=============================================================================================================================
//
// Modem TCP API Functions
//
//=============================================================================================================================
void CTOS_TCP_ModemInit(void);
USHORT CTOS_TCP_ModemOpen(BYTE bMode , BYTE bHandShake , BYTE bCountryCode);
USHORT CTOS_TCP_ModemClose(void);
USHORT CTOS_TCP_ModemDialup(BYTE *baPhone, BYTE *baID, BYTE *baPW, ULONG ulTimeout);
USHORT CTOS_TCP_ModemOnHook(void);
USHORT CTOS_TCP_ModemGetIP(BYTE *baIP);
USHORT CTOS_TCP_ModemSetIP(BYTE *baIP);
USHORT CTOS_TCP_ModemConnectEx(BYTE *bSocket, BYTE *baIP, USHORT usPort );
USHORT CTOS_TCP_ModemDisconnect(BYTE bSocket);
USHORT CTOS_TCP_ModemTx(BYTE bSocket , BYTE *baBuffer, USHORT usLen);
USHORT CTOS_TCP_ModemRx(BYTE bSocket , BYTE *baBuffer, USHORT *usLen);
USHORT CTOS_UDP_ModemTx(BYTE *baIP , USHORT usPort , BYTE *baBuffer, USHORT usLen);
USHORT CTOS_UDP_ModemRx(BYTE *bSrcDestIP, USHORT *usPort , BYTE *baBuffer, USHORT *usLen);
USHORT CTOS_TCP_ModemStatus(DWORD *pdwState);
*/

//=============================================================================================================================
//
// GPRS API Functions
//
//=============================================================================================================================


//void CTOS_TCP_GPRSInit(void);
BINDING(CTOS_TCP_GPRSInit)
{
    CTOS_TCP_GPRSInit();
    return 0;
}

//USHORT CTOS_TCP_GPRSOpen(BYTE *baIP, STR *strAPN, STR *baID, STR *baPW);
BINDING(CTOS_TCP_GPRSOpen)
{
    BYTE ip[4] = {0, 0, 0, 0};
    const char *ipAsString = luaL_optstring(L, 1, NULL);
    const char *apn        = luaL_checkstring(L, 2);
    const char *username   = luaL_checkstring(L, 3);
    const char *password   = luaL_checkstring(L, 4);

    if (ipAsString && strlen(ipAsString) > 0)
        return luaL_error(L, "The Lua binding for CTOS_TCP_GPRSOpen does not support the IP argument");

    CTOS_CALL(TCP_GPRSOpen(ip, (char *)apn, (char *)username, (char *)password));

    return 0;
}

//USHORT CTOS_TCP_GPRSClose(void);
BINDING(CTOS_TCP_GPRSClose)
{
    CTOS_CALL(TCP_GPRSClose());
    return 0;
}

//USHORT CTOS_TCP_GPRSClose_A(void);
BINDING(CTOS_TCP_GPRSClose_A)
{
    CTOS_CALL(TCP_GPRSClose_A());
    return 0;
}

//USHORT CTOS_TCP_GPRSGetIP(BYTE *baIP);
BINDING(CTOS_TCP_GPRSGetIP)
{
    BYTE ip[4] = {0, 0, 0, 0};
    char ipAsString[32];

    CTOS_CALL(TCP_GPRSGetIP(ip));

    sprintf(ipAsString, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

    lua_pushstring(L, ipAsString);
    return 1;
}

/*
USHORT CTOS_TCP_GPRSConnectEx(BYTE *bSocket, BYTE *baIP, USHORT usPort);
USHORT CTOS_TCP_GPRSDisconnect(BYTE bSocket);
USHORT CTOS_TCP_GPRSTx(BYTE bSocket, BYTE *baBuffer, USHORT usLen);
USHORT CTOS_TCP_GPRSRx(BYTE bSocket, BYTE *baBuffer, USHORT *usLen);
*/

//USHORT CTOS_TCP_GPRSStatus (DWORD* pdwState);
BINDING(CTOS_TCP_GPRSStatus)
{
    DWORD state = 0;
    USHORT result = CTOS_TCP_GPRSStatus(&state);

    // the return value can also holds important information in this case, so we return it instead of using CTOS_CALL
    if ( ! (result == d_OK || result == d_TCP_IO_PROCESSING))
        return luaL_error(L, "error from CTOS call 'CTOS_TCP_GPRSStatus(&state)': %d", (unsigned int)result);

    lua_pushnumber(L, state);
    lua_pushnumber(L, result);
    return 2;
}

/*

USHORT CTOS_TCP_GPRSSwitchAPN(BYTE *baIP, STR *strAPN, STR *baID, STR *baPW);   //SYNC
USHORT CTOS_TCP_GPRSSwitchAPN_A(BYTE *baIP, STR *strAPN, STR *baID, STR *baPW);  //ASYNC
USHORT CTOS_TCP_GPRSCancelTask(void *RFU);

//=============================================================================================================================
//
// Modem/GPRS Setting Functions
//
//=============================================================================================================================
USHORT CTOS_TCP_SetConnectTO(ULONG ulTime);
USHORT CTOS_TCP_GetConnectTO(ULONG *ulTime);
USHORT CTOS_TCP_SetRxAckTO(ULONG ulTime);	//NA
USHORT CTOS_TCP_GetRxAckTO(ULONG *ulTime); //NA
USHORT CTOS_TCP_SetRxTO(ULONG ulTime);
USHORT CTOS_TCP_GetRxTO(ULONG *ulTime);
USHORT CTOS_TCP_SetRetryCounter(USHORT usTime);
USHORT CTOS_TCP_GetRetryCounter(USHORT *usTime);
USHORT CTOS_PPP_SetTO(ULONG ulTime);	
USHORT CTOS_PPP_GetTO(ULONG *ulTime);
USHORT CTOS_PPP_SetRetryCounter(USHORT usTime);
USHORT CTOS_PPP_GetRetryCounter(USHORT *usTime);

//=============================================================================================================================
//
// Ethernet Functions
//
//=============================================================================================================================

USHORT CTOS_EthernetOpen(void);
USHORT CTOS_EthernetClose(void);
USHORT CTOS_EthernetTxReady(void);
USHORT CTOS_EthernetTxData(BYTE* baData, USHORT usLen);
USHORT CTOS_EthernetRxReady(USHORT* pusLen);
USHORT CTOS_EthernetRxData(BYTE* baData, USHORT* pusLen);
USHORT CTOS_EthernetConfigSet(BYTE bTag, BYTE* baValue, BYTE bLen);
USHORT CTOS_EthernetConfigGet(BYTE bTag, BYTE* baValue, BYTE* pbLen);
USHORT CTOS_EthernetStatus(DWORD* pdwStatus);
USHORT CTOS_EthernetConnect(void);
USHORT CTOS_EthernetConnectEx(BYTE* baDestIP, BYTE bIPLen, BYTE* baPort, BYTE bPortLen);
USHORT CTOS_EthernetDisconnect(void);
USHORT CTOS_EthernetPing(BYTE* baDestIP, BYTE bLen);
USHORT CTOS_EthernetFlushRxData(void);

USHORT CTOS_EthernetMSConnect(BYTE bMSSocket);
USHORT CTOS_EthernetMSConnectEx(BYTE bMSsocket, BYTE* baDestIP, BYTE bIPLen, BYTE* baPort, BYTE bPortLen);
USHORT CTOS_EthernetMSDisconnect(BYTE bMSSocket);
USHORT CTOS_EthernetMSTxReady(BYTE bMSSocket);
USHORT CTOS_EthernetMSTxData(BYTE bMSSocket, BYTE *baData, USHORT usLen);
USHORT CTOS_EthernetMSRxReady(BYTE bMSSocket, DWORD* pdwLen);
USHORT CTOS_EthernetMSRxData(BYTE bMSSocket, BYTE *baData, DWORD* pdwLen);
USHORT CTOS_EthernetURL2IP(BYTE* baDestURL, BYTE bURLLen, BYTE* baRspIP, BYTE* pbRspIPLen);
USHORT CTOS_EthernetIP2MAC(BYTE* baDestIP, BYTE bIPLen, BYTE* baRspMAC, BYTE* pbRspMACLen);
USHORT CTOS_EthernetConnectURL(void);
USHORT CTOS_EthernetConnectURLEx(BYTE* baDestURL, BYTE bURLLen);
USHORT CTOS_EthernetMSListen(BYTE bMSSocket, BYTE* baListenPort, BYTE bListenPortLen);
USHORT CTOS_EthernetMSUnlisten(BYTE bMSSocket);
USHORT CTOS_EthernetEnable(void);
USHORT CTOS_EthernetDisable(void);
USHORT CTOS_EthernetMSStatus(DWORD* pdwMSStatus);

//=============================================================================================================================
//
// USB Functions
//
//=============================================================================================================================

USHORT CTOS_USBSelectMode(BYTE bMode);
USHORT CTOS_USBSetCDCMode(void);
USHORT CTOS_USBSetSTDMode(void);
USHORT CTOS_USBSetVidPid(DWORD dwVidPid);


USHORT CTOS_USBOpen(void);
USHORT CTOS_USBClose(void);
USHORT CTOS_USBTxReady(void);
USHORT CTOS_USBTxData(BYTE* baSBuf, USHORT usSLen);
USHORT CTOS_USBRxReady(USHORT* pusRLen);
USHORT CTOS_USBRxData(BYTE* baRBuf, USHORT* pusRLen);
USHORT CTOS_USBTxFlush(void);
USHORT CTOS_USBRxFlush(void);

USHORT CTOS_USBHostOpen(IN USHORT usVendorID, IN USHORT usProductID);
USHORT CTOS_USBHostClose(void);
USHORT CTOS_USBHostTxData(IN BYTE* baSBuf, IN ULONG ulTxLen, IN ULONG ulMSec);
USHORT CTOS_USBHostRxData(OUT BYTE* baRBuf, IN OUT ULONG* ulRxLen, IN ULONG ulMSec);

USHORT CTOS_BaseUSBOpen(void);
USHORT CTOS_BaseUSBClose(void);
USHORT CTOS_BaseUSBTxReady(void);
USHORT CTOS_BaseUSBTxData(BYTE* baSBuf, USHORT usSLen);
USHORT CTOS_BaseUSBRxReady(USHORT* pusRLen);
USHORT CTOS_BaseUSBRxData(BYTE* baRBuf, USHORT* pusRLen);
USHORT CTOS_BaseUSBTxFlush(void);
USHORT CTOS_BaseUSBRxFlush(void);
USHORT CTOS_BaseUSBSetCDCMode(void);
USHORT CTOS_BaseUSBSetSTDMode(void);
USHORT CTOS_BaseUSBStatus(DWORD* pdwUSBStatus);
//=============================================================================================================================
//
// GSM Functions
//
//=============================================================================================================================

typedef struct
{
    BYTE bIndex;
    BYTE baName[20];
    BYTE bNameLen;
    BYTE baNumber[30];
    BYTE bNumberLen;
    BYTE bAddrType;
} stPhoneBook;

typedef struct
{
    BYTE bIndex;
    BYTE bType;
    BYTE baPhoneNumber[20];
    BYTE bPhoneNumberLen;
    BYTE baTimeStamp[20];
    BYTE bTimeStampLen;
    BYTE baMessage[300];
    USHORT usMessageLen;
} stSMS;

typedef struct
{
    BYTE bVp;
    BYTE bDcs;
    BYTE baDa[20];
    BYTE bDaLen;
    BYTE baMessage[300];
    USHORT usMessageLen;
} stSMS_Submit;

//System Commands
void CTOS_GSMReset(void);
USHORT CTOS_GSMOpen(ULONG ulBaud, BYTE bInit);
USHORT CTOS_GSMClose(void);
USHORT CTOS_GSMPowerOff(void);
USHORT CTOS_GSMPowerOn(void);
USHORT CTOS_GSMQueryOperatorName(BYTE *baName, BYTE *bpLen);
USHORT CTOS_GSMHangup(void);
USHORT CTOS_GSMSendData(BYTE *baData, USHORT usLen);
USHORT CTOS_GSMRecvData(BYTE *baData, USHORT *pusLen);
USHORT CTOS_GSMSendATCmd(BYTE *baCmd, ULONG ulLen, ULONG ulTimeout);	//USHORT CTOS_GSMSendATCmd(char *caCmd, UINT uiLen, UINT uiTimeout);
USHORT CTOS_GSMSignalQuality(BYTE *bpStrength);
USHORT CTOS_GSMSetBAND(BYTE bID);
USHORT CTOS_GSMGetBAND(BYTE *bID);

//SIM Commands
USHORT CTOS_GSMSelectSIM(BYTE bID);
USHORT CTOS_GSMGetCurrentSIM(BYTE* bID);
USHORT CTOS_SIMGetIMSI(BYTE *pbInfo);	//USHORT CTOS_SIMGetIMSI(char *caInfo);
USHORT CTOS_SIMCheckReady(void);

//PIN
USHORT CTOS_PINGetAuthStatus(void);
USHORT CTOS_PINVerify(BYTE bPinType, BYTE *baPin, BYTE bPinLen);
USHORT CTOS_PINCheckLock(BYTE locktype);
USHORT CTOS_PINLock(BYTE locktype, BYTE *pin, BYTE pinlen);
USHORT CTOS_PINUnLock(BYTE locktype, BYTE *pin, BYTE pinlen);
USHORT CTOS_PINUpdate(BYTE pintype, BYTE *oldpin, BYTE oldpinlen, BYTE *newpin, BYTE newpinlen);

//GSM
USHORT CTOS_GSMDial(BYTE *baNumber, BYTE bLen);
USHORT CTOS_GSMSwitchToCmdMode(void);
USHORT CTOS_GSMSwitchToDataMode(void);

//GPRS
USHORT CTOS_GPRSAttach(BYTE *baAPN, BYTE bAPNLen);
USHORT CTOS_GPRSDetach(void);
USHORT CTOS_GPRSGetRegState(BYTE *baState);
USHORT CTOS_GPRSPPPConnect(void);

//Phonebook
USHORT CTOS_PBSelect(BYTE bPhoneBookType, BYTE *pbUsed, BYTE *pbCapacity);
USHORT CTOS_PBRead(BYTE bIndex, stPhoneBook *pstPhoneBook);
USHORT CTOS_PBWrite(stPhoneBook *pstPhoneBook);
USHORT CTOS_PBDelete(BYTE bIndex);

//SMS
USHORT CTOS_SMSGetSCNumber(BYTE *baNumber, BYTE *pbLen);
USHORT CTOS_SMSSetSCNumber(BYTE *baNumber, BYTE bLen);
USHORT CTOS_SMSGetList(BYTE bStorage, stSMS *pstSMS, BYTE *pbNum);
USHORT CTOS_SMSRead(BYTE bIndex, stSMS *pstSMS);
USHORT CTOS_SMSDelete(BYTE bIndex);
USHORT CTOS_SMSSend(stSMS_Submit *pstSMS, BYTE *pbIndex);
USHORT CTOS_SMSSendPDU(BYTE *baPDU, ULONG ulLen, BYTE *pbIndex);

//=============================================================================================================================
//
// TMS Functions
//
//=============================================================================================================================
void	CTOS_TMSUtil(void);
void	CTOS_TMSUtil_Modem(BYTE bMode, BYTE bHandShake, BYTE bCountryCode, STR *strPhone, STR *strID, STR *strPW, ULONG ulTimeout);
void	CTOS_TMSUtil_GPRS(STR *strAPN, STR *strID, STR *strPW);
USHORT	CTOS_TMSConfig(IN BYTE bSetGet, IN BYTE bItem, INOUT BYTE *baValue);
USHORT	CTOS_TMSSetUploadFile(IN BYTE bRIndex, IN BYTE *baFileName, IN BYTE bDelete);
USHORT	CTOS_TMSGetUploadFile(IN BYTE bRIndex, IN BYTE *baFileName);
USHORT	CTOS_TMSGetStatus(OUT USHORT *usStatus);
void    CTOS_TMSUpdateByMMCI(BYTE *baMMCI, BYTE bShowUI);
USHORT  CTOS_TMSGetUpdateResult(void);

//=============================================================================================================================
//
// Font Functions
//
//=============================================================================================================================

USHORT CTOS_LanguageConfig(USHORT usLanguage,USHORT usFontSize,USHORT usFontStyle, BOOL boSetDefault);
USHORT CTOS_LanguageLCDFontSize(USHORT usFontSize,USHORT usFontStyle);
USHORT CTOS_LanguagePrinterFontSize(USHORT usFontSize,USHORT usFontStyle, USHORT usSetType);
USHORT CTOS_LanguageInfo(USHORT usIndex,USHORT* pusLanguage,USHORT* pusFontSize,USHORT* pusFontStyle);
USHORT CTOS_LanguageNum(USHORT* pusIndex);
USHORT CTOS_LanguagePrinterSelectASCII(USHORT usASCIIFontID);
USHORT CTOS_LanguageLCDSelectASCII(USHORT usASCIIFontID);
USHORT CTOS_LanguagePrinterGetFontInfo(USHORT* usASCIIFontID,USHORT* usFontSize,USHORT* usFontStyle);
USHORT CTOS_LanguageLCDGetFontInfo(USHORT* usASCIIFontID,USHORT* usFontSize,USHORT* usFontStyle);

//=============================================================================================================================
//
// LCD Functions
//
//=============================================================================================================================

// For CTOS_LCDGMenu().
enum TAttribute { 
    _taNormal  = 0x00,
    _taReverse = 0x01,
    //_taClrOne  = 0x02,
    _taRight   = 0x04,
    _taCenter  = 0x08,
    _taErrCode = 0x10,
    _taDelay   = 0x20
};
// For CTOS_LCDTMenu().
enum TAttribute_T { 
    _taTNormal  = 0x00,
    _taTReverse = 0x01,
    //_taTClrOne  = 0x02,
    _taTRight   = 0x04,
    _taTCenter  = 0x08,
    _taTErrCode = 0x10,
    _taTDelay   = 0x20
};

*/

// General
//USHORT CTOS_LCDSelectMode(BYTE bMode);
BINDING(CTOS_LCDSelectMode)
{
    CTOS_CALL(LCDSelectMode(luaL_checknumber(L, 1)));
    return 0;
}

//USHORT CTOS_LCDSetContrast(BYTE bValue);
BINDING(CTOS_LCDSetContrast)
{
    CTOS_CALL(LCDSetContrast(luaL_checknumber(L, 1)));
    return 0;
}

//USHORT CTOS_LCDReflesh(void)
BINDING(CTOS_LCDReflesh)
{
    CTOS_CALL(LCDReflesh());
    return 0;
}


// Graphic Mode

//USHORT CTOS_LCDGClearCanvas(void);
BINDING(CTOS_LCDGClearCanvas)
{
    CTOS_CALL(LCDGClearCanvas());
    return 0;
}

//USHORT CTOS_LCDGPixel(USHORT usX,USHORT usY, BOOL boPat);
BINDING(CTOS_LCDGPixel)
{
    USHORT x = luaL_checknumber(L, 1);
    USHORT y = luaL_checknumber(L, 2);
    BOOL pixel = lua_checkBoolean(L, 1);
    CTOS_CALL(LCDGPixel(x, y, pixel));
    return 0;
}

//USHORT CTOS_LCDGTextOut(USHORT usX, USHORT usY, UCHAR* pusBuf, USHORT usFontSize, BOOL boReverse);
BINDING(CTOS_LCDGTextOut)
{
    USHORT x        = luaL_checknumber(L, 1);
    USHORT y        = luaL_checknumber(L, 2);
    UCHAR *text     = (UCHAR *)luaL_checkstring(L, 3);
    USHORT fontSize = luaL_optint(L, 4, d_FONT_8x8);
    BOOL reverse    = lua_toboolean(L, 5);

    CTOS_CALL(LCDGTextOut(x, y, text, fontSize, reverse));
    return 0;
}

/*
USHORT CTOS_LCDGSetBox(USHORT usX, USHORT usY, USHORT usXSize, USHORT usYSize, BYTE bFill);
USHORT CTOS_LCDGShowPic(USHORT usX, USHORT usY, BYTE* baPat, USHORT usPatLen, USHORT usXSize);
USHORT CTOS_LCDGClearWindow(void);
USHORT CTOS_LCDGMoveWindow(USHORT usOffset);
USHORT CTOS_LCDGGetWindowOffset(void);
USHORT CTOS_LCDGMenu(BYTE bAttribute, STR *pbaHeaderString, STR *pcaItemString);
USHORT CTOS_LCDGMenuEx(BYTE bAttribute, STR *pbaHeaderString, STR *pcaItemString,USHORT usShowItem);
*/
// Text Mode

//USHORT CTOS_LCDTClearDisplay(void);
BINDING(CTOS_LCDTClearDisplay)
{
    CTOS_CALL(LCDTClearDisplay());
    return 0;
}

//USHORT CTOS_LCDTGotoXY(USHORT usX,USHORT usY);
BINDING(CTOS_LCDTGotoXY)
{
    USHORT x = luaL_checknumber(L, 1);
    USHORT y = luaL_checknumber(L, 2);
    CTOS_CALL(LCDTGotoXY(x, y));
    return 0;
}

//USHORT CTOS_LCDTWhereX(void);
BINDING(CTOS_LCDTWhereX)
{
    lua_pushnumber(L, CTOS_LCDTWhereX());
    return 1;
}

//USHORT CTOS_LCDTWhereY(void);
BINDING(CTOS_LCDTWhereY)
{
    lua_pushnumber(L, CTOS_LCDTWhereY());
    return 1;
}

//USHORT CTOS_LCDTPrint(UCHAR* sBuf);
BINDING(CTOS_LCDTPrint)
{
    const char *message = luaL_optstring(L, 1, "nil");

    USHORT result = CTOS_LCDTPrint((char*)message);
    if (result != d_OK && result != d_LCD_OUT_OF_RANGE)
        return luaL_error(L, "error from CTOS call 'CTOS_LCDTPrint', %d", (int)result);

    return 0;
}

//USHORT CTOS_LCDTPrintXY(USHORT usX, USHORT usY, UCHAR* pbBuf);
BINDING(CTOS_LCDTPrintXY)
{
    USHORT x = luaL_checknumber(L, 1);
    USHORT y = luaL_checknumber(L, 2);
    const char *message = luaL_optstring(L, 3, "nil");

    USHORT result = CTOS_LCDTPrintXY(x, y, (char*)message);
    if (result != d_OK && result != d_LCD_OUT_OF_RANGE)
        return luaL_error(L, "error from CTOS call 'CTOS_LCDTPrintXY', %d", (int)result);

    return 0;
}

//USHORT CTOS_LCDTPutch(UCHAR ch);
BINDING(CTOS_LCDTPutch)
{
    const char ch = lua_checkChar(L, 1);
    CTOS_CALL(LCDTPutch(ch));
    return 0;
}

//USHORT CTOS_LCDTPutchXY (USHORT usX, USHORT usY, UCHAR bChar);
BINDING(CTOS_LCDTPutchXY)
{
    USHORT x = luaL_checknumber(L, 1);
    USHORT y = luaL_checknumber(L, 2);
    const char ch = lua_checkChar(L, 1);
    CTOS_CALL(LCDTPutchXY(x, y, ch));
    return 0;
}

//USHORT CTOS_LCDTClear2EOL(void);
BINDING(CTOS_LCDTClear2EOL)
{
    CTOS_CALL(LCDTClear2EOL());
    return 0;
}

//USHORT CTOS_LCDTSetReverse(BOOL boReverse);
BINDING(CTOS_LCDTSetReverse)
{
    CTOS_CALL(LCDTSetReverse(lua_checkBoolean(L, 1)));
    return 0;
}

/*
USHORT CTOS_LCDTSelectFontSize(USHORT usFontSize);
USHORT CTOS_LCDTSetASCIIVerticalOffset(BOOL fVDirection, BYTE bVOffect);
USHORT CTOS_LCDTSetASCIIHorizontalOffset(BOOL fHDirection, BYTE bHOffect);
USHORT CTOS_LCDTMenu(BYTE bAttribute, STR *pbaHeaderString, STR *pcaItemString);


// User Mode
void CTOS_LCDUClearDisplay(void);
void CTOS_LCDUInit(BYTE bCXSize, BYTE bCYSize, BYTE bTWXSize, BYTE bTWYSize, BYTE *baPattern);
USHORT CTOS_LCDUPrintXY(USHORT usX, USHORT usY, BYTE *pbBuf);
USHORT CTOS_LCDUPrint(BYTE *pbBuf);
USHORT CTOS_UIKeypad(USHORT usX, USHORT usY, STR *pcaKeyboardLayout[], UCHAR ucCursorBlinkInterval, UCHAR ucDelayToNextChar, BOOL boEnableCursorMove, BOOL boOneRadixPointOnly, UCHAR ucDigitAfterRadixPoint, BYTE bPasswordMask, STR caData[], UCHAR ucDataLen);
//=============================================================================================================================
//
// Keyboard Functions
//
//=============================================================================================================================

*/

//USHORT CTOS_KBDGet(BYTE* pbKey);
BINDING(CTOS_KBDGet)
{
    BYTE key = 0;
    CTOS_CALL(KBDGet(&key));
    lua_pushnumber(L, key);
    return 1;
}

//USHORT CTOS_KBDHit(BYTE* pbKey);
BINDING(CTOS_KBDHit)
{
    BYTE key = 0;
    CTOS_CALL(KBDHit(&key));
    lua_pushnumber(L, key);
    return 1;
}

//USHORT CTOS_KBDSetSound(BYTE bOnOff);
BINDING(CTOS_KBDSetSound)
{
    CTOS_CALL(KBDSetSound(lua_checkBoolean(L, 1)));
    return 0;
}

//USHORT CTOS_KBDSetFrequence(USHORT usFreq, USHORT usDuration);
BINDING(CTOS_KBDSetFrequence)
{
    CTOS_CALL(KBDSetFrequence(luaL_checknumber(L, 1),
                              luaL_checknumber(L, 2)));
    return 0;
}


//USHORT CTOS_KBDInKey(BOOL* pboIsKeyTriggle);
BINDING(CTOS_KBDInKey)
{
    BOOL value;
    CTOS_CALL(KBDInKey(&value));
    lua_pushboolean(L, value);
    return 1;
}

//USHORT CTOS_KBDInKeyCheck(BYTE *pbKey);
BINDING(CTOS_KBDInKeyCheck)
{
    BYTE key;
    CTOS_CALL(KBDInKeyCheck(&key));
    lua_pushnumber(L, key);
    return 1;
}

//USHORT CTOS_KBDSetResetEnable(BOOL boIsEnable);
BINDING(CTOS_KBDSetResetEnable)
{
    CTOS_CALL(KBDSetResetEnable(lua_checkBoolean(L, 1)));
    return 0;
}

/*

//=============================================================================================================================
//
// Printer Functions
//
//=============================================================================================================================
typedef struct
{
	USHORT FontSize;
	USHORT X_Zoom;
	USHORT Y_Zoom;
	USHORT X_Space;
	USHORT Y_Space;
} CTOS_FONT_ATTRIB;

USHORT CTOS_PrinterPutString(UCHAR* baBuf);
USHORT CTOS_PrinterLogo(BYTE* baLogo, USHORT usXstart, USHORT usXsize, USHORT usY8Size);
USHORT CTOS_PrinterFline(USHORT usLines);
USHORT CTOS_PrinterStatus(void);

USHORT CTOS_PrinterBufferPutString(BYTE* pbPtr, USHORT usXPos, USHORT usYPos, BYTE *baStr, CTOS_FONT_ATTRIB* ATTRIB);
USHORT CTOS_PrinterBufferOutput(BYTE* pbPtr, USHORT usY8Len);
USHORT CTOS_PrinterSetWorkTime(USHORT usWorkTime, USHORT usCoolTime);
USHORT CTOS_PrinterSetHeatLevel(UCHAR bHeatLevel);

USHORT CTOS_PrinterCodaBarBarcode(IN USHORT x, IN USHORT y, IN BYTE *baCodeContent, IN BYTE bContentLen, IN BYTE bXExtend, IN BYTE bY8Extend, IN BOOL bShowChar);
USHORT CTOS_PrinterCode39Barcode(IN USHORT x, IN USHORT y, IN BYTE *baCodeContent, IN BYTE bContentLen, IN BYTE bXExtend, IN BYTE bY8Extend, IN BOOL bShowChar);
USHORT CTOS_PrinterEAN13Barcode(IN USHORT x, IN USHORT y, IN BYTE *baCodeContent, IN BYTE bXExtend, IN BYTE bY8Extend, IN BOOL bShowChar);

//=============================================================================================================================
//
// Printer Buffer Functions
//
//=============================================================================================================================
void CTOS_PrinterBufferEnable(void);
USHORT CTOS_PrinterBufferInit(BYTE* pPtr, USHORT usHeight);
USHORT CTOS_PrinterBufferSelectActiveAddress (BYTE *pPtr);
USHORT CTOS_PrinterBufferFill(USHORT usXStart, USHORT usYStart, USHORT usXEnd, USHORT usYEnd, BOOL fPat);
USHORT CTOS_PrinterBufferHLine(USHORT usXStart, USHORT usYStart, USHORT usXEnd, BOOL fPat);
USHORT CTOS_PrinterBufferVLine(USHORT usXStart, USHORT usYStart, USHORT usYEnd, BOOL fPat);
USHORT CTOS_PrinterBufferLogo(USHORT usXPos, USHORT usYPos, USHORT usWidth, USHORT usHeight, BYTE *baPat);
USHORT CTOS_PrinterBufferPixel(USHORT usXPos, USHORT usYPos,BOOL fPat);
USHORT CTOS_PrinterBufferEAN13Barcode(IN USHORT x, IN USHORT y, IN BYTE *baCodeContent, IN BYTE bXExtend, IN BYTE bY8Extend, IN BOOL bShowChar);
USHORT CTOS_PrinterBufferCodaBarBarcode(IN USHORT x, IN USHORT y, IN BYTE *baCodeContent, IN BYTE bContentLen, IN BYTE bXExtend, IN BYTE bY8Extend, IN BOOL bShowChar);
USHORT CTOS_PrinterBufferCode39Barcode(IN USHORT x, IN USHORT y, IN BYTE *baCodeContent, IN BYTE bContentLen, IN BYTE bXExtend, IN BYTE bY8Extend, IN BOOL bShowChar);
//=============================================================================================================================
//
// Smart Card  & Memory Card Functions
//
//=============================================================================================================================

USHORT CTOS_SCStatus(BYTE bID, BYTE* baStatus);
USHORT CTOS_SCPowerOff(BYTE bID);
USHORT CTOS_SCResetEMV(BYTE bID, BYTE bVolt, BYTE* baATR, BYTE* baATRLen, BYTE* baCardType);
USHORT CTOS_SCResetISO(BYTE bID, BYTE bVolt, BYTE* baATR, BYTE* baATRLen, BYTE* baCardType);
USHORT CTOS_SCSendAPDU(BYTE bID, BYTE* baSBuf, USHORT usSLen, BYTE* baRBuf, USHORT* pusRLen);

USHORT CTOS_SCSetPBOC(BOOL fEnable);
//Memory Card Functions
USHORT CTOS_SyncICCReset(UCHAR* baBuf,BYTE* bLen);

USHORT CTOS_44x2ReadMainMemory(BYTE bAddr,UCHAR* baBuf,USHORT* usLen);
USHORT CTOS_44x2WriteMainMemory(BYTE bAddr,UCHAR* baBuf,USHORT usLen);
USHORT CTOS_44x2ReadProtectionMemory(UCHAR* baBuf);
USHORT CTOS_44x2SetProtectionBit(UCHAR bAddr);
USHORT CTOS_44x2ReadSecurityMemory(UCHAR* baBuf);
USHORT CTOS_44x2ChangePSC(UCHAR* baBuf);
USHORT CTOS_44x2VerifyPSC(UCHAR* baBuf);	

USHORT CTOS_44x8WriteMemoryWithoutPBit(USHORT usAddr,UCHAR* baBuf,USHORT usLen);	
USHORT CTOS_44x8WriteMemoryWithPBit(USHORT usAddr,UCHAR* baBuf,USHORT usLen);
USHORT CTOS_44x8WritePBitWithDataCompare(USHORT usAddr,UCHAR* baBuf,USHORT usLen);
USHORT CTOS_44x8ReadMemoryWithoutPBit(USHORT usAddr,UCHAR* baBuf,USHORT* usLen);
USHORT CTOS_44x8ReadMemoryWithPBit(USHORT usAddr,UCHAR* baBuf,UCHAR* baPBit,USHORT* usLen);
USHORT CTOS_44x8VerifyPSC(UCHAR* baBuf);

USHORT CTOS_44x6ReadData(UCHAR bAddr,UCHAR* baBuf,USHORT* usLen);
USHORT CTOS_44x6WriteData(UCHAR bAddr,UCHAR* baBuf,USHORT usLen);
USHORT CTOS_44x6Reload(BOOL fCounterBak,BYTE bBitNum);
USHORT CTOS_44x6VerifyTSC(UCHAR* baBuf);
USHORT CTOS_44x6Auth(BOOL fExtAuth,BYTE bKey,BYTE bNumClk,BYTE* baChallenge,BYTE* baRepBits);

USHORT CTOS_I2CCReset(UCHAR* baBuf,BYTE* bLen);	
USHORT CTOS_I2CReadMemory(BYTE bCmd,BYTE bNumAddr,USHORT usAddr,BOOL fDummyRead,UCHAR* baBuf,USHORT* usLen);
USHORT CTOS_I2CWriteMemory(BYTE bCmd,BYTE bNumAddr,USHORT usAddr,UCHAR* baBuf,USHORT usLen);	

USHORT CTOS_SLE44x4ReadMemory(BYTE bAddr,UCHAR* baBuf,BYTE* bLen);
USHORT CTOS_SLE44x4VerifyUserCode(UCHAR* baBuf);
USHORT CTOS_SLE44x4VerifyMemoryCode(UCHAR* baBuf);
USHORT CTOS_SLE44x4WriteMemory(BYTE bAddr,UCHAR* baBuf,BYTE bLen);
USHORT CTOS_SLE44x4EraseMemory(BYTE bAddr,BYTE bLen);
USHORT CTOS_SLE44x4BlowFuse(void);
USHORT CTOS_SLE44x4EnterTestMode(void);
USHORT CTOS_SLE44x4ExitTestMode(void);

USHORT CTOS_GPM896ReadMemory(BYTE bAddr,UCHAR* baBuf,BYTE* bLen);
USHORT CTOS_GPM896VerifySecurityCode(UCHAR* baKey);
USHORT CTOS_GPM896WriteMemory(BYTE bAddr,UCHAR* baBuf,BYTE bLen);
USHORT CTOS_GPM896EraseMemory(BYTE bAddr, BYTE bLen);
USHORT CTOS_GPM896EraseArea1(UCHAR* baKey);
USHORT CTOS_GPM896EraseArea2(UCHAR* baKey);  

USHORT CTOS_SLE44x4ReadMemory(BYTE bAddr,UCHAR* baBuf,BYTE* bLen);
USHORT CTOS_SLE44x4VerifyUserCode(UCHAR* baBuf);
USHORT CTOS_SLE44x4VerifyMemoryCode(UCHAR* baBuf);
USHORT CTOS_SLE44x4WriteMemory(BYTE bAddr,UCHAR* baBuf,BYTE bLen);
USHORT CTOS_SLE44x4EraseMemory(BYTE bAddr,BYTE bLen);
USHORT CTOS_SLE44x4BlowFuse(void);
USHORT CTOS_SLE44x4EnterTestMode(void);
USHORT CTOS_SLE44x4ExitTestMode(void);

USHORT CTOS_GPM896ReadMemory(BYTE bAddr,UCHAR* baBuf,BYTE* bLen);
USHORT CTOS_GPM896VerifySecurityCode(UCHAR* baKey);
USHORT CTOS_GPM896WriteMemory(BYTE bAddr,UCHAR* baBuf,BYTE bLen);
USHORT CTOS_GPM896EraseMemory(BYTE bAddr, BYTE bLen);
USHORT CTOS_GPM896EraseArea1(UCHAR* baKey);
USHORT CTOS_GPM896EraseArea2(UCHAR* baKey);  

//=============================================================================================================================
//
// MSR Functions
//
//=============================================================================================================================

USHORT CTOS_MSRRead(BYTE* baTk1Buf, USHORT* pusTk1Len, BYTE* baTk2Buf, USHORT* pusTk2Len, BYTE* baTk3Buf, USHORT* pusTk3Len);
USHORT CTOS_MSRGetLastErr (BYTE* baTk1Err, BYTE* baTk2Err, BYTE* baTk3Err);

//=============================================================================================================================
//
// Power Saving Functions
//
//=============================================================================================================================
*/

//USHORT CTOS_PwrMngDisable(void);
BINDING(CTOS_PwrMngDisable)
{
    CTOS_CALL(PwrMngDisable());
    return 0;
}

//USHORT CTOS_PwrMngEnable(UCHAR bPwrType,ULONG ulPwrTime);
BINDING(CTOS_PwrMngEnable)
{
    CTOS_CALL(PwrMngEnable(luaL_checknumber(L, 1),
                           luaL_checknumber(L, 2)));
    return 0;
}

//USHORT CTOS_PwrMngGetParameter(UCHAR* bPwrEnable, UCHAR* bPwrType, ULONG* ulPwrTime);
BINDING(CTOS_PwrMngGetParameter)
{
    UCHAR state = 0, type = 0;
    ULONG time = 0;
    CTOS_CALL(PwrMngGetParameter(&state, &type, &time));
    lua_pushboolean(L, state);
    lua_pushnumber(L, type);
    lua_pushnumber(L, time);
    return 3;
}

//USHORT CTOS_PwrMngSetParameter(UCHAR bPwrEnable, UCHAR bPwrType, ULONG ulPwrTime);
BINDING(CTOS_PwrMngSetParameter)
{
    CTOS_CALL(PwrMngSetParameter(lua_checkBoolean(L, 1),
                                 luaL_checknumber(L, 2),
                                 luaL_checknumber(L, 3)));
    return 0;
}
 
///USHORT CTOS_PwrMngRegisterCallBack(void *PwrCallBack);
///USHORT CTOS_PwrMngUnRegisterCallBack(void);

//USHORT CTOS_PwrMngSetBusy(void);
BINDING(CTOS_PwrMngSetBusy)
{
    CTOS_CALL(PwrMngSetBusy());
    return 0;
}

//USHORT CTOS_PwrMngClearBusy(void);
BINDING(CTOS_PwrMngClearBusy)
{
    CTOS_CALL(PwrMngClearBusy());
    return 0;
}

//USHORT CTOS_PwrMngGetBusy(BOOL *fBusy);
BINDING(CTOS_PwrMngGetBusy)
{
    UCHAR state = 0;
    CTOS_CALL(PwrMngGetBusy(&state));
    lua_pushboolean(L, state);
    return 1;
}

//USHORT CTOS_PwrMngGetState(UCHAR *bState);
BINDING(CTOS_PwrMngGetState)
{
    UCHAR state = 0;
    CTOS_CALL(PwrMngGetState(&state));
    lua_pushboolean(L, state);
    return 1;
}

//USHORT CTOS_PowerMode(BYTE bMode);
BINDING(CTOS_PowerMode)
{
    CTOS_CALL(PowerMode(luaL_checknumber(L, 1)));
    return 0;
}

//USHORT CTOS_PowerSource(UCHAR* bSrc);
BINDING(CTOS_PowerSource)
{
    UCHAR source = 0;
    CTOS_CALL(PowerSource(&source));
    lua_pushnumber(L, source);
    return 1;
}

//USHORT CTOS_Get_Battery_Capacity_ByIC(BYTE* bPercentage);
//USHORT CTOS_Get_Battery_Capacity_ByADC(BYTE* bPercentage);

//USHORT CTOS_BatteryGetCapacityByIC(BYTE* bPercentage);
BINDING(CTOS_BatteryGetCapacityByIC)
{
    BYTE capacity = 0;
    CTOS_CALL(BatteryGetCapacityByIC(&capacity));
    lua_pushnumber(L, capacity);
    return 1;
}

//USHORT CTOS_BatteryGetCapacityByADC(BYTE* bPercentage);
BINDING(CTOS_BatteryGetCapacityByADC)
{
    BYTE capacity = 0;
    CTOS_CALL(BatteryGetCapacityByADC(&capacity));
    lua_pushnumber(L, capacity);
    return 1;
}

///USHORT CTOS_BatteryReadSN(BYTE* baSN, BYTE* bLen);

/*

//=============================================================================================================================
//
// ULDPM Functions
//
//=============================================================================================================================
//Should be padding to 128 bytes
typedef struct
{
	BYTE bFlag;
	BYTE bCAPType;
	BYTE baGeneratedDateTime[13];
	BYTE baName[21];
	BYTE baVersion[2];
	BYTE baCompany[21];
	BYTE baExeName[25];
	BYTE RFU[44];
} CTOS_stCAPInfo;
*/

/////////// System ///////////

//void CTOS_PowerOff(void);
BINDING(CTOS_PowerOff)
{
    CTOS_PowerOff();
    return 0;
}

//void CTOS_SystemReset(void);
BINDING(CTOS_SystemReset)
{
    CTOS_SystemReset();
    return 0;
}

/*

USHORT CTOS_GetSystemInfo(BYTE bID, BYTE baBuf[17]);
USHORT CTOS_GetSystemInfo_TMS(BYTE baBuf[17]);
USHORT CTOS_GetKeyHash(BYTE bKeyIndex, BYTE* baHash);
USHORT CTOS_GetBuringSN(BYTE *baBuringSN);
USHORT CTOS_SetBuringSN(BYTE *baBuringSN);
*/

//USHORT CTOS_GetFactorySN(BYTE *baFactorySN);
BINDING(CTOS_GetFactorySN)
{
    BYTE serial[16];
    bzero(serial, sizeof(serial));
    CTOS_CALL(GetFactorySN(serial));

    lua_pushstring(L, serial);
    return 1;
}

//USHORT CTOS_SetFactorySN(BYTE *baFactorySN);
BINDING(CTOS_SetFactorySN)
{
    char serial[16];
    bzero(serial, sizeof(serial));
    strncpy(serial, luaL_checkstring(L, 1), sizeof(serial) - 1);
    CTOS_CALL(SetFactorySN(serial));
    return 0;
}

/*
USHORT CTOS_SetUSBMode(BYTE baUSBMode);
void CTOS_UpdateFromMMCI(BYTE *baMMCI, BYTE bShowUI);
USHORT CTOS_UpdateFromCAPs(BYTE *pCAPData, DWORD dwDataLen, BYTE Indicator);
USHORT CTOS_UpdateGetResult(void);
*/

//USHORT CTOS_SystemWait(DWORD dwTimeout, DWORD dwWaitEvent, DWORD* pdwWakeupEvent);
BINDING(CTOS_SystemWait)
{
    DWORD timeout    = luaL_checknumber(L, 1) * 100; // sec -> 10 millisec
    DWORD waitEvents = luaL_checknumber(L, 2);
    DWORD triggeredEvents = 0;

    CTOS_CALL(SystemWait(timeout, waitEvents, &triggeredEvents));

    lua_pushnumber(L, triggeredEvents);
    return 1;
}

/*
USHORT CTOS_FactoryReset( BOOL IsNeedConfirm );
USHORT CTOS_CradleAttached(void);

/////////// PM ///////////
USHORT CTOS_APFind(BYTE *baAPName, USHORT *usAPIndex);
USHORT CTOS_APGet(USHORT usAPIndex, CTOS_stCAPInfo *stInfo);
USHORT CTOS_APSet(USHORT usAPIndex, BYTE bFlag);
USHORT CTOS_APDel(USHORT usAPIndex);
void CTOS_PM_Reset(void);
USHORT CTOS_SetFunKeyPassword(BYTE *baPasswordString, BOOL IsEnable);
USHORT CTOS_SetPMEnterPassword(BYTE *baPasswordString, BOOL IsEnable);

// AP Stack
USHORT CTOS_APJump(BYTE* baAPName);
USHORT CTOS_APCall(BYTE* baAPName);

/////////// ULD ///////////
USHORT CTOS_EnterDownloadMode(void);
USHORT CTOS_SetULDPassword(BYTE *baPasswordString);
USHORT CTOS_SealULD(BYTE *baPasswordString, BOOL IsEnable);
*/

#undef BINDING

static const luaL_Reg ctosapi_functions[] = {
#define BINDING(name) {#name, lua__##name},
    BINDING(CTOS_GetSerialNumber)

    BINDING(CTOS_TickGet)

    BINDING(CTOS_RS232Open)
    BINDING(CTOS_RS232TxReady)
    BINDING(CTOS_RS232TxData)
    BINDING(CTOS_RS232RxReady)
    BINDING(CTOS_RS232RxData)
    BINDING(CTOS_RS232SetRTS)
    BINDING(CTOS_RS232GetCTS)
    BINDING(CTOS_RS232FlushRxBuffer)
    BINDING(CTOS_RS232FlushTxBuffer)
    BINDING(CTOS_RS232Close)

    BINDING(CTOS_TCP_GPRSInit)
    BINDING(CTOS_TCP_GPRSOpen)
    BINDING(CTOS_TCP_GPRSClose)
    BINDING(CTOS_TCP_GPRSClose_A)
    BINDING(CTOS_TCP_GPRSGetIP)
    BINDING(CTOS_TCP_GPRSStatus)

    BINDING(CTOS_LCDSelectMode)
    BINDING(CTOS_LCDSetContrast)
    BINDING(CTOS_LCDReflesh)
    BINDING(CTOS_LCDGClearCanvas)
    BINDING(CTOS_LCDGPixel)
    BINDING(CTOS_LCDGTextOut)

    BINDING(CTOS_LCDTClearDisplay)

    BINDING(CTOS_LCDTGotoXY)
    BINDING(CTOS_LCDTWhereX)
    BINDING(CTOS_LCDTWhereY)
    BINDING(CTOS_LCDTPrint)
    BINDING(CTOS_LCDTPrintXY)
    BINDING(CTOS_LCDTPutch)
    BINDING(CTOS_LCDTPutchXY)
    BINDING(CTOS_LCDTClear2EOL)
    BINDING(CTOS_LCDTSetReverse)
    BINDING(CTOS_KBDGet)
    BINDING(CTOS_KBDHit)
    BINDING(CTOS_KBDSetSound)
    BINDING(CTOS_KBDSetFrequence)
    BINDING(CTOS_KBDInKey)
    BINDING(CTOS_KBDInKeyCheck)
    BINDING(CTOS_KBDSetResetEnable)
    BINDING(CTOS_PwrMngDisable)
    BINDING(CTOS_PwrMngEnable)
    BINDING(CTOS_PwrMngGetParameter)
    BINDING(CTOS_PwrMngSetParameter)
    ///USHORT CTOS_PwrMngRegisterCallBack(void *PwrCallBack);
    ///USHORT CTOS_PwrMngUnRegisterCallBack(void);
    BINDING(CTOS_PwrMngSetBusy)
    BINDING(CTOS_PwrMngClearBusy)
    BINDING(CTOS_PwrMngGetBusy)
    BINDING(CTOS_PwrMngGetState)

    BINDING(CTOS_PowerMode)
    BINDING(CTOS_PowerOff)
    BINDING(CTOS_SystemReset)
    BINDING(CTOS_PowerSource)
    BINDING(CTOS_BatteryGetCapacityByIC)
    BINDING(CTOS_BatteryGetCapacityByADC)

    BINDING(CTOS_GetFactorySN)
    BINDING(CTOS_SetFactorySN)

    BINDING(CTOS_SystemWait)

    {NULL, NULL}
#undef BINDING
};
