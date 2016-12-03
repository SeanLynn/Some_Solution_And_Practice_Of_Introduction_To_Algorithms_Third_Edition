#include "stdafx.h"
#include "distributionGraph.h"

//�û�����Ĺؼ��ֽ��о���Ĳ���
unsigned distributionGraph::searchWithKeyWords(char* msg) {

	if (isGraphEmpty())
		return 0;

	string key(msg);
	string MSG = ""; //��Ϊ��ʱ�������¼string
	vector<shared_ptr<vertex>> vtHasInfo;
	queryWords(key, vtHasInfo);//KMPƥ��
	if (vtHasInfo.empty()) {
		MSG.append("û���롰" + key + "����ص����ݡ�");
		return 0;
	}
	MSG.append("���������Ϣ���У�\n");
	for each (shared_ptr<vertex> var in vtHasInfo)
	{
		MSG.append("���㣺" + var->getName().substr(1));
		MSG.append("\n������ܣ�" + var->getInfo() + "\n");
	}

	auto buf = MSG.c_str();
	strcpy_s(msg, strlen(buf) + 1, buf);
	return 1;
}

//�����㻶ӭ�Ƚ�������
unsigned distributionGraph::sortedByPopularity(char* msg) {
	vector<shared_ptr<vertex>> vertexSet;
	for each (auto& var in *pVertexAdj)
		vertexSet.push_back(var.second);

	quickSort(vertexSet, 0, vertexSet.size());

	string MSG = "�������ȶ�����\n";
	for each (auto v in vertexSet)
	{
		MSG.append(v->getName().substr(1) + " " + to_string(v->getPopularity()) + "\n");
	}

	auto buf = MSG.c_str();
	strcpy_s(msg, strlen(buf) + 1, buf);
	return 1;
}

//�����·�޽��滮ͼ
unsigned distributionGraph::miniSpanTree(char* msg) {
	if (isGraphEmpty()) {
		return 0;
	}

	vector<unsigned> newVertexSet(vertexNumber + 1, 0); //�洢�¾���ļ���
	vector<shared_ptr<edge>> newEdgeSet;				//�洢Ҫ����ıߵļ���
	auto& rVertexAdj = *pVertexAdj;						//�ڽӾ���
	auto& rVertexNames = *pVertexNames;					//���㼯��
	newVertexSet[1] = 1;

	//newVertexSet��λ�洢Ŀǰ�¾��㼯�ϵĵ�����
	while (++newVertexSet[0] < vertexNumber) {

		unsigned v, e = 32767;
		shared_ptr<edge> minPowerEdge = nullptr;
		//�ҳ�����ڡ��յ㲻���µ㼯�У�����power��С�ı�
		for (unsigned i = 1; i <= vertexNumber; ++i)
			if (newVertexSet[i] == 1) {//����˵����¾��㼯��
				auto& r = *rVertexAdj[rVertexNames[i]]->getEdgeAdj();
				for each (weak_ptr<edge> eg in r) { //�����˵���������б�
					shared_ptr<edge> seg = eg.lock();
					if (seg != nullptr) {
						v = seg->getTo()->getNumber();
						//���������������յ㲻���¾��㼯�У���power��С
						if (newVertexSet[v] == 0 && seg->getPower() < e) {
							e = seg->getPower();
							minPowerEdge = seg;
						}
					}
				}
			}

		//��¼�������������ıߺͱ��յ����Ϣ
		if (minPowerEdge != nullptr) {
			newVertexSet[minPowerEdge->getTo()->getNumber()] = 1;
			newEdgeSet.push_back(minPowerEdge);
		}
	}

	string MSG = "��С��ͨͼ��\n";

	//������з��������ıߣ�������С������
	for each (shared_ptr<edge> eg in newEdgeSet)
		MSG.append("��" + eg->getFrom()->getName().substr(1) + "��"
			+ eg->getTo()->getName().substr(1) + "��һ��·\n");

	auto buf = MSG.c_str();
	strcpy_s(msg, strlen(buf) + 1, buf);
	return newEdgeSet.size();
}

