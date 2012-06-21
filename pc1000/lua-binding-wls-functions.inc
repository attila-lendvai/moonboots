// -*- mode: c; coding: utf-8 -*-

#define WLS_CALL_2(expr, cleanup)                                       \
    {                                                                   \
        int ___wls_result = (Wls_##expr);                               \
        if (___wls_result != WLS_OK)                                    \
        {                                                               \
            cleanup;                                                    \
            return luaL_error(L, "error from '" #expr "', code %d", ___wls_result); \
        }                                                               \
    }

#define WLS_CALL(expr) WLS_CALL_2(expr, )

#define BINDING(name) static int lua__##name(lua_State* L)

//int Wls_Init(uchar ModuleType);
BINDING(Wls_Init)
{
    BYTE moduleType = luaL_checknumber(L, 1);
    WLS_CALL(Init(moduleType));
    return 0;
}


//int Wls_SelectSim(int simno);
BINDING(Wls_SelectSim)
{
    int simNumber;
    getLuaNumberArgument(1, simNumber, 1, 2);
    WLS_CALL(SelectSim(simNumber - 1));
    return 0;
}

//int Wls_CheckSignal(int * pSignalLevel);
BINDING(Wls_CheckSignal)
{
    int signal = 0;
    WLS_CALL(CheckSignal(&signal));
    lua_pushnumber(L, signal);
    return 1;
}

//int Wls_CheckSim(void);
BINDING(Wls_CheckSim)
{
    lua_pushnumber(L, Wls_CheckSim());
    return 1;
}

//int Wls_InputSimPin(uchar * pin);
BINDING(Wls_InputSimPin)
{
    const unsigned char *pin = luaL_checkstring(L, 1);
    WLS_CALL(InputSimPin((unsigned char *)pin));
    return 0;
}

//int Wls_InputUidPwd(uchar * uid, uchar *pwd);
BINDING(Wls_InputUidPwd)
{
    const unsigned char *username = luaL_checkstring(L, 1);
    const unsigned char *password = luaL_checkstring(L, 2);
    WLS_CALL(InputUidPwd((unsigned char *)username, (unsigned char *)password));
    return 0;
}

//int Wls_Dial(uchar *dialNum);
BINDING(Wls_Dial)
{
    const unsigned char *numberOrAPN = luaL_checkstring(L, 1);
    WLS_CALL(Dial((unsigned char *)numberOrAPN));
    return 0;
}

//int Wls_CheckDial(void);
BINDING(Wls_CheckDial)
{
    lua_pushnumber(L, Wls_CheckDial());
    return 1;
}


/*
  there are better variants of these below with explicit socket number, so let's ignore these...

int Wls_TcpConnect(uchar *destIP, uchar *destPort);
int Wls_TcpPreConnect(uchar *destIP, uchar *destPort);
int Wls_TcpSend(uchar *sendData, ushort sendLen);
*/

//int Wls_TcpRecv(uchar *recvData, ushort *pRecvLen, ushort timeOutMs);
BINDING(Wls_TcpRecv)
{
    size_t expectedLength = luaL_checknumber(L, 1);
    size_t timeout = luaL_checknumber(L, 2) * 1000;

    ushort actualLength = expectedLength;
    uchar *buffer = malloc(expectedLength);

    WLS_CALL_2(TcpRecv(buffer, &actualLength, timeout),
               free(buffer));

    lua_pushlstring(L, buffer, actualLength);
    free(buffer);
    return 1;
}

/*
int Wls_TcpClose(void);
int Wls_CheckTcpLink(void);
*/

//int Wls_NetClose(void);
BINDING(Wls_NetClose)
{
    WLS_CALL(NetClose());
    return 0;
}

//int Wls_CheckNetLink (void);
BINDING(Wls_CheckNetLink)
{
    lua_pushnumber(L, Wls_CheckNetLink());
    return 1;
}

//int Wls_Reset(uchar ModuleType);
BINDING(Wls_Reset)
{
    BYTE moduleType = luaL_checknumber(L, 1);
    WLS_CALL(Reset(moduleType));
    return 0;
}

//int Wls_SendCmd(uchar *cmd, uchar *rsp, ushort rspLen, ushort timeOutMs);
BINDING(Wls_SendCmd)
{
    const uchar *command = luaL_checkstring(L, 1);
    ushort timeout;
    uchar response[128]; // FIXME: arbitrary number, potential buffer overflow
    getLuaNumberArgument(2, timeout, 0, 65535);

    WLS_CALL(SendCmd((uchar *)command, response, 128, timeout));

    lua_pushstring(L, response);

    return 1;
}


//int Wls_MTcpConnect(int socketNo, uchar *destIP, uchar *destPort);
BINDING(Wls_MTcpConnect)
{
    int socket = luaL_checknumber(L, 1);
    const uchar *ipAddress = luaL_checkstring(L, 2);
    uchar portAsString[32];
    uint port;
    getLuaNumberArgument(3, port, 0, 65535);
    snprintf(portAsString, 32, "%d", port);

    WLS_CALL(MTcpConnect(socket, (uchar *)ipAddress, portAsString));

    return 0;
}

//int Wls_MTcpPreConnect(int socketNo, uchar *destIP, uchar *destPort);
BINDING(Wls_MTcpPreConnect)
{
    int socket = luaL_checknumber(L, 1);
    const uchar *ipAddress = luaL_checkstring(L, 2);
    uchar portAsString[32];
    uint port;
    getLuaNumberArgument(3, port, 0, 65535);
    snprintf(portAsString, 32, "%d", port);

    WLS_CALL(MTcpPreConnect(socket, (uchar *)ipAddress, portAsString));

    return 0;
}

//int Wls_CheckMTcpLink(int socketNo);
BINDING(Wls_CheckMTcpLink)
{
    int socket = luaL_checknumber(L, 1);
    lua_pushnumber(L, Wls_CheckMTcpLink(socket));
    return 1;
}

//int Wls_MTcpSend(int socketNo, uchar *sendData, ushort sendLen);
BINDING(Wls_MTcpSend)
{
    int socket = luaL_checknumber(L, 1);
    size_t dataLength = 0;
    size_t maxChunkLength = 1536;
    const uchar *data = luaL_checklstring(L, 2, &dataLength);

    while (dataLength > 0)
    {
        size_t chunkLength = maxChunkLength;
        if (dataLength > maxChunkLength)
        {
            dataLength -= maxChunkLength;
        }
        else
        {
            chunkLength = dataLength;
            dataLength = 0;
        }

        WLS_CALL(MTcpSend(socket, (uchar *)data, chunkLength));

        data += chunkLength;
    }

    return 0;
}

//int Wls_MTcpRecv(int socketNo, uchar *recvData, ushort *pRecvLen, ushort timeOutMs);
BINDING(Wls_MTcpRecv)
{
    int socket = luaL_checknumber(L, 1);
    size_t expectedLength = luaL_checknumber(L, 2);
    size_t timeout = luaL_checknumber(L, 3) * 1000;

    ushort actualLength = expectedLength;
    uchar *buffer = malloc(expectedLength);

    WLS_CALL_2(MTcpRecv(socket, buffer, &actualLength, timeout),
               free(buffer));

    lua_pushlstring(L, buffer, actualLength);
    free(buffer);
    return 1;
}

//int Wls_MTcpClose(int socketNo);
BINDING(Wls_MTcpClose)
{
    int socket = luaL_checknumber(L, 1);
    WLS_CALL(MTcpClose(socket));
    return 0;
}

/*

int Wls_MTcpConnectExt(int socketNo, uchar *destIP, uchar *destPort, uchar typeProtocol);
int Wls_MTcpPreConnectExt(int socketNo, uchar *destIP, uchar *destPort, uchar typeProtocol); 

*/

//int Wls_GetVerInfo(uchar *wlsType, uchar *moduleHwVer, uchar *moduleSwVer, uchar *boardVer, uchar *driverVer); 
BINDING(Wls_GetVerInfo)
{
    // CHECKME this is fragile with all those fixed length buffers...
    uchar wlsType[2] = {0, 0};
    uchar moduleHwVer[33], moduleSwVer[33], boardVer[33], driverVer[33];
    WLS_CALL(GetVerInfo(wlsType, moduleHwVer, moduleSwVer, boardVer, driverVer));
    lua_pushstring(L, wlsType);
    lua_pushstring(L, moduleHwVer);
    lua_pushstring(L, moduleSwVer);
    lua_pushstring(L, boardVer);
    lua_pushstring(L, driverVer);
    return 5;
}

///int Wls_SwitchMode(int mode);

//int Wls_GetTime(uchar *BcdTime);
BINDING(Wls_GetTime)
{
    BYTE dateBuffer[8];

    WLS_CALL(GetTime(dateBuffer));

    return pushBCDDateAsLuaTable(L, dateBuffer);
}

///int Wls_SetTime(uchar *BcdTime);


#undef BINDING

static const luaL_Reg wlsapi_functions[] = {
#define BINDING(name) {#name, lua__##name},
    BINDING(Wls_Init)
    BINDING(Wls_SelectSim)
    BINDING(Wls_CheckSignal)
    BINDING(Wls_CheckSim)
    BINDING(Wls_InputSimPin)
    BINDING(Wls_InputUidPwd)
    BINDING(Wls_Dial)
    BINDING(Wls_CheckDial)
    BINDING(Wls_TcpRecv)
    BINDING(Wls_NetClose)
    BINDING(Wls_CheckNetLink)
    BINDING(Wls_Reset)
    BINDING(Wls_SendCmd)
    BINDING(Wls_MTcpConnect)
    BINDING(Wls_MTcpPreConnect)
    BINDING(Wls_CheckMTcpLink)
    BINDING(Wls_MTcpSend)
    BINDING(Wls_MTcpRecv)
    BINDING(Wls_MTcpClose)
    BINDING(Wls_GetVerInfo)
    BINDING(Wls_GetTime)
#undef BINDING
    {NULL, NULL}
};
