/* --------------------------------------------------------------
Project:	NetSockLib
Purpose:	Network Socket Library
Author:		Ho-Jung Kim (godmode2k@hotmail.com)
Date:		Since June 26, 2014
Filename: 	CNetSockLib.cpp

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
	
TODO:
	* SIGPIPE:
	{
		 - GDB:
		 	(gdb) "cont" or
			(gdb) "handle SIGPIPE nostop" or
			(gdb) "handle SIGPIPE nostop pass pass" or
			-
			$ cat "handle SIGPIPE nostop pass pass" > $HOME/.gdbinit
			(gdb) just use as before

		- signal( SIGPIPE, SIG_IGN );
		or
		- send( sockfd, buf, size, MSG_NOSIGNAL );	// MSG_NOSIGNAL: SUSv4 2006
	}

	* re-transfer when non-blocking: check a re-transfer(poll(), epoll()) then retry...
		- in non-blocking, performed send( sockfd, buf, 10000, 0 );
		- NEED to re-transfer; set a delay for high latency or use the poll()/epoll() for detect the sendable
-------------------------------------------------------------- */



//! Header
// ---------------------------------------------------------------
#include "CNetSockLib.h"

//#include "util/CUtil_DEF.h"
// ---------------------------------------------------------------



//! Definition
// ---------------------------------------------------------------
/*
// TCP State
const char* g_TCPStateList[] =  {
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

// UDP State
const char* g_UDPStateList[] =  {
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

// POSIX Thread Mutex Lock
pthread_mutex_t g_pthread_mutex_lock = PTHREAD_MUTEX_INITIALIZER;
// ---------------------------------------------------------------



//! Prototype
// ---------------------------------------------------------------
// ---------------------------------------------------------------



//! Class
// ---------------------------------------------------------------
// Ctor
CNetSockLib::CNetSockLib(void) : TAG("CNetSockLib") {
	__LOGT__( TAG, "CNetSockLib()" );

	__init();
}

// Dtor
CNetSockLib::~CNetSockLib() {
	__LOGT__( TAG, "~CNetSockLib()" );

#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
	___release();
#else
	__release();
#endif
}

void CNetSockLib::__set_class_tag(const char* tag) {
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "_TAG" for variable here, in NDK, it may occured error the following:
	//		"error: expected unqualified-id before string constant"
	//
	//	wtf? are you kiddin me? i will have to rename the variable "_TAG" to avoid this shit.
	const char* default_TAG = "CNetSockLib";
#else
	const char* _TAG = "CNetSockLib";
#endif

	std::stringstream ss;
	std::string str;

#ifdef __ANDROID_NDK__
	ss << default_TAG << ":[" << tag << "]";
#else
	ss << _TAG << ":[" << tag << "]";
#endif
	str = ss.str();
	TAG = str.c_str();
}

// Initialize
void CNetSockLib::__init(void) {
	__LOGT__( TAG, "__init()" );
}

// Release
#ifdef __ANDROID_NDK__
	//! NOTE: Don't use named "__release", which is reserved in NDK
void CNetSockLib::___release(void) {
#else
void CNetSockLib::__release(void) {
#endif
	__LOGT__( TAG, "__release()" );
}



//! Server
// ---------------------------------------------------------------
CAsyncTaskServer::CAsyncTaskServer(void) : CThreadTask("SERVER"), TAG("CAsyncTaskServer") {
	__LOGT__( TAG, "CAsyncTaskServer()" );

	m_sockfd = -1;
	m_protocol_type = e_proto_TCP;
	m_network_type = e_nettype_ServerToPeer;
	m_backlog = DEFAULT_SERVER_MAX_BACKLOG;
	m_timeout = DEFAULT_SERVER_MAX_TIMEOUT;
	m_use_timeout = false;
	m_port = DEFAULT_SERVER_PORT;
	memset( (void*)&m_ipaddr, 0x00, sizeof(m_ipaddr) );

	m_pvecClient = new std::vector<CAsyncTaskClient*>;

	//set_tag<char*>( "Main Server" );
	set_tag( "Main Server" );
}

CAsyncTaskServer::~CAsyncTaskServer(void) {
	__LOGT__( TAG, "~CAsyncTaskServer()" );

	server_close();
}

//void CAsyncTaskServer::set_parent(CNetSockLib& parent) {
//	m_pParent = &parent;
//}
//CNetSockLib& CAsyncTaskServer::get_parent(void) {
//	return (*m_pParent);
//}
void CAsyncTaskServer::set_sockfd(int sockfd) {
	m_sockfd = sockfd;
}
const int CAsyncTaskServer::get_sockfd(void) {
	return m_sockfd;
}
void CAsyncTaskServer::set_protocol_type(g_e_ProtocolType_t type) {
	m_protocol_type = type;
}
g_e_ProtocolType_t CAsyncTaskServer::get_protocol_type(void) {
	return (g_e_ProtocolType_t)m_protocol_type;
}
void CAsyncTaskServer::set_network_type(g_e_NetworkType_t type) {
	m_network_type = type;
}
g_e_NetworkType_t CAsyncTaskServer::get_network_type(void) {
	return m_network_type;
}
void CAsyncTaskServer::set_backlog(int backlog) {
	m_backlog = backlog;
}
const int CAsyncTaskServer::get_backlog(void) {
	return m_backlog;
}
void CAsyncTaskServer::set_timeout(int sec) {
	m_timeout = sec;
}
const int CAsyncTaskServer::get_timeout(void) {
	return m_timeout;
}
void CAsyncTaskServer::set_use_timeout(bool use) {
	m_use_timeout = use;
}
bool CAsyncTaskServer::get_use_timeout(void) {
	return m_use_timeout;
}
void CAsyncTaskServer::set_port(unsigned short port) {
	m_port = port;
}
const unsigned short CAsyncTaskServer::get_port(void) {
	return m_port;
}
void CAsyncTaskServer::set_ipaddr(const char* ipaddr) {
	memset( (void*)&m_ipaddr, 0x00, sizeof(m_ipaddr) );
	snprintf( m_ipaddr, sizeof(m_ipaddr), "%s", ipaddr );
}
const char* CAsyncTaskServer::get_ipaddr(void) {
	return (m_ipaddr[0] == '\0')? NULL : (char*)m_ipaddr;
}
const std::vector<CAsyncTaskClient*>* CAsyncTaskServer::get_clients(void) {
	return m_pvecClient;
}

void CAsyncTaskServer::start_server(void) {
	this->execute( NULL );
}
void CAsyncTaskServer::stop_server(void) {
	this->cancel();
}


void* CAsyncTaskServer::inBackground(std::vector<void*>* pvecVal) {
	__LOGT__( TAG, "inBackground()" );

	server_open();

	return ((void*)true);
}

void CAsyncTaskServer::progressUpdate(void* pVal) {
	__LOGT__( TAG, "progressUpdate()" );

	char* pStr = reinterpret_cast<char*>(pVal);
	if ( pStr )
		__LOGT__( TAG, "%s", pStr );
}

void CAsyncTaskServer::postExecute(void* pResult, void* pExtraVal) {
	__LOGT__( TAG, "postExecute()" );

	int result = reinterpret_cast<int>(pResult);
	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );

	if ( pExtraVal ) {
	}


	server_close();
}

bool CAsyncTaskServer::server_open(void) {
	__LOGT__( TAG, "server_open()" );

	bool ret = false;

	switch ( get_protocol_type() ) {
		case e_proto_RAW:
			{
				__LOGT__( TAG, "server_open(): Protocol: RAW" );
			} break;
		case e_proto_TCP:
			{
				__LOGT__( TAG, "server_open(): Protocol: TCP" );
				ret = server_open_tcp();
			} break;
		case e_proto_UDP:
			{
				__LOGT__( TAG, "server_open(): Protocol: UDP" );
				ret = server_open_udp();
			} break;
		default:
			{
				__LOGT__( TAG, "server_open(): Unsupported protocol" );
			} break;
	}

	return ret;
}

bool CAsyncTaskServer::server_open_tcp(void) {
	__LOGT__( TAG, "server_open_tcp()" );

	int ret = 0;
	int reuse = 1;
	int connect_count = 0;


	m_sockfd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_sockfd < 0 ) {
		__LOGT__( TAG, "server_open_tcp(): socket() [FAIL]" );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_tcp(): socket(): (socket = %d)", m_sockfd );
	}

	memset( (void*)&m_sockaddr, 0x00, sizeof(sockaddr_in) );
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons( get_port() );

	if ( get_ipaddr() ) {
#ifdef _WINDOWS
		m_sockaddr.sin_addr.S_un.S_addr = inet_addr( get_ipaddr() );
#else
		m_sockaddr.sin_addr.s_addr = inet_addr( get_ipaddr() );
#endif
	}
	else {
#ifdef _WINDOWS
		m_sockaddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );	// inet_addr( "192.168.0.x" );
#else
		m_sockaddr.sin_addr.s_addr = htonl( INADDR_ANY );	// inet_addr( "192.168.0.x" );
#endif
	}


	// Prevent bind error
	ret = setsockopt( m_sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );
	if ( ret < 0 ) {
		__LOGT__( TAG, "server_open_tcp(): setsockopt(): prevent bind error [FAIL] (code = %d)", ret );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_tcp(): setsockopt()" );
	}

	ret = bind( m_sockfd, (struct sockaddr *)&m_sockaddr, sizeof(sockaddr) );
	if ( ret < 0 ) {
		__LOGT__( TAG, "server_open_tcp(): bind() [FAIL] (code = %d)", ret );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_tcp(): bind()" );
	}

	ret = listen( m_sockfd, get_backlog() );
	if ( ret < 0 ) {
		__LOGT__( TAG, "server_open_tcp(): listen() [FAIL] (code = %d)", ret );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_tcp(): listen(): backlog = %d", get_backlog() );
	}


	__LOGT__( TAG, "server_open_tcp(): host IP address: %s:%d\n",
			inet_ntoa(m_sockaddr.sin_addr), (unsigned short)ntohs(m_sockaddr.sin_port) );


	if ( get_use_timeout() ) {
		struct timeval tv;

		tv.tv_sec = get_timeout();
		tv.tv_usec = 0;

		ret = setsockopt( m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(timeval) );
		if ( ret < 0 ) {
			__LOGT__( TAG, "server_open_tcp(): setsockopt(): SO_SNDTIMEO [FAIL] (code = %d)", ret );
			server_close();
			return false;
		}
		else {
			__LOGT__( TAG, "server_open_tcp(): setsockopt(): SO_SNDTIMEO" );
		}

		ret = setsockopt( m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval) );
		if ( ret < 0 ) {
			__LOGT__( TAG, "server_open_tcp(): setsockopt(): SO_RCVTIMEO [FAIL] (code = %d)", ret );
			server_close();
			return false;
		}
		else {
			__LOGT__( TAG, "server_open_tcp(): setsockopt(): SO_RCVTIMEO" );
		}
	}


	while ( 1 ) {
		int sockfd_client;
		struct sockaddr_in sockaddr_client;
		//unsigned int client_len = sizeof( sockaddr_in );
		socklen_t sockaddr_client_len = sizeof( sockaddr_in );
		char peer_ipaddr[INET_ADDRSTRLEN] = { 0, };

		__LOGT__( TAG, "server_open_tcp(): accept(): waiting..." );
		sockfd_client = accept( m_sockfd, (struct sockaddr *)&sockaddr_client, &sockaddr_client_len );
		if ( sockfd_client < 0 ) {
			__LOGT__( TAG, "server_open_tcp(): accept(): [FAIL] (sock_client = %d)", sockfd_client );
			server_close();
			return false;
		}

		// Get client IP address
		__LOGT__( TAG, "server_open_tcp(): client socket = %d, IP address: %s:%d",
				sockfd_client, 
				//! DO NOT USE 'inet_ntoa()' due to that
				//		"The string returned by inet_ntoa() resides in a static memory area."
				//inet_ntoa(sockaddr_client.sin_addr), sockaddr_client.sin_port );
				inet_ntop(AF_INET, &sockaddr_client.sin_addr, peer_ipaddr, sizeof(peer_ipaddr)),
				sockaddr_client.sin_port );


		// Client
		{
			CAsyncTaskClient* client = new CAsyncTaskClient;

			++connect_count;

			if ( client ) {
				client->set_sockfd( sockfd_client );
				client->set_tag_id( connect_count );
				client->set_my_ipaddr( peer_ipaddr );
				client->set_my_port( sockaddr_client.sin_port );
				client->set_parent( this );
				//client->established();
				client->detach();

				if ( m_pvecClient ) {
					__LOGT__( TAG, "server_open_tcp(): client tag id = %d", client->get_tag_id() );
					m_pvecClient->push_back( client );
				}
				else {
					__LOGT__( TAG, "server_open_tcp(): client list == NULL" );
					break;
				}
			}
		}
	} // while()

	server_close();

	return true;
}

bool CAsyncTaskServer::server_open_udp(void) {
	__LOGT__( TAG, "server_open_udp()" );

	int ret = 0;
	int reuse = 1;
	int connect_count = 0;


	m_sockfd = socket( PF_INET, SOCK_DGRAM, IPPROTO_UDP );
	if ( m_sockfd < 0 ) {
		__LOGT__( TAG, "server_open_udp(): socket() [FAIL]" );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_udp(): socket(): (socket = %d)", m_sockfd );
	}

	memset( (void*)&m_sockaddr, 0x00, sizeof(sockaddr_in) );
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons( get_port() );

	if ( get_ipaddr() ) {
#ifdef _WINDOWS
		m_sockaddr.sin_addr.S_un.S_addr = inet_addr( get_ipaddr() );
#else
		m_sockaddr.sin_addr.s_addr = inet_addr( get_ipaddr() );
#endif
	}
	else {
#ifdef _WINDOWS
		m_sockaddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );	// inet_addr( "192.168.0.x" );
#else
		m_sockaddr.sin_addr.s_addr = htonl( INADDR_ANY );	// inet_addr( "192.168.0.x" );
#endif
	}


	// Prevent bind error
	ret = setsockopt( m_sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );
	if ( ret < 0 ) {
		__LOGT__( TAG, "server_open_udp(): setsockopt(): prevent bind error [FAIL] (code = %d)", ret );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_udp(): setsockopt()" );
	}

	/*
	ret = bind( m_sockfd, (struct sockaddr *)&m_sockaddr, sizeof(sockaddr) );
	if ( ret < 0 ) {
		__LOGT__( TAG, "server_open_udp(): bind() [FAIL] (code = %d)", ret );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_udp(): bind()" );
	}

	ret = listen( m_sockfd, get_backlog() );
	if ( ret < 0 ) {
		__LOGT__( TAG, "server_open_udp(): listen() [FAIL] (code = %d)", ret );
		server_close();
		return false;
	}
	else {
		__LOGT__( TAG, "server_open_udp(): listen(): backlog = %d", get_backlog() );
	}
	*/


	__LOGT__( TAG, "server_open_udp(): host IP address: %s:%d\n",
			inet_ntoa(m_sockaddr.sin_addr), (unsigned short)ntohs(m_sockaddr.sin_port) );


	if ( get_use_timeout() ) {
		struct timeval tv;

		tv.tv_sec = get_timeout();
		tv.tv_usec = 0;

		ret = setsockopt( m_sockfd, SOL_SOCKET, SO_SNDTIMEO, (char*)&tv, sizeof(timeval) );
		if ( ret < 0 ) {
			__LOGT__( TAG, "server_open_udp(): setsockopt(): SO_SNDTIMEO [FAIL] (code = %d)", ret );
			server_close();
			return false;
		}
		else {
			__LOGT__( TAG, "server_open_udp(): setsockopt(): SO_SNDTIMEO" );
		}

		ret = setsockopt( m_sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(timeval) );
		if ( ret < 0 ) {
			__LOGT__( TAG, "server_open_udp(): setsockopt(): SO_RCVTIMEO [FAIL] (code = %d)", ret );
			server_close();
			return false;
		}
		else {
			__LOGT__( TAG, "server_open_udp(): setsockopt(): SO_RCVTIMEO" );
		}
	}


	while ( 1 ) {
		break;

		//! TODO:
		// - unique id: Create a unique id for clients
		//
		// - struct sockaddr_in sockaddr_client;
		// - ...
		// - recvfrom( ... );
		// - if ( !has_unique_id ) {
		// -     get ip address
		// -     creates_unique_id( ... );
		// -     sendto( ...unique_id, data... );
		// - }
		// ...
		// - add client info(..., unique-id) to client-list (m_pvecClient)
		// - ...
		// - sendto( ...unique_id, data... );



		//! Leave below code
		/*
		int sockfd_client;
		struct sockaddr_in sockaddr_client;
		//unsigned int client_len = sizeof( sockaddr_in );
		socklen_t sockaddr_client_len = sizeof( sockaddr_in );
		char peer_ipaddr[INET_ADDRSTRLEN] = { 0, };

		__LOGT__( TAG, "server_open_udp(): accept(): waiting..." );
		sockfd_client = accept( m_sockfd, (struct sockaddr *)&sockaddr_client, &sockaddr_client_len );
		if ( sockfd_client < 0 ) {
			__LOGT__( TAG, "server_open_udp(): accept(): [FAIL] (sock_client = %d)", sockfd_client );
			server_close();
			return false;
		}

		// Get client IP address
		__LOGT__( TAG, "server_open_udp(): client socket = %d, IP address: %s:%d",
				//! DO NOT USE 'inet_ntoa()' due to that
				//		"The string returned by inet_ntoa() resides in a static memory area."
				//inet_ntoa(sockaddr_client.sin_addr), sockaddr_client.sin_port );
				inet_ntop(AF_INET, &sockaddr_client.sin_addr, peer_ipaddr, sizeof(peer_ipaddr)),
				sockaddr_client.sin_port );
		*/

		/*
		// Client
		{
			CAsyncTaskClient* client = new CAsyncTaskClient;

			++connect_count;

			if ( client ) {
				client->set_sockfd( sockfd_client );
				client->set_tag_id( connect_count );
				client->set_parent( this );
				//client->established();
				client->detach();

				if ( m_pvecClient ) {
					__LOGT__( TAG, "server_open_udp(): client tag id = %d", client->get_tag_id() );
					m_pvecClient->push_back( client );
				}
				else {
					__LOGT__( TAG, "server_open_udp(): client list == NULL" );
					break;
				}
			}
		}
		*/
	} // while()

	server_close();

	return true;
}


