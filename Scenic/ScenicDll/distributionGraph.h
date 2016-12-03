#pragma once
#include "vertex.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <stack>
#include <algorithm>
class distributionGraph {
public:
	unsigned createLoopGraph(char *);				//����ĿҪ�����ɻ�·��
	unsigned createTourGuide(char*);				//��������ͼ
	unsigned createVertex(char*, unsigned, char*, bool, bool);	//�����½ڵ���߸��¾����
	unsigned createEdge(char*, char*, unsigned);	//�����µ������
	unsigned searchWithKeyWords(char*);				//�û�����Ĺؼ��ֽ��о���Ĳ���
	unsigned sortedByPopularity(char* msg);			//�����㻶ӭ�Ƚ�������
	unsigned miniDistanse(char*, char*, char*);		//�������������·��
	unsigned topoSort(string&);						//�жϵ�����·ͼ���޻�·
	unsigned miniSpanTree(char*);					//�����·�޽��滮ͼ
	distributionGraph();							//���캯��
private:
	typedef vector<vector<unsigned>> matrix;		//matrix��typedef
	shared_ptr<unordered_map<string,
		shared_ptr<vertex>>> pVertexAdj;             //����->���� hash��
	shared_ptr<vector<shared_ptr<edge>>> pEdgeVec;  //���бߵļ��ϣ���������
	shared_ptr<vector<string>> pVertexNames;			//�������ʼ��˳��洢���������
	shared_ptr<list<shared_ptr<vertex>>> pTourGuide;	//�洢����·��ͼ
	shared_ptr<matrix> vertexMtx;					//�洢�ڽӾ���
	unsigned vertexNumber;							//��������
	unsigned edgeNumber;							//�����������ʵ�ʱ�*2

