/**************************************************************************************
���ƣ�test.h
���ã��ο�Googletest���Կ�ܣ�ʵ��һ��С�͵Ĳ��Ե�Ԫ��ʹ���˵���ģʽ.
		��Ҫ����������TestCase��UnitTest�Լ�һϵ�в����õĺ�
**************************************************************************************/
#ifndef TEST_H
#define TEST_H

#include<ctime>
#include<cstring>

#include<iostream>
#include<string>
#include<vector>

#include"lib/redbud/io/color.h"

namespace TinySTL {
	namespace test {

#define green	redbud::io::state::manual<<redbud::io::hfg::green
#define red	redbud::io::state::manual<<redbud::io::hfg::red

//**************************************************************************************************************
	//TestCase��
	//�����������԰���
	class TestCase {
	public:
		TestCase(const char* case_name)
			: testcase_name(case_name)
		{	//���뵥�����԰������ֽ��й���
		}

		virtual void Run() = 0;

	public:
		const char*		testcase_name;	//���԰�������
		int					nTestResult;			//���԰�����ִ�н��
		double				nFailed;				//����ʧ�ܵİ�����
		double				nPassed;				//����ͨ���İ�����
	};	//end of TestCase class

	//UnitTest��
	//�������еĲ��԰���
	class UnitTest {
	public:
		static UnitTest* GetInstance() {
			//��ȡUnitTest����	
			static UnitTest instance;
			return &instance;
		}

		TestCase* RegisterTestCase(TestCase* testcase) {
			//ע����԰���
			_All_case.push_back(testcase);
			return testcase;
		}

		void Run();

	public:
		TestCase* CurrentTestCase;	//��¼��ǰִ�еĲ��԰���
		int nTestResult;						//�ܵ�ִ�н��
		int nFailed;								//ʧ�ܰ�����
		int nPassed;								//ͨ��������
	protected:
		std::vector<TestCase*> _All_case;	//���еĲ��԰�������

	};	//end of UnitTest class


//**************************************************************************************************************










	} //namespace test
} //namespace TinySTL
#endif // !TEST_H

