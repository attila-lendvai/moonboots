// -*- mode: c; coding: utf-8 -*-

BIND(TM_WLS)			// 3               //无线定时器，用于COM 接收超时

BIND(SIM_NO_MAX)			// 2
BIND(SIM_NO_1)			// 0
BIND(SIM_NO_2)			// 1

//define recv data type
BIND(NORMAL_DATA)			// 0x00 
BIND(AT_CMD)			// 0x01 

//Module status related macros
BIND(WLS_UNKNOWN)			// 0x00 
BIND(WLS_GPRS_SIM340W)			// 0x01  
BIND(WLS_GPRS_G610)			// 0x02


BIND(TCP_PROTOCOL)			// 0
BIND(UDP_PROTOCOL)			// 1

BIND(MAXIDLEN)			// 64
BIND(MAXPWDLEN)			// 16

BIND(PORT_CLOSED)			// 0x00
BIND(PORT_OPENED)			// 0x01 

BIND(PSEND_OK)			// 0x00    //serial port send successfully
BIND(PSEND_ERR)			// 0x01    //serial port send error
BIND(PRECV_UNOPEN)			// (-2)
BIND(PRECV_ERR)			// (-1)    //serial port receive error
BIND(PRECV_TIMEOUT)			// (-6504) //serial port receive timeout

//Signal macros
BIND(SIGNAL_VERY_STRONG)			// 0x00
BIND(SIGNAL_STRONG)			// 0x01
BIND(SIGNAL_NORMAL)			// 0x02
BIND(SIGNAL_WEAK)			// 0x03
BIND(SIGNAL_VERY_WEAK)			// 0x04
BIND(NO_SIGNAL)			// 0x05

//**********************************************************************
//General macros
BIND(WLS_OK)			// (0)         //return successfully
BIND(WLS_ERR)			// (-5501)     //error occurred
BIND(WLS_NORSP)			// (-5502)     //no response from the module
BIND(WLS_RSPERR)			// (-5503)     //"ERROR" is return from the module
BIND(WLS_NOSIM)			// (-5504)     //SIM/UIM card is not inserted or not detected
BIND(WLS_NEEDPIN)			// (-5505)     //SIM PIN is required
BIND(WLS_NEEDPUK)			// (-5506)     //SIM PUK is required
BIND(WLS_SIMBLOCKED)			// (-5507)     //SIM Card is permanently blocked
BIND(WLS_SIMERR)			// (-5508)     //SIM card does not exist or needs SIM PIN
BIND(WLS_PINERR)			// (-5509)     //SIM PIN is incorrect
BIND(WLS_NOTALLOWED)			// (-5510)     //operation not allowed
BIND(WLS_PARAMERR)			// (-5511)     //parameter error
BIND(WLS_USERORPWDERR)			// (-5512)     //user name or password error
BIND(WLS_SNLTOOWEAK)			// (-5513)     //the signal is too weak, please check the antenna
BIND(WLS_LINKCLOSED)			// (-5514)     //the module is offline
BIND(WLS_LINKOPENED)			// (-5515)     //the module is online
BIND(WLS_LINKOPENING)			// (-5516)     //the module is connecting to the network
BIND(WLS_TCPCLOSED)			// (-5517)     //tcp socket is closed
BIND(WLS_TCPOPENED)			// (-5518)     //tcp socket is opened
BIND(WLS_TCPOPENING)			// (-5519)     //the module is trying to open a TCP socket
BIND(WLS_ATTACHED)			// (-5520)     //Attached
BIND(WLS_DETTACHED)			// (-5521)     //Dettached
BIND(WLS_ATTACHING)			// (-5522)     //the module is looking for the base station.
BIND(WLS_NOBOARD)			// (-5523)     //no GPRS or CDMA board exist
BIND(WLS_UNKNOWNTYPE)			// (-5524)     //unknown type
BIND(WLS_DIALERR)			// (-5525)     //用户名和密码注册被网络侧拒绝，或者APN错误，卡欠费等
BIND(WLS_TCPSENDERR)			// (-5526)     //TCP socket send  err
BIND(WLS_TCPCONNECTOVERTIME)			// (-5527)     //TCP connect overtime 
BIND(WLS_DIALING)			// (-5528)     //dialing 
BIND(WLS_PORTERR)			// (-5529)     //serial port error
BIND(WLS_PORTINUSE)			// (-5530)     //serial port is in use by another program 
BIND(WLS_DETECTERR)			// (-5531)     //识别无线模块错误（设置VPOS与无线模块通信错误）
BIND(WLS_SOCKETOVERRUN)			// (-5532)     //打开TCP socket连接过多,已超过3个,溢出 
BIND(WLS_TCPCONNECTREFUSE)			// (-5533)   
BIND(WLS_OTHERR)			// (-5534)     //其他错误
BIND(WLS_TCPSENDTIMEOUT)			// (-5535)     //send timeout
