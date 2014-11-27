
#pragma once

#include "iobuffer.h"

namespace xhnet
{
	class IIOServer;

	enum udpsocket_err
	{
		udp_ok = 0,

		udp_listenfail_iperr,
		udp_listenfail_createerr,
		udp_listenfail_binderr,

		udp_recvfail_closedbypeer,
		udp_recvfail_recverr,

		udp_sendfail_iperr,
		udp_sendfail_senderr,

		udp_close,
	};

	// ���лص�������io�߳���
	class ICBUdpSocket : virtual public CPPRef
	{
	public:
		virtual ~ICBUdpSocket() { }

		virtual void On_Listen(unsigned int socketid, int errid) = 0;
		virtual void On_Send(unsigned int socketid, int errid, const std::string& ip, unsigned int port, CIOBuffer* buffer) = 0;
		virtual void On_Recv(unsigned int socketid, int errid, const std::string& ip, unsigned int port, CIOBuffer* buffer) = 0;
	};

	class IUdpSocket : virtual public CPPRef
	{
	public:
		// ��socket��ʼ�����ж���ʽ��ʼ��
		// ����socket��ֻ������һ�����󣬲���ԭʼsocket���κδ���
		static IUdpSocket* Create(void);

	public:
		virtual ~IUdpSocket(void)						{  }

		// ����socket��ʼ��������������io�ͻص�����
		virtual bool Init(IIOServer* io, ICBUdpSocket* cb) = 0;
		// �첽�ر�socket���첽������-������������0��ʱ���Զ�����
		virtual void Fini(void) = 0;

		// �첽����socket�����ڼ�����ʱ���ٴε��ô˺���������Ա��β���������false
		virtual bool Listen(const std::string& ip, unsigned int port) = 0;

		//--���̰߳�ȫ��ֻ����io�߳��ڵ���
		// �첽����ָ��������Ϣ��
		virtual bool Async_SendTo(const std::string& ip, unsigned int port, CIOBuffer* buffer) = 0;
		// �첽�������ⳤ����Ϣ
		virtual bool Async_RecvSomeFrom(CIOBuffer* buffer) = 0;
		//--end

		// ����ip�Ͷ˿�
		virtual std::string GetLocalIP(void) = 0;
		virtual unsigned int GetLocalPort(void) = 0;

		//
		virtual unsigned int GetSocketID(void) = 0;
	};

};

