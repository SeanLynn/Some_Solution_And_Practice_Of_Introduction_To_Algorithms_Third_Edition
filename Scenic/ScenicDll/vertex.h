#pragma once
#include <string>
#include <list>
#include <memory>
#include "edge.h"
using namespace std;
class vertex {
public:
	vertex(string n, unsigned i, unsigned p = 0, string f = "", bool r = true, bool w = true)
		: name(n), number(i), visited(false), info(f),
		edgeAdj(make_shared<list<weak_ptr<edge>>>()),
		popularity(p), restArea(r), WC(w) {}

	void addEdge(const shared_ptr<edge> e) {
		edgeAdj->push_back(e);
	}

	const shared_ptr<list<weak_ptr<edge>>> getEdgeAdj() const {
		return edgeAdj;
	}

	unsigned int getNumber() const {
		return number;
	}

	bool isVisited() const {
		return visited;
	}

	void setVisited(bool bIV) {
		visited = bIV;
	}

	const string& getName() const {
		return name;
	}

	bool isAccessDirect(const shared_ptr<vertex> t) {
		for each (const auto eg in *edgeAdj)
			if (eg.lock()->getTo() == t) //һ����Ϊeg.lock()���ǿ���ָ��
				return true;
		return false;
	}

	const string& getInfo() {
		return info;
	}

	void setInfo(const char* i) {
		info = i;
	}

	void setPopularity(unsigned p) {
		popularity = p;
	}

	unsigned getPopularity() {
		return popularity;
	}

	void setRestArea(bool r) {
		restArea = r;
	}

	bool getRestArea() {
		return restArea;
	}

	void setWC(bool w) {
		WC = w;
	}

	bool getWC() {
		return WC;
	}
private:
	string name;         //��������
	unsigned number;     //������
	bool visited;        //�����Ƿ񱻷��ʹ�
	string info;		 //�������
	shared_ptr<list<weak_ptr<edge>>> edgeAdj;
	//�Ӿ�������ıߣ�ʹ��weak_ptr��ԭ����
	//��ֹ�໥shared_ptrָ����޷��ͷ��ڴ档
	unsigned popularity; //���㻶ӭ��
	bool restArea;       //������Ϣ��
	bool WC;			 //���޲���
};