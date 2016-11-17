#include "stdafx.h"
#include "parkingLot.h"
#include <string>
#include <iostream>
#include <stack>
#include <iomanip>
#include <time.h>
using std::cout;
using std::endl;
using std::cin;
using std::make_shared;
//����������
void parkingLot::management() {
	while (true) {
		printWelcome();
		char select;
		cin >> select;
		if (select == 'A') {
			pushBackCar();
		}
		else if (select == 'D') {
			popBackCar();
		}
		else if (select == 'E') {
			break;
		}
	}
}

//��ӡ��ӭ��
inline void parkingLot::printWelcome() {
	cout << "\t\t** ͣ����������� **\n";
	cout << "============================================================\n";
	cout << "**\t\t\t\t\t\t\t**\n";
	cout << "**\tA --- ���� �� ����\tD --- ���� �� ����\t**\n";
	cout << "**\t\t\t\t\t\t\t**\n";
	cout << "**\t\t\tE --- �˳� ����\t\t\t**\n";
	cout << "============================================================" << endl;
	cout << " ��ѡ��<A,D,E>:";
}

//���������������ӿڣ�ֻ�����ݽṹ��΢С�ĸı�
void parkingLot::popBackCar() {
	if (carStack.empty()) {
		cout << "ͣ�����ǿյġ�\n" << endl;
		//cout << "=========================================================" << endl;
		return;
	}
	string lic;
	cout << "���������ƺţ�";
	cin >> lic;

	if (popBack(lic) && !carQueue.empty()) { //popBack�ɹ����ұ�����г�
		shared_ptr<car> r = carQueue.front();
		r->start = std::chrono::system_clock::now(); //���ý���ʱ��
		carQueue.pop();
		cout << "��������Ϊ��" << r->license << endl;
		pushBack(r);
	}
}

//����������
bool parkingLot::popBack(const string& lic) {

	std::stack<shared_ptr<car>> tempCarStack;
	shared_ptr<car> r = nullptr;
	unsigned size = carStack.size();
	for (unsigned i = 0; i < size; ++i) {
		shared_ptr<car> tempCar = carStack.top();
		if (tempCar->license == lic) {
			r = tempCar;
			carStack.pop();
			break;
		}
		tempCarStack.push(tempCar);
		carStack.pop();
	}
	//��ԭcarStack
	while (!tempCarStack.empty()) {
		carStack.push(tempCarStack.top());
		tempCarStack.pop();
	}

	if (r == nullptr) //����Ҳ���licenceΪlic��car
		return false;

	auto end = std::chrono::system_clock::now(); //����ʱ��
	auto t = std::chrono::system_clock::to_time_t(end); //ת��ʱ���ʽ
	auto parkingTime = std::chrono::duration_cast<std::chrono::seconds>(end - r->start).count();//ͣ��ʱ��

																								//����ʱ��ת���ɱ���ʱ��
	char buf[32];
	getLocalTime(t, buf);

	cout << "�������뿪ͣ�����ĳ�����Ϣ��" << endl;
	cout << "�����ƺţ�" << r->license << endl;
	cout << "������ʱ��:" << *buf << endl;
	cout << "ͣ��ʱ����" << parkingTime << " seconds" << endl;//ͣ��ʱ��
	cout << "ͣ�����ѣ�" << parkingCost(parkingTime) << " Ԫ��\n" << endl;	//����ͣ������
																	//cout << "=========================================================" << endl;
	return true;
}

//����������
void parkingLot::pushBackCar() {
	//�������복����Ϣ����������
	string lic;
	cout << "��������Ϊ��";
	cin >> lic;
	auto time = std::chrono::system_clock::now();

	//��ӽ�����ջ������������Ϣ
	shared_ptr<car> r(new car(lic, time));
	pushBack(r);
}

//����������
void parkingLot::pushBack(shared_ptr<car> r) {
	if (carStack.size() < capacity) {
		carStack.push(r);
		auto t = std::chrono::system_clock::to_time_t(r->start); //��ʱ��ת��������
		char buf[32];
		getLocalTime(t, buf);
		//�����������ʱ����Ϣ��λ����Ϣ
		cout << "������ʱ�̣�" << buf;
		cout << "�ó��ѽ���ͣ�����ڣ�" << carStack.size() << "�ų���\n" << endl;
	}
	else {
		carQueue.push(r);
		cout << "ͣ�����������ó�ͣ�ڱ���ϡ�\n" << endl;
	}
	Sleep(500);
	//cout << "=========================================================\n" << endl;
}


//����ͣ������
inline unsigned long long parkingLot::parkingCost(long long parkingTime) {
	return (unsigned long long)parkingTime * 2;
}

//���t������ı���ʱ��
inline void parkingLot::getLocalTime(__time64_t t, char* buf) {
	struct tm newtime;
	localtime_s(&newtime, &t);
	asctime_s(buf, 32, &newtime);
}