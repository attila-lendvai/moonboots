// -*- mode: c -*-

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