void CAsyncTaskServer::server_close(void) {
	__LOGT__( TAG, "server_close()" );

#ifdef _WINDOWS
	:shutdown( m_sockfd, SD_BOTH );
	::closesocket( m_sockfd );
#else
	shutdown( m_sockfd, SHUT_RDWR );
	close( m_sockfd );
#endif
	m_sockfd = -1;


	if ( m_pvecClient ) {
		__LOGT__( TAG, "server_close(): (remove) client count = %d", m_pvecClient->size() );

		if ( m_pvecClient->size() > 0 ) {
			std::vector<CAsyncTaskClient*>::iterator iter;
			for ( iter = m_pvecClient->begin(); iter != m_pvecClient->end(); ++iter ) {
				//__LOGT__( TAG, "server_close(): port = %d", (*iter) );
				CAsyncTaskClient* client = (*iter);
				if ( client ) {
					__LOGT__( TAG, "server_close(): client TAG = %s, TAG id = %d", client->get_tag(), client->get_tag_id() );
					// ...
				}

				//iter = m_pvecClient->erase( iter );
			}

			m_pvecClient->erase( m_pvecClient->begin(), m_pvecClient->end() );
			__LOGT__( TAG, "server_close(): (result) client count = %d", m_pvecClient->size() );
		}

		m_pvecClient->clear();
		delete m_pvecClient;
		m_pvecClient = NULL;
	}
}



