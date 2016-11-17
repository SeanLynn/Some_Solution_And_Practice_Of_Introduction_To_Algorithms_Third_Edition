#pragma once
#include "vertex.h"
#include "parkingLot.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>
class distributionGraph {
public:
	void searchWithKeyWords();//�û�����Ĺؼ��ֽ��о���Ĳ���
	void parkingLotMagm();    //ͣ��������ϵͳ����ӿ�
	void sortedByPopularity();//�����㻶ӭ�Ƚ�������
	void miniDistanse();	  //�������������·��
	void creatTourSortGraph();//���������·ͼ
	void topoSort();		  //�жϵ�����·ͼ���޻�·
	void outputGraph();		  //���ͼ���ڽӱ�
	void createGraph();		  //����ͼ���ڽӱ�洢
	void miniSpanTree();	  //�����·�޽��滮ͼ
	distributionGraph();	  //���캯��
private:
	typedef vector<vector<unsigned>> matrix;		//matrix��typedef
	shared_ptr<unordered_map<string, 
		shared_ptr<vertex>>> vertexAdj;             //����->���� hash��
	shared_ptr<vector<shared_ptr<edge>>> pEdgeVec;  //���бߵļ��ϣ�������������ʱ����C++11
	shared_ptr<vector<string>> vertexNames;			//�������ʼ��˳��洢���������
	shared_ptr<list<shared_ptr<vertex>>> tourGuide;	//�洢����·��ͼ
	shared_ptr<matrix> vertexMtx;	//�洢�ڽӾ���
	shared_ptr<parkingLot> lot;						//ͣ����
	unsigned vertexNumber;							//��������
	unsigned edgeNumber;							//�����������ʵ�ʱ�*2
	void DFSTraverse(list<shared_ptr<vertex>>&);	//������ȱ���
	bool isEdge(const string&, const string&);		//�ж���������֮���Ƿ���ֱ�������ı�
	void toposort(vector<int>&);					//�����޻�·,��������ı�
	void findLoopUsingDFS(vector<string>&);			//��������·��ͼ���·
	void Adj2Mtx(matrix&);		//�ڽ�����ת�ڽӾ���
	bool isGraphEmpty();							//�ж�ͼ�Ƿ��
	void updateVertexVisited();						//�������е�ķ���״̬
	void shortedstPathDijkstra(unsigned,
		vector<unsigned>&, vector<unsigned>&);		//�Ͻ�˹�����㷨�����·��
	void outPutShortestPath(unsigned,
		vector<unsigned>&, vector<unsigned>&);		//�����������·��
	void quickSort(vector<shared_ptr<vertex>>&,
		unsigned, unsigned);						//�������򰴾����ܻ�ӭ��
	void insertSort(vector<shared_ptr<vertex>>&,
		unsigned, unsigned);						//�������򰴾����ܻ�ӭ��
	void queryWords(const string&, 
		vector<shared_ptr<vertex>>&);				//�ھ��㼰�����������ؼ���
	bool kmp(const string&, const string&);			//kmp�㷨ƥ��ͼ�������ƻ��߼��
	void computePrefixFunction(vector<int>&,
		const string&);								//kmpǰ׺����
};

//�û�����Ĺؼ��ֽ��о���Ĳ���
void distributionGraph::searchWithKeyWords() {

	if (isGraphEmpty()) {
		return;
	}

	cout << "��������Ҫ���ҵĹؼ��֣�";
	string key;
	cin >> key;
	vector<shared_ptr<vertex>> vtHasInfo;
	queryWords(key, vtHasInfo);
	if (vtHasInfo.empty()) {
		cout << "û���롰" << key << "����ص����ݡ�" << endl;
		return;
	}
	cout << "���������Ϣ���У�" << endl;
	for each (shared_ptr<vertex> var in vtHasInfo)
	{
		cout << "���㣺" << var->getName() << endl;
		cout << "������ܣ�" << var->getInfo() << endl << endl;
	}
}

//ͣ��������ϵͳ����ӿ�
void distributionGraph::parkingLotMagm() {
	lot->management();
}

//�����㻶ӭ�Ƚ�������
void distributionGraph::sortedByPopularity() {
	vector<shared_ptr<vertex>> vertexSet;
	for each (auto& var in *vertexAdj)
		vertexSet.push_back(var.second);

	quickSort(vertexSet, 0, vertexSet.size());

	for_each(vertexSet.begin(), vertexSet.end(),
		[](shared_ptr<vertex> v) {cout << v->getName() << ' ' << v->getPopularity() << ' ';});
}

