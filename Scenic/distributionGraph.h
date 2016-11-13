#pragma once
#include <unordered_map>
#include "vertex.h"
#include <iostream>
#include <fstream>
#include <stack>

class distributionGraph {
public:
	void miniDistanse();	  //�������������·��
	void creatTourSortGraph();//���������·ͼ
	void topoSort();		  //�жϵ�����·ͼ���޻�·
	void outputGraph();		  //���ͼ���ڽӱ�
	void createGraph();		  //����ͼ���ڽӱ�洢
	void miniSpanTree();	  //�����·�޽��滮ͼ
	distributionGraph() {}	  //���캯��
	~distributionGraph();	  //��������
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
	void toposort(vector<int>&);            //�����޻�·,��������ı�
	void findLoopUsingDFS(vector<string>&);    //��������·��ͼ���·
	void Adj2Mtx(vector<vector<unsigned>>&);   //�ڽ�����ת�ڽӾ���
	bool isGraphEmpty();					   //�ж�ͼ�Ƿ��
	void updateVertexVisited();                //�������е�ķ���״̬
	void shortedstPathDijkstra(unsigned,
		vector<unsigned>&, vector<unsigned>&); //�Ͻ�˹�����㷨�����·��
	void outPutShortestPath(unsigned,
		vector<unsigned>&, vector<unsigned>&); //�����������·��
};

//�����·�޽��滮ͼ
void distributionGraph::miniSpanTree() {
	vector<unsigned> newVertexSet(vertexNumber+1, 0);
	vector<edge*> newEdgeSet;
	auto& rVertexAdj = *vertexAdj;
	auto& rVertexNames = *vertexNames;
	newVertexSet[1] = 1;
	while (++newVertexSet[0] < vertexNumber) {
		
		unsigned v, e = 32767;
		edge* minPowerEdge = nullptr;
		//�ҳ�����ڡ��յ㲻���µ㼯�У�����
		for (unsigned i = 1; i <= vertexNumber; ++i) {
			if (newVertexSet[i] == 1) {
				auto& r = *rVertexAdj[rVertexNames[i]]->getEdgeAdj();
				for each (edge* eg in r){
					v = eg->getTo()->getNumber();
					if (newVertexSet[v] == 0 && eg->getPower() < e) {
						e = eg->getPower();
						minPowerEdge = eg;
					}
				}
			}
		}
		newVertexSet[minPowerEdge->getTo()->getNumber()] = 1;
		newEdgeSet.push_back(minPowerEdge);

	}

	for each (edge* eg in newEdgeSet){
		cout << "��" << eg->getFrom()->getName() << "��" << eg->getTo()->getName() << "��һ��·" << endl;
	}

}

void distributionGraph::miniDistanse(){

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

	auto lvcbg = rTourGuide.begin();
	auto lvced = rTourGuide.end();
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
void distributionGraph::topoSort() {

	//�ж�ͼ�Ƿ�Ϊ�պ͵���ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;

	if (tourGuide == nullptr) {
		cout << "��Ӧ���ȴ���������·ͼ��������4��" << endl;
		return;
	}

	//�������򷽷�
	//�ڲ��߼�,����ֵΪvector<string>*
	vector<int> result(vertexNumber+1);
	toposort(result);
	for (unsigned i = 1; i <= vertexNumber; ++i)
		if (result[i] > 0) {
			cout << "ͼ���л�·" << endl;
			//�����·
			auto& rNames = *vertexNames;
			for (unsigned j = i; j <= vertexNumber; ++j)
				if (result[j] > 0)
					cout << rNames[j] << ' ';
			cout << endl;
			result[0] = 1;
			break;
		}
	
	if (result[0] == 0) {
		cout << "ͼ��û�л�·" << endl;
	}

	//��������ͼ����
	//vector<string> result;
	//findLoopUsingDFS(result);	//��������ͼ�õ�
	//if (!result.empty()) {
	//	cout << "ͼ���л�·:" << endl;
	//	for each (string var in result){
	//		cout << var;
	//	}
	//	cout << endl;
	//}
}

//�������򷽷���ֻΪ������д�ġ�
void distributionGraph::toposort(vector<int>& result) {
	//��ʼ������ͼ�еľ������
	vector<vector<unsigned>>* map = new vector<vector<unsigned>>(
									vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 0));
	vector<int>* indegree = new vector<int>(vertexNumber + 1, 0);

	auto& rMap = *map;
	auto& rIndegree = *indegree;
	auto  it = tourGuide->begin();
	auto  iy = it;
	auto  itEnd = tourGuide->end();
	unsigned  startNumber = (*(it))->getNumber();
	//��ʼ������ͼ���ڽӾ���
	while (++iy != itEnd) {
		unsigned from = (*it++)->getNumber();
		unsigned in = (*iy)->getNumber();
		++rMap[from][in];//Ȩֵ+1
		++rIndegree[in];//���+1
	}
	
	//�±����ר��Ϊ����д
	rIndegree[startNumber] = -1;
	for (unsigned k = 1; k <= vertexNumber; k++) {
		if (rMap[startNumber][k] == 1)
			--rIndegree[k];
		rMap[k][startNumber] = 0;
	}
	//������������
	for (unsigned i = 1; i <= vertexNumber; ++i) //����n��
		for (unsigned j = 1; j <= vertexNumber; ++j) //�ҳ����Ϊ0�Ľڵ�
			if (rIndegree[j] == 0){

				rIndegree[j]--;//��ȼ�Ϊ-1��˵��������

				for (unsigned k = 1; k <= vertexNumber; k++) //ɾ����ýڵ�����ı�
					if (rMap[j][k] == 1)
						rIndegree[k]--;
				break;
			}

	result = rIndegree;
	delete map;
	delete indegree;
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