unsigned distributionGraph::miniDistanse(char* msg, char* start, char* end) {

	if (isGraphEmpty())
		return 0;

	string from(start), to(end);
	string MSG = "";

	auto iterator_from = pVertexAdj->find(from);
	auto iterator_to = pVertexAdj->find(to);
	if (iterator_from == (*pVertexAdj).end() || iterator_to == (*pVertexAdj).end()) {
		MSG.append("������ľ������Ʋ����ڣ����飡\n");
		return 0;
	}


	if (from == to) {
		MSG.append(from.substr(1) + " -> " + to.substr(1));
		MSG.append("\n��̾��룺0");
		return 0;
	}

	unsigned int uintFrom = pVertexAdj->at(from)->getNumber();   //�����
	unsigned int uintTo = pVertexAdj->at(to)->getNumber();     //�յ���
	vector<unsigned int> shortPathTable(vertexNumber + 1);//��ʾV0��Vw����������·��Ȩֵ��
	vector<unsigned int> path(vertexNumber + 1);		  //��ʾV0��Vw�������ǰ�������±�ֵ


														  //shortestPathDijkstra(f0, path, shortPathTable);		  //�Ͻ�˹�����㷨Ѱ�����·��
														  //outPutShortestPath(t1, path, shortPathTable, MSG);     //������������·������̾���


	shortestPathFloyd(uintFrom, uintTo, path, MSG);

	auto buf = MSG.c_str();
	strcpy_s(msg, strlen(buf) + 1, buf);
	return 1;
}