//�����·�޽��滮ͼ
void distributionGraph::miniSpanTree() {
	vector<unsigned> newVertexSet(vertexNumber+1, 0); //�洢�¾���ļ���
	vector<shared_ptr<edge>> newEdgeSet;			  //�洢Ҫ����ıߵļ���
	auto& rVertexAdj = *vertexAdj;					  //�ڽӾ���
	auto& rVertexNames = *vertexNames;				  //���㼯��
	newVertexSet[1] = 1;
	//newVertexSet��λ�洢Ŀǰ�¾��㼯�ϵĵ�����
	while (++newVertexSet[0] < vertexNumber) {

		unsigned v, e = 32767;
		shared_ptr<edge> minPowerEdge = nullptr;
		//�ҳ�����ڡ��յ㲻���µ㼯�У�����power��С�ı�
		for (unsigned i = 1; i <= vertexNumber; ++i)
			if (newVertexSet[i] == 1) {//����˵����¾��㼯��
				auto& r = *rVertexAdj[rVertexNames[i]]->getEdgeAdj();
				for each (shared_ptr<edge> eg in r){ //�����˵���������б�
					v = eg->getTo()->getNumber();
					//����������������power��С���յ㲻���¾��㼯��
					if (newVertexSet[v] == 0 && eg->getPower() < e) {
						e = eg->getPower();
						minPowerEdge = eg;
					}
				}
			}

		//��¼�������������ıߺͱ��յ����Ϣ
		newVertexSet[minPowerEdge->getTo()->getNumber()] = 1;
		newEdgeSet.push_back(minPowerEdge);
	}

	//������з��������ıߣ�������С������
	for each (shared_ptr<edge> eg in newEdgeSet)
		cout << "��" << eg->getFrom()->getName() << "��"
			 << eg->getTo()->getName() << "��һ��·" << endl;

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
}

