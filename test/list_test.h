#pragma once
#include"../src/container/list.h"

#include<list>
#include<iostream>
#include<ctime>

bool val_to_delete(int x) { return x == 4; }

void list_test() {
	TinySTL::list<int> l1;
	TinySTL::list<int>l2(1);
	TinySTL::list<int>l3(10, 1);
	TinySTL::list<int>l4(l3.begin(), l3.end());
	TinySTL::list<int>l5(l3);
	l5.push_back(11);
	l5.push_front(9);
	l5.pop_front();
	l5.pop_back();
	l1.push_back(1);
	l1.push_back(1);
	l1.push_back(2);
	l1.push_back(3);
	l1.push_back(3);
	l1.push_back(4);
	l1.push_back(5);
	l1.push_back(6);
	l1.push_back(7);
	l1.push_back(8);
	l1.remove_if(val_to_delete);
	l1.unique();
	l1.splice(l1.end(), l1, l1.begin());
	for (auto n : l1) {
		std::cout << n << std::endl;
	}


	TinySTL::list<int> test1;
	std::list<int>test2;

	double start_time = clock();
	for (int i = 0; i < 100000; i++) {
		test1.push_front(i);
	}
	double end_time = clock();
	std::cout << end_time - start_time << std::endl;

	double start_time_std = clock();
	for (int i = 0; i < 100000; i++) {
		test2.push_front(i);
	}
	double end_time_std = clock();
	std::cout << end_time_std - start_time_std << std::endl;
}