	void DFSTraverse(list<shared_ptr<vertex>>&);	//������ȱ���
	bool isEdge(const string&, const string&);		//�ж���������֮���Ƿ���ֱ�������ı�
	void aluToposort(shared_ptr<vector<int>>&);		//�����޻�·,��������ı�
	void Adj2Mtx(matrix&);					    	//�ڽ�����ת�ڽӾ���
	bool isGraphEmpty();							//�ж�ͼ�Ƿ��
	void updateVertexVisited();						//�������е�ķ���״̬
	void shortestPathDijkstra(unsigned,
		vector<unsigned>&, vector<unsigned>&);		//�Ͻ�˹�����㷨�����·��
	void shortestPathFloyd(unsigned, unsigned,
		vector<unsigned>&, string&);				//���������㷨�����·��
	void outPutShortestPath(unsigned, vector<unsigned>&,
		vector<unsigned>&, string&);				//�����������·��
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

//����ĿҪ�����ɻ�·��
inline unsigned distributionGraph::createLoopGraph(char * msg)
{
	if (isGraphEmpty()) {
		return 0;
	}
	string MSG = "";
	unsigned i = topoSort(MSG); //����������ݵ���ͼ���ɻ�·
	auto buf = MSG.c_str();
	strcpy_s(msg, strlen(buf) + 1, buf);
	return i;
}

//��������ͼ
inline unsigned distributionGraph::createTourGuide(char* msg) {
	//�ж�ͼ�Ƿ�Ϊ��
	if (isGraphEmpty())
		return 0;

	//����ǰ������tourGuide���
	if (!pTourGuide->empty()) {
		pTourGuide.swap(make_shared<list<shared_ptr<vertex>>>());
	}

	auto& rTourGuide = *pTourGuide;
	//�ǵݹ�ջ����������ȱ���,����·��ͼ�򵥵ش洢��tourGuide��
	DFSTraverse(rTourGuide);

	auto iterator_current = rTourGuide.begin();
	auto iterator_end = rTourGuide.end();
	//��¼���б���·�Ͼ����Ľڵ�
	string MSG = (*iterator_current)->getName().substr(1);;
	while (++iterator_current != iterator_end)
		MSG.append(" --> " + (*iterator_current)->getName().substr(1));

	auto buf = MSG.c_str();
	strcpy_s(msg, strlen(buf) + 1, buf);
	return rTourGuide.size();
}

//����������ߣ������������
inline unsigned distributionGraph::createEdge(char* start, char* end, unsigned distance) {
	//��Ĵ�����ȷ���ģ��Ѿ���UI���ж�
	auto pStartVertex = (*pVertexAdj)[start];
	auto pEndVertex = (*pVertexAdj)[end];

	//�������бߣ�ȷ�����ظ��������ظ�����Ȩֵ����
	for each (auto pEdge in *pEdgeVec)
		if (pEdge->getFrom() == pStartVertex && pEdge->getTo() == pEndVertex) {
			pEdge->setPower(distance);
			return pEdgeVec->size();
		}
	shared_ptr<edge> pEdge1 = make_shared<edge>(pStartVertex, pEndVertex, distance);
	shared_ptr<edge> pEdge2 = make_shared<edge>(pEndVertex, pStartVertex, distance);
	pEdgeVec->push_back(pEdge1);
	pEdgeVec->push_back(pEdge2);
	++edgeNumber;
	pStartVertex->addEdge(pEdge1);
	pEndVertex->addEdge(pEdge2);
	return pEdgeVec->size();
}

//��WPF���ô�������Ľڵ�
inline unsigned distributionGraph::createVertex(char* name, unsigned popu, char* info, bool r, bool w)
{
	auto it = (*pVertexAdj).find(name);
	unsigned i;
	if (it != (*pVertexAdj).end()) { //����Ѿ���������㣬������������Ϣ����
		it->second->setPopularity(popu);
		it->second->setInfo(info);
		it->second->setRestArea(r);
		it->second->setWC(w);
		i = it->second->getNumber();
	}
	else {//�������������㴴��
		i = (*pVertexAdj).size() + 1;
		(*pVertexAdj)[name] = make_shared<vertex>(name, i, popu, info, r, w);
		pVertexNames->push_back(name);
		++vertexNumber;
	}
	return vertexNumber;
}

//�ж��Ƿ����f->t�ı�
inline bool distributionGraph::isEdge(const string& f, const string& t) {
	return pVertexAdj->at(f)->isAccessDirect(pVertexAdj->at(t));
}

//�ж�ͼ�Ƿ��
inline bool distributionGraph::isGraphEmpty() {
	//�ж���vertexAdj�ǿյģ����˳����½������ѡ��
	if (!pVertexAdj->empty())
		return false;
	return true;
}

//�������е�ķ���״̬
inline void distributionGraph::updateVertexVisited() {
	for each (auto& var in *pVertexAdj)
		var.second->setVisited(false);
}

//�������򰴾����ܻ�ӭ��
inline void distributionGraph::insertSort(vector<shared_ptr<vertex>>& set, unsigned from, unsigned to) {
	if (to - from <= 1)
		return;
	for (unsigned i = from + 1; i < to; ++i)
		for (unsigned j = i;
			j > from && set[j - 1]->getPopularity() < set[j]->getPopularity();
			--j)
			swap(set[j - 1], set[j]); //��jλ����ǰһλ���Ƚϣ��������󣬽���
}

//���캯��
inline distributionGraph::distributionGraph()
	:pVertexAdj(make_shared<unordered_map<string, shared_ptr<vertex>>>()),
	pEdgeVec(make_shared<vector<shared_ptr<edge>>>()),
	pVertexNames(make_shared<vector<string>>()),
	pTourGuide(make_shared<list<shared_ptr<vertex>>>()),
	vertexNumber(0),
	edgeNumber(0)
{
	pVertexNames->push_back(" ");//����λ��λ
}

//�ھ��㼰�����������ؼ���
inline void distributionGraph::queryWords(const string& key, vector<shared_ptr<vertex>>& vtHasInfo) {
	for each (auto& var in *pVertexAdj) {
		const string& name = var.first;
		const string& info = var.second->getInfo();
		if (kmp(key, name) || kmp(key, info)) {//ƥ�侰�����ֺͼ��
			vtHasInfo.push_back(var.second);
		}
	}
}