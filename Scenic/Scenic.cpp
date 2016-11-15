// Scenic.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "distributionGraph.h"
#include <Windows.h>
void welcomeWords();
int main()
{
	distributionGraph* graph = new distributionGraph();
	int cmd;
	while (true) {
		welcomeWords();
		cin >> cmd;
		switch (cmd)
		{
		case 0:
			break;
		case 1:
			graph->createGraph();
			break;
		case 2:
			graph->outputGraph();
			break;
		case 3:
			graph->creatTourSortGraph();
			break;
		case 4:
			graph->topoSort();
			break;
		case 5:
			graph->miniDistanse();
			break;
		case 6:
			graph->miniSpanTree();
			break;
		case 7:
			graph->sortedByPopularity();
			break;
		case 8:
			graph->parkingLotMagm();
			break;
		default:
			break;
		}
		if (cmd == 0 || cmd < 0 || cmd > 7)
			break;
		Sleep(1000);
	}
	delete graph;
	return 0;
}

void welcomeWords() {
	cout << "================================\n";
	cout << "     ��ӭʹ�þ�����Ϣ����ϵͳ\n";
	cout << "         ***��ѡ��˵�***\n";
	cout << "================================\n";
	cout << "1��������������ֲ�ͼ��\n";
	cout << "2�������������ֲ�ͼ��\n";
	cout << "3�����������·ͼ��\n";
	cout << "4�����������·ͼ�еĻ�·��\n";
	cout << "5�����������������·������̾��롣\n";
	cout << "6�������·�޽��滮ͼ��\n";
	cout << "7�������㻶ӭ�Ƚ������������\n";
	cout << "8��ͣ��������������¼��Ϣ��\n";
	cout << "0���˳�ϵͳ��" << endl;
	cout << "��������Ҫѡ��Ĳ˵��";
}