#ifndef __CNETSOCKLIB_H__
#define __CNETSOCKLIB_H__

/* --------------------------------------------------------------
Project:	NetSockLib
Purpose:	Network Socket Library
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 26, 2014
Filename: 	CNetSockLib.h

Last modified: September 24, 2014
License:

*
* Copyright (C) 2014 Ho-Jung Kim (godmode2k@hotmail.com)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
-----------------------------------------------------------------
Note:
-----------------------------------------------------------------
1. Build:
	$ g++ ... -D__LINUX__ -D_REENTRANT -lpthread -lrt
-------------------------------------------------------------- */



//! Header and Definition
// ---------------------------------------------------------------
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>		// stringstream

#include <cstdio>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>		// Variable Argument Lists
#include <cstdint>

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <stdarg.h>
//#include <stdint.h>
#include <errno.h>		// error code

#ifdef __LINUX__
#include <signal.h>		// SIGKILL
#include <stdbool.h>	// bool
#include <errno.h>		// errno, strerror(errno)
#include <fcntl.h>
#include <unistd.h>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include <dirent.h>		// DIR

#include <termios.h>	// Keyboard event (_kbhit)
#elif _WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#else
#endif



#include "util/CUtil_DEF.h"
#include "util/CUtil.h"
#include "util/CThreadTask.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
// LOG
//#define __REQ_DEBUG_MESSAGE__

#ifdef __LINUX__
	#define __PACK__	__attribute__((packed))
#else
	#define __PACK__
#endif

#define DEFAULT_STR_IP_ZERO			"00000000"	// 0.0.0.0
#define DEFAULT_STR_IP_LOOPBACK		"0100007F"	// 127.0.0.1
#define DEFAULT_MAX_SIZE_IPV4		15			// "255.255.255.255"
#define DEFAULT_MAX_SIZE_LINE		255

#define DEFAULT_SERVER_PORT					9090
#define DEFAULT_SERVER_MAX_BACKLOG			10
#define DEFAULT_SERVER_MAX_TIMEOUT			5		// second
#define DEFAULT_CLIENT_MAX_TIMEOUT			10		// second
#define DEFAULT_MAX_SIZE_PROTOCOL_VER		128
#define DEFAULT_MAX_SEND_BUF_SIZE			1024
#define DEFAULT_MAX_RECV_BUF_SIZE			1024
#define DEFAULT_MAX_SEND_BUF_EXTRA_SIZE		20	// "[client(%d)]: "


#ifdef __cplusplus
extern "C" {
#endif

// Protocol Type
typedef enum g_e_ProtocolType {
	e_proto_RAW = 0,
	e_proto_TCP = 1,
	e_proto_UDP,
	e_proto_BROADCAST
} g_e_ProtocolType_t;

// Network Type
typedef enum g_e_NetworkType {
	e_nettype_ServerToPeer = 1,
	e_nettype_ServerToAllPeers
} g_e_NetworkType_t;


#define g_STR_STATE_UNKNOWN		"Unknown"

// TCP State
enum g_e_TCPState {
	e_state_TCP_ESTABLISHED = 1,
	e_state_TCP_SYN_SENT,
	e_state_TCP_SYN_RECV,
	e_state_TCP_FIN_WAIT1,
	e_state_TCP_FIN_WAIT2,
	e_state_TCP_TIME_WAIT,
	e_state_TCP_CLOSE,
	e_state_TCP_CLOSE_WAIT,
	e_state_TCP_LAST_ACK,
	e_state_TCP_LISTEN,
	e_state_TCP_CLOSING
};
typedef g_e_TCPState g_e_TCPState_t;
/*
static const char* g_TCPStateList[] =  {
	"ESTABLISHED",
	"SYN_SENT",
	"SYN_RECV",
	"FIN_WAIT1",
	"FIN_WAIT2",
	"TIME_WAIT",
	"CLOSE",
	"CLOSE_WAIT",
	"LAST_ACK",
	"LISTEN",
	"CLOSING",
	NULL
};
*/

// UDP State
enum g_e_UDPState {
	e_state_UDP_ESTABLISHED = 1,
	e_state_UDP_SYN_SENT,
	e_state_UDP_SYN_RECV,
	e_state_UDP_FIN_WAIT1,
	e_state_UDP_FIN_WAIT2,
	e_state_UDP_TIME_WAIT,
	e_state_UDP_CLOSE,
	e_state_UDP_CLOSE_WAIT,
	e_state_UDP_LAST_ACK,
	e_state_UDP_LISTEN,
	e_state_UDP_CLOSING
};
/*
static const char* g_UDPStateList[] =  {
	"ESTABLISHED",
	"SYN_SENT",
	"SYN_RECV",
	"FIN_WAIT1",
	"FIN_WAIT2",
	"TIME_WAIT",
	"CLOSE",
	"CLOSE_WAIT",
	"LAST_ACK",
	"LISTEN",
	"CLOSING",
	NULL
};
*/

// POSIX Thread
/*
typedef struct {
	int a;
	int b;
} __PACK__ netlib_pthread_args_st;
*/

/*
// Host information
typedef struct {
	int uid;
	int inode;
	int state;
	unsigned short host_port;
	unsigned short remote_port;
	char* pStrHostAddr;
	char* pStrRemoteAddr;
	char* pStrCmdline;
} __PACK__ host_info_st;
*/

#ifdef __cplusplus
}
#endif



