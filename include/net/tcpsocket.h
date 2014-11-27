
#pragma once

#include "iobuffer.h"

namespace xhnet
{
	class IIOServer;
	class ITcpSocket;

	enum tcpsocket_err
	{
		tcp_ok	= 0,

		tcp_listenfail_iperr,
		tcp_listenfail_createerr,
		tcp_listenfail_binderr,
		tcp_listenfail_listenerr,

		tcp_connectfail_iperr,
		tcp_connectfail_createerr,
		tcp_connectfail_connecterr,
		tcp_connectfail_timeout,

		tcp_recvfail_closedbypeer,
		tcp_recvfail_recverr,
		tcp_sendfail_senderr,

		tcp_close_bylocal,
	};

	//
	//
	// socket״̬����
	//
	//       Init             Listen/Connect               On_Listen/On_Connect/On_Connect
	// null-------->init--------------------------->connect------------------------------>common
	//   ^           ^									|									|
	//   |           |									|									|
	//	 |		     ------------------------------------------------------------------------
	//   |										On_Close/On_Close/On_Close
	//   |                                                 |
	//   |--------------------------------------------------
	//			Fini
	//   (init״̬�µ���Fini�ǲ������On_Close��)
	//	 (On_Accept��socket��״̬��null����Ҫ��On_Accept��Init_Accepter������socket�Զ��ͷ�)
	//	 (init�Ժ�������Fini)
	//
	//
	// 1���κ�socket Init�Ժ󣬶���Ҫ����Fini��������
	// 2��socket����Fini�����رգ�tcp_close_bylocal��������socket�����ԭ��رգ��������On_Close
	// 3��Fini���Զ�ε��ã�Fini�Ժ���Ҫ���ö�����ٴ�Init
	// 4��socket�ڻص�On_Close��ʱ��δ�����ù�Fini��������ٴ�Listen��Connect
	// 5��Async_Send��Async_Recv��Async_RecvSomeֻ���ڻص�On_Listen��On_Connect��On_Connect�ɹ���ʱ�����Ч������socket����common״̬
	//
	//
	//
	class ICBTcpListener : virtual public CPPRef
	{
	public:
		virtual ~ICBTcpListener() { }

		virtual void On_Listen(unsigned int socketid, int errid) = 0;
		virtual void On_Accept(unsigned int socketid, ITcpSocket* socket) = 0;
		virtual void On_Close(unsigned int socketid, int errid) = 0;
	};

	class ICBTcpAccepter : virtual public CPPRef
	{
	public:
		virtual ~ICBTcpAccepter() { }

		virtual void On_Connect(unsigned int socketid, int errid) = 0;
		virtual void On_Send(unsigned int socketid, int errid, CIOBuffer* buffer) = 0;
		virtual void On_Recv(unsigned int socketid, int errid, CIOBuffer* buffer) = 0;
		virtual void On_Close(unsigned int socketid, int errid) = 0;
	};

	class ICBTcpConneter : virtual public CPPRef
	{
	public:
		virtual ~ICBTcpConneter() { }

		virtual void On_Connect(unsigned int socketid, int errid) = 0;
		virtual void On_Send(unsigned int socketid, int errid, CIOBuffer* buffer) = 0;
		virtual void On_Recv(unsigned int socketid, int errid, CIOBuffer* buffer) = 0;
		virtual void On_Close(unsigned int socketid, int errid) = 0;
	};

	class ITcpSocket : virtual public CPPRef
	{
	public:
		// ��socket��ʼ�����ж���ʽ��ʼ��
		// ����socket��ֻ������һ�����󣬲���ԭʼsocket���κδ���
		// listener ָ���Ǽ���socket
		// accepter ָ���Ǽ���socket���յ�socket
		// connecterָ�������ӳ�ȥ��socket
		// listener��accepter��connecter�����ⲿ������
		static ITcpSocket* Create(void);

	public:
		virtual ~ITcpSocket(void) {  }

		// ����socket��ʼ��������������io�ͻص�����
		virtual bool Init_Listener(IIOServer* io, ICBTcpListener* cb) = 0;
		virtual bool Init_Accepter(IIOServer* io, ICBTcpAccepter* cb) = 0;
		virtual bool Init_Connecter(IIOServer* io, ICBTcpConneter* cb) = 0;
		// �첽�ر�socket��
		virtual void Fini(void) = 0;

		// �첽����socket�����ڼ�����ʱ���ٴε��ô˺���������Ա��β���������false
		virtual bool Listen(const std::string& ip, unsigned int port) = 0;
		// �첽���ӣ��������ӵ�ʱ���ٴε��ô˺���������Ա��β���������false
		virtual bool Connect(const std::string& ip, unsigned int port) = 0;

		//--���̰߳�ȫ��ֻ����io�߳��ڵ���
		// �첽����ָ��������Ϣ���������пɶ�����
		// ���ú�buffer����������һ������
		virtual bool Async_Send(CIOBuffer* buffer) = 0;
		// �첽����ָ��������Ϣ���������пɶ��ռ�
		// ���ú�buffer����������һ������
		virtual bool Async_Recv(CIOBuffer* buffer) = 0;
		// �첽�������ⳤ����Ϣ��һ���������ݾ���������
		// ���ú�buffer����������һ������
		virtual bool Async_RecvSome(CIOBuffer* buffer) = 0;
		//--end

		// ����ip�Ͷ˿�
		virtual std::string GetLocalIP(void) = 0;
		virtual unsigned int GetLocalPort(void) = 0;
		// Զ��ip�Ͷ˿�
		virtual std::string GetPeerIP(void) = 0;
		virtual unsigned int GetPeerPort(void) = 0;
		//
		virtual unsigned int GetSocketID(void) = 0;
	};
};