//! Client
// ---------------------------------------------------------------
CAsyncTaskClient::CAsyncTaskClient(void) : CThreadTask("CLIENT"), TAG("CAsyncTaskClient") {
	__LOGT__( TAG, "CAsyncTaskClient()" );

	m_sockfd = -1;
	m_protocol_type = e_proto_TCP;
	m_timeout = DEFAULT_CLIENT_MAX_TIMEOUT;
	//m_ready_for_send = false;
	m_pParent = NULL;
	memset( (void*)&m_ipaddr, 0x00, sizeof(m_ipaddr) );
	memset( (void*)&m_my_ipaddr, 0x00, sizeof(m_my_ipaddr) );

	//set_tag( "thread test" );
	//set_tag( 111222 );
	
	__LOGT__( TAG, "CAsyncTaskClient(): TAG = %s, TAG id = %d", get_tag(), get_tag_id() );
}

CAsyncTaskClient::~CAsyncTaskClient(void) {
	__LOGT__( TAG, "~CAsyncTaskClient()" );

	//disconnect_server();
	client_close();
}

void CAsyncTaskClient::set_parent(const void* parent) {
	m_pParent = (void*)parent;
}
const void* CAsyncTaskClient::get_parent(void) {
	return m_pParent;
}
void CAsyncTaskClient::detach(void) {
	this->execute( NULL );
}
void CAsyncTaskClient::disconnect_server(void) {
	this->cancel();
	client_close();
}
void CAsyncTaskClient::set_sockfd(int sockfd) {
	m_sockfd = sockfd;
}
int CAsyncTaskClient::get_sockfd(void) {
	return m_sockfd;
}
void CAsyncTaskClient::set_protocol_type(g_e_ProtocolType_t type) {
	m_protocol_type = type;
}
g_e_ProtocolType_t CAsyncTaskClient::get_protocol_type(void) {
	return (g_e_ProtocolType_t)m_protocol_type;
}
void CAsyncTaskClient::set_timeout(int sec) {
	m_timeout = sec;
}
int CAsyncTaskClient::get_timeout(void) {
	return m_timeout;
}
void CAsyncTaskClient::set_server_port(unsigned short port) {
	m_port = port;
}
const unsigned short CAsyncTaskClient::get_server_port(void) {
	return m_port;
}
void CAsyncTaskClient::set_server_ipaddr(const char* ipaddr) {
	memset( (void*)&m_ipaddr, 0x00, sizeof(m_ipaddr) );
	snprintf( m_ipaddr, sizeof(m_ipaddr), "%s", ipaddr );
}
const char* CAsyncTaskClient::get_server_ipaddr(void) {
	return (m_ipaddr[0] == '\0')? NULL : (char*)m_ipaddr;
}
void CAsyncTaskClient::set_my_port(unsigned short port) {
	m_my_port = port;
}
const unsigned short CAsyncTaskClient::get_my_port(void) {
	return m_my_port;
}
void CAsyncTaskClient::set_my_ipaddr(const char* ipaddr) {
	memset( (void*)&m_my_ipaddr, 0x00, sizeof(m_my_ipaddr) );
	snprintf( m_my_ipaddr, sizeof(m_my_ipaddr), "%s", ipaddr );
}
const char* CAsyncTaskClient::get_my_ipaddr(void) {
	return (m_my_ipaddr[0] == '\0')? NULL : (char*)m_my_ipaddr;
}