//������ȱ���
void distributionGraph::DFSTraverse(list<shared_ptr<vertex>>& rTourGuide) {

	stack<shared_ptr<vertex>> stackVertex; //�洢·��
	size_t         count = 1;              //�ѷ��ֵľ�������

										   //ѡ��һ�����㿪ʼ����ѹ��ջ,��ӽ�·��ͼ
	shared_ptr<vertex> pCurrentVertex = (*pVertexAdj)[(*pVertexNames)[1]];
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
			auto sile = (*ile).lock();
			if (sile != nullptr && !sile->getTo()->isVisited()) {
				pCurrentVertex = sile->getTo(); //һ��ȷ�� (*ile).lock()���ǿ���ָ��
				pCurrentVertex->setVisited(true);
				stackVertex.push(pCurrentVertex);
				rTourGuide.push_back(pCurrentVertex);

				if (++count >= pVertexAdj->size()) {//������ʹ����еľ��㣬�˳�����
					updateVertexVisited();
					return;
				}

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
unsigned distributionGraph::topoSort(string& MSG) {

	//�������ɵ���ͼ
	if (!pTourGuide->empty()) {
		pTourGuide.swap(make_shared<list<shared_ptr<vertex>>>());
	}

	auto& rTourGuide = *pTourGuide;
	//�ǵݹ�ջ����������ȱ���,����·��ͼ�򵥵ش洢��tourGuide��
	DFSTraverse(rTourGuide);

	//�������򷽷�
	//�ڲ��߼�,����ֵΪvector<string>*
	shared_ptr<vector<int>> result = make_shared<vector<int>>();
	//����������
	aluToposort(result);
	auto& rResult = *result;

	//�����д��msg
	for (unsigned i = 1; i <= vertexNumber; ++i)
		if (rResult[i] > 0) {
			MSG.append("ͼ���л�·\n");

			auto& rNames = *pVertexNames;
			for (unsigned j = i; j <= vertexNumber; ++j)
				if (rResult[j] > 0)
					MSG.append(rNames[j].substr(1) + " ");

			rResult[0] = 1;
			break;
		}

	if (rResult[0] == 0) {
		MSG.append("ͼ��û�л�·");
	}
	return vertexNumber;
}

//�������򷽷���ֻΪ������д�ġ�
void distributionGraph::aluToposort(shared_ptr<vector<int>>& result) {
	//��ʼ������ͼ�еľ������
	shared_ptr<matrix> map = make_shared<matrix>
		(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 0));

	shared_ptr<vector<int>> indegree = make_shared<vector<int>>(vertexNumber + 1, 0);
	auto& rMap = *map;
	auto& rIndegree = *indegree;
	auto  it = pTourGuide->begin();
	auto  iy = it;
	auto  itEnd = pTourGuide->end();
	unsigned  startNumber = (*(it))->getNumber();
	//��ʼ������ͼ���ڽӾ���
	while (++iy != itEnd) {
		unsigned from = (*it++)->getNumber();
		unsigned in = (*iy)->getNumber();
		++rMap[from][in];//Ȩֵ+1
		++rIndegree[in];//���+1
	}

	//�±����ר��Ϊ����д,ȥ����������ʼ���й�ϵ�ı�
	rIndegree[startNumber] = -1;
	for (unsigned k = 1; k <= vertexNumber; k++) {
		if (rMap[startNumber][k] == 1)
			--rIndegree[k];
		rMap[k][startNumber] = 0;
	}
	//������������
	for (unsigned i = 1; i <= vertexNumber; ++i) //����n��
		for (unsigned j = 1; j <= vertexNumber; ++j) //�ҳ����Ϊ0�Ľڵ�
			if (rIndegree[j] == 0) {

				rIndegree[j]--;//��ȼ�Ϊ-1��˵��������

				for (unsigned k = 1; k <= vertexNumber; k++) //ɾ����ýڵ�����ı�
					if (rMap[j][k] == 1)
						rIndegree[k]--;
				break;
			}

	result.swap(indegree);
}

//�Ͻ�˹�����㷨�����·��
void distributionGraph::shortestPathDijkstra(
	unsigned startPoint, vector<unsigned>& path, vector<unsigned>& shortPathTable)
{
	vertexMtx = make_shared<matrix>(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 32767));
	Adj2Mtx(*vertexMtx);

	auto& rMatrix = *vertexMtx;
	unsigned v, w, k, min, num = vertexNumber + 1;
	vector<int> final(num); //final[k]=1��ʾ���startPoint��k������Ȩֵ��
	for (v = 1; v < num; ++v) {
		final[v] = 0;
		shortPathTable[v] = rMatrix[startPoint][v];
		path[v] = startPoint; //��ʼ�������е��ǰ������startPoint��֮���𽥸���
	}
	final[startPoint] = 1;	//startPoint��startPoint����Ҫ��·��
	path[startPoint] = 0;
	//��ʼ��ѭ����ÿ�����startPoint��ĳ����������·��
	for (v = 1; v < num; ++v) {
		min = 32767;
		for (w = 1; w < num; ++w) {//Ѱ����startPoint����ľ���
			if (!final[w] && shortPathTable[w] < min) {
				k = w;
				min = shortPathTable[w];
			}
		}
		final[k] = 1; //��Ŀǰ�ҵ�������ľ�����Ϊ1,�����Ҳ�����startPoint��k�������·��
		for (w = 1; w <num;++w) {
			//�����startPoint�������k����� + k�㵽w����� С��startPoint��w��ľ��룬���²���¼
			if (!final[w] && (min + rMatrix[k][w]) < shortPathTable[w]) {
				shortPathTable[w] = min + rMatrix[k][w];  //����
				path[w] = k;                                //��¼w���ǰ����k��
			}
		}
	}
}


