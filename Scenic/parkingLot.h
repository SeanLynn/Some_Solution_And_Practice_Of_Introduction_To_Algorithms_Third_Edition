#pragma once

#include <Windows.h>
#include <chrono>
#include <queue>
#include <stack>
#include <queue>
#include <string>
using std::string;
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
	~parkingLot();		 //��������
private:
	unsigned capacity;		  //ͣ��������
	std::stack<car*> carStack; //ͣ����
	std::queue<car*> carQueue; //���
	void pushBackCar();  //���������������ӿ�
	void popBackCar();   //���������������ӿ�
	void printWelcome(); //��ӡ��ӭ��
	unsigned long long parkingCost(long long);//����ͣ������
	void pushBack(car*); //����������
	bool popBack(const string&); //����������
	void getLocalTime(__time64_t, char*); //��ȡ����ʱ��
};