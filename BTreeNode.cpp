#include "BTreeNode.h"
#include <iostream>
using namespace std;


BTreeNode::BTreeNode(Entry* entry, BTreeNode* _parent) {
	keyList[0] = entry;
	for (int i = 1; i < keyCount; i++) {
		keyList[i] = nullptr;
	}
	for (int i = 0; i < keyCount + 1; i++) {
		nextList[i] = nullptr;
	}
	isFull = false;
	isLeaf = true;
	parent = _parent;
}

BTreeNode::~BTreeNode() {
	delete[] keyList;
	delete[] nextList;
	delete parent;
}

float BTreeNode::Selection(Entry* entry, int& inputChoice) {
	if (inputChoice == 0) { return (float) entry->dateInt; }
	else if (inputChoice == 1) { return (float) entry->total; }
	else if (inputChoice == 2) { return (float) entry->changeInTotal; }
	else if (inputChoice == 3) { return entry->totalPerThou; }
	else if (inputChoice == 4) { return entry->changeInTotalPerThou; }
}

BTreeNode* BTreeNode::InsertEntry(Entry* entry, BTreeNode* Head, int& inputChoice) {
	if (Head == nullptr) {
		Head = new BTreeNode(entry, nullptr);
		Head->keyList[0] = entry;
		return Head;
	}
	if (Head->isLeaf) {
		cout << "Leaf" << endl;
		if (!(isFull)) {
			cout << "isFull" << endl;
			int i = 0;
			while (i < keyCount) {
				if (Head->keyList[i] == nullptr || Selection(entry, inputChoice) < Selection(Head->keyList[i], inputChoice)) {
					break;
				}
				i++;
			}
			if (i == keyCount-1) {
				Head->keyList[i] = entry;
				isFull = true;
				//cout << "true";

			}
			else {
				Entry* shiftEntry = Head->keyList[i];
				Entry* tempEntry;
				Head->keyList[i] = entry;
				i++;
				if (shiftEntry != nullptr) {
					while (i < keyCount) {
						tempEntry = Head->keyList[i];
						Head->keyList[i] = shiftEntry;
						shiftEntry = tempEntry;
						if (i == keyCount - 1) {
							isFull = true;
							break;
						}
						if (shiftEntry == nullptr) { break; }
						i++;
					}
				}
			}
			return Head;
		}
		else {
			Entry* tempList[keyCount + 1];
			bool isInserted = false;
			int j = 0;
			for (int i = 0; i < keyCount + 1; i++) {
				if ((i == keyCount && !isInserted) || (!isInserted && Selection(entry, inputChoice) < Selection(Head->keyList[i], inputChoice))) {
					tempList[j] = entry;
					isInserted = true;
					j++;
					i--;
					//cout << "true";
				}
				else if (i < keyCount) {
					tempList[j] = Head->keyList[i];
					j++;
				}
			}
			//for (int i = 0; i < keyCount + 1; i++) { tempList[i]->PrintEntry(); }
			Head = UpShift(Head, parent, tempList, inputChoice);
			return Head;
		}
	}
	else {
		int i = 0;
		while (i < keyCount+1) {
			cout << i << endl;
			if (Head->keyList[i] == nullptr) { cout << "NULL" << endl; }
			if (i == keyCount || Head->keyList[i] == nullptr || Selection(entry, inputChoice) < Selection(Head->keyList[i], inputChoice)) {
				break;
			}
			i++;
		}
		//cout << "1" << endl;
		InsertEntry(entry, Head->nextList[i], inputChoice);
		//cout << "2" << endl;
		return Head;
	}
}

BTreeNode* BTreeNode::UpShift(BTreeNode* Head, BTreeNode* parent, Entry** tempList, int& inputChoice) {
	if (parent == nullptr) {
		parent = new BTreeNode(tempList[(keyCount + 1) / 2], nullptr);
		parent->nextList[1] = new BTreeNode(tempList[((keyCount + 1) / 2) + 1], parent);
		parent->nextList[0] = Head;
		for (int i = 0; i < ((keyCount + 1) / 2); i++) {
			keyList[i] = tempList[i];
		}
		for (int i = ((keyCount + 1) / 2); i < keyCount; i++) {
			keyList[i] = nullptr;
		}
		for (int i = 1; i < keyCount / 2; i++) {
			parent->nextList[1]->keyList[i] = tempList[((keyCount + 1) / 2) + 1 + i];
		}
		parent->isLeaf = false;
		return parent;
	}
	else if (!parent->isFull) {
		Entry* newEntry = tempList[(keyCount + 1) / 2];
		int newEntryIndex;
		int i = 0;
		while (i < keyCount) {
			if (parent->keyList[i] == nullptr || Selection(newEntry, inputChoice) < Selection(parent->keyList[i], inputChoice)) {
				break;
			}
			i++;
		}
		Entry* shiftEntry = parent->keyList[i];
		BTreeNode* shiftNext = parent->nextList[i+1];
		Entry* tempEntry;
		BTreeNode* tempNext;
		parent->keyList[i] = newEntry;
		newEntryIndex = i;
		i++;
		if (shiftEntry != nullptr) {
			while (i < keyCount) {
				tempEntry = parent->keyList[i];
				tempNext = parent->nextList[i + 1];
				parent->keyList[i] = shiftEntry;
				parent->nextList[i + 1] = shiftNext;
				shiftEntry = tempEntry;
				shiftNext = tempNext;
				if (i == keyCount - 1) {
					parent->isFull = true;
					break;
				}
				if (shiftEntry == nullptr) { break; }
				i++;
			}
		}

		parent->nextList[newEntryIndex + 1] = new BTreeNode(tempList[((keyCount + 1) / 2) + 1], parent);
		for (int i = 0; i < ((keyCount + 1) / 2); i++) {
			keyList[i] = tempList[i];
		}
		for (int i = ((keyCount + 1) / 2); i < keyCount; i++) {
			keyList[i] = nullptr;
		}
		for (int i = 1; i < keyCount / 2; i++) {
			parent->nextList[newEntryIndex + 1]->keyList[i] = tempList[((keyCount + 1) / 2) + 1 + i];
		}


	}
	isFull = false;
}


vector<Entry*> BTreeNode::ReturnList(BTreeNode* Head){
	vector<Entry*> CompleteList;
	return CompleteList;
}