void* CAsyncTaskClient::inBackground(std::vector<void*>* pvecVal) {
	__LOGT__( TAG, "inBackground()" );

	established();

	return ((void*)true);
}

void CAsyncTaskClient::progressUpdate(void* pVal) {
	__LOGT__( TAG, "progressUpdate()" );

	char* pStr = reinterpret_cast<char*>(pVal);
	if ( pStr )
		__LOGT__( TAG, "%s", pStr );
}

void CAsyncTaskClient::postExecute(void* pResult, void* pExtraVal) {
	__LOGT__( TAG, "postExecute()" );

	int result = reinterpret_cast<int>(pResult);
	__LOGT__( TAG, "postExecute(): result = %s", (result? "TRUE" : "FALSE") );

	if ( pExtraVal ) {
	}


	client_close();
}

bool CAsyncTaskClient::connect_server(void) {
	__LOGT__( TAG, "connect_server()" );

	int ret = 0;
	int reuse = 1;
	socklen_t sockaddr_len = 0;


	__LOGT__( TAG, "connect_server(): Server IP address = %s:%d",
			get_server_ipaddr(), get_server_port() );

	m_sockfd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_sockfd < 0 ) {
		__LOGT__( TAG, "connect_server(): socket() [FAIL]" );
		client_close();
		return false;
	}
	else {
		__LOGT__( TAG, "connect_server(): socket(): (socket = %d)", m_sockfd );
	}

	__LOGT__( TAG, "connect_server(): client socket = %d", m_sockfd );

	memset( (void*)&m_sockaddr, 0x00, sizeof(sockaddr_in) );
	m_sockaddr.sin_family = AF_INET;
	m_sockaddr.sin_port = htons( get_server_port() );

	if ( get_server_ipaddr() ) {
#ifdef _WINDOWS
		m_sockaddr.sin_addr.S_un.S_addr = inet_addr( get_server_ipaddr() );
#else
		m_sockaddr.sin_addr.s_addr = inet_addr( get_server_ipaddr() );
#endif
	}
	else {
#ifdef _WINDOWS
		m_sockaddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );	// inet_addr( "192.168.0.x" );
