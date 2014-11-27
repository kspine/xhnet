
#pragma once

#include "iobuffer.h"

namespace xhnet
{
	class IIOServer;


	enum timer_err
	{
		timer_ok = 0,
		timer_begin,
		timer_end,
	};

	// ���лص�������io�߳���
	class ICBTimer : virtual public CPPRef
	{
	public:
		virtual ~ICBTimer() { }

		virtual void On_Timer(unsigned int timerid, int errid) = 0;
	};

	class ITimer : virtual public CPPRef
	{
	public:
		// ��ʼ�����ж���ʽ��ʼ��
		// ������ֻ������һ������
		static ITimer* Create(void);

	public:
		virtual ~ITimer(void) {  }

		// ��ʼ���ص�
		virtual bool Init(IIOServer* io, ICBTimer* cb) = 0;
		// �첽�ر�socket���첽������-������������0��ʱ���Զ�����
		virtual void Fini(void) = 0;

		//--���̰߳�ȫ��ֻ����io�߳��ڵ���
		// ����ÿ��ms���룬�����һ��callback�����������ظ����ã�
		// �ص�����errid���ٻص�
		virtual bool Async_Wait(unsigned int ms) = 0;
		//--end

		virtual unsigned int GetTimerID(void) = 0;
	};
};

