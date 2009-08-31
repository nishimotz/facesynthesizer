// Sock.cpp: CSock クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Sock.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

//
// 名前		:CSock::CSock()
// 機能		:デフォルトコンストラクタ
// 引数		:なし
// 戻り値	:なし
//
CSock::CSock()
{
	m_hSocket = INVALID_SOCKET;
	InitializeCriticalSection( &m_CriticalSection );
}

//
// 名前		:CSock::~CSock()
// 機能		:デストラクタ
// 引数		:なし
// 戻り値	:なし
//
CSock::~CSock()
{
}

/*
//
// 名前		:void CSock::Attach( SOCKET hSocket )
// 機能		:ソケットハンドルを関連付ける
// 引数		:hSocket - ソケットハンドル
// 戻り値	:なし
//
void CSock::Attach( SOCKET hSocket )
{
	m_hSocket = hSocket;
}

//
// 名前		:void CSock::Detach()
// 機能		:メンバのソケットハンドルを閉じる
// 引数		:なし
// 戻り値	:なし
//
void CSock::Detach()
{
	if( m_hSocket != INVALID_SOCKET )
	{
		shutdown( m_hSocket, 0x02 );
		closesocket( m_hSocket );
		m_hSocket = INVALID_SOCKET;
	}
}
*/

//
// 名前		:bool CSock::Socket( int nAddrFamily, int nSocketType, int nProtocol )
// 機能		:新しいソケットハンドルを作成する
// 引数		:nAddrFamily - アドレスファミリ
//			:nSocketType - ソケットの種類
//			:nProtocol - プロトコル
// 戻り値	:成功時 - true 失敗時 - false
//
bool CSock::Socket( int nAddrFamily, int nSocketType, int nProtocol )
{
	m_hSocket = socket( nAddrFamily, nSocketType, nProtocol );
	if( m_hSocket == INVALID_SOCKET ){return false;}
	return true;
}

//
// 名前		:bool CSock::Bind( CSockAddr *addr )
// 機能		:ソケットを引数のアドレスにバインドする
// 引数		:*addr - バインドするアドレス情報を保持したインスタンスのポインタ
// 戻り値	:成功時 - true 失敗時 - false
//
bool CSock::Bind(int nPort )
{
//	this->m_InetSockAddrFrom.sin_addr.s_addr = inet_addr( lpszIP );
	this->m_InetSockAddrTo.sin_addr.s_addr = htonl(INADDR_ANY);
	this->m_InetSockAddrFrom.sin_port = htons( (u_short)nPort );
	this->m_InetSockAddrFrom.sin_family = AF_INET;

	if( m_hSocket == INVALID_SOCKET ){return false;}

	if( bind( m_hSocket, (struct sockaddr*)&m_InetSockAddrFrom, sizeof( sockaddr_in ) ) == SOCKET_ERROR )
		return false;
	return true;
}

//
// 名前		:bool CSock::Listen( int nMaxCon )
// 機能		:ソケットを引数のアドレスにバインドする
// 引数		:*addr - バインドするアドレス情報を保持したインスタンスのポインタ
// 戻り値	:成功時 - true 失敗時 - false
//
bool CSock::Listen( int nMaxCon )
{
	if( m_hSocket == INVALID_SOCKET ||
		nMaxCon < 1
		){return false;}

	if( listen( m_hSocket, nMaxCon ) == SOCKET_ERROR )
		return false;
	return true;
}

//
// 名前		:bool CSock::Accept()
// 機能		:引数のソケットで接続要求のアクセプト
// 引数		:&sock - アクセプト用のソケット
// 戻り値	:成功時 - TRUE 失敗時 - FALSE
//
bool CSock::Accept()
{
	if( m_hSocket == INVALID_SOCKET ){return false;}

	printf("FSM Server mode. waiting...\n");

	int nSize = sizeof( sockaddr );
	m_hClientSocket = accept( m_hSocket, NULL, &nSize );

	printf("conected\n");

	return true;
}

/*
//
// 名前		:bool CSock::Connect( CSockAddr *addr )
// 機能		:引数のアドレス先に接続
// 引数		:*addr - 接続するアドレス情報を保持したインスタンスのポインタ
// 戻り値	:成功時 - true 失敗時 - false
//
bool CSock::Connect( LPCTSTR lpszIP, int nPort )
{
	this->m_InetSockAddrTo.sin_addr.s_addr = inet_addr( lpszIP );
	this->m_InetSockAddrTo.sin_port = htons( (u_short)nPort );
	this->m_InetSockAddrTo.sin_family = AF_INET;

	if( m_hSocket == INVALID_SOCKET ){return false;}

	if( connect( m_hSocket, (struct sockaddr*)&m_InetSockAddrTo, sizeof( sockaddr_in ) ) == SOCKET_ERROR )
		return false;
	return true;
}
*/

//
// 名前		:void CSock::Close()
// 機能		:メンバのソケットハンドルを閉じる
// 引数		:
// 戻り値	:
//
void CSock::Close()
{
	shutdown( m_hClientSocket, 0x02 );
	closesocket( m_hClientSocket );
	m_hClientSocket = INVALID_SOCKET;

	shutdown( m_hSocket, 0x02 );
	closesocket( m_hSocket );
	m_hSocket = INVALID_SOCKET;
}

//
// 名前		:int CSock::Send( const char *szMsg, int nByte )
// 機能		:sMsgをnByteバイト送る
// 引数		:sMsg - 送信するバッファのポインタ
//			:nByte - 送るバイト数
// 戻り値	:実際に送られたバイト数
//
int CSock::Send( const char *szMsg, size_t nByte )
{
	if( m_hClientSocket == INVALID_SOCKET ){return 0;}
	return send( m_hClientSocket, szMsg, nByte, 0 );
}

//
// 名前		:int CSock::Recv( char *szBuff )
// 機能		:受信して、sBuffに格納する
// 引数		:szBuff - 受信用バッファのポインタ
// 戻り値	:受信したバイト数
//
int CSock::Recv( char *szBuff )
{
	if( m_hClientSocket == INVALID_SOCKET ){return 0;}
	return recv( m_hClientSocket, szBuff, RECV_BUFFER_SIZE, 0 );
}

//
// 名前		:bool CSock::Detach()
// 機能		:メンバのソケットハンドルが有効か調べる
// 引数		:
// 戻り値	:有効 - true 無効 - false
//
bool CSock::IsValid()
{
	if( m_hSocket == INVALID_SOCKET ){return false;}
	return true;
}
