#pragma once
#include <unordered_map>
#include "vertex.h"
#include <iostream>
#include <fstream>
#include <stack>
using namespace std;
class distributionGraph {
public:
	void MiniDistanse();			 //�������������·��
	void creatTourSortGraph();		 //���������·ͼ
	void TopoSort();				 //�жϵ�����·ͼ���޻�·
	void outputGraph();				 //���ͼ���ڽӱ�
	void createGraph();				 //����ͼ���ڽӱ�洢
	distributionGraph() {}			 //���캯��
	~distributionGraph();			 //��������
private:
	unordered_map<string ,vertex*>* vertexAdj; //����->���� hash��
	vector<edge*>* pEdgeVec;                   //���бߵļ��ϣ�������������ʱ����C++11
	vector<string>* vertexNames;               //�������ʼ��˳��洢���������
	list<vertex*>* tourGuide;                  //�洢����·��ͼ
	vector<vector<unsigned>>* vertexMtx;       //�洢�ڽӾ���
	unsigned vertexNumber;                     //��������
	unsigned edgeNumber;                       //�����������ʵ�ʱ�*2
	void DFSTraverse(list<vertex*>&);		   //������ȱ���
	bool isEdge(const string&, const string&); //�ж���������֮���Ƿ���ֱ�������ı�
	vector<string>* toposort(
		vector<vector<unsigned>>*, vector<unsigned>*);
											   //�����޻�·,��������ı�
	void findLoopUsingDFS(vector<string>&);    //��������·��ͼ���·
	void Adj2Mtx(vector<vector<unsigned>>&);   //�ڽ�����ת�ڽӾ���
	bool isGraphEmpty();					   //�ж�ͼ�Ƿ��
	void updateVertexVisited();                //�������е�ķ���״̬
	void shortedstPathDijkstra(unsigned,
		vector<unsigned>&, vector<unsigned>&); //�Ͻ�˹�����㷨�����·��
	void outPutShortestPath(unsigned,
		vector<unsigned>&, vector<unsigned>&); //�����������·��
};

void distributionGraph::MiniDistanse(){

	if (isGraphEmpty())
		return;

	cout << "������Ҫ��ѯ��������������ƣ�";
	string from, to;
	cin >> from >> to; //��ʱĬ��������ȷ
	if (from == to) {
		cout << from << " -> " << to << endl;
		cout << "��̾��룺" << 0 << endl;
		return;
	}

	unsigned int f0 = vertexAdj->at(from)->getNumber();//�����
	unsigned int t1 = vertexAdj->at(to)->getNumber();  //�յ���
	vector<unsigned int> shortPathTable(vertexNumber+1); //��ʾV0��Vw����������·��Ȩֵ��
	vector<unsigned int> path(vertexNumber+1); //��ʾV0��Vw�������ǰ�������±�ֵ
	shortedstPathDijkstra(f0, path, shortPathTable);   //�Ͻ�˹�����㷨Ѱ�����·��

	outPutShortestPath(t1, path, shortPathTable);      //������������·������̾���
}

//���������·ͼ
void distributionGraph::creatTourSortGraph() {
	
	//�ж�ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;

	tourGuide = new list<vertex*>();
	auto& rTourGuide = *tourGuide;
	//�ǵݹ�ջ����������ȱ���,����·��ͼ�򵥵ش洢��tourGuide��
	DFSTraverse(rTourGuide);

	auto lvcbg = tourGuide->begin();
	auto lvced = tourGuide->end();
	//��ӡ���б���·�Ͼ����Ľڵ�
	cout << (*lvcbg)->getName();
	while (++lvcbg != lvced)
		cout << " --> " << (*lvcbg)->getName();
	cout << endl;

	//�������е�ķ���״̬��
	updateVertexVisited();
}

//������ȱ���
void distributionGraph::DFSTraverse(list<vertex*>& rTourGuide) {

	stack<vertex*> stackVertex; //�洢·��
	size_t         count = 1;   //�ѷ��ֵľ�������

	//ѡ��һ�����㿪ʼ����ѹ��ջ,��ӽ�·��ͼ
	vertex*        pCurrentVertex = (*vertexAdj)[(*vertexNames)[1]];
	pCurrentVertex->setVisited(true);
	stackVertex.push(pCurrentVertex);
	rTourGuide.push_back(pCurrentVertex);

	//ջ�л��е�˵������Щ��û�г���
	while (!stackVertex.empty()) {

		const list<edge*>* pAulEdgeAdj = stackVertex.top()->getEdgeAdj();//�Ըþ���Ϊ���ȵı�
		auto			   ile = pAulEdgeAdj->begin();   //��ʼ��iterator
		auto			   ileEnd = pAulEdgeAdj->end();

		//�������п��ܾ����ı�
		while (ile != ileEnd) {
			//�ж��Ƿ���ʹ��ñ���Ⱦ���
			//����þ���û�б����ʹ������þ���״̬�ı䣬��ѹ��ջ�����·��ͼ
			if (!(*ile)->getTo()->isVisited()) {
				pCurrentVertex = (*ile)->getTo();
				pCurrentVertex->setVisited(true);
				stackVertex.push(pCurrentVertex);
				rTourGuide.push_back(pCurrentVertex);
				
				if (++count >= vertexAdj->size()) //������ʹ����еľ��㣬�˳�����
					return;

				break;
			}
			else {			//���з��ʹ��������iterator++
				++ile;
			}
		}
		//�������Ϊ�����޹��˳��ϱߵ�ѭ����pop����ջ�ľ���
		if (ile == ileEnd) {
			stackVertex.pop();
			if (!stackVertex.empty()) //pop��ջ���ľ��㽫���ٴα�����
				rTourGuide.push_back(stackVertex.top());
		}
	}
}

