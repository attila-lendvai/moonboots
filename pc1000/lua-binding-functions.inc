// -*- mode: c -*-

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

/*
int   Lib_SetDateTime(BYTE *datetime);
int   Lib_GetDateTime(BYTE *datetime);

//for timer event
void  Lib_SetTimer(BYTE TimerNo, WORD Cnt100ms);
WORD  Lib_CheckTimer(BYTE TimerNo);
void  Lib_StopTimer(BYTE TimerNo);

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
    lua_pushnumber(L, Lib_KbGetCh());
    return 1;
}

/*

BYTE  Lib_KbUnGetCh(void);
int   Lib_KbCheck(void);
void  Lib_KbFlush(void);
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

/*

void Lib_LcdClrLine(BYTE startline, BYTE endline);
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

/*
void Lib_LcdDrawPlot (BYTE XO,BYTE YO,BYTE Color);
void Lib_LcdDrawLogo(BYTE *pDataBuffer);
void Lib_LcdDrawBox(BYTE x1,BYTE y1,BYTE x2,BYTE y2);
int  Lib_LcdRestore(BYTE mode);

void  Lib_LcdGetSize(BYTE * x, BYTE *y);
void  Lib_LcdDrawLine (BYTE x1,BYTE y1,BYTE x2,BYTE y2, BYTE byColor);

BYTE Lib_LcdGetSelectItem(BYTE *pbyItemString, BYTE byItemNumber, BYTE byExitMode);
BYTE Lib_LcdGetSelectItemCE(BYTE *pbyChnItemString, BYTE *pbyEngItemString,
                     BYTE byItemNumber, BYTE byExitMode);
void Lib_LcdPrintfCE(char *pCHN , char *pEN);
void Lib_LcdPrintxyCE(BYTE col, BYTE row, BYTE mode, char *pCHN , char *pEN);
void Lib_LcdPrintxyExtCE(BYTE col, BYTE row, BYTE mode, char * pCHN, char * pEN, int iPara);

void  Lib_Des(uchar *input, uchar *output,uchar *deskey, int mode);

int  Lib_ComReset(BYTE port);
int  Lib_ComWrite(BYTE port,BYTE *writebyte,int write_len);
int  Lib_ComRecvByte(BYTE port,BYTE *recv_byte,int waitms);
int  Lib_ComRecv(BYTE port,BYTE *recv_data,int max_len,int *recv_len,int waitms);
int  Lib_ComSendByte(BYTE port,BYTE send_byte);
int  Lib_ComSend(BYTE port,BYTE *send_data,int send_len);
int  Lib_ComClose(BYTE port);
int  Lib_ComOpen(BYTE port, char *ComPara);

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
    BINDING(Lib_KbGetCh)

    BINDING(Lib_LcdCls)
    BINDING(Lib_LcdGotoxy)
    BINDING(Lib_Lcdprintf)
    BINDING(Lib_LcdPrintxy)
    {NULL, NULL}
#undef BINDING
};