#else
		m_sockaddr.sin_addr.s_addr = htonl( INADDR_ANY );	// inet_addr( "192.168.0.x" );
#endif
	}

	// Prevent bind error
	ret = setsockopt( m_sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );
	if ( ret < 0 ) {
		__LOGT__( TAG, "connect_server(): setsockopt(): prevent bind error [FAIL] (code = %d)", ret );
		client_close();
		return false;
	}
	else {
		__LOGT__( TAG, "connect_server(): setsockopt()" );
	}

	sockaddr_len = sizeof( m_sockaddr );
	ret = connect( m_sockfd, (struct sockaddr*)&m_sockaddr, sockaddr_len );
	if ( ret < 0 ) {
		__LOGT__( TAG, "connect_server(): connect() [FAIL]" );
		client_close();
		return false;
	}
	else {
		__LOGT__( TAG, "connect_server(): connect()" );
	}



	this->execute( NULL );

	return true;
}

void CAsyncTaskClient::client_close(void) {
	__LOGT__( TAG, "client_close()" );

#ifdef _WINDOWS
	::shutdown( m_sockfd, SD_BOTH );	// linux: SHUT_RDWR
	::closesocket( m_sockfd );
#else
	shutdown( m_sockfd, SHUT_RDWR );
	close( m_sockfd );
#endif
	m_sockfd = -1;
}

int CAsyncTaskClient::state(int sockfd, bool read, long usec, long nsec, fd_set* pfds) {
//int CAsyncTaskClient::state(int sockfd, bool read, long usec, long nsec) {
	//__LOGT__( TAG, "state()" );

	int ret = 0;
	struct timeval tv = { 0, 0 };


	if ( sockfd < 0 ) {
		__LOGT__( TAG, "state(): sockfd(%d) < 0", sockfd );
		return -1;
	}


	/*
	fd_set fds;
	FD_ZERO( &fds );
	FD_SET( sockfd, &fds );
	*/
	///*
	//FD_ZERO( pfds );
	//FD_SET( sockfd, pfds );
	//*/

	tv.tv_sec = usec;	// e.g., 1 (1 sec)
	tv.tv_usec = nsec;	// e.g., 500000 (0.5 sec)
	// e.g., 10.5 second
	//tv.tv_sec = 1;			// 1 (1 sec)
	//tv.tv_usec = 500000;	// 500000 (0.5 sec)

	__LOGT__( TAG, "state(): sockfd = %d", sockfd );

	// ret value
	//  (-1): error
	//  (0): timeout
	//  (> 0): ready to read/write
	ret = select( (sockfd + 1),
			/*
			(read? &fds : 0),
			(read? 0 : &fds),
			*/
			///*
			(read? pfds : 0),
			(read? 0 : pfds),
			//*/
			0,
			&tv );

	// -1: error
	if ( ret < 0 )
		;
	// 0: timeout
	else if ( ret == 0 )
		;
	// > 0: ready to read/write
	else {
		/*
		FD_ISSET( sockfd, &fds ) {
			recv( ... );
		}
		*/
	}

	return ret;
}

