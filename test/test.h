/**************************************************************************************
名称：test.h
作用：参考Googletest测试框架，实现一个小型的测试单元。使用了单例模式.
		主要定义两个类TestCase、UnitTest以及一系列测试用的宏
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
	//TestCase类
	//产生单个测试案例
	class TestCase {
	public:
		TestCase(const char* case_name)
			: testcase_name(case_name)
		{	//输入单个测试案例名字进行构造
		}

		virtual void Run() = 0;

	public:
		const char*		testcase_name;	//测试案例名称
		int					nTestResult;			//测试案例的执行结果
		double				nFailed;				//测试失败的案例数
		double				nPassed;				//测试通过的案例数
	};	//end of TestCase class

	//UnitTest类
	//管理所有的测试案例
	class UnitTest {
	public:
		static UnitTest* GetInstance() {
			//获取UnitTest单例	
			static UnitTest instance;
			return &instance;
		}

		TestCase* RegisterTestCase(TestCase* testcase) {
			//注册测试案例
			_All_case.push_back(testcase);
			return testcase;
		}

		void Run();

	public:
		TestCase* CurrentTestCase;	//记录当前执行的测试案例
		int nTestResult;						//总的执行结果
		int nFailed;								//失败案例数
		int nPassed;								//通过案例数
	protected:
		std::vector<TestCase*> _All_case;	//所有的测试案例集合

	};	//end of UnitTest class


//**************************************************************************************************************










	} //namespace test
} //namespace TinySTL
#endif // !TEST_H

