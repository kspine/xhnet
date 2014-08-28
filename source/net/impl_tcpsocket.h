
#pragma once

#include "impl_nethead.h"

namespace xhnet
{
	class CIOServer;

	class CTcpSocket : public ITcpSocket, public CInheritOPool<CTcpSocket, std::mutex>
	{
	public:
		enum type
		{
			type_null = 0,
			type_listener,
			type_accepter,
			type_connecter,
		};

		enum status
		{
			status_null = 0,
			status_connect,
			status_common,
		};
	public:
		CTcpSocket();
		virtual ~CTcpSocket();

		// ����socket��ʼ��������������io�ͻص�����
		virtual bool Init_Listener(IIOServer* io, ICBTcpListener* cb);
		virtual bool Init_Accepter(IIOServer* io, ICBTcpAccepter* cb);
		virtual bool Init_Connecter(IIOServer* io, ICBTcpConneter* cb);
		// �첽�ر�socket��
		virtual void Fini(void);

		// �첽����socket�����ڼ�����ʱ���ٴε��ô˺���������Ա��β���������false
		virtual bool Listen(const std::string& ip, unsigned int port);
		// �첽���ӣ��������ӵ�ʱ���ٴε��ô˺���������Ա��β���������false
		virtual bool Connect(const std::string& ip, unsigned int port);

		//--���̰߳�ȫ��ֻ����io�߳��ڵ���
		// �첽����ָ��������Ϣ���������пɶ�����
		// ���ú�buffer����������һ������
		virtual bool Async_Send(CIOBuffer* buffer);
		// �첽����ָ��������Ϣ���������пɶ��ռ�
		// ���ú�buffer����������һ������
		virtual bool Async_Recv(CIOBuffer* buffer);
		// �첽�������ⳤ����Ϣ��һ���������ݾ���������
		// ���ú�buffer����������һ������
		virtual bool Async_RecvSome(CIOBuffer* buffer);
		//--end

		// ����ip�Ͷ˿�
		virtual std::string GetLocalIP(void);
		virtual unsigned int GetLocalPort(void);
		// Զ��ip�Ͷ˿�
		virtual std::string GetPeerIP(void);
		virtual unsigned int GetPeerPort(void);
		//
		virtual unsigned int GetSocketID(void);

	public:
		static void on_accept_callback(evutil_socket_t sock, short flag, void* p);
		static void on_recv_callback(evutil_socket_t sock, short flag, void* p);
		static void on_send_callback(evutil_socket_t sock, short flag, void* p);

	protected:
		void real_listen(std::string ip, unsigned int port);
		void real_connect(std::string ip, unsigned int port);
		void real_fini();

		void on_inter_listen_accept(evutil_socket_t listenfd);
		void on_inter_accept_accept(evutil_socket_t fd, socketaddr* peeraddr, int addrlen);
		void on_inter_recv(short flag);
		void on_inter_send(short flag);
		void on_inter_close(int errid, bool bconnecting);
	protected:
		unsigned char	m_type;
		bool			m_binit;
		unsigned char	m_status;
		unsigned int	m_id;
		evutil_socket_t	m_socket;

		CIOServer*		m_io;

		std::string		m_localip;
		unsigned int	m_localport;

		std::string		m_peerip;
		unsigned int	m_peerport;

		// ��������
		union
		{
			ICBTcpListener*	m_lcb;
			ICBTcpAccepter*	m_acb;
			ICBTcpConneter*	m_ccb;
		};

		unsigned char	m_bassign;	// 0x01 ��ʾ�Ѿ���m_ev_readassign�� 0x02��m_ev_write assign��
		::event			m_ev_read;
		::event			m_ev_write;

		std::queue<ITcpSocket*>	m_wait_accept;
		std::queue<CIOBuffer*>	m_wait_send;
		std::queue<CIOBuffer*>	m_wait_recv;

	private:
		CTcpSocket(const CTcpSocket&) = delete;
		CTcpSocket& operator=(const CTcpSocket&) = delete;
	};
};