//���������㷨�����·��
void distributionGraph::shortestPathFloyd(unsigned startPoint, unsigned endPoint,
	vector<unsigned>& path, string& MSG)
{
	vertexMtx = make_shared<matrix>(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 32767));
	Adj2Mtx(*vertexMtx);
	auto vPath = make_shared<matrix>(vertexNumber + 1, vector<unsigned>(vertexNumber + 1, 0));
	auto& rvPath = *vPath;
	auto& rMatrix = *vertexMtx;
	//��ʼ��·������
	for (unsigned i = 1; i <= vertexNumber; ++i)
		for (unsigned j = 1; j <= vertexNumber; ++j)
			rvPath[i][j] = j;                // "����i"��"����j"�����·���Ǿ�������j��

	for (unsigned k = 1; k <= vertexNumber; ++k) {
		for (unsigned i = 1; i <= vertexNumber; ++i) {
			for (unsigned j = 1; j <= vertexNumber; ++j) {
				unsigned tmp = (rMatrix[i][k] == 32767 || rMatrix[k][j] == 32767)
					? 32767 : (rMatrix[i][k] + rMatrix[k][j]);
				if (rMatrix[i][j] > tmp) { //���i->j����k��С�ڴ�ǰ��¼������
					rMatrix[i][j] = tmp;
					rvPath[i][j] = rvPath[i][k];
				}
			}
		}
	}

	//����v0->v1��·���;���
	unsigned distance = 0;
	unsigned s = startPoint;
	path[1] = startPoint;
	auto& rVertexNames = *pVertexNames;
	MSG.append("�� " + rVertexNames[startPoint].substr(1) + " �� " + rVertexNames[endPoint].substr(1) + " :\n");
	for (unsigned i = 2; i <= vertexNumber; ++i) {
		if (rvPath[s][endPoint] != endPoint) {
			path[i] = rvPath[s][endPoint];
			distance += rMatrix[s][path[i]];
			MSG.append(rVertexNames[s].substr(1) + " -> " + rVertexNames[path[i]].substr(1) + "~~~\n");
		}
		else {
			path[i] = endPoint;
			distance += rMatrix[s][endPoint];
			MSG.append(rVertexNames[s].substr(1) + " -> " + rVertexNames[endPoint].substr(1) + "\n");
			MSG.append("��̾��룺 " + to_string(distance));
			break;
		}
		s = path[i];
	}
}

//������·������̾���
void distributionGraph::outPutShortestPath(unsigned endPoint, vector<unsigned>& path,
	vector<unsigned>& shortPathTable, string& MSG)
{
	vector<unsigned int> result;
	auto& rVertexNames = *pVertexNames;
	unsigned int v = endPoint;
	do {
		result.push_back(v);
		v = path[v];
	} while (v != 0);

	v = result.size() - 1;
	while (v > 0) //����յ㲻ƥ��
		MSG.append(rVertexNames[result[v--]].substr(1) + " -> " + rVertexNames[result[v]].substr(1) + "\n");

	MSG.append("��̾��룺" + to_string(shortPathTable[endPoint]));
}


//�ڽ�����ת�ڽӾ���
void distributionGraph::Adj2Mtx(matrix& rMatrix) {
	size_t         size = pVertexAdj->size();

	//�������еıߣ������ڽӾ��������
	for each (shared_ptr<edge> eg in *pEdgeVec) {
		unsigned int from = eg->getFrom()->getNumber();
		unsigned int to = eg->getTo()->getNumber();
		rMatrix[from][to] = eg->getPower();
		rMatrix[from][from] = 0;
	}
}


//�������򰴾����ܻ�ӭ�� [from, to)
void distributionGraph::quickSort(vector<shared_ptr<vertex>>& set, unsigned from, unsigned end) {
	if (end - from < 4) { //�������С��4��ʹ�ò�������
		insertSort(set, from, end);
		return;
	}
	shared_ptr<vertex> mark = set[(from + end) >> 1];
	unsigned popl = mark->getPopularity();
	unsigned left = from;
	unsigned right = end - 1;
	while (right > left) {
		while (set[left]->getPopularity() > popl)
			++left;
		while (set[right]->getPopularity() < popl)
			--right;
		//��������ֱ���������������������У�û������֮ǰ�����������ϵ���
		if (right > left) {
			swap(set[left], set[right]);
			++left;
			--right;
		}
	}
	if (from < right)
		quickSort(set, from, right + 1);
	if (left < end)
		quickSort(set, left, end);
}

//kmp�㷨ƥ��ͼ�������ƻ��߼�ӣ����content�к���target������true
//KMP�㷨�ĺ���˼���������Ѿ��õ��Ĳ���ƥ����Ϣ�����к����ƥ����̡� O(m+n)
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
		if (q == tgLen - 1)
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