bool CAsyncTaskClient::established(void) {
	__LOGT__( TAG, "established()" );

	bool ret = true;
	fd_set _fds;
	fd_set fds;
	const int timeout = get_timeout();
	g_UTIL::datetime_st cur_dt;
	//long long total_bytes = 0l;

	memset( (void*)&cur_dt, 0x00, sizeof(g_UTIL::datetime_st) );
	
	/*
	get_date( cur_dt.year, cur_dt.month,
		cur_dt.day, cur_dt.hour,
		cur_dt.min, cur_dt.sec,
		cur_dt.millis );
	__LOGT__( TAG, "established(): [%s:%s:%s:%s:%s:%s:%s]\
			(socket = %d)",
			cur_dt.year, cur_dt.month,
			cur_dt.day, cur_dt.hour,
			cur_dt.min, cur_dt.sec,
			cur_dt.millis );

	*/
	get_date_d( &cur_dt.d_year, &cur_dt.d_month,
		&cur_dt.d_day, &cur_dt.d_hour,
		&cur_dt.d_min, &cur_dt.d_sec,
		&cur_dt.d_millis );
	__LOGT__( TAG, "established(): [%d:%d:%d:%d:%d:%d:%d] (socket = %d), ipaddr: %s:%d",
			cur_dt.d_year, cur_dt.d_month,
			cur_dt.d_day, cur_dt.d_hour,
			cur_dt.d_min, cur_dt.d_sec,
			cur_dt.d_millis,
			get_sockfd(), get_my_ipaddr(), get_my_port() );


	if ( get_sockfd() < 0 ) {
		__LOGT__( TAG, "established(): client sockfd(%d) < 0", get_sockfd() );
		ret = false;
		return ret;
	}

	FD_ZERO( &_fds );
	FD_SET( get_sockfd(), &_fds );



	while ( 1 ) {
		int bytes = 0;
		int ret_state = 0;



		get_date_d( &cur_dt.d_year, &cur_dt.d_month,
			&cur_dt.d_day, &cur_dt.d_hour,
			&cur_dt.d_min, &cur_dt.d_sec,
			&cur_dt.d_millis );
		__LOGT__( TAG, "established(): [%d:%d:%d:%d:%d:%d:%d] (socket = %d), ipaddr: %s:%d",
				cur_dt.d_year, cur_dt.d_month,
				cur_dt.d_day, cur_dt.d_hour,
				cur_dt.d_min, cur_dt.d_sec,
				cur_dt.d_millis,
				get_sockfd(), get_my_ipaddr(), get_my_port() );


		if ( get_cancel_state() ) {
			ret = false;
			break;
		}


		if ( get_sockfd() < 0 ) {
			__LOGT__( TAG, "established(): client sockfd(%d) < 0", get_sockfd() );
			ret = false;
			break;
		}

		fds = _fds;

		//__LOGT__( TAG, "established(): check a client state: sockfd = %d", get_sockfd() );
		//ret_state = g_UTIL::NET::state( get_sockfd(), true, timeout, 0, &fds );
		ret_state = state( get_sockfd(), true, timeout, 0, &fds );
		//ret_state = state( get_sockfd(), true, timeout, 0 );
		__LOGT__( TAG, "established(): select(): ret_state = %d", ret_state );


		if ( get_cancel_state() ) {
			ret = false;
			break;
		}

		if ( get_sockfd() < 0 ) {
			__LOGT__( TAG, "established(): client sockfd(%d) < 0", get_sockfd() );
			ret = false;
			break;
		}



		if ( ret_state == -1 ) {
			// SIGPIPE
			if ( errno == EPIPE ) {
				//! NOTE: write(), send()
			}
			// ???
			//else if ( errno == ??? ) {}
			// ...
			// SELECT error
			else {
				// error
				__LOGT__( TAG, "established(): error; select() [FAIL]" );

				ret = false;
				break;
			}
		}
		else if ( ret_state == 0 ) {
			// timeout
			__LOGT__( TAG, "established(): timeout; retry to check an event after %d seconds", get_timeout() );
		}
		else if ( ret_state > 0 ) {
			// ready to read/write
			__LOGT__( TAG, "established(): ready to read/write" );

			if ( FD_ISSET(get_sockfd(), &fds) ) {
				bytes = recv_data();

				if ( bytes <= 0 ) {
					//__LOGT__( TAG, "established(): received } (%ld bytes)", total_bytes );
					__LOGT__( TAG, "established(): received } (%d bytes)", bytes );
					__LOGT__( TAG, "established(): closed" );

					ret = false;
					break;
				}

				//total_bytes += bytes;
				//__LOGT__( TAG, "established(): (total %ld bytes)", total_bytes );
				__LOGT__( TAG, "established(): received }" );

				//send_data();
				send_data_motd();
			}
			else {
				__LOGT__( TAG, "established(): error; select(); ret_state = %d; FD_ISSET[FALSE] [FAIL]", ret_state );

				ret = false;
				break;
			}
		}
		else {
			// error
			__LOGT__( TAG, "established(): error; select(); ret_state = %d [FAIL]", ret_state );

			ret = false;
			break;
		}



		/*
		// Checks status
		if ( get_ready_for_send() ) {
			__LOGT__( TAG, "established(): (socket = %d), READY FOR SEND = %s",
					m_sockfd, (m_ready_for_send? "TRUE" : "FALSE") );

			set_ready_for_send( false );

			// Send data
			//setSendDataBuffer( L"echo 'hello~'" );
			//sendData();
		}
		*/

		//g_UTIL::usleep( 100 );
	} // while()

	return ret;
}

bool CAsyncTaskClient::send_data_motd(void) {
	__LOGT__( TAG, "send_data_motd()" );

	{
		char buf[DEFAULT_MAX_RECV_BUF_SIZE] = {0,};

		/*
		CAsyncTaskServer* pParent = (CAsyncTaskServer*)get_parent();
		if ( pParent ) {
			__LOGT__( TAG, "send_data_motd(): PARENT TAG = %s", pParent->get_tag() );
		}
		*/

		// Message
		snprintf( buf, sizeof(buf), "(server) [MOTD] hello client (%d)! [%s:%d]\n",
				get_tag_id(), get_my_ipaddr(), get_my_port() );

		if ( send(get_sockfd(), (char*)buf, sizeof(buf), 0) < 0 ) {
			__LOGT__( TAG, "send_data_motd(): send() [FAIL]; [MOTD] client sockfd(%d) < 0",
					get_sockfd() );
			__LOGT__( TAG, "send_data_motd(): SKIP MOTD" );
			return false;
		}
	}


	return true;
}

