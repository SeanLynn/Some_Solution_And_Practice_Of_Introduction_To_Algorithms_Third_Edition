#pragma once
#include <unordered_map>
#include "vertex.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
class distributionGraph {
public:
	distributionGraph() {}
	~distributionGraph() {
		for each (pair<string, vertex*> var in vertexAdj)
			delete var.second;
		for each (edge* var in edgeVec){
			delete var;
		}
	}
	void createGraph();//����ͼ���ڽӱ�洢
	void outputGraph();//���ͼ���ڽӱ�
private:
	unordered_map<string ,vertex*> vertexAdj;
	vector<edge*> edgeVec;
	vector<string> vertexNames;//�������ʼ��˳��洢���������
};

void distributionGraph::createGraph() {
	//�򿪴��о�����Ϣ���ļ�
	ifstream in("info.txt");

	//��ȡ�ڵ����ͱ���,�ַ���ת����
	char c[128];
	memset(c, '\0', 128);
	in.getline(c, 128, ' ');
	int vertexs = atoi(c);
	memset(c, '\0', 128);
	in.getline(c, 128);
	int edges = atoi(c);
	cout << "�����붨�����ͱ�����" << vertexs << ' ' << edges << endl;

	//ѭ����ȡ�ļ��еľ���������Ϣ
	cout << "\n\t\t\"��������������Ϣ\"\n���������������֣�";
	vertexNames.push_back(string(" "));
	string name;
	int i = 1;
	for (i = 1; i < vertexs; ++i) {
		memset(c, '\0', 128);
		in.getline(c, 128, ' ');
		name = c;
		cout << name << ' ';
		vertexAdj[name] = new vertex(name, i);
		vertexNames.push_back(name);
	}
	memset(c, '\0', 128);
	in.getline(c, 128);
	name = c;
	cout << name << ' ';
	vertexAdj[name] = new vertex(name, i);
	vertexNames.push_back(name);
	cout << endl;
	
	for (i = 1; i <= edges; ++i) {
		string f, t;
		int power;
		cout << "�������" << i << "���ߵ����������Լ��ñߵ�Ȩֵ��";
		
		//��ñߵ���㡢�յ㡢Ȩֵ
		memset(c, '\0', 128);
		in.getline(c, 128, ' ');
		f = c;
		cout << f << ' ';
		memset(c, '\0', 128);
		in.getline(c, 128, ' ');
		t = c;
		cout << t << ' ';
		memset(c, '\0', 128);
		in.getline(c, 128);
		power = atoi(c);
		cout << power << endl;

		//Ϊ�ڵ�ͱ���ӹ�����ϵ
		vertex* from = vertexAdj[f];
		vertex* to = vertexAdj[t];
		edge* e1 = new edge(from, to, power);
		edge* e2 = new edge(to, from, power);
		edgeVec.push_back(e1);
		edgeVec.push_back(e2);
		from->addEdge(e1);
		to->addEdge(e2);
	}
	in.close();
}

//���ͼ���ڽӱ�
void distributionGraph::outputGraph(){

	//�ж���vertexAdj�ǿյģ����˳����½������ѡ��
	if (vertexAdj.empty()) {
		cout << "����!\n�㻹û�д�����������ֲ�ͼ�������� 1 ��" << endl;
	}

	size_t              size = vertexAdj.size();
	vector<int>         aulVec(size+1, 32767);
	vector<vector<int>> vertexMtx(size+1, aulVec); //��Ҫ������ڽӾ���
	
	//�������еıߣ������ڽӾ��������
	for each (edge* eg in edgeVec){
		unsigned int from = eg->getFrom()->getNumber();
		unsigned int to   = eg->getTo()->getNumber();
		vertexMtx[from][to] = eg->getPower();
		vertexMtx[from][from] = 0;
	}

	//�������
	for each (string str in vertexNames){
		printf("%8s", str.c_str());
	}
	cout << endl;

	for(size_t j = 1; j <= size; ++j){
		printf("%8s", vertexNames[j].c_str());
		for (size_t k = 1; k <= size; ++k) {
			printf("%8d", vertexMtx[j][k]);
		}
		cout << endl;
	}

}
