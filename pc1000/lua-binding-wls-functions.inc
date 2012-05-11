// -*- mode: c; coding: utf-8 -*-

#define WLS_CALL(expr)                                                  \
    {                                                                   \
        int ___wls_result = (Wls_##expr);                               \
        if (___wls_result != WLS_OK)                                    \
            return luaL_error(L, "error from '" #expr "', %d", ___wls_result); \
    }

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
    int simNumber = luaL_checknumber(L, 1);
    WLS_CALL(SelectSim(simNumber));
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
  there are better variants of these below with explicit socket number

int Wls_TcpConnect(uchar *destIP, uchar *destPort);
int Wls_TcpPreConnect(uchar *destIP, uchar *destPort);
int Wls_TcpSend(uchar *sendData, ushort sendLen);
int Wls_TcpRecv(uchar *recvData, ushort *pRecvLen, ushort timeOutMs);
int Wls_TcpClose(void);
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

//int Wls_CheckTcpLink(void);
BINDING(Wls_CheckTcpLink)
{
    lua_pushnumber(L, Wls_CheckTcpLink());
    return 1;
}

//int Wls_Reset(uchar ModuleType);
BINDING(Wls_Reset)
{
    BYTE moduleType = luaL_checknumber(L, 1);
    WLS_CALL(Reset(moduleType));
    return 0;
}

/*
int Wls_SendCmd(uchar *cmd, uchar *rsp, ushort rspLen, ushort timeOutMs);  
int Wls_MTcpConnect(int socketNo, uchar *destIP, uchar *destPort);
int Wls_MTcpPreConnect(int socketNo, uchar *destIP, uchar *destPort);
int Wls_CheckMTcpLink(int socketNo);
int Wls_MTcpSend(int socketNo, uchar *sendData, ushort sendLen);
int Wls_MTcpRecv(int socketNo, uchar *recvData, ushort *pRecvLen, ushort timeOutMs);
int Wls_MTcpClose(int socketNo); 
//int Wls_TcpConnectExt(uchar *destIP, uchar *destPort, uchar typeProtocol);
//int Wls_TcpPreConnectExt(uchar *destIP, uchar *destPort, uchar typeProtocol);
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
    BINDING(Wls_NetClose)
    BINDING(Wls_CheckNetLink)
    BINDING(Wls_CheckTcpLink)
    BINDING(Wls_Reset)
    BINDING(Wls_GetVerInfo)
    BINDING(Wls_GetTime)
#undef BINDING
    {NULL, NULL}
};
