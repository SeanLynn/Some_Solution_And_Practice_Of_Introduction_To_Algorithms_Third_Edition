#pragma once

#include <string>
#include <chrono>
#include <iostream>
#include <stack>
#include <iomanip>
#include <queue>
using std::cout;
using std::endl;
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
	parkingLot(unsigned s = 5):size(s) {}      //���캯��
private:
	unsigned size;
	stack<car*> carStack;
	queue<car*> carQueue;
	void pushBackCar();  //����������
	void printWelcome(); //��ӡ��ӭ��
};

//����������
void parkingLot::management() {

	while (true) {
		char select;

		cin >> select;
		if (select == 'A') {
			pushBackCar();
		}


		//std::chrono::system_clock::time_point t2 = std::chrono::system_clock::now();
		//cout << (t2 - t1).count() << " tick count" << endl;
		//cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microseconds" << endl;
	}
}

//��ӡ��ӭ��
void parkingLot::printWelcome() {
	cout << "\t\t\t** ͣ����������� **\n";
	cout << "=========================================================\n";
	cout << "**\t\t\t\t\t**\n";
	cout << "**\tA --- ���� �� ����\tD --- ���� �� ����\t**\n";
	cout << "**\t\t\t\t\t**\n";
	cout << "**\t\t\tE --- �˳�\t����\t\t\t**\n";
	cout << "=========================================================" << endl;
	cout << " ��ѡ��<A,D,E>:";
}

//����������
void parkingLot::pushBackCar() {
	string lic;
	cin >> lic;
	auto time = std::chrono::system_clock::now();
	auto t = chrono::system_clock::to_time_t(time);
	car* r = new car(lic, time);
	cout << "������ʱ�̣�" << std::put_time(std::localtime(&t), "%Y-%m-%d %X") << endl;
	if (carStack.size() < size) {
		carStack.push(r);
	}else {
		carQueue.push(r);
	}
}
