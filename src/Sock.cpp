// Sock.cpp: CSock �N���X�̃C���v�������e�[�V����
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
// �\�z/����
//////////////////////////////////////////////////////////////////////

//
// ���O		:CSock::CSock()
// �@�\		:�f�t�H���g�R���X�g���N�^
// ����		:�Ȃ�
// �߂�l	:�Ȃ�
//
CSock::CSock()
{
	m_hSocket = INVALID_SOCKET;
	InitializeCriticalSection( &m_CriticalSection );
}

//
// ���O		:CSock::~CSock()
// �@�\		:�f�X�g���N�^
// ����		:�Ȃ�
// �߂�l	:�Ȃ�
//
CSock::~CSock()
{
}

/*
//
// ���O		:void CSock::Attach( SOCKET hSocket )
// �@�\		:�\�P�b�g�n���h�����֘A�t����
// ����		:hSocket - �\�P�b�g�n���h��
// �߂�l	:�Ȃ�
//
void CSock::Attach( SOCKET hSocket )
{
	m_hSocket = hSocket;
}

//
// ���O		:void CSock::Detach()
// �@�\		:�����o�̃\�P�b�g�n���h�������
// ����		:�Ȃ�
// �߂�l	:�Ȃ�
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
// ���O		:bool CSock::Socket( int nAddrFamily, int nSocketType, int nProtocol )
// �@�\		:�V�����\�P�b�g�n���h�����쐬����
// ����		:nAddrFamily - �A�h���X�t�@�~��
//			:nSocketType - �\�P�b�g�̎��
//			:nProtocol - �v���g�R��
// �߂�l	:������ - true ���s�� - false
//
bool CSock::Socket( int nAddrFamily, int nSocketType, int nProtocol )
{
	m_hSocket = socket( nAddrFamily, nSocketType, nProtocol );
	if( m_hSocket == INVALID_SOCKET ){return false;}
	return true;
}

//
// ���O		:bool CSock::Bind( CSockAddr *addr )
// �@�\		:�\�P�b�g�������̃A�h���X�Ƀo�C���h����
// ����		:*addr - �o�C���h����A�h���X����ێ������C���X�^���X�̃|�C���^
// �߂�l	:������ - true ���s�� - false
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
// ���O		:bool CSock::Listen( int nMaxCon )
// �@�\		:�\�P�b�g�������̃A�h���X�Ƀo�C���h����
// ����		:*addr - �o�C���h����A�h���X����ێ������C���X�^���X�̃|�C���^
// �߂�l	:������ - true ���s�� - false
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
// ���O		:bool CSock::Accept()
// �@�\		:�����̃\�P�b�g�Őڑ��v���̃A�N�Z�v�g
// ����		:&sock - �A�N�Z�v�g�p�̃\�P�b�g
// �߂�l	:������ - TRUE ���s�� - FALSE
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
// ���O		:bool CSock::Connect( CSockAddr *addr )
// �@�\		:�����̃A�h���X��ɐڑ�
// ����		:*addr - �ڑ�����A�h���X����ێ������C���X�^���X�̃|�C���^
// �߂�l	:������ - true ���s�� - false
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
// ���O		:void CSock::Close()
// �@�\		:�����o�̃\�P�b�g�n���h�������
// ����		:
// �߂�l	:
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
// ���O		:int CSock::Send( const char *szMsg, int nByte )
// �@�\		:sMsg��nByte�o�C�g����
// ����		:sMsg - ���M����o�b�t�@�̃|�C���^
//			:nByte - ����o�C�g��
// �߂�l	:���ۂɑ���ꂽ�o�C�g��
//
int CSock::Send( const char *szMsg, size_t nByte )
{
	if( m_hClientSocket == INVALID_SOCKET ){return 0;}
	return send( m_hClientSocket, szMsg, nByte, 0 );
}

//
// ���O		:int CSock::Recv( char *szBuff )
// �@�\		:��M���āAsBuff�Ɋi�[����
// ����		:szBuff - ��M�p�o�b�t�@�̃|�C���^
// �߂�l	:��M�����o�C�g��
//
int CSock::Recv( char *szBuff )
{
	if( m_hClientSocket == INVALID_SOCKET ){return 0;}
	return recv( m_hClientSocket, szBuff, RECV_BUFFER_SIZE, 0 );
}

//
// ���O		:bool CSock::Detach()
// �@�\		:�����o�̃\�P�b�g�n���h�����L�������ׂ�
// ����		:
// �߂�l	:�L�� - true ���� - false
//
bool CSock::IsValid()
{
	if( m_hSocket == INVALID_SOCKET ){return false;}
	return true;
}
