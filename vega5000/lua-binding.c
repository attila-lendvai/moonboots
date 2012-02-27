#include <string.h>

#include <lua.h>
#include <lauxlib.h>

#include "ctosapi.h"

/// TODO: comments marked with three /// means that it's not converted yet

typedef struct luaL_Cst {
	const char* name;
	int value;
} luaL_Cst;

static luaL_Cst ctosapi_constants[] = {
#define BIND(name) {#name, name},
	BIND(d_TRUE)			// 1
	BIND(d_FALSE)			// 0

	BIND(d_ON)			// 1
	BIND(d_OFF)			// 0

//=============================================================================================================================
//
// System Functions
//
//=============================================================================================================================

	BIND(ID_BOOTSULD)			// 0
	BIND(ID_CRYPTO_HAL)			// 1
	BIND(ID_KMS)			// 2
	BIND(ID_LINUX_KERNEL)			// 3
	BIND(ID_SECURITY_KO)			// 4
	BIND(ID_SYSUPD_KO)			// 5
	BIND(ID_KMODEM_KO)			// 6
	BIND(ID_CADRV_KO)			// 7
	BIND(ID_CAUSB_KO)			// 8
	BIND(ID_LIBCAUART_SO)			// 9
	BIND(ID_LIBCAUSBH_SO)			// 10
	BIND(ID_LIBCAMODEM_SO)			// 11
	BIND(ID_LIBCAETHERNET_SO)			// 12
	BIND(ID_LIBCAFONT_SO)			// 13
	BIND(ID_LIBCALCD_SO)			// 14
	BIND(ID_LIBCAPRT_SO)			// 15
	BIND(ID_LIBCARTC_SO)			// 16
	BIND(ID_LIBCAULDPM_SO)			// 17
	BIND(ID_LIBCAPMODEM_SO)			// 18
	BIND(ID_LIBCAGSM_SO)			// 19
	BIND(ID_LIBCAEMVL2_SO)			// 20
	BIND(ID_LIBCAKMS_SO)			// 21
	BIND(ID_LIBCAFS_SO)			// 22
	BIND(ID_LIBCABARCODE_SO)			// 23
	BIND(ID_CRADLE_MP)			// 24 //for Cradle-Base Main-program

	BIND(ID_MAXIMUM)			// 24 // Maximum ID number of object files

//System Wait Event
	BIND(d_EVENT_KBD)			// 0x00000001
	BIND(d_EVENT_SC)			// 0x00000002
	BIND(d_EVENT_MSR)			// 0x00000004
	BIND(d_EVENT_MODEM)			// 0x00000008
	BIND(d_EVENT_ETHERNET)			// 0x00000010
	BIND(d_EVENT_COM1)			// 0x00000020
	BIND(d_EVENT_COM2)			// 0x00000040
	BIND(d_TIME_INFINITE)			// 0xFFFFFFFF

//=============================================================================================================================
//
// LED & Back Light Functions
//
//=============================================================================================================================

//LED  parameter
	BIND(d_LED1)			// 0	// => RED
	BIND(d_LED2)			// 1	// => ORANGE
	BIND(d_LED3)			// 2	// => GREEN

// Back Light parameter
	BIND(d_BKLIT_LCD)			// 0
	BIND(d_BKLIT_KBD)			// 1

//Time parameter
	BIND(TIMER_ID_1)			// 0
	BIND(TIMER_ID_2)			// 1
	BIND(TIMER_ID_3)			// 2
	BIND(TIMER_ID_4)			// 3

//=============================================================================================================================
//
// Buzzer Functions
//
//=============================================================================================================================

// Key Frequence
	BIND(C_KEY)			// 264
	BIND(D_KEY)			// 297
	BIND(E_KEY)			// 330
	BIND(F_KEY)			// 352
	BIND(G_KEY)			// 396
	BIND(A_KEY)			// 440
	BIND(B_KEY)			// 495
	BIND(C2_KEY)			// 528

//=============================================================================================================================
//
// Encryption/Decryption Functions
//
//=============================================================================================================================

	BIND(d_ENCRYPTION)			// 0
	BIND(d_DECRYPTION)			// 1

//=============================================================================================================================
//
// File System Functions
//
//=============================================================================================================================

// Storage Type
	BIND(d_STORAGE_FLASH)			// 0
	BIND(d_STORAGE_SRAM)			// 1

// Attribute
	BIND(d_FA_PRIVATE)			// 0
	BIND(d_FA_READ)			// 1		//	Read by other APs
	BIND(d_FA_WRITE)			// 2		//	Written by other APs
	BIND(d_FA_PUBLIC)			// 0xFF

	BIND(d_SEEK_FROM_BEGINNING)			// 0
	BIND(d_SEEK_FROM_CURRENT)			// 1
	BIND(d_SEEK_FROM_EOF)			// 2

//For the whole useless blocks in the file system.
	BIND(d_FORMAT_TYPE_0)			// 0

//For the last ## blocks, the flash FAT size assigned by this AP, in the file system.
	BIND(d_FORMAT_TYPE_1)			// 1

//=============================================================================================================================
//
// RS232 Functions
//
//=============================================================================================================================

	BIND(d_COM1)			// 0
	BIND(d_COM2)			// 1
	BIND(d_COM3)			// 2
	BIND(d_COM4)			// 3

//=============================================================================================================================
//
// Modem Functions
//
//=============================================================================================================================

// Para Mode
	BIND(d_M_MODE_AYNC_NORMAL)			// 1
	BIND(d_M_MODE_AYNC_FAST)			// 2
	BIND(d_M_MODE_SDLC_NORMAL)			// 3
	BIND(d_M_MODE_SDLC_FAST)			// 4

// HANDSHAKE
	BIND(d_M_HANDSHAKE_V90_AUTO_FB)			// 0
	BIND(d_M_HANDSHAKE_V90_ONLY)			// 1
	BIND(d_M_HANDSHAKE_V34_AUTO_FB)			// 2
	BIND(d_M_HANDSHAKE_V34_ONLY)			// 3
	BIND(d_M_HANDSHAKE_V32BIS_AUTO_FB)			// 4
	BIND(d_M_HANDSHAKE_V32BIS_ONLY)			// 5
	BIND(d_M_HANDSHAKE_V22BIS_ONLY)			// 6
	BIND(d_M_HANDSHAKE_V22_ONLY)			// 7
	BIND(d_M_HANDSHAKE_BELL_212)			// 8
	BIND(d_M_HANDSHAKE_BELL_103)			// 9
	BIND(d_M_HANDSHAKE_V21_ONLY)			// 10
	BIND(d_M_HANDSHAKE_V23)			// 11

// Country Code
	BIND(d_M_COUNTRY_TAIWAN)			// 0
	BIND(d_M_COUNTRY_CHINA)			// 1
	BIND(d_M_COUNTRY_USA)			// 2
	BIND(d_M_COUNTRY_GERMANY)			// 3
	BIND(d_M_COUNTRY_FRANCE)			// 4
	BIND(d_M_COUNTRY_MALAYSIA)			// 5
	BIND(d_M_COUNTRY_ENGLISH)			// 6
	BIND(d_M_COUNTRY_THAILAND)			// 7
	BIND(d_M_COUNTRY_SINGAPORE)			// 8
	BIND(d_M_COUNTRY_JAPEN)			// 9
	BIND(d_M_COUNTRY_TURKEY)			// 10
	BIND(d_M_COUNTRY_RUSSIA)			// 11
	BIND(d_M_COUNTRY_HONGKONG)			// 12
	BIND(d_M_COUNTRY_INDONESIA)			// 13
	BIND(d_M_COUNTRY_AUSTRALIA)			// 14
	BIND(d_M_COUNTRY_BRAZIL)			// 15
	BIND(d_M_COUNTRY_CANADA)			// 16
	BIND(d_M_COUNTRY_SOUTH_AFRICA)			// 17
	BIND(d_M_COUNTRY_SOUTH_KOREA)			// 18
	BIND(d_M_COUNTRY_QATAR)			// 19

// PreDialCheck
	BIND(d_M_PRECHECK_NO_DETECT)			// 0 
	BIND(d_M_PRECHECK_DIAL_TONE_DETECT_ONLY)			// 1
	BIND(d_M_PRECHECK_BUSY_DETECT_ONLY)			// 2
	BIND(d_M_PRECHECK_DETECT_BOTH)			// 3
	BIND(d_M_PRECHECK_DETECT_ALL)			// 4

// CONFIG
	BIND(d_M_CONFIG_DAILING_DURATION)			// 1
	BIND(d_M_CONFIG_MIN_ONHOOK_DURATION)			// 2
	BIND(d_M_CONFIG_PREDIAL_DELAY_TIME)			// 3
	BIND(d_M_CONFIG_DIAL_TONE_DETECT_DURATION)			// 4
	BIND(d_M_CONFIG_DIAL_TONE_MIN_ON_TIME)			// 5
	BIND(d_M_CONFIG_DTMF_ON_TIME)			// 6
	BIND(d_M_CONFIG_DTMF_OFF_TIME)			// 7
	BIND(d_M_CONFIG_BUSY_TONE_MIN_TOTAL_TIME)			// 8
	BIND(d_M_CONFIG_BUSY_TONE_DELTA_TIME)			// 9
	BIND(d_M_CONFIG_BUSY_TONE_MIN_ON_TIME)			// 10
	BIND(d_M_CONFIG_RINGBACK_TONE_MIN_TOTAL_TIME)			// 11
	BIND(d_M_CONFIG_RINGBACK_TONE_DELTA_TIME)			// 12
	BIND(d_M_CONFIG_RINGBACK_TONE_MIN_ON_TIME)			// 13
	BIND(d_M_CONFIG_ANSWER_TONE_WAIT_DURATION)			// 14
	BIND(d_M_CONFIG_BLIND_DIAL_DELAY_TIME)			// 15
	BIND(d_M_CONFIG_CARRIER_PRESENT_TIME)			// 16

// Status
	BIND(d_M_STATUS_MODEM_OPEN)			// 0x00000001
	BIND(d_M_STATUS_MODEM_ONLINE)			// 0x00000002
	BIND(d_M_STATUS_SDLC_MODE)			// 0x00000004
	BIND(d_M_STATUS_SDLC_ONLINE)			// 0x00000008
	BIND(d_M_STATUS_DIALING)			// 0x00000010
	BIND(d_M_STATUS_NO_DIAL_TONE)			// 0x00000020	// Dail up error
	BIND(d_M_STATUS_LINE_BUSY)			// 0x00000040	// Dail up error
	BIND(d_M_STATUS_RING_BACK)			// 0x00000080	// Dail up error
	BIND(d_M_STATUS_TX_BUSY)			// 0x00000100
	BIND(d_M_STATUS_REMOTE_NOT_ANSWER)			// 0x00000200	// Dail up error
	BIND(d_M_STATUS_NO_CARRIER)			// 0x00000400	// Dail up error
	BIND(d_M_STATUS_ALL_DATA_SENT)			// 0x00000800
	BIND(d_M_STATUS_RX_DATA_VALID)			// 0x00001000
	BIND(d_M_STATUS_LISTENING)			// 0x00002000
	BIND(d_M_STATUS_OTHER_ERROR)			// 0x00008000	//Dail up error
	BIND(d_M_STATUS_DATA_SENT_ERROR)			// 0x00010000	//Over USHORT range
	BIND(d_M_STATUS_DATA_RECEIVE_ERROR)			// 0x00020000	//Over USHORT range
	BIND(d_M_STATUS_TIMEOUT)			// 0x00040000	//Over USHORT range

//=============================================================================================================================
//
// Modem API Functions
//
//=============================================================================================================================


//=============================================================================================================================
//
// Modem TCP API Functions
//
//=============================================================================================================================

	BIND(TCP_MODEM_STATE_ONLINE)			// 0x00000002
	BIND(TCP_MODEM_STATE_DIALING)			// 0x00000010
	BIND(TCP_MODEM_STATE_CONNECTING)			// 0x00000020
	BIND(TCP_MODEM_STATE_SENDING)			// 0x00000040
	BIND(TCP_MODEM_STATE_RECEIVING)			// 0x00000080
	BIND(TCP_MODEM_STATE_DISCONNECTING)			// 0x00000100
	BIND(TCP_MODEM_STATE_ONHOOKING)			// 0x00000200


//=============================================================================================================================
//
// GPRS API Functions
//
//=============================================================================================================================

	BIND(TCP_GPRS_STATE_ESTABLISHED)			// 0x00000002
	BIND(TCP_GPRS_STATE_ESTABLISHING)			// 0x00000010
	BIND(TCP_GPRS_STATE_CONNECTING)			// 0x00000020
	BIND(TCP_GPRS_STATE_SENDING)			// 0x00000040
	BIND(TCP_GPRS_STATE_RECEIVING)			// 0x00000080
	BIND(TCP_GPRS_STATE_DISCONNECTING)			// 0x00000100
	BIND(TCP_GPRS_STATE_ONHOOKING)			// 0x00000200

//=============================================================================================================================
//
// Ethernet Functions
//
//=============================================================================================================================

// ConfigSet/Get
	BIND(d_ETHERNET_CONFIG_IP)			// 1
	BIND(d_ETHERNET_CONFIG_MASK)			// 2
	BIND(d_ETHERNET_CONFIG_GATEWAY)			// 3
	BIND(d_ETHERNET_CONFIG_HOSTIP)			// 4
	BIND(d_ETHERNET_CONFIG_HOSTPORT)			// 8
	BIND(d_ETHERNET_CONFIG_VERSION)			// 10
	BIND(d_ETHERNET_CONFIG_MAC)			// 11
	BIND(d_ETHERNET_CONFIG_DHCP)			// 12
	BIND(d_ETHERNET_CONFIG_AUTOCON)			// 14
	BIND(d_ETHERNET_CONFIG_UPDATE_EXIT)			// 15
	BIND(d_ETHERNET_CONFIG_DNSIP)			// 16
	BIND(d_ETHERNET_CONFIG_HOSTURL)			// 17
	BIND(d_ETHERNET_CONFIG_CONNTIMEOUT)			// 18

// Status
	BIND(d_STATUS_ETHERNET_CONNECTED)			// 0x00000001
	BIND(d_STATUS_ETHERNET_PHYICAL_ONLINE)			// 0x00000010
	BIND(d_STATUS_ETHERNET_RX_READY)			// 0x00000020
	BIND(d_STATUS_ETHERNET_TX_BUSY)			// 0x00000040
	BIND(d_STATUS_ETHERNET_COMMAND_MODE)			// 0x00000080

	BIND(ETHERNET_MS_CONNECTED_SOCKET0)			// 0x00000100
	BIND(ETHERNET_MS_CONNECTED_SOCKET1)			// 0x00000200
	BIND(ETHERNET_MS_CONNECTED_SOCKET2)			// 0x00000400

// Multi-socket
	BIND(d_ETHERNET_NUM_SOCKET)			// 3
	BIND(d_ETHERNET_MAX_SOCKET)			// 2
	BIND(d_ETHERNET_SOCKET_0)			// 0
	BIND(d_ETHERNET_SOCKET_1)			// 1
	BIND(d_ETHERNET_SOCKET_2)			// 2

//=============================================================================================================================
//
// USB Functions
//
//=============================================================================================================================

// Status
	BIND(d_STATUS_BASEUSB_MODECDC)			// 0x00000001

	BIND(d_USB_DEVICE_MODE)			// 0
	BIND(d_USB_HOST_MODE)			// 1

//=============================================================================================================================
//
// GSM Functions
//
//=============================================================================================================================

	BIND(d_GSM_GPRS_STATE_NOT_REG)			// 0
	BIND(d_GSM_GPRS_STATE_REG)			// 1
	BIND(d_GSM_GPRS_STATE_TRYING)			// 2
	BIND(d_GSM_GPRS_STATE_DENY)			// 3
	BIND(d_GSM_GPRS_STATE_UNKNOW)			// 4
	BIND(d_GSM_GPRS_STATE_ROAM)			// 5

	BIND(d_GSM_POWER_SAVE_ON)			// 1
	BIND(d_GSM_POWER_SAVE_OFF)			// 0

	BIND(d_GPRS_SIM1)			// 0
	BIND(d_GPRS_SIM2)			// 1

	BIND(d_GSM_900_1800)			// 0
	BIND(d_GSM_900_1900)			// 1
	BIND(d_GSM_850_1800)			// 2
	BIND(d_GSM_850_1900)			// 3

	BIND(d_GSM_PIN_PUK_1)			// 0
	BIND(d_GSM_PIN_PUK_2)			// 1

	BIND(d_GSM_AUTH_SC)			// 0
	BIND(d_GSM_AUTH_PS)			// 1
	BIND(d_GSM_AUTH_FD)			// 2

//Phonebook                      		
	BIND(d_GSM_PB_FD)			// 0
	BIND(d_GSM_PB_SM)			// 1
	BIND(d_GSM_PB_ON)			// 2
	BIND(d_GSM_PB_ME)			// 3
	BIND(d_GSM_PB_LD)			// 4
	BIND(d_GSM_PB_MC)			// 5
	BIND(d_GSM_PB_RC)			// 6
	BIND(d_GSM_PB_UNKNOW)			// 0xFF

//Addr Type                      		
	BIND(d_GSM_ADDR_NUMBER)			// 145
	BIND(d_GSM_ADDR_CHAR)			// 209
	BIND(d_GSM_ADDR_OTHER)			// 129

	BIND(d_GSM_SMS_REC_UNREAD)			// 0
	BIND(d_GSM_SMS_REC_READ)			// 1
	BIND(d_GSM_SMS_STO_UNSEND)			// 2
	BIND(d_GSM_SMS_STO_SEND)			// 3
	BIND(d_GSM_SMS_ALL)			// 4

//=============================================================================================================================
//
// TMS Functions
//
//=============================================================================================================================
	BIND(d_TMS_CONFIG_SET)			// 0x00
	BIND(d_TMS_CONFIG_GET)			// 0x01

// The option of configuration
	BIND(d_TMS_CONFIG_CONNECTION)			// 0x00
	BIND(d_TMS_CONFIG_IP)			// 0x01
	BIND(d_TMS_CONFIG_PORT)			// 0x02
	BIND(d_TMS_CONFIG_LOCALIP)			// 0x08
	BIND(d_TMS_CONFIG_TIME)			// 0x0B
	BIND(d_TMS_CONFIG_SHOWMSG)			// 0x0C
	BIND(d_TMS_CONFIG_AUTOCONNECT)			// 0x0D
	BIND(d_TMS_CONFIG_TIMEOUT)			// 0x0F
	BIND(d_TMS_CONFIG_APN)			// 0x11
	BIND(d_TMS_CONFIG_ISP_ID)			// 0x12
	BIND(d_TMS_CONFIG_ISP_PW)			// 0x13
	BIND(d_TMS_CONFIG_M_TCP_MODE)			// 0x14
	BIND(d_TMS_CONFIG_M_TCP_HANDSHAKE)			// 0x15
	BIND(d_TMS_CONFIG_M_TCP_COUNTRYCODE)			// 0x16
	BIND(d_TMS_CONFIG_M_TCP_PHONE)			// 0x17
	BIND(d_TMS_CONFIG_M_TCP_TIMEOUT)			// 0x18

// The type of connection
	BIND(d_CONNECTION_ETHERNET)			// 0x02
	BIND(d_CONNECTION_GPRS)			// 0x04
	BIND(d_CONNECTION_MODEMTCP)			// 0x08

//=============================================================================================================================
//
// Font Functions
//
//=============================================================================================================================
// Font language
// ASCII < 0x80 is world-wide same
// ASCII >=0x80 is language dependent 
	BIND(d_FONT_EMPTY)			// 0//No font 
	BIND(d_FONT_CHINESE_TAIWAN)			// 1028	//Chinese(Taiwan)
	BIND(d_FONT_CZECH)			// 1029	//Czech
	BIND(d_FONT_JAPANESE)			// 1041    //Japanese
	BIND(d_FONT_KOREAN)			// 1042    //Korean
	BIND(d_FONT_PORTUGUESE_BRAZIL)			// 1046    //Portuguese(Brazil)
	BIND(d_FONT_RUSSIAN)			// 1049    //Russian
	BIND(d_FONT_THAI)			// 1054    //Thai
	BIND(d_FONT_TURKISH)			// 1055    //Turkish
	BIND(d_FONT_FARSI)			// 1065    //Farsi
	BIND(d_FONT_VIETNAMESE)			// 1066    //Vietnamese
	BIND(d_FONT_CHINESE_PRC)			// 2052    //Chinese(PRC)
	BIND(d_FONT_PORTUGUESE_PORTUGAL)			// 2070	//Portuguese(Portugal) 
	BIND(d_FONT_SPANISH)			// 3082    //Spanish(Spain)
	BIND(d_FONT_ARABIC_QATAR)			// 16385 	//Arabic(Qatar)
	BIND(d_FONT_CHINESE_TAIWAN_PLUS)			// 64507	//Chinese(Taiwan) with Symbol
	BIND(d_FONT_DEFAULTASCII)			// 36864
// Font size
	BIND(d_FONT_8x8)			// 0x0808
	BIND(d_FONT_8x16)			// 0x0810
	BIND(d_FONT_16x16)			// 0x1010
	BIND(d_FONT_12x24)			// 0x0C18
	BIND(d_FONT_24x24)			// 0x1818

// for printer font type set
	BIND(d_FONT_NO_SET_TYPE)			// 0
	BIND(d_FONT_TYPE1)			// 1
	BIND(d_FONT_TYPE2)			// 2
	BIND(d_FONT_TYPE3)			// 3

// for font usage
	BIND(d_FONT_LCD_USE)			// 0x00
	BIND(d_FONT_PRINTER_USE)			// 0x01

//=============================================================================================================================
//
// LCD Functions
//
//=============================================================================================================================

// Graphic mode canvas x/y size
	BIND(d_LCD_CAVANS_X_SIZE)			// 128
	BIND(d_LCD_CAVANS_Y_SIZE)			// 256

// Graphic/text mode window x/y size
	BIND(d_LCD_WINDOW_X_SIZE)			// 128
	BIND(d_LCD_WINDOW_Y_SIZE)			// 64

// value for bMode
	BIND(d_LCD_GRAPHIC_MODE)			// 0
	BIND(d_LCD_TEXT_MODE)			// 1

// value for English Size
	BIND(d_LCD_FONT_8x8)			// d_FONT_8x8
	BIND(d_LCD_FONT_8x16)			// d_FONT_8x16
	BIND(d_LCD_FONT_12x24)			// d_FONT_12x24

// value for bFill
	BIND(d_LCD_FILL_0)			// 0
	BIND(d_LCD_FILL_1)			// 1
	BIND(d_LCD_FILL_XOR)			// 2

// for font shift
	BIND(d_LCD_SHIFTLEFT)			// 0
	BIND(d_LCD_SHIFTRIGHT)			// 1
	BIND(d_LCD_SHIFTUP)			// 0
	BIND(d_LCD_SHIFTDOWN)			// 1

// For CTOS_UIKeypad().
	///BIND(d_UI_KEYPAD_ENGLISH)			// {" 0", "qzQZ1", "abcABC2", "defDEF3", "ghiGHI4", "jklJKL5", "mnoMNO6", "prsPRS7", "tuvTUV8", "wxyWXY9", ":;/\\|?,.<>", ".!@#$%^&*()"}
	///BIND(d_UI_KEYPAD_NUM_RAD)			// {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", "."}
	///BIND(d_UI_KEYPAD_NUM)			// {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "", ""}

//=============================================================================================================================
//
// Keyboard Functions
//
//=============================================================================================================================

	BIND(d_KBD_INVALID)			// 0xFF

	BIND(d_KBD_1)			// '1'
	BIND(d_KBD_2)			// '2'
	BIND(d_KBD_3)			// '3'
	BIND(d_KBD_4)			// '4'
	BIND(d_KBD_5)			// '5'
	BIND(d_KBD_6)			// '6'
	BIND(d_KBD_7)			// '7'
	BIND(d_KBD_8)			// '8'
	BIND(d_KBD_9)			// '9'
	BIND(d_KBD_0)			// '0'

	BIND(d_KBD_F1)			// 'X'
	BIND(d_KBD_F2)			// 'Y'
	BIND(d_KBD_F3)			// 'I'
	BIND(d_KBD_F4)			// 'J'
	BIND(d_KBD_UP)			// 'U'
	BIND(d_KBD_DOWN)			// 'D'

	BIND(d_KBD_CANCEL)			// 'C'
	BIND(d_KBD_CLEAR)			// 'R'
	BIND(d_KBD_ENTER)			// 'A'
	BIND(d_KBD_DOT)			// 'Q'
	BIND(d_KBD_00)			// 'P'

//=============================================================================================================================
//
// Printer Functions
//
//=============================================================================================================================
	BIND(PAPER_X_SIZE)			// 384		

//=============================================================================================================================
//
// Printer Buffer Functions
//
//=============================================================================================================================
	BIND(PB_CANVAS_X_SIZE)			// 384
	BIND(PB_CANVAS_Y_SIZE)			// 80
	BIND(MAX_PB_CANVAS_NUM)			// 2	

//=============================================================================================================================
//
// Smart Card  & Memory Card Functions
//
//=============================================================================================================================

// value for bID
	BIND(d_SC_USER)			// 0
	BIND(d_SC_SAM1)			// 1
	BIND(d_SC_SAM2)			// 2
	BIND(d_SC_SAM3)			// 3
	BIND(d_SC_SAM4)			// 4	
	BIND(d_SC_MAX_SOCKET)			// 4

// value for bVolt
	BIND(d_SC_5V)			// 1
	BIND(d_SC_3V)			// 2
	BIND(d_SC_1_8V)			// 3

// mask for bStatus
	BIND(d_MK_SC_PRESENT)			// 1
	BIND(d_MK_SC_ACTIVE)			// 2

// value for CardType of CTOS_ATR
	BIND(d_SC_TYPE_T0)			// 0
	BIND(d_SC_TYPE_T1)			// 1
	BIND(d_SC_TYPE_MEM)			// 2
	BIND(d_SC_TYPE_I2C)			// 3
	BIND(d_SC_TYPE_RFU)			// 4

//=============================================================================================================================
//
// MSR Functions
//
//=============================================================================================================================

// output value for bTkxErr
	BIND(d_MSR_SUCCESS)			// 0x00
	BIND(d_MSR_STX_NOT_FOUND)			// 0x81
	BIND(d_MSR_NO_DATA)			// 0x82
	BIND(d_MSR_BUF_OVERFLOW)			// 0x83
	BIND(d_MSR_ETX_NOT_FOUND)			// 0x84
	BIND(d_MSR_LRC_NOT_FOUND)			// 0x85
	BIND(d_MSR_LRC_ERR)			// 0x86
	BIND(d_MSR_UNKNOWN_CHAR)			// 0x87

// mask for return value of CTOS_MSRRead()
	BIND(d_MK_MSR_TK1)			// 0x0001
	BIND(d_MK_MSR_TK2)			// 0x0002
	BIND(d_MK_MSR_TK3)			// 0x0004
	BIND(d_MK_MSR_TK1_ERR)			// 0x0010
	BIND(d_MK_MSR_TK2_ERR)			// 0x0020
	BIND(d_MK_MSR_TK3_ERR)			// 0x0040

//=============================================================================================================================
//
// Power Saving Functions
//
//=============================================================================================================================
// value for bMode
	BIND(d_PWR_STANDBY_MODE)			// 0x00
	BIND(d_PWR_SLEEP_MODE)			// 0x01
	BIND(d_PWR_REBOOT)			// 0x02
	BIND(d_PWR_POWER_OFF)			// 0x03
		
// for bPwrType
	BIND(d_PWRMNG_ENTER_STANDBY)			// 1
	BIND(d_PWRMNG_ENTER_SLEEP)			// 2

// for bPwrEnable
	BIND(d_PWRMNG_DISABLE)			// 0
	BIND(d_PWRMNG_ENABLE)			// 1



	BIND(d_PWR_ACTION_QUERY)			// 0x0
	BIND(d_PWR_ACTION_SET)			// 0x1



	BIND(d_PWR_STATUS_STANDBY)			// 0x0
	BIND(d_PWR_STATUS_SLEEP)			// 0x1
	BIND(d_PWR_STATUS_WORKING)			// 0x2
	BIND(d_PWR_STATUS_CANCEL)			// 0x3

// definition for power state 
	BIND(d_PWRMNG_STATE_UNKNOWN)			// 0
	BIND(d_PWRMNG_STATE_ENTERING_STANDBY)			// 1
	BIND(d_PWRMNG_STATE_STANDBY)			// 2
	BIND(d_PWRMNG_STATE_ENTERING_SLEEP)			// 3
	BIND(d_PWRMNG_STATE_SLEEP)			// 4
	BIND(d_PWRMNG_STATE_WORKING)			// 5


	BIND(PWR_QUERY_STANDBY_SIGNAL)			// 100
	BIND(PWR_QUERY_SLEEP_SIGNAL)			// 101
	BIND(PWR_SET_STANDBY_SIGNAL)			// 102
	BIND(PWR_SET_SLEEP_SIGNAL)			// 103
	BIND(PWR_SET_WORKING_SIGNAL)			// 104
	BIND(PWR_QUERY_CANCEL_SIGNAL)			// 105
	BIND(PWR_SET_CANCEL_SIGNAL)			// 106

// Power Source
	BIND(d_PWRSRC_DCJACK)			// 0
	BIND(d_PWRSRC_CRADLE)			// 1
	BIND(d_PWRSRC_BATTERY)			// 2

//=============================================================================================================================
//
// ULDPM Functions
//
//=============================================================================================================================

	BIND(d_MAX_AP_TABLE)			// 25

	BIND(d_AP_FLAG_DEF_SEL)			// 0x40

	BIND(PROGRAM_UPDATE_WHOLE)			// 0x00 
//#define PROGRAM_UPDATE_START		0x01    
//#define PROGRAM_UPDATE_CONTINUE		0x02 
//#define PROGRAM_UPDATE_FINAL		0x03 
//#define PROGRAM_FORCE_REBOOT		0x04  

#undef BIND
	{0, 0},
};


