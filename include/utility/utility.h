
#pragma once

#include "../xhhead.h"

#include <string>
#include <sstream>
#include <vector>

namespace xhnet
{
	// ����ת��
	// һ��serverֻ�ö��ֽڱ����utf8������ֻ���ַ�������
	//
	std::string	Convert_String(const std::string &text, char* out, unsigned int outlen, const std::string &to_encoding, const std::string &from_encoding);
	// end ����ת��


	// 
	// ����ת��
	//
	template<class T>
	std::string T2S(const T& t)
	{
		std::ostringstream os;
		os << t;
		return os.str();
	}
	template<class T>
	T S2T(const std::string& s, T& t)
	{
		std::istringstream is(s);
		is >> t;
		return t;
	}
	int			Str2Int(const std::string& data, int default = 0);
	std::string	Int2Str(int data, const std::string& default = "");
	unsigned int Str2UInt(const std::string& data, unsigned int default = 0);
	std::string	UInt2Str(unsigned int data, const std::string& default = "");
	// end ����ת��

	//
	// �ַ�������
	//
	// �ַ����ָ�
	std::vector<std::string> SplitString(const std::string& str, const std::string& spt);

	// ��Сдת��
	std::string UpLetters(std::string& inoutstr);
	std::string LowLetters(std::string& inoutstr);
	// end �ַ�������


	//
	// �ַ���hash�� ʹ��BKDR Hash 
	unsigned int StrHash(const std::string& data);
	// end hash


	//
	// Ŀ¼�� ����Ŀ¼������ϵͳΪϵͳ����
	std::string	GetModulePath(void);
	bool		CreateDirectory(const std::string& path);
	bool		IsSamePath(const std::string& path1, const std::string& path2);
	// end Ŀ¼


	//
	// ʱ�䣬
	// ���µ�time_tʹ��UTCʱ�䣬 time_t���л���ʱ�������ȷָ���������ͣ��������ƽ̨���������³��Ȳ�һ
	// ���µ�tmΪ����ʱ��
	//
	// ����ʱ����utcʱ��� ����� ���� �ɸ�
	int			LocalTime_UTCTime(void);
	// ���ݱ���������ȡutc����
	time_t		LocalTime2UTCTime(time_t localtime);
	// ����utc������ȡ��������
	time_t		UTCTime2LocalTime(time_t utctime);
	unsigned long long GetNowTimeMs(void);
	time_t		GetNowTime(void);
	tm			GetNowTM(void);

	tm			Time2TM(time_t intime);
	time_t		TM2Time(tm& intime);

	// since 1900
	int			GetNowYear(void);
	// 0-11
	int			GetNowMonth(void);
	// 0-365
	int			GetNowYearDay(void);
	// 1-31
	int			GetNowMonthDay(void);
	// 0-6 since Sunday
	int			GetNowWeekDay(void);

	//
	void		SleepMilliseconds(unsigned int ms);
	// ���cpu����������ʱ���������, �뼶�� ��������
	unsigned long GetCPUTickCount(void);

	// �ǲ��Ƕ�������
	bool		IsDigit(const std::string& str);
	// 2012-10-22 10:22:52.1111
	bool		IsTimeStr(const std::string& timestr);

	// time_t 2 std::string "2012-10-22 10:22:52.1111"
	std::string	Time2Str(time_t timesecond);
	// 
	time_t		Str2Time(const std::string& timestr);

	// �����0��0��0��
	time_t		Get_0H0M0S(tm intm);

	// ���Ա���ʱ��Ϊ׼
	// ���ܿ�ʼʱ��
	time_t		GetSunday_0H0M0S(tm intm);
	// ���¿�ʼʱ��
	time_t		GetFirstMDay_0H0M0S(tm intm);
	// �������һ��
	time_t		GetLastMDay_23H59M59S(tm intm);
	// ���꿪ʼʱ��
	time_t		GetFirstYDay_0H0M0S(tm intm);
	// �������ʱ��
	time_t		GetLastYDay_23H59M59S(tm intm);

	// �ǲ���ͬһ��
	bool		IsSameDay(time_t dayl, time_t dayr);
	// dayl>dayr
	bool		IsBigDay(time_t dayl, time_t dayr);
	// dayl<dayr
	bool		IsSmallDay(time_t dayl, time_t dayr);
	// end ʱ�����


	//
	// ���ֵ
	//
	unsigned int GenRandUINT(unsigned int min = 0x0, unsigned int max = 0xffffffff);
	// end ���ֵ


	//
	// UUID
	// 
	std::string	GenRandUUID(void);
	// end UUID


	//
	// MD5
	//
	// ����32λmd5
	std::string GenMd5(unsigned char* data, unsigned int datalen);
	//
	void GenMd5(unsigned char md5[16], unsigned char* data, unsigned int datalen);
	// end md5


	// uint�Ӽ�
	// unsigned int ++
	unsigned int UINTAdd(unsigned int src, unsigned int &add, unsigned int max = 0xffffffff);
	unsigned int UINTAddConst(unsigned int src, const unsigned int add, unsigned int max = 0xffffffff);
	// unsigned int --
	unsigned int UINTSub(unsigned int src, unsigned int &sub, unsigned int max = 0xffffffff);
	unsigned int UINTSubConst(unsigned int src, const unsigned int sub, unsigned int max = 0xffffffff);
	// end uint op


	//
	// DLL\SO ����
	//
	// ʧ�ܷ���0
	void* OpenDyLib(const std::string& path);
	void CloseDyLib(void* dy);
	void* GetDyLibFun(void* dy, const std::string& funname);
	// end DLL\SO ����
};