class CNetSockLib;
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __REQ_MAIN_FUNCTION__
#endif //#ifdef __REQ_MAIN_FUNCTION__

namespace g_UTIL {
	typedef struct _datetime_st {
		char year[4];
		char month[2];
		char day[2];
		char hour[2];
		char min[2];
		char sec[2];
		char millis[10];
		int d_year;
		int d_month;
		int d_day;
		int d_hour;
		int d_min;
		int d_sec;
		int d_millis;
	} datetime_st;

	void usleep(int millisecond);
	std::string intToStr(int val);

	// Check a pressed key
	int _kbhit(void);

	// Network
	namespace NET {
		//! thread-unsafe
		int state(int sockfd, bool read, long usec, long nsec, fd_set* pfds);

		//! TCP/UDP hole punching for NAT
		//bool get_remote_ip_port(const char* hosting, char* _ipaddr, int* _port);
	}  // namespace NET

#ifdef _WINDOWS
#else
	// htonf, ntohf
	// Source: https://beej.us/guide/bgnet/examples/pack.c
	uint32_t htonf(float f);
	float ntohf(uint32_t p);
#endif
} // namespace g_UTIL

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
class CHandleIO {
private:
	const char* TAG;
public:
	// Ctor/Dtor
	explicit CHandleIO(void);
	~CHandleIO();

#ifdef _WINDOWS
	int write_int(const SOCKET socket, const int val);
	int write_float(const SOCKET socket, const float val);
	int write_bool(const SOCKET socket, const unsigned char val);
	int write(const SOCKET socket, const unsigned char* buffer, const int len);

	int read_int(const SOCKET socket, int* val);
	int read_float(const SOCKET socket, float* val);
	int read_bool(const SOCKET socket, unsigned char* val);
	int read(const SOCKET socket, unsigned char* buffer, const int len);
	int read(const SOCKET socket, unsigned char* buffer, const int len, const int read_len);
#else
	int write_int(const int socket, const int val);
	int write_float(const int socket, const float val);
	int write_bool(const int socket, const unsigned char val);
	int write(const int socket, const unsigned char* buffer, const int len);

	int read_int(const int socket, int* val);
	int read_float(const int socket, float* val);
	int read_bool(const int socket, unsigned char* val);
	int read(const int socket, unsigned char* buffer, const int len);
	int read(const int socket, unsigned char* buffer, const int len, const int read_len);
#endif
};

class CAsyncTaskClient;
class CAsyncTaskServer : public CThreadTask, CHandleIO {
private:
	const char* TAG;

	// Parent object
	//CNetSockLib* m_pParent;

#ifdef _WINDOWS
	SOCKET m_sockfd;
#else
	int m_sockfd;
#endif
	unsigned short m_port;
	int m_backlog;
	int m_timeout;
	g_e_ProtocolType_t m_protocol_type;	// RAW, TCP, UDP, ...
	g_e_NetworkType_t m_network_type;	// Server to Peer, Server to all Peers
	bool m_use_timeout;		// default: false

	struct sockaddr_in m_sockaddr;
	char m_ipaddr[DEFAULT_MAX_SIZE_IPV4];
	unsigned char m_protocol_ver[DEFAULT_MAX_SIZE_PROTOCOL_VER];	// GUID (Byte array)
	
	std::vector<CAsyncTaskClient*>* m_pvecClient;	// peer thread container
protected:
public:
	// Ctor/Dtor
	explicit CAsyncTaskServer(void);
	~CAsyncTaskServer();

	//void set_parent(CNetSockLib& parent);
	//CNetSockLib& get_parent(void);

	void close_socket(void);

	void set_sockfd(int sockfd);
	const int get_sockfd(void);
	void set_protocol_type(g_e_ProtocolType_t type);
	g_e_ProtocolType_t get_protocol_type(void);
	void set_network_type(g_e_NetworkType_t type);
	g_e_NetworkType_t get_network_type(void);
	void set_backlog(int backlog);
	const int get_backlog(void);
	void set_timeout(int sec);
	const int get_timeout(void);
	void set_use_timeout(bool use);
	bool get_use_timeout(void);
	void set_port(unsigned short port);
	const unsigned short get_port(void);
	void set_ipaddr(const char* ipaddr);
	const char* get_ipaddr(void);
	const std::vector<CAsyncTaskClient*>* get_clients(void);