#define BINDING(name) static int lua__##name(lua_State* L)

/*

USHORT CTOS_GetSerialNumber(BYTE baBuf[16]);
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
ULONG CTOS_TickGet(void); // in 10 ms
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

USHORT CTOS_RS232Open(BYTE bPort, ULONG ulBaudRate, BYTE bParity, BYTE bDataBits, BYTE bStopBits);
USHORT CTOS_RS232TxReady(BYTE bPort);
USHORT CTOS_RS232TxData(BYTE bPort, BYTE* baBuf, USHORT usLen);
USHORT CTOS_RS232RxReady(BYTE bPort, USHORT* pusLen);
USHORT CTOS_RS232RxData(BYTE bPort, BYTE* baBuf, USHORT* pusLen);
USHORT CTOS_RS232SetRTS(BYTE bPort, BYTE bOnOff);
USHORT CTOS_RS232GetCTS(BYTE bPort);
USHORT CTOS_RS232FlushRxBuffer(BYTE bPort);
USHORT CTOS_RS232FlushTxBuffer(BYTE bPort);
USHORT CTOS_RS232Close(BYTE bPort);

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

//=============================================================================================================================
//
// GPRS API Functions
//
//=============================================================================================================================
void CTOS_TCP_GPRSInit(void);
USHORT CTOS_TCP_GPRSOpen(BYTE *baIP, STR *strAPN, STR *baID, STR *baPW);
USHORT CTOS_TCP_GPRSClose(void);
USHORT CTOS_TCP_GPRSClose_A(void);
USHORT CTOS_TCP_GPRSGetIP(BYTE *baIP);
USHORT CTOS_TCP_GPRSConnectEx(BYTE *bSocket, BYTE *baIP, USHORT usPort);
USHORT CTOS_TCP_GPRSDisconnect(BYTE bSocket);
USHORT CTOS_TCP_GPRSTx(BYTE bSocket, BYTE *baBuffer, USHORT usLen);
USHORT CTOS_TCP_GPRSRx(BYTE bSocket, BYTE *baBuffer, USHORT *usLen);
USHORT CTOS_TCP_GPRSStatus (DWORD* pdwState);
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

// General
USHORT CTOS_LCDSelectMode(BYTE bMode);
USHORT CTOS_LCDSetContrast(BYTE bValue);
USHORT CTOS_LCDReflesh(void);

// Graphic Mode
USHORT CTOS_LCDGClearCanvas(void);
USHORT CTOS_LCDGPixel(USHORT usX,USHORT usY, BOOL boPat);
USHORT CTOS_LCDGTextOut(USHORT usX, USHORT usY, UCHAR* pusBuf, USHORT usFontSize, BOOL boReverse);
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
    lua_pushnumber(L, CTOS_LCDTClearDisplay());
    return 1;
}

//USHORT CTOS_LCDTGotoXY(USHORT usX,USHORT usY);
BINDING(CTOS_LCDTGotoXY)
{
    USHORT x = luaL_checknumber(L, 1);
    USHORT y = luaL_checknumber(L, 2);
    lua_pushnumber(L, CTOS_LCDTGotoXY(x, y));
    return 1;
}

///USHORT CTOS_LCDTWhereX(void);
///USHORT CTOS_LCDTWhereY(void);

//USHORT CTOS_LCDTPrint(UCHAR* sBuf);
BINDING(CTOS_LCDTPrint)
{
    const char *message = luaL_checkstring(L, 1);
    lua_pushnumber(L, CTOS_LCDTPrint((char*)message));
    return 1;
}

//USHORT CTOS_LCDTPrintXY(USHORT usX, USHORT usY, UCHAR* pbBuf);
BINDING(CTOS_LCDTPrintXY)
{
    USHORT x = luaL_checknumber(L, 1);
    USHORT y = luaL_checknumber(L, 2);
    const char *message = luaL_checkstring(L, 3);
    lua_pushnumber(L, CTOS_LCDTPrintXY(x, y, (char*)message));
    return 1;
}

///USHORT CTOS_LCDTPutch(UCHAR ch);
///USHORT CTOS_LCDTPutchXY (USHORT usX, USHORT usY, UCHAR bChar);

//USHORT CTOS_LCDTClear2EOL(void);
BINDING(CTOS_LCDTClear2EOL)
{
    lua_pushnumber(L, CTOS_LCDTClear2EOL());
    return 1;
}

/*

USHORT CTOS_LCDTSetReverse(BOOL boReverse);
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
    BYTE key;
    USHORT result = CTOS_KBDGet(&key);
    lua_pushnumber(L, key);
    lua_pushnumber(L, result);
    return 2;
}

/*
USHORT CTOS_KBDHit(BYTE* pbKey);
USHORT CTOS_KBDSetSound(BYTE bOnOff);
USHORT CTOS_KBDSetFrequence(USHORT usFreq, USHORT usDuration);
USHORT CTOS_KBDInKey(BOOL* pboIsKeyTriggle);
USHORT CTOS_KBDInKeyCheck(BYTE *pbKey);
USHORT CTOS_KBDSetResetEnable(BOOL boIsEnable);

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
USHORT CTOS_PwrMngDisable(void);
USHORT CTOS_PwrMngEnable(UCHAR bPwrType,ULONG ulPwrTime);
USHORT CTOS_PwrMngGetParameter(UCHAR* bPwrEnable, UCHAR* bPwrType, ULONG* ulPwrTime);
USHORT CTOS_PwrMngSetParameter(UCHAR bPwrEnable, UCHAR bPwrType, ULONG ulPwrTime);
 
USHORT CTOS_PwrMngRegisterCallBack(void *PwrCallBack);
USHORT CTOS_PwrMngUnRegisterCallBack(void);
USHORT CTOS_PwrMngSetBusy(void);
USHORT CTOS_PwrMngClearBusy(void);
USHORT CTOS_PwrMngGetBusy(BOOL *fBusy);
USHORT CTOS_PwrMngGetState(UCHAR *bState);


USHORT CTOS_PowerMode(BYTE bMode);
void CTOS_PowerOff(void);
void CTOS_SystemReset(void);
USHORT CTOS_PowerSource(UCHAR* bSrc);

USHORT CTOS_Get_Battery_Capacity_ByIC(BYTE* bPercentage);
USHORT CTOS_Get_Battery_Capacity_ByADC(BYTE* bPercentage);

USHORT CTOS_BatteryGetCapacityByIC(BYTE* bPercentage);
USHORT CTOS_BatteryGetCapacityByADC(BYTE* bPercentage);
USHORT CTOS_BatteryReadSN(BYTE* baSN, BYTE* bLen);
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

/////////// System ///////////
void CTOS_SystemReset(void);
void CTOS_PowerOff(void);
USHORT CTOS_GetSerialNumber(BYTE baBuf[16]);
USHORT CTOS_GetSystemInfo(BYTE bID, BYTE baBuf[17]);
USHORT CTOS_GetSystemInfo_TMS(BYTE baBuf[17]);
USHORT CTOS_GetKeyHash(BYTE bKeyIndex, BYTE* baHash);
USHORT CTOS_GetBuringSN(BYTE *baBuringSN);
USHORT CTOS_SetBuringSN(BYTE *baBuringSN);
USHORT CTOS_GetFactorySN(BYTE *baFactorySN);
USHORT CTOS_SetFactorySN(BYTE *baFactorySN);
USHORT CTOS_SetUSBMode(BYTE baUSBMode);
void CTOS_UpdateFromMMCI(BYTE *baMMCI, BYTE bShowUI);
USHORT CTOS_UpdateFromCAPs(BYTE *pCAPData, DWORD dwDataLen, BYTE Indicator);
USHORT CTOS_UpdateGetResult(void);
USHORT CTOS_SystemWait(DWORD dwTimeout, DWORD dwWaitEvent, DWORD* pdwWakeupEvent);
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
#define BIND(name) {#name, lua__##name},
    BIND(CTOS_LCDTClearDisplay)
    BIND(CTOS_LCDTGotoXY)
    BIND(CTOS_LCDTPrint)
    BIND(CTOS_LCDTPrintXY)
    BIND(CTOS_LCDTClear2EOL)
    BIND(CTOS_KBDGet)
    {NULL, NULL}
#undef BIND
};


static luaL_Reg dummy[] = {
	{0, 0},
};

LUAMOD_API int luaopen_ctosapi(lua_State* L)
{
    // functions
    {
        char* _copy_storage[sizeof(ctosapi_functions)];
        memset(_copy_storage, 0, sizeof(_copy_storage));

        const luaL_Reg* original = ctosapi_functions;
        luaL_Reg* copy = (luaL_Reg*)_copy_storage;

        while (original -> name)
        {
            const char* name = original -> name;
            if (strncmp(name, "CTOS_", 5) == 0)
                name = name + 5;

            copy -> name = name;
            copy -> func = original -> func;
            ++original;
            ++copy;
        }
        luaL_newlib(L, (luaL_Reg*)_copy_storage);
    }

    // constants
    {
	for (luaL_Cst* it = ctosapi_constants; it -> name; ++it)
        {
            const char* name = it -> name;
            if (strncmp(name, "d_", 2))
                name = name + 2;

            lua_pushnumber(L, it->value);
            lua_setfield(L, -2, name);
        }
    }
    return 1;
}