bool CAsyncTaskClient::send_data(void) {
	__LOGT__( TAG, "send_data()" );

	return true;
}

bool CAsyncTaskClient::send_data(int sockfd, const char* data, int len) {
	__LOGT__( TAG, "send_data()" );

	int bytes = 0;
	int total_bytes = 0;
	char buf[len + DEFAULT_MAX_SEND_BUF_EXTRA_SIZE];// = {0,};


	memset( (void*)&buf, 0x00, sizeof(buf) );


	__LOGT__( TAG, "send_data(): sent = {" );
	{
		CAsyncTaskServer* pParent = (CAsyncTaskServer*)get_parent();

		if ( pParent ) {
			const std::vector<CAsyncTaskClient*>* clients = pParent->get_clients();
			std::vector<CAsyncTaskClient*>::const_iterator iter;

			if ( clients ) {
				// C++11
				//for ( const auto& obj: clients ) {}
				//for ( auto iter = clients->begin(); iter != clients->end(); iter++ ) {}

				for ( iter = clients->begin(); iter != clients->end(); ++iter ) {
					CAsyncTaskClient* peer = (*iter);
					int sockfd_peer = -1;

					bytes = 0;
					total_bytes = 0;

					if ( peer == NULL )
						continue;

					if ( peer->get_tag_id() == get_tag_id() ) {
						__LOGT__( TAG, "send_data(): SKIP own tag id (%d)", get_tag_id() );
						continue;
					}

					if ( pParent->get_network_type() == e_nettype_ServerToPeer ) {
						if ( peer->get_tag_id() == sockfd ) {
							__LOGT__( TAG, "send_data(): target sockfd (%d), tag id (%d)",
									sockfd, peer->get_tag_id() );
						}
						else {
							continue;
						}
					}

					sockfd_peer = peer->get_sockfd();
					snprintf( buf, sizeof(buf), "[client(%d)]: %s\n", get_tag_id() );

					while ( 1 ) {
						bytes = send( sockfd_peer, (char*)buf, sizeof(buf), 0 );
						total_bytes += bytes;

						if ( total_bytes >= strlen(buf) )
							break;
					}

					if ( pParent->get_network_type() == e_nettype_ServerToPeer ) {
						break;
					}
				}
			}
		}
	}

	if ( bytes <= 0 ) {
		__LOGT__( TAG, "send_data(): error = %d; send() [FAIL]", bytes );
		return false;
	}

	__LOGT__( TAG, "send_data(): data(total %d bytes) = %s", total_bytes, buf );
	__LOGT__( TAG, "send_data(): sent }" );


	return true;
}

bool CAsyncTaskClient::recv_data(void) {
	__LOGT__( TAG, "recv_data()" );

	int bytes = 0;
	unsigned char buf[DEFAULT_MAX_RECV_BUF_SIZE] = {0,};


	__LOGT__( TAG, "recv_data(): received = {" );

	bytes = recv( get_sockfd(), buf, sizeof(buf), 0 );

	if ( bytes <= 0 ) {
		__LOGT__( TAG, "recv_data(): error = %d; recv() [FAIL]", bytes );
		return false;
	}

	__LOGT__( TAG, "recv_data(): data(%d bytes) = %s", bytes, buf );


	// Parse the message
	{
	}


	return true;
}

bool CAsyncTaskClient::send_data_motd_udp(void) {
	__LOGT__( TAG, "send_data_motd_udp()" );

	{
	}

	return true;
}

bool CAsyncTaskClient::send_data_udp(void) {
	__LOGT__( TAG, "send_data_udp()" );

	return true;
}

bool CAsyncTaskClient::send_data_udp(int sockfd, const char* data, int len) {
	__LOGT__( TAG, "send_data_udp()" );

	{
	}

	return true;
}

bool CAsyncTaskClient::recv_data_udp(void) {
	__LOGT__( TAG, "recv_data_udp()" );

	return true;
}
// ---------------------------------------------------------------



//! Implementation
// ---------------------------------------------------------------
// Global variable

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __REQ_MAIN_FUNCTION__
#endif //#ifdef __REQ_MAIN_FUNCTION__

namespace g_UTIL {
	void usleep(int millisecond) {
		struct timeval tv;

		tv.tv_sec = 0;
		tv.tv_usec = millisecond * 1000;	// microsecond

		select( 0, NULL, NULL, NULL, &tv );
	}

	std::string intToStr(int val) {
		std::stringstream ss;

		ss << val;

		return ss.str();
	}

	int _kbhit(void) {
		struct termios oldt, newt;
		int ch;

		tcgetattr( STDIN_FILENO, &oldt );

		newt = oldt;
		newt.c_lflag &= ~( ICANON | ECHO );

		tcsetattr( STDIN_FILENO, TCSANOW, &newt );

		ch = getchar();
		tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

		return ch;
	}

