#include <lua.h>
#include <lauxlib.h>

#include "josapi.h"
#include "wlsapi.h"

typedef struct luaL_Cst {
	const char* name;
	int value;
} luaL_Cst;

static luaL_Cst josapi_constants[] = {
#define BIND(name) {#name, name},
	BIND(APP_RUN_SPACE)			// 0x20500000
	BIND(APPMEM_SHARE_SPACE)			// 0x20400000

//for real time
	BIND(TIME_FORMAT_ERR)			// (-900)
	BIND(TIME_YEAR_ERR)			// (-901)
	BIND(TIME_MONTH_ERR)			// (-902)
	BIND(TIME_DAY_ERR)			// (-903)
	BIND(TIME_HOUR_ERR)			// (-904)
	BIND(TIME_MINUTE_ERR)			// (-905)
	BIND(TIME_SECOND_ERR)			// (-906)
	BIND(TIME_WEEK_ERR)			// (-907)
	BIND(TIME_SET_ERR)			// (-908)
	BIND(TIME_GET_ERR)			// (-909)
	BIND(TIME_RAMADDR_OVER)			// (-910)
	BIND(TIME_RAMLEN_OVER)			// (-911)

//=================================================
//               KeyBoard functions
//==================================================
	BIND(KEY1)			// 0x31
	BIND(KEY2)			// 0x32
	BIND(KEY3)			// 0x33
	BIND(KEY4)			// 0x34
	BIND(KEY5)			// 0x35
	BIND(KEY6)			// 0x36
	BIND(KEY7)			// 0x37
	BIND(KEY8)			// 0x38
	BIND(KEY9)			// 0x39
	BIND(KEY0)			// 0x30

	BIND(KEYCLEAR)			// 0x08
	BIND(KEYALPHA)			// 0x07
	BIND(KEYUP)			// 0x05
	BIND(KEYDOWN)			// 0x06
	BIND(KEYFN)			// 0x15
	BIND(KEYMENU)			// 0x14
	BIND(KEYENTER)			// 0x0d
	BIND(KEYCANCEL)			// 0x1b
	BIND(KEYBACKSPACE)			// 0x1c
	BIND(KEYPRINTERUP)			// 0x1d
	BIND(NOKEY)			// 0xff
	BIND(KEYDOT)			// 0x2e
	BIND(KEY00)			// 0x3a
	BIND(KEYOK)			// 0x3b

	BIND(KB_NoKey_Err)			// (-1000)
	BIND(KB_InputPara_Err)			// (-1010)
	BIND(KB_InputTimeOut_Err)			// (-1011)
	BIND(KB_UserCancel_Err)			// (-1012)
	BIND(KB_UserEnter_Exit)			// (-1013)

//=================================================
//               LCD Display functions
//==================================================
	BIND(ASCII)			// 0x00
	BIND(CFONT)			// 0x01
	BIND(REVER)			// 0x80 

	BIND(LCD_SetFont_Err)			// (-1500)
	BIND(LCD_GetFont_Err)			// (-1501)

//========================================
//     Encrypt and decrypt functions
//=========================================
	BIND(ENCRYPT)			// 1
	BIND(DECRYPT)			// 0
 

//=============================================
//     Asynchronism communication functions
//
//=============================================
	BIND(VCOM1)			// 1
	BIND(VCOM2)			// 2
	BIND(VCOM3)			// 3
	BIND(VCOM4)			// 4
	BIND(VCOM5)			// 5
	BIND(VCOM_DEBUG)			// 0

	BIND(RS232A)			// VCOM1
	BIND(RS232B)			// VCOM2
	BIND(COM1)			// VCOM1
	BIND(COM2)			// VCOM2

	BIND(COM_DEBUG)			// VCOM1 

	BIND(DEBUG_COM)			// 1  //RS232a
	BIND(AT_COM)			// 5 

	BIND(COM_INVALID_PARA)			// (-6501)
	BIND(COM_NOT_OPEN)			// (-6502)
	BIND(COM_INVALID_PORTNO)			// (-6503)
	BIND(COM_RX_TIMEOUT)			// (-6504)
	BIND(COM_TX_TIMEOUT)			// (-6505)
	BIND(COM_TX_MEMOVER)			// (-6506)
	BIND(COM_NO_EMPTYPORT)			// (-6507)


//============================================================
//       USB Host and Device Operation Functions
//============================================================
	BIND(USBD)			// 0
	BIND(USBH)			// 1 

	BIND(USB_SUCCESS)			// (0)
	BIND(USB_INVALID_PORT)			// (-6000)
	BIND(USB_NOT_CONNECT)			// (-6001)
	BIND(USB_RECV_TIMEOUT)			// (-6002)
	BIND(USB_SEND_OVERFLOW)			// (-6003)
	BIND(USB_RECV_OVERFLOW)			// (-6004)
	BIND(USB_SEND_TIMEOUT)			// (-6005)
	BIND(USBD_NOT_CONFIG)			// (-6100) 
 
//===========================================
//        Defined for file system
//
//============================================
	BIND(FILE_EXIST)			// (-8001)
	BIND(FILE_NOTEXIST)			// (-8002)
	BIND(FILE_MEM_OVERFLOW)			// (-8003)
	BIND(FILE_TOO_MANY_FILES)			// (-8004)
	BIND(FILE_INVALID_HANDLE)			// (-8005)
	BIND(FILE_INVALID_MODE)			// (-8006)
	BIND(FILE_NO_FILESYS)			// (-8007)
	BIND(FILE_NOT_OPENED)			// (-8008)
	BIND(FILE_OPENED)			// (-8009)
	BIND(FILE_END_OVERFLOW)			// (-8010)
	BIND(FILE_TOP_OVERFLOW)			// (-8011)
	BIND(FILE_NO_PERMISSION)			// (-8012)
	BIND(FILE_FS_CORRUPT)			// (-8013)
	BIND(FILE_INVALID_PARA)			// (-8014)
	BIND(FILE_WRITE_ERR)			// (-8015)
	BIND(FILE_READ_ERR)			// (-8016)
	BIND(FILE_NO_MATCH_RECORD)			// (-8017)

	BIND(O_RDWR)			// 0x01
	BIND(O_CREATE)			// 0x02

	BIND(FILE_SEEK_CUR)			// 0
	BIND(FILE_SEEK_SET)			// 1
	BIND(FILE_SEEK_END)			// 2

	BIND(PCI_Locked_Err)			// (-7000)
	BIND(PCI_KeyType_Err)			// (-7001)
	BIND(PCI_KeyLrc_Err)			// (-7002)
	BIND(PCI_KeyNo_Err)			// (-7003)
	BIND(PCI_KeyLen_Err)			// (-7004)
	BIND(PCI_KeyMode_Err)			// (-7005)

	BIND(PCI_InputLen_Err)			// (-7006)
	BIND(PCI_InputCancel_Err)			// (-7007)
	BIND(PCI_InputNotMatch_Err)			// (-7008)
	BIND(PCI_InputTimeOut_Err)			// (-7009)
	BIND(PCI_CallTimeInte_Err)			// (-7010)

	BIND(PCI_NoKey_Err)			// (-7011)
	BIND(PCI_WriteKey_Err)			// (-7012)
	BIND(PCI_ReadKey_Err)			// (-7013)
	BIND(PCI_RsaKey_HashErr)			// (-7014)
	BIND(PCI_DataLen_Err)			// (-7015)

	BIND(PCI_NoInput_Err)			// (-7016)
	BIND(PCI_AppNumOver_Err)			// (-7017)

	BIND(PCI_ReadMMK_Err)			// (-7020)
	BIND(PCI_WriteMMK_Err)			// (-7021)

//==================================================================
//    Printer API
//===================================================================
	BIND(PRN_BUSY)			// (-4001)
	BIND(PRN_NOPAPER)			// (-4002)
	BIND(PRN_DATAERR)			// (-4003)
	BIND(PRN_FAULT)			// (-4004)
	BIND(PRN_TOOHEAT)			// (-4005)
	BIND(PRN_UNFINISHED)			// (-4006)
	BIND(PRN_NOFONTLIB)			// (-4007)
	BIND(PRN_BUFFOVERFLOW)			// (-4008)
	BIND(PRN_SETFONTERR)			// (-4009)
	BIND(PRN_GETFONTERR)			// (-4010)
	BIND(PRN_BATFORBID)			// (-4011)
	BIND(PRN_OK)			// (0)

//==================================================================
//    FONT API
//===================================================================
	BIND(FONT_NOEXISTERR)			// (-1600)
	BIND(FONT_NOSUPPORTERR)			// (-1601)

//========================================================
//                   Wireless comm API
//==========================================================

	BIND(WLS_PWREN_IO)			// 1	
	BIND(WLS_RST_IO)			// 2	
	BIND(WLS_SIM1EN_IO)			// 3
	BIND(WLS_SIM2EN_IO)			// 4
	BIND(WLS_RESERVE1_IO)			// 5
	BIND(WLS_RESERVE2_IO)			// 6

	BIND(WLS_IO_LOW)			// 0
	BIND(WLS_IO_HIGH)			// 1

	BIND(PICC_OK)			// (0)
	BIND(PICC_ChipIDErr)			// (-3500)
	BIND(PICC_OpenErr)			// (-3501)
	BIND(PICC_NotOpen)			// (-3502)
	BIND(PICC_ParameterErr)			// (-3503)
	BIND(PICC_TxTimerOut)			// (-3504)
	BIND(PICC_RxTimerOut)			// (-3505)
	BIND(PICC_RxDataOver)			// (-3506)
	BIND(PICC_TypeAColl)			// (-3507)
	BIND(PICC_FifoOver)			// (-3508)
	BIND(PICC_CRCErr)			// (-3509)
	BIND(PICC_SOFErr)			// (-3510)
	BIND(PICC_ParityErr)			// (-3511)
	BIND(PICC_KeyFormatErr)			// (-3512) 
	BIND(PICC_RequestErr)			// (-3513)
	BIND(PICC_AntiCollErr)			// (-3514)
	BIND(PICC_UidCRCErr)			// (-3515)
	BIND(PICC_SelectErr)			// (-3516)
	BIND(PICC_RatsErr)			// (-3517)
	BIND(PICC_AttribErr)			// (-3518)
	BIND(PICC_HaltErr)			// (-3519)
	BIND(PICC_OperateErr)			// (-3520)
	BIND(PICC_WriteBlockErr)			// (-3521)
	BIND(PICC_ReadBlockErr)			// (-3522)
	BIND(PICC_AuthErr)			// (-3523)
	BIND(PICC_ApduErr)			// (-3524)
	BIND(PICC_HaveCard)			// (-3525) //ÓÐ¿¨
	BIND(PICC_Collision)			// (-3526) //¶à¿¨
	BIND(PICC_CardTyepErr)			// (-3527) //A ¿¨·¢ËÍRATS 
	BIND(PICC_CardStateErr)			// (-3528)
	BIND(PICC_PCBErr)			// (-3529)
	BIND(PICC_PROTOCOLErr)			// (-3530)
#undef BIND
	{0, 0},
};


