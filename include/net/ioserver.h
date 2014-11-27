
#pragma once

#include "iobuffer.h"

// 
// io�����ж�����ڴ������� ���������ʽ
// 

namespace xhnet
{
	typedef std::function<void()> postio;

	//
	// run ֻ����һ���߳������� one thread one run
	//
	class IIOServer : virtual public CPPRef
	{
	public:
		// ��ʼ�����ж���ʽ��ʼ��
		static IIOServer* Create(void);

		static std::vector<std::string> Resolve_DNS(const std::string& hostname);
	public:
		virtual ~IIOServer(void) {  }

		virtual bool Init(void) = 0;
		virtual void Fini(void) = 0;

		virtual void Run(void) = 0;
		virtual bool IsFinshed(void) = 0;

		virtual void Post(postio io) = 0;
	};
};
