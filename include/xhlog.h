
#pragma once

#include "log/filelog.h"
#include "log/stacktrace.h"
#include "log/actionstat.h"


namespace xhnet
{
	// ��logplus�ĵȼ�һ��
	const int LOGLEVEL_OFF = log4cplus::OFF_LOG_LEVEL;
	const int LOGLEVEL_FATAL = log4cplus::FATAL_LOG_LEVEL;
	const int LOGLEVEL_ERROR = log4cplus::ERROR_LOG_LEVEL;
	const int LOGLEVEL_WARN = log4cplus::WARN_LOG_LEVEL;
	const int LOGLEVEL_INFO = log4cplus::INFO_LOG_LEVEL;
	const int LOGLEVEL_DEBUG = log4cplus::DEBUG_LOG_LEVEL;
	const int LOGLEVEL_TRACE = log4cplus::TRACE_LOG_LEVEL;
	const int LOGLEVEL_ALL = LOGLEVEL_TRACE;

	// log names ϵͳ�ڲ���log name
	extern char const logname_base[];
	extern char const logname_trace[];
};


// �Ƿ� ���� console log
#define XH_OPEN_CONSOLELOGGER() \
	do { \
	::xhnet::CFileLogger::GetInstance().OpenConsole(); \
	} while( 0 )

// ���logger
#define XH_ADD_LOGGER(logname, logpath, loglevel) \
	do { \
	::xhnet::CFileLogger::GetInstance().AddLogger( ::xhnet::CLogger<logname>::GetInstance().m_logger, logpath, loglevel ); \
	} while( 0 )

#define XH_ADD_SAMEFILE_LOGGER(subname, mainname, loglevel) \
	do { \
	::xhnet::CFileLogger::GetInstance().AddSameFileLogger( ::xhnet::CLogger<subname>::GetInstance().m_logger, ::xhnet::CLogger<mainname>::GetInstance().m_logger, loglevel ); \
	} while( 0 )

// �޸���־�ȼ�
#define XH_MOD_LOGLEVEL(logname, loglevel) \
	do { \
	::xhnet::CFileLogger::GetInstance().SetLogger( ::xhnet::CLogger<logname>::GetInstance().m_logger, loglevel ); \
	} while( 0 )

// log
#define XH_LOG_FATAL(logname, logevent) \
	LOG4CPLUS_FATAL(::xhnet::CLogger<logname>::GetInstance().m_logger, logevent)

#define XH_LOG_ERROR(logname, logevent) \
	LOG4CPLUS_ERROR(::xhnet::CLogger<logname>::GetInstance().m_logger, logevent)

#define XH_LOG_WARN(logname, logevent) \
	LOG4CPLUS_WARN(::xhnet::CLogger<logname>::GetInstance().m_logger, logevent)

#define XH_LOG_INFO(logname, logevent) \
	LOG4CPLUS_INFO(::xhnet::CLogger<logname>::GetInstance().m_logger, logevent)

#define XH_LOG_DEBUG(logname, logevent) \
	LOG4CPLUS_DEBUG(::xhnet::CLogger<logname>::GetInstance().m_logger, logevent)

#define XH_LOG_TRACE(logname, logevent) \
	LOG4CPLUS_TRACE(::xhnet::CLogger<logname>::GetInstance().m_logger, logevent)



// ��������ջ��¼
#ifdef _DEBUG
#define XH_STACK_TRACE()				__STACK_TRACE_IN__()
#else
#define XH_STACK_TRACE()				( (void)0 )
#endif


// ��Ϊ��¼
#define XH_ACTION_GUARD()				::xhnet::COneAction XH_ANONYMOUS_VARIABLE(actionguard)(__FUNCTION__)
inline
void XH_ACTION_TRAVERSE(std::function<void(const std::string& action, const ::xhnet::CActionRecord& record)> f)	
{
	::xhnet::CActionStat::GetInstance().TraverseActions(f); 
}


// ��Ϊ��¼�ͺ���ջͬʱ��¼
#define XH_ACTION_TRACE() XH_ACTION_GUARD();XH_STACK_TRACE()
