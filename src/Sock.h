// Sock.h: CSock �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCK_H__B69C7ABE_CD77_4E9B_892E_2E34147BCE2F__INCLUDED_)
#define AFX_SOCK_H__B69C7ABE_CD77_4E9B_892E_2E34147BCE2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma comment( lib, "wsock32.lib" ) 

#include <windows.h>
#include <winsock.h>
#include <sys/types.h>
#include <process.h>

#define RECV_BUFFER_SIZE			2048			// recv()�̎�M�p�o�b�t�@�T�C�Y

class CSock  
{
	CRITICAL_SECTION m_CriticalSection;
protected:
	struct sockaddr_in m_InetSockAddrFrom;		// �C���^�[�l�b�g�p�\�P�b�g�A�h���X�\����
	struct sockaddr_in m_InetSockAddrTo;		// �C���^�[�l�b�g�p�\�P�b�g�A�h���X�\����

public:
	CSock();
	virtual ~CSock();
public:
	SOCKET m_hSocket;	// �\�P�b�g�n���h��
	SOCKET m_hClientSocket;

//	void Attach( SOCKET hSocket );
//	void Detach();

	bool Socket( int nAddrFamily = AF_INET, int nSocketType = SOCK_STREAM, int nProtocol = IPPROTO_TCP );
	bool Bind(int nPort );
	bool Listen( int nMaxCon = 5 );
	bool Accept();
//	bool Connect( LPCTSTR lpszIP, int nPort );
	void Close();

	int Send( const char *szMsg, size_t nByte );

	int Recv( char *szBuff );

	bool IsValid();

	void Lock(){ EnterCriticalSection( &m_CriticalSection ); };
    void Unlock(){ LeaveCriticalSection( &m_CriticalSection ); };
};

#endif // !defined(AFX_SOCK_H__B69C7ABE_CD77_4E9B_892E_2E34147BCE2F__INCLUDED_)