#define BINDING(name) static int lua__##name(lua_State* L)

/*

int    Lib_ReadLibVer(void);
//=================================================
//               System functions
//==================================================
int    Lib_AppInit(void);
void   Lib_DelayMs(int ms); 
void   Lib_Beep(void);
void   Lib_Beef(BYTE mode,int DlyTimeMs);

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

BYTE  Lib_KbGetCh(void);
BYTE  Lib_KbUnGetCh(void);
int   Lib_KbCheck(void);
void  Lib_KbFlush(void);
int   Lib_KbSound(BYTE mode,WORD DlyTimeMs);
int   Lib_KbMute(BYTE mode);
int   Lib_KbGetStr(BYTE *str,BYTE minlen, BYTE maxlen,BYTE mode,WORD timeoutsec); 

void Lib_LcdCls(void);
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
    const char *message = luaL_checkstring(L, 1);
    lua_pushnumber(L, Lib_Lcdprintf((char *)message));
    return 1;
}


/*

void Lib_SprintfFloat(char *strOut, float fNumber, int iDecFractionLen);
void Lib_LcdPrintFloat(float fNumber, int iDecFractionLen);
void Lib_LcdPrintxy(BYTE col,BYTE row,BYTE mode,char *str,...);
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
#define BIND(name) {#name, lua__##name},
    BIND(Lib_Lcdprintf)
    BIND(Lib_LcdGotoxy)
    {NULL, NULL}
#undef BIND
};


static luaL_Reg dummy[] = {
	{0, 0},
};

LUAMOD_API int luaopen_josapi(lua_State* L)
{
    luaL_newlib(L, josapi_functions);
    {
	for (luaL_Cst* it = josapi_constants; it -> name; ++it)
        {
            lua_pushnumber(L, it->value);
            lua_setfield(L, -2, it->name);
        }
    }
    return 0;
}