//������ȱ���
void distributionGraph::DFSTraverse(list<shared_ptr<vertex>>& rTourGuide) {

	stack<shared_ptr<vertex>> stackVertex; //�洢·��
	size_t         count = 1;   //�ѷ��ֵľ�������

	//ѡ��һ�����㿪ʼ����ѹ��ջ,��ӽ�·��ͼ
	shared_ptr<vertex> pCurrentVertex = (*vertexAdj)[(*vertexNames)[1]];
	pCurrentVertex->setVisited(true);
	stackVertex.push(pCurrentVertex);
	rTourGuide.push_back(pCurrentVertex);

	//ջ�л��е�˵������Щ��û�г���
	while (!stackVertex.empty()) {

		const shared_ptr<list<weak_ptr<edge>>> pAulEdgeAdj = stackVertex.top()->getEdgeAdj();//�Ըþ���Ϊ���ȵı�
		auto ile = pAulEdgeAdj->begin();   //��ʼ��iterator
		auto ileEnd = pAulEdgeAdj->end();

		//�������п��ܾ����ı�
		while (ile != ileEnd) {
			//�ж��Ƿ���ʹ��ñ���Ⱦ���
			//����þ���û�б����ʹ������þ���״̬�ı䣬��ѹ��ջ�����·��ͼ
			if (!(*ile).lock()->getTo()->isVisited()) {
				pCurrentVertex = (*ile).lock()->getTo(); //һ��ȷ�� (*ile).lock()���ǿ���ָ��
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

	//�������е�ķ���״̬��
	updateVertexVisited();
}

//�жϵ�����·ͼ���޻�·
void distributionGraph::topoSort() {

	//�ж�ͼ�Ƿ�Ϊ�պ͵���ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return;

	if (tourGuide->empty())
		DFSTraverse(*tourGuide);

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
	shared_ptr<matrix> map = make_shared<matrix>
		(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 0));
	shared_ptr<vector<int>> indegree = make_shared<vector<int>>(vertexNumber + 1, 0);

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
}

//ʹ��������ȷ���������������
void distributionGraph::findLoopUsingDFS(vector<string>& loop){//loop�洢��·
	//����ͼ�����ǿյ�
	if (tourGuide->empty())
		DFSTraverse(*tourGuide);

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
	vertexMtx = make_shared<matrix>(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 32767));
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

	auto& rVertexAdj = *vertexAdj;
	//�򿪴��о�����Ϣ���ļ�
	ifstream in("info.txt");

	//��ȡ�ڵ����ͱ���,�ַ���ת����
	char c[128];
	char n[16];
	memset(n, '\0', 16);
	in.getline(n, 16, ' ');
	vertexNumber = atoi(n);
	memset(n, '\0', 16);
	in.getline(n, 16);
	edgeNumber = atoi(n);
	cout << "�����붨�����ͱ�����" << vertexNumber << ' ' << edgeNumber << endl;

	//ѭ����ȡ�ļ��еľ���������Ϣ
	cout << "\n\t\t\"��������������Ϣ\"\n���������������֣�";
	
	vertexNames->push_back(" ");
	
	for (unsigned i = 1; i <= vertexNumber; ++i) {
		unsigned p;
		memset(c, '\0', 128);
		in.getline(c, 128, ' ');
		string name(c);
		cout << name << ' ';
		memset(n, '\0', 16);
		in.getline(n, 16, ' ');
		p = atoi(n);
		memset(c, '\0', 128);
		in.getline(c, 128);
		string info(c);
		rVertexAdj[name] = make_shared<vertex>(name, i, p, c);
		vertexNames->push_back(name);
	}
	cout << endl;
	
	//��ȡ����Ϣ
	for (unsigned i = 1; i <= edgeNumber; ++i) {
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
		memset(n, '\0', 16);
		in.getline(n, 128);
		power = atoi(n);
		cout << power << endl;

		//Ϊ�ڵ�ͱ���ӹ�����ϵ
		shared_ptr<vertex> from = rVertexAdj[f];
		shared_ptr<vertex> to = rVertexAdj[t];
		shared_ptr<edge> e1 = make_shared<edge>(from, to, power);
		shared_ptr<edge> e2 = make_shared<edge>(to, from, power);
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
	vertexMtx = make_shared<matrix>(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 32767));
	Adj2Mtx(*vertexMtx);

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
void distributionGraph::Adj2Mtx(matrix& rMatrix){
	size_t         size = vertexAdj->size();
	
	//�������еıߣ������ڽӾ��������
	for each (shared_ptr<edge> eg in *pEdgeVec){
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

//�������򰴾����ܻ�ӭ�� [from, to)
void distributionGraph::quickSort(vector<shared_ptr<vertex>>& set, unsigned from, unsigned to) {
	if (to - from < 4) {
		insertSort(set, from, to);
		return;
	}
	shared_ptr<vertex> mark = set[(from + to) >> 1];
	unsigned popl = mark->getPopularity();
	unsigned left = from;
	unsigned right = to - 1;
	while (right > left) {
		while (set[left]->getPopularity() > popl)
			++left;
		while (set[right]->getPopularity() < popl)
			--right;
		if (right > left) {
			swap(set[left], set[right]);
			++left;
			--right;
		}
	}
	if (from < right)
		quickSort(set, from, right + 1);
	if (left < to)
		quickSort(set, left, to);
}

//�������򰴾����ܻ�ӭ��
void distributionGraph::insertSort(vector<shared_ptr<vertex>>& set, unsigned from, unsigned to) {
	if (to - from <= 1)
		return;
	for (unsigned i = from + 1; i < to; ++i)
		for (unsigned j = i; j > from && set[j - 1]->getPopularity() < set[j]->getPopularity(); --j)
			swap(set[j - 1], set[j]);
}

//���캯��
distributionGraph::distributionGraph()
	:vertexAdj(make_shared<unordered_map<string, shared_ptr<vertex>>>()),
	pEdgeVec(make_shared<vector<shared_ptr<edge>>>()),
	vertexNames(make_shared<vector<string>>()),
	tourGuide(make_shared<list<shared_ptr<vertex>>>()),
	lot(make_shared<parkingLot>())
{
}


//�ھ��㼰�����������ؼ���
void distributionGraph::queryWords(const string& key, vector<shared_ptr<vertex>>& vtHasInfo) {
	for each (auto& var in *vertexAdj) {
		const string& name = var.first;
		const string& info = var.second->getInfo();
		if (kmp(key, name) || kmp(key, info)) {
			vtHasInfo.push_back(var.second);
		}
	}
}

//kmp�㷨ƥ��ͼ�������ƻ��߼�ӣ����content�к���target������true
bool distributionGraph::kmp(const string& target, const string& content) {
	unsigned tgLen = target.size();
	unsigned ctLen = content.size();
	if (tgLen > ctLen) {
		return false;
	}
	vector<int> vi;
	computePrefixFunction(vi, target); //kmpǰ׺����
	int q = -1;
	for (unsigned i = 0; i < ctLen; ++i) {
		while (q > -1 && target[q + 1] != content[i])
			q = vi[q]; //������ֲ���ȵģ�����
		if (target[q + 1] == content[i])
			++q;
		if (q == tgLen-1)
			return true;
	}
	return false;
}

//kmpǰ׺����
void distributionGraph::computePrefixFunction(vector<int>& vi, const string& str) {
	vi.resize(str.size());
	unsigned m = str.size();
	vi[0] = -1;
	int k = -1;
	for (unsigned q = 1; q < m; ++q) {
		while (k >= 0 && str[k + 1] != str[q])
			k = vi[k];
		if (str[k + 1] == str[q])
			++k;
		vi[q] = k;
	}
}