#pragma once
#include <unordered_map>
#include "vertex.h"
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;
class distributionGraph {
public:
	void creatTourSortGraph();//���������·ͼ
	void DFSTraverse(list<vertex*>*);//������ȱ���
	void TopoSort();//�жϵ�����·ͼ���޻�·
	bool isEdge(const string&, const string&);//�ж���������֮���Ƿ���ֱ�������ı�
	void outputGraph();//���ͼ���ڽӱ�
	void createGraph();//����ͼ���ڽӱ�洢
	distributionGraph() {}
	~distributionGraph();
private:
	unordered_map<string ,vertex*>* vertexAdj;
	vector<edge*>* pEdgeVec;
	vector<string>* vertexNames;//�������ʼ��˳��洢���������
	list<vertex*>* tourGuide;//�洢����·��ͼ
	vector<string>* toposort(vector<vector<int>>*, vector<int>*);
	vector<vector<int>>* Adj2Mtx();
	bool isGraphEmpty(); //�ж�ͼ�Ƿ��
	void updateVertexVisited();//�������е�ķ���״̬
};

//���������·ͼ
void distributionGraph::creatTourSortGraph() {
	
	//�ж�ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;

	list<vertex*>* tempTourGuide = new list<vertex*>();
	DFSTraverse(tempTourGuide);//�ǵݹ�ջ����������ȱ���

	list<vertex*>::const_iterator lvcbg = tempTourGuide->begin();
	list<vertex*>::const_iterator lvced = tempTourGuide->end();
	//��ӡ���б���·�Ͼ����Ľڵ�
	cout << (*lvcbg)->getName();
	while (++lvcbg != lvced)
		cout << " --> " << (*lvcbg)->getName();
	cout << endl;

	//�������е�ķ���״̬��
	updateVertexVisited();
	tourGuide = tempTourGuide;
}

//������ȱ���
void distributionGraph::DFSTraverse(list<vertex*>* tourGuide) {

	stack<vertex*>* stackVertex = new stack<vertex*>(); //�洢·��
	size_t         count = 1;   //�ѷ��ֵľ�������

	//ѡ��һ�����㿪ʼ����ѹ��ջ,��ӽ�·��ͼ
	vertex*        pCurrentVertex = (*vertexAdj)[(*vertexNames)[1]];
	pCurrentVertex->setVisited(true);
	stackVertex->push(pCurrentVertex);
	tourGuide->push_back(pCurrentVertex);

	//ջ�л��е�˵������Щ��û�г���
	while (!stackVertex->empty()) {

		const list<edge*>*          aulEdgeAdj = stackVertex->top()->getEdgeAdj();//�Ըþ���Ϊ���ȵı�
		list<edge*>::const_iterator lcibg = aulEdgeAdj->begin();   //��ʼ��iterator
		list<edge*>::const_iterator lcied = aulEdgeAdj->end();

		//�������п��ܾ����ı�
		while (lcibg != lcied) {
			//�ж��Ƿ���ʹ��ñ���Ⱦ���
			//����þ���û�б����ʹ������þ���״̬�ı䣬��ѹ��ջ�����·��ͼ
			if (!(*lcibg)->getTo()->isVisited()) {
				pCurrentVertex = (*lcibg)->getTo();
				pCurrentVertex->setVisited(true);
				stackVertex->push(pCurrentVertex);
				tourGuide->push_back(pCurrentVertex);
				
				if (++count >= vertexAdj->size()) { //������ʹ����еľ��㣬�˳�����
					delete stackVertex;
					return;
				}

				break;
			}
			else {			//���з��ʹ��������iterator++
				lcibg++;
			}
		}
		//�������Ϊ�����޹��˳��ϱߵ�ѭ����pop����ջ�ľ���
		if (lcibg == lcied) {
			stackVertex->pop();
			if (!stackVertex->empty()) //pop��ջ���ľ��㽫���ٴα�����
				tourGuide->push_back(stackVertex->top());
		}
	}
	delete stackVertex;
}

//�жϵ�����·ͼ���޻�·
void distributionGraph::TopoSort() {
	auto size = vertexNames->size();
	//��ʼ������ͼ�еľ������
	vector<vector<int>>* map = new vector<vector<int>>(size, vector<int>(size, 0));
	vector<int>*         indegree = new vector<int>(size, 0);
	auto                 it = tourGuide->begin();

	while(it != tourGuide->end()){
		auto iy = ++it;
		if (iy != tourGuide->end()) {
			int & power = (*map)[(*it)->getNumber()][(*iy)->getNumber()];
			if (!power) {
				power++;
				(*indegree)[(*it)->getNumber()]++;
			}
		}
	}
	//�ڲ��߼�,����ֵΪvector<string>*
	auto result = toposort(map, indegree);

	if (result->size() != size-1) {
		cout << "ͼ���л�·" << endl;
	}

	delete result;
	delete map;
	delete indegree;
}