//�жϵ�����·ͼ���޻�·
void distributionGraph::TopoSort() {

	//�������򷽷�
	//auto size = vertexNames->size();
	////��ʼ������ͼ�еľ������
	//vector<vector<unsigned int>>* map = new vector<vector<unsigned int>>(size, vector<int>(size, 0));
	//vector<unsigned int>*         indegree = new vector<unsigned int>(size, 0);
	//auto                 it = tourGuide->begin();

	//while(it != tourGuide->end()){
	//	auto iy = ++it;
	//	if (iy != tourGuide->end()) {
	//		int & power = (*map)[(*it)->getNumber()][(*iy)->getNumber()];
	//		if (!power) {
	//			power++;
	//			(*indegree)[(*it)->getNumber()]++;
	//		}
	//	}
	//}
	//�ڲ��߼�,����ֵΪvector<string>*
	//auto result = toposort(map, indegree);
	//if (result->size() != size-1) {
	//	cout << "ͼ���л�·" << endl;
	//}

	vector<string> result;
	findLoopUsingDFS(result);	//��������ͼ�õ�
	if (!result.empty()) {
		cout << "ͼ���л�·:" << endl;
		for each (string var in result){
			cout << var;
		}
		cout << endl;
	}
	//delete map;
	//delete indegree;
}

vector<string>* distributionGraph::toposort(vector<vector<unsigned>>* map, vector<unsigned>* indegree) {
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

//ʹ��������ȷ���������������
void distributionGraph::findLoopUsingDFS(vector<string>& loop){//loop�洢��·
	//����ͼ�����ǿյ�
	if (tourGuide->empty()) {
		return;
	}

	auto tourIt = tourGuide->begin();
	auto pCurrentVertex = *tourIt;
	pCurrentVertex->setVisited(true); //�����Ѿ����ʵı��
	while (true) {
		//��ɵ���ͼ�ı���
		if (++tourIt == tourGuide->end())
			break;
		if ((*tourIt)->isVisited()) {
			//��������˻�·��ǰֱ̽��δ���ʽڵ�
			loop.push_back(pCurrentVertex->getName());
			do {
				pCurrentVertex = *tourIt;
				loop.push_back(string("->"));
				loop.push_back(pCurrentVertex->getName());
			} while ((*++tourIt)->isVisited());
		}else {
			pCurrentVertex = *tourIt;
			pCurrentVertex->setVisited(true);
		}
	}
}

//�ж��Ƿ����f->t�ı�
inline bool distributionGraph::isEdge(const string& f, const string& t) {
	return vertexAdj->at(f)->isAccessDirect(vertexAdj->at(t));
}

//���ͼ���ڽӱ�
void distributionGraph::outputGraph(){

	//�ж�ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;
	if (vertexMtx == nullptr) {
		vertexMtx =	new vector<vector<unsigned>>
			(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 32767));
	}
	auto& rMatrix = *vertexMtx;
	Adj2Mtx(rMatrix);
	auto& rVertexNames = *vertexNames;
	auto size = vertexAdj->size();

	//�������
	for each (string str in rVertexNames)
		printf("%8s", str.c_str());
	cout << endl;

	for(size_t j = 1; j <= size; ++j){
		printf("%8s", rVertexNames[j].c_str());
		for (size_t k = 1; k <= size; ++k)
			printf("%8d", rMatrix[j][k]);
		cout << endl;
	}
}