	void start_server(void);
	void stop_server(void);


	int write_int(const int val);
	int write_float(const float val);
	int write_bool(const unsigned char val);
	int write(const unsigned char* buffer, const int len);

	int read_int(int* val);
	int read_float(float* val);
	int read_bool(unsigned char* val);
	int read(unsigned char* buffer, const int len);
	int read(unsigned char* buffer, const int len, const int read_len);

	//virtual int send_data(void);


	// Thread
	void* inBackground(std::vector<void*>* pvecVal = NULL);
	void progressUpdate(void* pVal);
	void postExecute(void* pResult, void* pExtraVal = NULL);

private:
	bool server_open(void);
	bool server_open_tcp(void);
	bool server_open_udp(void);
	void server_close(void);
};

class CAsyncTaskClient : public CThreadTask {
private:
	const char* TAG;

	// Parent object
	//CNetSockLib* m_pParent;
	void* m_pParent;

	int m_sockfd;
	unsigned short m_port;
	unsigned short m_my_port;
	int m_timeout;
	g_e_ProtocolType_t m_protocol_type;	// RAW, TCP, UDP, ...

	struct sockaddr_in m_sockaddr;
	char m_ipaddr[DEFAULT_MAX_SIZE_IPV4];
	char m_my_ipaddr[DEFAULT_MAX_SIZE_IPV4];
	unsigned char m_protocol_ver[DEFAULT_MAX_SIZE_PROTOCOL_VER];	// GUID (Byte array)

	//bool m_ready_for_send;
	char send_buf[DEFAULT_MAX_SEND_BUF_SIZE];
protected:
public:
	// Ctor/Dtor
	explicit CAsyncTaskClient(void);
	~CAsyncTaskClient();

	void set_parent(const void* parent);
	const void* get_parent(void);

	bool connect_server(void);
	int state(int sockfd, bool read, long usec, long nsec, fd_set* pfds);
	//int state(int sockfd, bool read, long usec, long nsec);
	virtual bool established(void);
	void detach(void);
	void disconnect_server(void);

	void set_sockfd(int sockfd);
	int get_sockfd(void);
	void set_protocol_type(g_e_ProtocolType_t type);
	g_e_ProtocolType_t get_protocol_type(void);

	void set_timeout(int sec);
	int get_timeout(void);

	void set_server_port(unsigned short port);
	const unsigned short get_server_port(void);
	void set_server_ipaddr(const char* ipaddr);
	const char* get_server_ipaddr(void);

	void set_my_port(unsigned short port);
	const unsigned short get_my_port(void);
	void set_my_ipaddr(const char* ipaddr);
	const char* get_my_ipaddr(void);


	int write_int(const int val);
	int write_float(const float val);
	int write_bool(const unsigned char val);
	int write(const unsigned char* buffer, const int len);

	int read_int(int* val);
	int read_float(float* val);
	int read_bool(unsigned char* val);
	int read(unsigned char* buffer, const int len);
	int read(unsigned char* buffer, const int len, const int read_len);

	//void set_ready_for_send(const bool ready) { m_ready_for_send = ready; }
	//bool get_ready_for_send(void) { return m_ready_for_send; }
	//void set_send_data_buffer(void);
	virtual bool send_data_motd(void);
	virtual bool send_data(void);
	virtual bool send_data(int sockfd, const char* data, int len);
	virtual bool recv_data(void);
	// UDP
	virtual bool send_data_motd_udp(void);
	virtual bool send_data_udp(void);
	virtual bool send_data_udp(int sockfd, const char* data, int len);
	virtual bool recv_data_udp(void);



	// Thread
	void* inBackground(std::vector<void*>* pvecVal = NULL);
	void progressUpdate(void* pVal);
	void postExecute(void* pResult, void* pExtraVal = NULL);
private:
	//bool client_open(void);
	void client_close(void);
};


class CNetSockLib {
private:
	const char* TAG;

	// Singleton
	//explicit CNetSockLib(void);
protected:
public:
	// Ctor/Dtor
	explicit CNetSockLib(void);
	virtual ~CNetSockLib();

	// Singleton
	/*
	static CNetSockLib& get_instance() {
		static CNetSockLib instance;
		return instance;
	}
	*/

	void __set_class_tag(const char* tag);

	void __init(void);
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
	void ___release(void);
#else
	void __release(void);
#endif
public:
#ifdef __REQ_MAIN_FUNCTION__
	// Network thread
	CAsyncTaskServer m_asyncTask_server;
	CAsyncTaskClient m_asyncTask_client;
#endif //#ifdef __REQ_MAIN_FUNCTION__
};
// ---------------------------------------------------------------



#endif	// __CNETSOCKLIB_H__