vector<string>* distributionGraph::toposort(vector<vector<int>>* map, vector<int>* indegree) {
	unsigned i, j, k, size = indegree->size();
	vector<string>* result = new vector<string>(indegree->size()-1);
	for (i = 1;i<size;i++){ //����n��
		for (j = 1;j<size;j++){ //�ҳ����Ϊ0�Ľڵ�
			if ((*indegree)[j] == 0){
				(*indegree)[j]--;
				result->push_back((*vertexNames)[j]);
				for (k = 1;k<size;k++) //ɾ����ýڵ�����ı�
					if ((*map)[j][k] == 1)
						(*indegree)[k]--;
				break;
			}
		}
	}
	return result;
}

inline bool distributionGraph::isEdge(const string& f, const string& t) {
	return vertexAdj->at(f)->isAccessDirect(vertexAdj->at(t));
}

//���ͼ���ڽӱ�
void distributionGraph::outputGraph(){

	//�ж�ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;

	auto vertexMtx = Adj2Mtx();
	auto tempVertexNames = vertexNames;
	auto size = vertexAdj->size();

	//�������
	for each (string str in *tempVertexNames)
		printf("%8s", str.c_str());
	cout << endl;

	for(size_t j = 1; j <= size; ++j){
		printf("%8s", (*tempVertexNames)[j].c_str());
		for (size_t k = 1; k <= size; ++k)
			printf("%8d", (*vertexMtx)[j][k]);
		cout << endl;
	}
}

//����ͼ���ڽӱ�洢
void distributionGraph::createGraph() {

	vertexNames  = new vector<string>();
	pEdgeVec     = new vector<edge*>();
	vertexAdj    = new unordered_map<string, vertex*>();
	string name;
	int i;
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
	
	vertexNames->push_back(string(" "));
	
	for (i = 1; i < vertexs; ++i) {
		memset(c, '\0', 128);
		in.getline(c, 128, ' ');
		name = c;
		cout << name << ' ';
		(*vertexAdj)[name] = new vertex(name, i);
		vertexNames->push_back(name);
	}
	memset(c, '\0', 128);
	in.getline(c, 128);
	name = c;
	cout << name << ' ';
	(*vertexAdj)[name] = new vertex(name, i);
	vertexNames->push_back(name);
	cout << endl;
	
	//��ȡ����Ϣ
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
		vertex* from = (*vertexAdj)[f];
		vertex* to = (*vertexAdj)[t];
		edge* e1 = new edge(from, to, power);
		edge* e2 = new edge(to, from, power);
		pEdgeVec->push_back(e1);
		pEdgeVec->push_back(e2);
		from->addEdge(e1);
		to->addEdge(e2);
	}
	in.close();
}

//�ж�ͼ�Ƿ��
inline bool distributionGraph::isGraphEmpty() {
	//�ж���vertexAdj�ǿյģ����˳����½������ѡ��
	if (!vertexAdj->empty())
		return false;

	cout << "����!\n�㻹û�д�����������ֲ�ͼ�������� 1 ��" << endl;
	return true;
}

//�ڽ�����ת�ڽӾ���
vector<vector<int>>* distributionGraph::Adj2Mtx(){
	vector<edge*>* tempPEdgeVec = pEdgeVec;
	size_t         size = vertexAdj->size();
	 
	//��Ҫ������ڽӾ������ʼ��
	vector<vector<int>>* vertexMtx = new vector<vector<int>>(size + 1, vector<int>(size + 1, 32767));

	//�������еıߣ������ڽӾ��������
	for each (edge* eg in *tempPEdgeVec){
		unsigned int from = eg->getFrom()->getNumber();
		unsigned int to   = eg->getTo()->getNumber();
		(*vertexMtx)[from][to] = eg->getPower();
		(*vertexMtx)[from][from] = 0;
	}

	return vertexMtx;
}

//��������
distributionGraph::~distributionGraph() {
	for each (const pair<string, vertex*>& var in *vertexAdj)
		delete var.second;
	for each (const edge* var in *pEdgeVec)
		delete var;
	delete vertexAdj;
	delete pEdgeVec;
	delete tourGuide;
}

//�������е�ķ���״̬
inline void distributionGraph::updateVertexVisited() {
	for each (auto var in *vertexAdj)
		var.second->setVisited(false);
}