	// Network
	namespace NET {
		//! thread-unsafe
		int state(int sockfd, bool read, long usec, long nsec, fd_set* pfds) {
			//pthread_mutex_lock( &g_pthread_mutex_lock );

			int ret = 0;
			struct timeval tv = { 0, 0 };


			if ( sockfd < 0 ) {
				__LOGT__( "", "NET::state(): sockfd(%d) < 0", sockfd );
				return -1;
			}


			/*
			fd_set fds;
			FD_ZERO( &fds );
			FD_SET( sockfd, &fds );
			*/
			FD_ZERO( pfds );
			FD_SET( sockfd, pfds );

			tv.tv_sec = usec;	// e.g., 1 (1 sec)
			tv.tv_usec = nsec;	// e.g., 500000 (0.5 sec)
			// e.g., 10.5 second
			//tv.tv_sec = 1;			// 1 (1 sec)
			//tv.tv_usec = 500000;	// 500000 (0.5 sec)


			// ret value
			//  (-1): error
			//  (0): timeout
			//  (> 0): ready to read/write
			ret = select( (sockfd + 1),
					/*
					(read? &fds : 0),
					(read? 0 : &fds),
					*/
					(read? pfds : 0),
					(read? 0 : pfds),
					0,
					&tv );

			// -1: error
			if ( ret < 0 )
				;
			// 0: timeout
			else if ( ret == 0 )
				;
			// > 0: ready to read/write
			else {
				/*
				FD_ISSET( sockfd, &fds ) {
					recv( ... );
				}
				*/
			}

			//pthread_mutex_unlock( &g_pthread_mutex_lock );
			return ret;
		}
		
		void close_socket(int* sockfd) {
#ifdef _WINDOWS
			::shutdown( *sockfd, SD_BOTH );	// linux: SHUT_RDWR
			::closesocket( *sockfd );
#else
			shutdown( *sockfd, SHUT_RDWR );
			close( *sockfd );
#endif
			*sockfd = -1;
		}

		//! TCP/UDP hole punching for NAT
		//	1. connect to target with local(public or private) ip address
		//
		//	2. connect to target with public(for local private) ip address and port
		//		---------------------------------------
		//		NAT A  |  NAT B  |  STUN Server (Relay)
		//		---------------------------------------
		//		  |---------|-------->| REQ: public ip address and port for private ip address
		//		  |<--------|---------| RETURN: public(remote) ip address and port
		//		  |         |-------->| REQ: public ip address and port for private ip address
		//		  |         |<--------| RETURN: public(remote) ip address and port
		//		  |<------->|         | NAT A and NAT B connected with their public ip address and port
		//		---------------------------------------
		//
		//	3. Use a STUN Server for relay
		//		---------------------------------------
		//		NAT A  |  NAT B  |  STUN Server (Relay)
		//		---------------------------------------
		//		  |---------|-------->| REQ: NAT A to NAT B
		//		  |         |<--------| RELAY: NAT B
		//		  |         |-------->| REQ: NAT B to NAT A
		//		  |<--------|---------| RELAY: NAT A
		//		---------------------------------------
		/*
		bool get_remote_ip_port(const char* hosting, char* _ipaddr, int* _port) {
			int ret = 0;
			int reuse = 1;
			int sockfd = -1;
			socklen_t sockaddr_len = 0;
			struct sockaddr_in sockaddr;


			__LOGT__( TAG, "get_remote_ip_port(): Hosting Server IP address = %s", hosting );

			sockfd = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP );
			if ( sockfd < 0 ) {
				__LOGT__( TAG, "get_remote_ip_port(): socket() [FAIL]" );
				close_socket( &sockfd );
				return false;
			}
			else {
				__LOGT__( TAG, "get_remote_ip_port(): socket(): (socket = %d)", sockfd );
			}

			__LOGT__( TAG, "get_remote_ip_port(): client socket = %d", sockfd );

			memset( (void*)&sockaddr, 0x00, sizeof(sockaddr_in) );
			sockaddr.sin_family = AF_INET;
			sockaddr.sin_port = htons( get_server_port() );

			if ( hosting ) {
#ifdef _WINDOWS
				sockaddr.sin_addr.S_un.S_addr = inet_addr( hosting );
#else
				sockaddr.sin_addr.s_addr = inet_addr( hosting );
#endif
			}
			else {
				close_socket( &sockfd );
				return false;
			}

			// Prevent bind error
			ret = setsockopt( sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse) );
			if ( ret < 0 ) {
				__LOGT__( TAG, "get_remote_ip_port(): setsockopt(): prevent bind error [FAIL] (code = %d)",
						ret );
				close_socket( &sockfd );
				return false;
			}
			else {
				__LOGT__( TAG, "get_remote_ip_port(): setsockopt()" );
			}

			sockaddr_len = sizeof( sockaddr );
			ret = connect( sockfd, (struct sockaddr*)&sockaddr, sockaddr_len );
			if ( ret < 0 ) {
				__LOGT__( TAG, "get_remote_ip_port(): connect() [FAIL]" );
				close_socket( &sockfd );
				return false;
			}
			else {
				__LOGT__( TAG, "get_remote_ip_port(): connect()" );
			}


			// ...


			close_socket( &sockfd );
		}
		*/
	}  // namespace NET
} // namespace g_UTIL

#ifdef __cplusplus
}
#endif
// ---------------------------------------------------------------



#ifdef __REQ_MAIN_FUNCTION__
//! Main
// ---------------------------------------------------------------
int main(int argc, char *argv[]) {
	const char* TAG = "MAIN";
	__LOG_FMT__( "main()" );
	__LOGT__( TAG, "main()");


	__LOGT__( TAG, "-----------CNetSockLib Test------------ [");
	CNetSockLib netLib;
	netLib.m_asyncTask_server.start_server();

	__LOGT__( TAG, "<Press 'ESC' key to quit>" );
	while ( 1 ) {
		int ch = g_UTIL::_kbhit();

		__LOGT__( TAG, "<Pressed>: %d", ch );
		if ( ch == 27 ) {	// 'ESC'
			__LOGT__( TAG, "Quit..." );
			break;
		}

		g_UTIL::usleep( 100 );
	}

	netLib.m_asyncTask_server.stop_server();

	/*
	netLib.set_enable_show_info( true );
	netLib.retrieve_network_state();
	*/
	__LOGT__( TAG, "-----------CNetSockLib Test------------ ]");



	return 0;
}
// ---------------------------------------------------------------
#endif //#ifdef __REQ_MAIN_FUNCTION__
