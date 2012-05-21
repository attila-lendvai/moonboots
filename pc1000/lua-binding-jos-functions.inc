// -*- mode: c; coding: utf-8 -*-

// NOTE: %x doesn't work here in the format string for an unknown reason
#define JOS_CALL(expr)                                                  \
    {                                                                   \
        int ___jos_result = (Lib_##expr);                               \
        if (___jos_result != 0)                                         \
            return luaL_error(L, "error from '" #expr "', %d", ___jos_result); \
    }

#define BINDING(name) static int lua__##name(lua_State* L)


//int    Lib_ReadLibVer(void);
BINDING(Lib_ReadLibVer)
{
    lua_pushnumber(L, Lib_ReadLibVer());
    return 1;
}

//=================================================
//               System functions
//==================================================

///int    Lib_AppInit(void);

//void   Lib_DelayMs(int ms);
BINDING(Lib_DelayMs)
{
    lua_Number delay = luaL_checknumber(L, 1);
    Lib_DelayMs(delay * 1000);
    return 0;
}

//void   Lib_Beep(void);
BINDING(Lib_Beep)
{
    Lib_Beep();
    return 0;
}

//void   Lib_Beef(BYTE mode,int DlyTimeMs);
BINDING(Lib_Beef)
{
    BYTE mode = luaL_checknumber(L, 1);
    int delay = luaL_checknumber(L, 2);
    Lib_Beef(mode, delay * 1000);
    return 0;
}

///int   Lib_SetDateTime(BYTE *datetime);

//int   Lib_GetDateTime(BYTE *datetime);
BINDING(Lib_GetDateTime)
{
    BYTE dateBuffer[8];

    JOS_CALL(GetDateTime(dateBuffer));

    return pushBCDDateAsLuaTable(L, dateBuffer);
}


//for timer event
//void  Lib_SetTimer(BYTE TimerNo, WORD Cnt100ms);
BINDING(Lib_SetTimer)
{
    BYTE timer;
    float timoutInSec;
    getLuaNumberArgument(1, timer, 0, 4);
    getLuaNumberArgument(2, timoutInSec, 1, 6553);

    Lib_SetTimer(timer, timoutInSec * 10);
    return 0;
}

//WORD  Lib_CheckTimer(BYTE TimerNo);
BINDING(Lib_CheckTimer)
{
    BYTE timer;
    ushort timeLeftInUnits;
    getLuaNumberArgument(1, timer, 0, 4);

    timeLeftInUnits = Lib_CheckTimer(timer);
    lua_pushnumber(L, ((double)timeLeftInUnits) / 10);

    return 1;
}

//void  Lib_StopTimer(BYTE TimerNo);
BINDING(Lib_StopTimer)
{
    BYTE timer;
    getLuaNumberArgument(1, timer, 0, 4);

    Lib_StopTimer(timer);
    return 0;
}

/*

//for all hardware and software version
void  Lib_ReadSN(BYTE *SerialNo);
int   Lib_EraseSN(void);
int   Lib_ReadVerInfo(BYTE *VerInfo); 

void  Lib_Reboot(void);
void  Lib_GetRand(uchar *out_rand ,uchar  out_len);
int   Lib_GetBatteryVolt(void); 
 
int   Lib_ShareMemWrite(DWORD offset, BYTE *buff, int wlen);
int   Lib_ShareMemRead(DWORD offset,BYTE *buff,int rlen);
*/

//BYTE  Lib_KbGetCh(void);
BINDING(Lib_KbGetCh)
{
    BYTE key = Lib_KbGetCh();
    lua_pushnumber(L, (lua_Number)key);
    return 1;
}

///BYTE  Lib_KbUnGetCh(void);

//int   Lib_KbCheck(void);
BINDING(Lib_KbCheck)
{
    lua_pushboolean(L, Lib_KbCheck() < 0 ? 0 : 1);
    return 1;
}

//void  Lib_KbFlush(void);
BINDING(Lib_KbFlush)
{
    Lib_KbFlush();
    return 0;
}

/*

int   Lib_KbSound(BYTE mode,WORD DlyTimeMs);
int   Lib_KbMute(BYTE mode);
int   Lib_KbGetStr(BYTE *str,BYTE minlen, BYTE maxlen,BYTE mode,WORD timeoutsec); 

*/

//void Lib_LcdCls(void);
BINDING(Lib_LcdCls)
{
    Lib_LcdCls();
    return 0;
}

//void Lib_LcdClrLine(BYTE startline, BYTE endline);
BINDING(Lib_LcdClrLine)
{
    BYTE start = luaL_checknumber(L, 1);
    BYTE end   = luaL_checknumber(L, 2);
    Lib_LcdClrLine(start, end);
    return 0;
}

/*

void Lib_LcdSetBackLight(BYTE mode);
void Lib_LcdSetGray(BYTE level);

*/

// void Lib_LcdGotoxy(BYTE x, BYTE y);
BINDING(Lib_LcdGotoxy)
{
    BYTE x = luaL_checknumber(L, 1);
    BYTE y = luaL_checknumber(L, 2);
    Lib_LcdGotoxy(x, y);
    return 0;
}


/*

int  Lib_LcdSetFont(BYTE AsciiFontHeight, BYTE ExtendFontHeight, BYTE Zoom);
int  Lib_LcdGetFont(BYTE *AsciiFontHeight, BYTE *ExtendFontHeight, BYTE *Zoom);
BYTE Lib_LcdSetAttr(BYTE attr);

*/

// int  Lib_Lcdprintf(char *fmt,...);
BINDING(Lib_Lcdprintf)
{
    // we can't call varargs in C, so we treat it as a simple non-formatting print function and do the formatting in lua
    // int argCount = lua_gettop(L);
    const char *message = luaL_checkstring(L, 1);
    lua_pushnumber(L, Lib_Lcdprintf((char *)message));
    return 1;
}


/*
void Lib_SprintfFloat(char *strOut, float fNumber, int iDecFractionLen);
void Lib_LcdPrintFloat(float fNumber, int iDecFractionLen);
*/

//void Lib_LcdPrintxy(BYTE col,BYTE row,BYTE mode,char *str,...);
BINDING(Lib_LcdPrintxy)
{
    // we can't call varargs in C, so we treat it as a simple non-formatting print function and do the formatting in lua
    BYTE x = luaL_checknumber(L, 1);
    BYTE y = luaL_checknumber(L, 2);
    BYTE mode = luaL_checknumber(L, 3);
    const char *message = luaL_checkstring(L, 4);

    Lib_LcdPrintxy(x, y, mode, (char *)message);

    return 0;
}

//void Lib_LcdDrawPlot (BYTE XO,BYTE YO,BYTE Color);
BINDING(Lib_LcdDrawPlot)
{
    BYTE x     = luaL_checknumber(L, 1);
    BYTE y     = luaL_checknumber(L, 2);
    BYTE color = luaL_checknumber(L, 3);
    Lib_LcdDrawPlot(x, y, color);
    return 0;
}

///void Lib_LcdDrawLogo(BYTE *pDataBuffer);

//void Lib_LcdDrawBox(BYTE x1,BYTE y1,BYTE x2,BYTE y2);
BINDING(Lib_LcdDrawBox)
{
    Lib_LcdDrawBox(luaL_checknumber(L, 1),
                   luaL_checknumber(L, 2),
                   luaL_checknumber(L, 3),
                   luaL_checknumber(L, 4));
    return 0;
}

///int  Lib_LcdRestore(BYTE mode);

//void  Lib_LcdGetSize(BYTE * x, BYTE *y);
BINDING(Lib_LcdGetSize)
{
    BYTE width, height;
    Lib_LcdGetSize(&width, &height);
    lua_pushnumber(L, width);
    lua_pushnumber(L, height);
    return 2;
}


//void  Lib_LcdDrawLine (BYTE x1,BYTE y1,BYTE x2,BYTE y2, BYTE byColor);
BINDING(Lib_LcdDrawLine)
{
    Lib_LcdDrawLine(luaL_checknumber(L, 1),
                    luaL_checknumber(L, 2),
                    luaL_checknumber(L, 3),
                    luaL_checknumber(L, 4),
                    luaL_checknumber(L, 5));
    return 0;
}

/*
BYTE Lib_LcdGetSelectItem(BYTE *pbyItemString, BYTE byItemNumber, BYTE byExitMode);
BYTE Lib_LcdGetSelectItemCE(BYTE *pbyChnItemString, BYTE *pbyEngItemString,
                     BYTE byItemNumber, BYTE byExitMode);
void Lib_LcdPrintfCE(char *pCHN , char *pEN);
void Lib_LcdPrintxyCE(BYTE col, BYTE row, BYTE mode, char *pCHN , char *pEN);
void Lib_LcdPrintxyExtCE(BYTE col, BYTE row, BYTE mode, char * pCHN, char * pEN, int iPara);

void  Lib_Des(uchar *input, uchar *output,uchar *deskey, int mode);
*/


//int  Lib_ComReset(BYTE port);
BINDING(Lib_ComReset)
{
    JOS_CALL(ComReset(luaL_checknumber(L, 1)));
    return 0;
}

///int  Lib_ComWrite(BYTE port,BYTE *writebyte,int write_len);

//int  Lib_ComRecvByte(BYTE port,BYTE *recv_byte,int waitms);
BINDING(Lib_ComRecvByte)
{
    BYTE port = luaL_checknumber(L, 1);
    int timeout = luaL_checknumber(L, 2) * 1000;
    BYTE data = 0;

    JOS_CALL(ComRecvByte(port, &data, timeout));

    lua_pushnumber(L, data);
    return 1;
}

//int  Lib_ComRecv(BYTE port,BYTE *recv_data,int max_len,int *recv_len,int waitms);
BINDING(Lib_ComRecv)
{
    BYTE port           = luaL_checknumber(L, 1);
    int requestedLength = luaL_checknumber(L, 2);
    int timeout         = luaL_checknumber(L, 3) * 1000;
    luaL_Buffer luaBuffer;
    luaL_buffinit(L, &luaBuffer);

    if (requestedLength <= 0)
        return luaL_error(L, "buffer size %d < 0 for Lib_ComRecv", requestedLength);

    {
        void *cBuffer = luaL_prepbuffsize(&luaBuffer, requestedLength);
        int receivedLength = 0;

        JOS_CALL(ComRecv(port, cBuffer, requestedLength, &receivedLength, timeout));

        luaL_addsize(&luaBuffer, receivedLength);
    }

    luaL_pushresult(&luaBuffer);
    return 1;
}

//int  Lib_ComSendByte(BYTE port,BYTE send_byte);
BINDING(Lib_ComSendByte)
{
    BYTE port = luaL_checknumber(L, 1);
    BYTE data = luaL_checknumber(L, 2);

    JOS_CALL(ComSendByte(port, data));
    return 0;
}

//int  Lib_ComSend(BYTE port,BYTE *send_data,int send_len);
BINDING(Lib_ComSend)
{
    BYTE port = luaL_checknumber(L, 1);
    BYTE *message = (BYTE *)luaL_checkstring(L, 2);

    JOS_CALL(ComSend(port, message, strlen(message)));
    return 0;
}

//int  Lib_ComClose(BYTE port);
BINDING(Lib_ComClose)
{
    JOS_CALL(ComClose(luaL_checknumber(L, 1)));
    return 0;
}

//int  Lib_ComOpen(BYTE port, char *ComPara);
BINDING(Lib_ComOpen)
{
    BYTE port = luaL_checknumber(L, 1);
    const char *config = luaL_checkstring(L, 2);

    JOS_CALL(ComOpen(port, (char *)config));
    return 0;
}

/*
int  Lib_UsbTempClear(uchar port);
int  Lib_UsbOpen(uchar port);
int  Lib_UsbClose(uchar port);
int  Lib_UsbReset(uchar port);
int  Lib_UsbSend(uchar port,uchar *send_data, int send_len);
int  Lib_UsbSendByte(uchar port,uchar send_byte);
int  Lib_UsbRecv(uchar port,uchar *recv_data,int max_len, int *recv_len,int  waitms);
int  Lib_UsbRecvByte(uchar port,uchar *recv_byte,int waitms);

typedef struct
{
     BYTE  fid;
     BYTE  attr;
     BYTE  type;
     char   name[17];
     DWORD  length;
} FILE_INFO;

//File operation functions
int  Lib_FileOpen(char *filename, BYTE mode);
int  Lib_FileRead(int fid, BYTE *dat, int len);
int  Lib_FileWrite(int fid, BYTE *dat, int len);
int  Lib_FileClose(int fid);
int  Lib_FileSeek(int fid, long offset, BYTE fromwhere);
long Lib_FileSize(char *filename);
long Lib_FileFreeSize(void);
int  Lib_FileTruncate(int fid,long len);
int  Lib_FileExist(char *filename);
int  Lib_FileInfo(FILE_INFO* finfo);
int  Lib_FileExOpen(char *filename, BYTE mode,BYTE* attr);
int  Lib_FileRemove(const char *filename);
int  Lib_FileGetEnv(char *name, BYTE *value);
int  Lib_FilePutEnv(char *name, BYTE *value);

typedef struct {
     uchar Name[32]; 
     uchar Version[16]; 
     uchar Num;
     uchar Priority;
     uchar NeedConfirm; 
     uchar Provider[32];
     uchar Descript[64];
     uchar LoadTime[14];
     ulong MainEntry;
     uchar Reserve[91];
}APP_MSG;


int  Lib_AppReadInfo(BYTE AppNo, APP_MSG* ai);
int  Lib_AppReadState(BYTE AppNo);
int  Lib_AppSetActive(BYTE AppNo, BYTE flag);
int  Lib_AppRun(BYTE AppNo);


int  Lib_PciWriteMKey(BYTE key_no,BYTE key_len,BYTE *key_data,BYTE mode);
int  Lib_PciWritePinKey(BYTE key_no,BYTE key_len,BYTE *key_data, BYTE mode, BYTE mkey_no);
int  Lib_PciWriteMacKey(BYTE key_no,BYTE key_len,BYTE *key_data, BYTE mode, BYTE mkey_no);
int  Lib_PciWriteDesKey(BYTE key_no,BYTE key_len,BYTE *key_data, BYTE mode, BYTE mkey_no);
int  Lib_PciDerivePinKey(BYTE mkey_n,BYTE pinkey_n1,BYTE pinkey_n2,BYTE mode);
int  Lib_PciDeriveMacKey(BYTE mkey_n,BYTE mackey_n1,BYTE mackey_n2,BYTE mode);
int  Lib_PciDeriveDesKey(BYTE mkey_n,BYTE deskey_n1,BYTE deskey_n2,BYTE mode);

int  Lib_PciGetPin(BYTE pinkey_n,BYTE min_len,BYTE max_len,BYTE *card_no,BYTE mode,BYTE *pin_block,WORD waittime_sec);
int  Lib_PciGetMac(BYTE mackey_n,WORD inlen,BYTE *indata,BYTE *macout,BYTE mode);
int  Lib_PciDes(BYTE deskey_n,BYTE *indata,BYTE *outdata,BYTE mode);

int  Lib_PciGetRnd (BYTE *rnd8);
int  Lib_PciAccessAuth(BYTE *auth_data,BYTE mode);

int  Lib_PrnInit(void);
int  Lib_PrnStr(char *str,...); 
int  Lib_PrnLogo(BYTE *logo);
int  Lib_PrnStart(void);
int  Lib_PrnCheck(void);
int  Lib_PrnStep(int pixel);
int  Lib_PrnSetLeftIndent(int x);
int  Lib_PrnSetSpace(BYTE x, BYTE y);
int  Lib_PrnGetFont(BYTE *AsciiFontHeight, BYTE *ExtendFontHeight, BYTE *Zoom);
int  Lib_PrnSetFont(BYTE AsciiFontHeight, BYTE ExtendFontHeight, BYTE Zoom);

int  Lib_PrnSetGray(BYTE nLevel);
int  Lib_PrnSetSpeed(int iSpeed);
int  Lib_PrnPaper(uchar mode, WORD pixel);
int  Lib_PrnReadVersion(uchar * VersionBuf);

int  Lib_FontFileCheck(void);
int  Lib_FontGetCharSet(BYTE *bCharSet, BYTE *bNum);
int  Lib_FontGetHeight(BYTE bCharSet, BYTE *bHeight, BYTE * bHeightNum);
int Lib_GetFontDotMatrix(BYTE *strIn, BYTE byFontHeight,
                     BYTE *pbyCharDotMatrix, BYTE *pbyOutCharLen);

void  Lib_WlsIoCtl(unsigned char ioname,unsigned char iomode);

typedef struct //__attribute__ ((__packed__))
{
	uchar  Command[4];
	ushort Lc;
	uchar  DataIn[512];
	ushort Le;
}APDU_SEND;

//#pragma pack()

typedef struct //__attribute__ ((__packed__))
{
	ushort LenOut;
	uchar  DataOut[512];
	uchar  SWA;
	uchar  SWB;
}APDU_RESP;

int  s_PiccInit(void);
int  Lib_PiccOpen(void);
int  Lib_PiccClose(void);
int  Lib_PiccCheck(uchar mode,uchar *CardType,uchar *SerialNo);
int  Lib_PiccCommand(APDU_SEND *ApduSend, APDU_RESP *ApduResp); 
void Lib_PiccHalt(void);
int  Lib_PiccReset(void);
int  Lib_PiccRemove(void);

*/

#undef BINDING

static const luaL_Reg josapi_functions[] = {
#define BINDING(name) {#name, lua__##name},
    BINDING(Lib_ReadLibVer)
    BINDING(Lib_DelayMs)
    BINDING(Lib_Beep)
    BINDING(Lib_Beef)

    BINDING(Lib_GetDateTime)
    BINDING(Lib_SetTimer)
    BINDING(Lib_CheckTimer)
    BINDING(Lib_StopTimer)

    BINDING(Lib_KbGetCh)
    BINDING(Lib_KbCheck)
    BINDING(Lib_KbFlush)

    BINDING(Lib_LcdCls)
    BINDING(Lib_LcdClrLine)
    BINDING(Lib_LcdGotoxy)
    BINDING(Lib_Lcdprintf)
    BINDING(Lib_LcdPrintxy)

    BINDING(Lib_LcdDrawPlot)
    BINDING(Lib_LcdDrawBox)
    BINDING(Lib_LcdGetSize)
    BINDING(Lib_LcdDrawLine)

    BINDING(Lib_ComReset)
    BINDING(Lib_ComRecvByte)
    BINDING(Lib_ComRecv)
    BINDING(Lib_ComSendByte)
    BINDING(Lib_ComSend)
    BINDING(Lib_ComClose)
    BINDING(Lib_ComOpen)

#undef BINDING

    {NULL, NULL}
};
