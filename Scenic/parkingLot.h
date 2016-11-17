#pragma once

#include <Windows.h>
#include <chrono>
#include <queue>
#include <stack>
#include <queue>
#include <string>
#include <memory>
using std::string;
using std::shared_ptr;
struct car {
	car(string l, std::chrono::system_clock::time_point s):
		license(l), start(s){}

	string license;
	std::chrono::system_clock::time_point start;
};

class parkingLot {
public:
	void management();   //����������
	parkingLot(unsigned s = 5) : capacity(s) {}      //���캯��
private:
	unsigned capacity;		  //ͣ��������
	std::stack<shared_ptr<car>> carStack; //ͣ����
	std::queue<shared_ptr<car>> carQueue; //���
	void pushBackCar();  //���������������ӿ�
	void popBackCar();   //���������������ӿ�
	void printWelcome(); //��ӡ��ӭ��
	unsigned long long parkingCost(long long);//����ͣ������
	void pushBack(shared_ptr<car>); //����������
	bool popBack(const string&); //����������
	void getLocalTime(__time64_t, char*); //��ȡ����ʱ��
};