#pragma once
#include <string>
#include <vector>
#include "Entry.h"

using namespace std;

class BTreeNode {
	static const int keyCount = 2;
	float Selection(Entry* entry, int& inputChoice);
	

public:
	BTreeNode* nextList[keyCount + 1];
	BTreeNode* parent;
	Entry* keyList[keyCount];
	bool isFull;
	bool isLeaf;
	
	BTreeNode(Entry* entry, BTreeNode* _parent);
	~BTreeNode();
	BTreeNode* InsertEntry(Entry* entry, BTreeNode* Head, int& inputChoice);
	vector<Entry*> ReturnList(BTreeNode* Head);
	BTreeNode* UpShift(BTreeNode* Head, BTreeNode* parent, Entry** tempList, int& inputChoice);
};