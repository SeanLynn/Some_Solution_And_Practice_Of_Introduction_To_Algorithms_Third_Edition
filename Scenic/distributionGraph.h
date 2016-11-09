#pragma once
#include <unordered_map>
#include "vertex.h"
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;
class distributionGraph {
public:
	void createGraph();//����ͼ���ڽӱ�洢
	void outputGraph();//���ͼ���ڽӱ�
	void creatTourSortGraph();//���������·ͼ
	void DFSTraverse(list<vertex*>&);//������ȱ���
	void TopoSort();//�жϵ�����·ͼ���޻�·
	bool isEdge();//�ж���������֮���Ƿ���ֱ�������ı�
	distributionGraph() {}
	~distributionGraph();
private:
	unordered_map<string ,vertex*> vertexAdj;
	vector<edge*> edgeVec;
	vector<string> vertexNames;//�������ʼ��˳��洢���������
	
	bool isGraphEmpty(); //�ж�ͼ�Ƿ��
	void updateVertexVisited();//�������е�ķ���״̬
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

	//�ж�ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;

	size_t              size = vertexAdj.size();
	vector<vector<int>> vertexMtx(size+1, vector<int>(size+1, 32767)); //��Ҫ������ڽӾ���
	
	//�������еıߣ������ڽӾ��������
	for each (edge* eg in edgeVec){
		unsigned int from = eg->getFrom()->getNumber();
		unsigned int to   = eg->getTo()->getNumber();
		vertexMtx[from][to] = eg->getPower();
		vertexMtx[from][from] = 0;
	}

	//�������
	for each (string str in vertexNames)
		printf("%8s", str.c_str());
	cout << endl;

	for(size_t j = 1; j <= size; ++j){
		printf("%8s", vertexNames[j].c_str());
		for (size_t k = 1; k <= size; ++k)
			printf("%8d", vertexMtx[j][k]);
		cout << endl;
	}
}

//���������·ͼ
void distributionGraph::creatTourSortGraph() {
	
	//�ж�ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;

	list<vertex*> tourGuide;//�洢����·��
	DFSTraverse(tourGuide);//�ǵݹ�ջ����������ȱ���

	list<vertex*>::const_iterator lvcbg = tourGuide.begin();
	list<vertex*>::const_iterator lvced = tourGuide.end();
	//��ӡ���б���·�Ͼ����Ľڵ�
	cout << (*lvcbg)->getName();
	while (++lvcbg != lvced)
		cout << " --> " << (*lvcbg)->getName();
	cout << endl;

	//�������е�ķ���״̬��
	updateVertexVisited();
}

//������ȱ���
void distributionGraph::DFSTraverse(list<vertex*>& tourGuide) {

	stack<vertex*> stackVertex; //�洢·��
	size_t         count = 1;   //�ѷ��ֵľ�������

	//ѡ��һ�����㿪ʼ����ѹ��ջ,��ӽ�·��ͼ
	vertex*        pCurrentVertex = vertexAdj[vertexNames[1]];
	pCurrentVertex->setVisited(true);
	stackVertex.push(pCurrentVertex);
	tourGuide.push_back(pCurrentVertex);

	//ջ�л��е�˵������Щ��û�г���
	while (!stackVertex.empty()) {

		const list<edge*>&          aulEdgeAdj = stackVertex.top()->getEdgeAdj();//�Ըþ���Ϊ���ȵı�
		list<edge*>::const_iterator lcibg = aulEdgeAdj.begin();   //��ʼ��iterator
		list<edge*>::const_iterator lcied = aulEdgeAdj.end();

		//�������п��ܾ����ı�
		while (lcibg != lcied) {
			//�ж��Ƿ���ʹ��ñ���Ⱦ���
			//����þ���û�б����ʹ������þ���״̬�ı䣬��ѹ��ջ�����·��ͼ
			if (!(*lcibg)->getTo()->isVisited()) {
				pCurrentVertex = (*lcibg)->getTo();
				pCurrentVertex->setVisited(true);
				stackVertex.push(pCurrentVertex);
				tourGuide.push_back(pCurrentVertex);
				
				if (++count >= vertexAdj.size()) //������ʹ����еľ��㣬�˳�����
					return;

				break;
			}
			else {			//���з��ʹ��������iterator++
				lcibg++;
			}
		}
		//�������Ϊ�����޹��˳��ϱߵ�ѭ����pop����ջ�ľ���
		if (lcibg == lcied) {
			stackVertex.pop();
			if (!stackVertex.empty()) //pop��ջ���ľ��㽫���ٴα�����
				tourGuide.push_back(stackVertex.top());
		}
	}
}


//�ж�ͼ�Ƿ��
bool distributionGraph::isGraphEmpty() {
	//�ж���vertexAdj�ǿյģ����˳����½������ѡ��
	if (!vertexAdj.empty())
		return false;

	cout << "����!\n�㻹û�д�����������ֲ�ͼ�������� 1 ��" << endl;
	return true;
}


//��������
distributionGraph::~distributionGraph() {
	for each (const pair<string, vertex*>& var in vertexAdj)
		delete var.second;
	for each (const edge* var in edgeVec)
		delete var;
}

//�������е�ķ���״̬
void distributionGraph::updateVertexVisited() {
	for each (const pair<string, vertex*>& var in vertexAdj)
		var.second->setVisited(false);
}