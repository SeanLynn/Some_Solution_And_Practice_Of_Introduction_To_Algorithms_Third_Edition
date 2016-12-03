// Scenic.cpp : �������̨Ӧ�ó������ڵ㡣
//


#include "stdafx.h"
#include "distributionGraph.h"
#include <memory>
#include <stdlib.h>
#include <crtdbg.h>
shared_ptr<distributionGraph> graph = make_shared<distributionGraph>();

//�������ṩ��������ĸ��ֽӿ�
extern "C" __declspec(dllexport) unsigned CreateNewVertex(
	char*, unsigned, char*, bool, bool);   //�������߸��µ�
extern "C" __declspec(dllexport) unsigned CreateNewEdge(char*, char*, unsigned);   //�������߸��±�
extern "C" __declspec(dllexport) unsigned GetTourGuide(char*);                     //��õ���ͼ
extern "C" __declspec(dllexport) unsigned GetLoopGraph(char*);					   //��û�·
extern "C" __declspec(dllexport) unsigned MiniDistance(char*, char*, char*);	   //�����������·���;���
extern "C" __declspec(dllexport) unsigned MiniSpanTree(char*);                     //����滮����С������
extern "C" __declspec(dllexport) unsigned SortByPopularity(char*);				   //���ݻ�ӭ������
extern "C" __declspec(dllexport) unsigned KeySearch(char*);						   //�ؼ�������


int main()
{
	_CrtDumpMemoryLeaks(); //����ڴ�й©
	return 0;
}

//�ؼ�������
extern "C" __declspec(dllexport) unsigned KeySearch(char* msg) {
	return graph->searchWithKeyWords(msg);
}

//���ݻ�ӭ������
extern "C" __declspec(dllexport) unsigned SortByPopularity(char* msg) {
	return graph->sortedByPopularity(msg);
}

//����滮����С������
extern "C" __declspec(dllexport) unsigned MiniSpanTree(char* msg) {
	return graph->miniSpanTree(msg);
}

//�����������·���;���
extern "C" __declspec(dllexport) unsigned MiniDistance(char* msg, char* start, char* end) {
	return graph->miniDistanse(msg, start, end);
}

//��û�·
extern "C" __declspec(dllexport) unsigned GetLoopGraph(char* msg) {
	return graph->createLoopGraph(msg);
}

//��õ���ͼ
extern "C" __declspec(dllexport) unsigned GetTourGuide(char* msg) {
	return graph->createTourGuide(msg);
}

//�������߸��±�
extern "C" __declspec(dllexport) unsigned CreateNewEdge(char* start, char* end, unsigned distance) {
	return graph->createEdge(start, end, distance);
}

//�������߸��µ�
extern "C" __declspec(dllexport) unsigned CreateNewVertex(char* name, unsigned popu, char* info, bool r, bool w) {
	return graph->createVertex(name, popu, info, r, w);
}