/// @license MIT license http://opensource.org/licenses/MIT
/// @author David Siroky <siroky@dasir.cz>
/// @file

#pragma once

namespace xhnet
{
	// try catch c++�쳣��Ȼ��ʹ��print_traceback��ӡ�ڴ��ջ

	// windows ��
	// 1��ʹ�� _set_se_translator ת��VC SEH Ϊ c++�쳣
	// 2������ EHa �쳣����ѡ�
	// ѡ��1�������߳���أ�ÿ���̶߳�Ҫ���ã�������������õ�ģ��Ҳ�д����ã���ô���ú����ᱻ�滻

	// linux ��
	// 1����˵�޷������쳣
	// 2������signal�����жˣ��ж˻ص������׳��Զ����쳣��Ȼ�����Զ����쳣�Ĺ�����������д�ӡ��ջ


	// ��ӡ��ǰ��ջ
	void print_traceback();


	// ת���쳣
	//
	// windows�� ÿ���̶߳�Ҫ������
	// ����ǳ�����Eha _set_se_translatorʵ�֣�
	// Ҳ����hook _CallSETranslator��ʵ��
	//
	// linux�� ֻ�����һ��
	void open_exception_translator();


	// ����dump
	// windows ��minidump, Ҳ����hook
	//
	// linux ֻҪ����ѡ���� -g �����ɿ���
	//
	void open_dump();
};