//����ͼ���ڽӱ�洢
void distributionGraph::createGraph() {

	vertexNames  = new vector<string>();
	pEdgeVec     = new vector<edge*>();
	vertexAdj    = new unordered_map<string, vertex*>();
	auto& rVertexAdj = *vertexAdj;
	string name;
	unsigned i;
	//�򿪴��о�����Ϣ���ļ�
	ifstream in("info.txt");

	//��ȡ�ڵ����ͱ���,�ַ���ת����
	char c[128];
	memset(c, '\0', 128);
	in.getline(c, 128, ' ');
	vertexNumber = atoi(c);
	memset(c, '\0', 128);
	in.getline(c, 128);
	edgeNumber = atoi(c);
	cout << "�����붨�����ͱ�����" << vertexNumber << ' ' << edgeNumber << endl;

	//ѭ����ȡ�ļ��еľ���������Ϣ
	cout << "\n\t\t\"��������������Ϣ\"\n���������������֣�";
	
	vertexNames->push_back(string(" "));
	
	for (i = 1; i < vertexNumber; ++i) {
		memset(c, '\0', 128);
		in.getline(c, 128, ' ');
		name = c;
		cout << name << ' ';
		rVertexAdj[name] = new vertex(name, i);
		vertexNames->push_back(name);
	}
	memset(c, '\0', 128);
	in.getline(c, 128);
	name = c;
	cout << name << ' ';
	rVertexAdj[name] = new vertex(name, i);
	vertexNames->push_back(name);
	cout << endl;
	
	//��ȡ����Ϣ
	for (i = 1; i <= edgeNumber; ++i) {
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
		vertex* from = rVertexAdj[f];
		vertex* to = rVertexAdj[t];
		edge* e1 = new edge(from, to, power);
		edge* e2 = new edge(to, from, power);
		pEdgeVec->push_back(e1);
		pEdgeVec->push_back(e2);
		from->addEdge(e1);
		to->addEdge(e2);
	}
	in.close();
}

//�Ͻ�˹�����㷨�����·��
void distributionGraph::shortedstPathDijkstra( 
	unsigned v0, vector<unsigned>& path, vector<unsigned>& shortPathTable)
{
	if (vertexMtx == nullptr) { //����ڽӾ���û�г�ʼ��
		vertexMtx = new vector<vector<unsigned>>
			(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 32767));
		Adj2Mtx(*vertexMtx);
	}
	auto& rMatrix = *vertexMtx;
	unsigned v, w, k, min, num = vertexNumber+1;
	vector<int> final(num); //final[k]=1��ʾ���v0��k������Ȩֵ��
	for (v = 1; v < num; ++v) {
		final[v] = 0;
		shortPathTable[v] = rMatrix[v0][v];
		path[v] =v0; //��ʼ�������е��ǰ������v0��֮���𽥸���
	}
	final[v0] = 1;	//v0��v0����Ҫ��·��
	path[v0] = 0;
	//��ʼ��ѭ����ÿ�����v0��ĳ����������·��
	for (v = 1; v < num; ++v) {
		min = 65535;
		for (w = 1; w < num; ++w) {//Ѱ����v0����ľ���
			if (!final[w] && shortPathTable[w] < min) {
				k = w;
				min = shortPathTable[w];
			}
		}
		final[k] = 1; //��Ŀǰ�ҵ�������ľ�����Ϊ1,�����Ҳ�����v0��k�������·��
		for (w = 1; w <num;++w) {
			//�����v0�������k����� + k�㵽w����� С��v0��w��ľ��룬���²���¼
			if (!final[w] && (min + rMatrix[k][w]) < shortPathTable[w]) {
				shortPathTable[w] = min + rMatrix[k][w];  //����
				path[w] = k;                                //��¼w���ǰ����k��
			}
		}
	}
}

//������·������̾���
void distributionGraph::outPutShortestPath(unsigned t1,
	vector<unsigned>& path, vector<unsigned>& shortPathTable)
{
	vector<unsigned int> result;
	auto& rVertexNames = *vertexNames;
	unsigned int v = t1;
	do {
		result.push_back(v);
		v = path[v];
	} while (v != 0);

	v = result.size() - 1;
	while (v > 0) //����յ㲻ƥ��
		cout << rVertexNames[result[v--]] << " -> " << rVertexNames[result[v]] << endl;

	cout << "��̾��룺" << shortPathTable[t1] << endl;
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
void distributionGraph::Adj2Mtx(vector<vector<unsigned>>& rMatrix){
	size_t         size = vertexAdj->size();
	
	//�������еıߣ������ڽӾ��������
	for each (edge* eg in *pEdgeVec){
		unsigned int from = eg->getFrom()->getNumber();
		unsigned int to   = eg->getTo()->getNumber();
		rMatrix[from][to] = eg->getPower();
		rMatrix[from][from] = 0;
	}
}

//�������е�ķ���״̬
inline void distributionGraph::updateVertexVisited() {
	for each (auto& var in *vertexAdj)
		var.second->setVisited(false);
}

//��������
distributionGraph::~distributionGraph() {
	if (vertexAdj != nullptr) {
		for each (const pair<string, vertex*>& var in *vertexAdj)
			delete var.second;
		delete vertexAdj;
	}
	if (pEdgeVec != nullptr) {
		for each (const edge* var in *pEdgeVec)
			delete var;
		delete pEdgeVec;
	}
	if (tourGuide != nullptr) {
		delete tourGuide;
	}
	if (vertexNames != nullptr) {
		delete vertexNames;
	}
	if (vertexMtx != nullptr) {
		delete vertexMtx;
	}
}