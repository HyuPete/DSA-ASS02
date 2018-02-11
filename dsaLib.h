/*
 * =========================================================================================
 * Name        : dsaLib.h
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 * =========================================================================================
 */

#ifndef A02_DSALIB_H
#define A02_DSALIB_H

#include <string>
#include <math.h>
#include <vector>

using namespace std;

#ifndef VM_RECORD
#define VM_RECORD
struct VM_Record 
{
	//char    id[ID_MAX_LENGTH];
	string id;
	time_t  timestamp;
	double  longitude, latitude;

	// default constructor
	VM_Record() {
		// id[0] = 0;
		id = "";
	}
	VM_Record(const char* busID) {
		//strcpy(id, busID);
		id = busID;
	}
	// copy constructor
	VM_Record(VM_Record& bus) : timestamp(bus.timestamp), longitude(bus.longitude), latitude(bus.latitude) {
		//strcpy(id, bus.id);
		id = bus.id;
	}
};
#endif // !VM_RECORD


class DSAException {
    int     _error;
    string  _text;
public:
    DSAException() : _error(0), _text("Success") {}
    DSAException(int err) : _error(err), _text("Unknown Error") {}
    DSAException(int err, const char* text) : _error(err), _text(text) {}

    int getError() { return _error; }
    string& getErrorText() { return _text; }
};

template <class T>
struct L1Item {
    T data;
    L1Item<T> *pNext;
    L1Item() : pNext(NULL) {}
    L1Item(T &a) : data(a), pNext(NULL) {}
};

template <class T>
class L1List {
	L1Item<T>   *_pHead;// The head pointer of linked list
	L1Item<T> *pTail;
	size_t      _size;// number of elements in this list
public:
	L1List() : _pHead(NULL), pTail(nullptr), _size(0) {}
	L1List(T &a) :_pHead{ new L1Item<T>(a) }, _size{ 1 } { pTail = _pHead; }
	L1List(T *a) {}
	//L1List(char a) {}
	// copy constructor
	L1List(L1List<T> &L) : L1List()
	{
		L1Item<T> *p{ L.getHead() };
		while (p)
		{
			push_back(p->data);
			p = p->pNext;
		}
	}
	~L1List()
	{
		clean();
	}

	void    clean()
	{
		while (removeHead() == 0) { continue; }
		return;
	}
	bool    isEmpty() {
		return _pHead == NULL;
	}
	size_t  getSize() {
		return _size;
	}
	L1Item<T>* getHead() { return _pHead; }
	L1Item<T>* getTail() { return pTail; }
	// decrease size, be carefull to use this one
	void decreaseSizeBy1() { _size--; }

	// access list member, i = 0..(_size - 1)
	T&      at(int i)
	{
		if (!_pHead) throw DSAException(1, "Empty list!");
		if (i<0 || i> _size - 1) throw DSAException(2, "Invalid index!");
		if (i == _size - 1)
		{
			if (pTail) return pTail->data;
			else throw DSAException(3, "Out of bound!");
		}
		L1Item<T> *p{ _pHead };
		while (true)
		{
			if (i == 0) return p->data;
			p = p->pNext; i--;
			//if (!p) throw DSAException(3, "Out of bound!");
		}
	}
	T&      operator[](int i)
	{
		if (!_pHead) throw DSAException(1, "Empty list!");
		if (i < 0 || i > _size - 1) throw DSAException(2, "Invalid index!");
		if (i == _size - 1)
		{
			if (pTail) return pTail->data;
			else throw DSAException(3, "Out of bound!");
		}
		L1Item<T> *p{ _pHead };
		while (true)
		{
			if (i == 0) return p->data;
			p = p->pNext; i--;
			//if (!p) throw DSAException(3, "Out of bound!");
		}
	}

	// find the element having the target data, return true/false
	// idx will be assigned to the index of the found element for future access
	// if not found idx = -1
	bool    find(T &a, int &idx)
	{
		try
		{
			if (!_pHead) throw DSAException(3, "Empty list!");
			L1Item<T> *p{ _pHead };
			idx = 0;
			while (p)
			{
				if (p->data == a) return 1;
				p = p->pNext; idx++;
			}
			idx = -1;
			return 0;
		}
		catch (DSAException &e)
		{
			cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
			idx = -1;
			return 0;
		}
	}
	// return true/false, if not found idx = -1
	bool find(char *ch, int &idx) {}
	// insert a item after the the item with index i
	// return 0 if success
	int     insert(int i, T &a)
	{
		try
		{
			if (!_pHead) throw DSAException(1, "Empty list!");
			if (i < 0 || i >_size - 1) throw DSAException(2, "Invalid index!");
			L1Item<T> *p{ _pHead };
			while (true)
			{
				if (i == 0)
				{
					L1Item<T> *pNew{ new L1Item<T>(a) };
					pNew->pNext = p->pNext;
					p->pNext = pNew;
					_size++;
					return 0;
				}
				p = p->pNext; i--;
				if (!p) throw DSAException(3, "Out of bound!");
			}
		}
		catch (DSAException &e)
		{
			cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
			return -1;
		}
	}
	// remove node past this one
	// return 0 if success
	int     remove(int i)
	{
		try
		{
			if (!_pHead) throw DSAException(1, "Empty list!");
			if (i < 0 || i >_size - 1) throw DSAException(2, "Invalid index!");
			if (i == _size - 1) throw DSAException(4, "There is no item after the last item in the list to be removed!");
			L1Item<T> *p{ _pHead };
			while (true)
			{
				if (!p->pNext) throw DSAException(3, "Out of bound!");
				if (i == 0)
				{
					L1Item<T> *pDel{ p->pNext };
					p->pNext = pDel->pNext;
					delete pDel;
					_size--;
					return 0;
				}
				p = p->pNext; i--;
			}
		}
		catch (DSAException &e)
		{
			cerr << '\n' << e.getError() << ": " << e.getErrorText() << '\n';
			return -1;
		}
	}
	// remove node past this one
	int     push_back(T &a); // implement below
	//int push_back(char *a) {} // using for type char[]
	int     insertHead(T &a); // implement below
							  //void insertHead(char){}
	//int insertHead(char *a) {} // using for type char[]
							   /// Remove the first item of the list
							   /// Return 0 if success
	int     removeHead()
	{
		if (_pHead) {
			if (pTail == _pHead) pTail = nullptr;
			L1Item<T> *p = _pHead;
			_pHead = p->pNext;
			delete p;
			_size--;
			return 0;
		}
		return -1;
	}
	int     removeLast(); // implement below

	// call the hiden function reverse implemented by recursion
	void reverse()
	{
		reverse(_pHead); return;
	}

	void    traverse(void(*op)(T&)) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data);
			p = p->pNext;
		}
	}
	void traverse(int(*op)(T&))
	{
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data);
			p = p->pNext;
		}
	}
	void traverse(void(*op)(L1Item<T>*))
	{
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p);
			p = p->pNext;
		}
	}
	// copy the entire list to another list
	void traverse(L1List<T> &pL)
	{
		L1Item<T>   *p = _pHead;
		while (p) {
			pL.push_back(p->data);
			p = p->pNext;
		}
	}
	// special traverse for 
	//void traverse(T &a) {}
	//void traverse(T *a) {}
	//
	void    traverse(void(*op)(T&, void*), void *pParam) {
		L1Item<T>   *p = _pHead;
		while (p) {
			op(p->data, pParam);
			p = p->pNext;
		}
	}
	// traverse until found item.data < a <= item.next.data, then insert a in between
	//void traverseAndInsert(T &a) {}
	// special traverse funtction for list ninjaInfo
	//void traverse(L1List<L1List<T>> &lL) {}
	// if not found idx = -1
	//const char* traverseAndRemove(char *killerID, int &targetIndex) {}
	// overload operator

protected:
	// reverse the list by recursion
	// parameter 'int i' to mark whether or not pH are pointing to the original head item of the list (true when i = 0)
	void    reverse(L1Item<T> *pH, short i = 0)
	{
		if (!pH)
		{
			cerr << "\nEmpty list cannot be reversed!\n";
			return;
		}
		if (!pH->pNext)
		{
			_pHead = pH;
			return;
		}
		reverse(pH->pNext, 1);
		pH->pNext->pNext = pH;
		if (i == 0)
		{
			pH->pNext = nullptr;
			pTail = pH;
		}
		return;
	}
};

/// Insert item to the end of the list
/// Return 0 if success
template <class T>
int L1List<T>::push_back(T &a) {
	if (_pHead == NULL) {
		_pHead = new L1Item<T>(a);
		pTail = _pHead;
	}
	else {
		/*L1Item<T>   *p = _pHead;
		while (p->pNext) p = p->pNext;
		p->pNext = new L1Item<T>(a);*/
		pTail->pNext = new L1Item<T>(a);
		pTail = pTail->pNext;
	}
	_size++;
	return 0;
}

/// Insert item to the front of the list
/// Return 0 if success
template <class T>
int L1List<T>::insertHead(T &a) {
	L1Item<T>   *p = new L1Item<T>(a);
	p->pNext = _pHead;
	_pHead = p;
	if (!pTail) pTail = _pHead;
	_size++;
	return 0;
}

/// Remove the last item of the list
/// Return 0 if success
template <class T>
int L1List<T>::removeLast() {
	if (_pHead) {
		if (_pHead->pNext) {
			L1Item<T>* prev = _pHead;
			L1Item<T>* pcur = prev->pNext;
			while (pcur->pNext) {
				prev = pcur;
				pcur = pcur->pNext;
			}
			delete pcur;
			prev->pNext = NULL;
			pTail = prev;
		}
		else {
			delete _pHead;
			_pHead = pTail = NULL;
		}
		_size--;
		return 0;
	}
	return -1;
}


/************************************************************************
 * This section is for AVL tree
 ************************************************************************/
//template <class T>
//struct AVLNode {
//    T           _data;
//    AVLNode<T>   *_pLeft, *_pRight;
//#ifdef AVL_USE_HEIGHT
//    int         _height;
//    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _height(1) {}
//#else
//    int         _bFactor;
//    AVLNode(T &a) : _data(a), _pLeft(NULL), _pRight(NULL), _bFactor(0) {}
//#endif
//};

//template <class T>
//class AVLTree {
//    AVLNode<T> *_pRoot;
//public:
//    AVLTree() : _pRoot(NULL) {}
//    ~AVLTree() { destroy(_pRoot); }
//
//    bool find(T& key, T* &ret) { return find(_pRoot, key, ret); }
//    bool insert(T& key) { return insert(_pRoot, key); }
//    bool remove(T& key) { return remove(_pRoot, key); }
//    void traverseNLR(void (*op)(T&)) { traverseNLR(_pRoot, op); }
//    void traverseLNR(void (*op)(T&)) { traverseLNR(_pRoot, op); }
//    void traverseLRN(void (*op)(T&)) { traverseLRN(_pRoot, op); }
//
//protected:
//    void destroy(AVLNode<T>* &pR);
//    bool find(AVLNode<T> *pR, T& key, T* &ret);
//    bool insert(AVLNode<T>* &pR, T& a);
//    bool remove(AVLNode<T>* &pR, T& a);
//    void traverseNLR(AVLNode<T> *pR, void (*op)(T&));
//    void traverseLNR(AVLNode<T> *pR, void (*op)(T&));
//    void traverseLRN(AVLNode<T> *pR, void (*op)(T&));
//
//    void rotLeft(AVLNode<T>* &pR);
//    void rotRight(AVLNode<T>* &pR);
//    void rotLR(AVLNode<T>* &pR);
//    void rotRL(AVLNode<T>* &pR);
//
//    bool balanceLeft(AVLNode<T>* &pR);
//    bool balanceRight(AVLNode<T>* &pR);
//};

//########################################################################################################
enum BalancedFactor
{
	B_LH = 1,
	B_EH = 0,
	B_RH = -1,
	UB_LH = 2,
	UB_RH = -2
};
//#####################################
template <class T, class U>
class TreeNode
{
public:
	U * key{ nullptr };
	T *data{ nullptr };
	TreeNode<T, U> *left{ nullptr };
	TreeNode<T, U> *right{ nullptr };
	TreeNode(T *d, U *k, TreeNode<T, U> *pL = nullptr, TreeNode<T, U> *pR = nullptr) : data(d), key(k), left(pL), right(pR)
	{
		//key = static_cast<U*>(data);
		if (left) leftH = left->getHeight();
		if (right) rightH = right->getHeight();
		height = (leftH < rightH) ? rightH + 1 : leftH + 1;
		b = static_cast<BalancedFactor>(leftH - rightH);
	}
	TreeNode(T *d, TreeNode<T, U> *pL = nullptr, TreeNode<T, U> *pR = nullptr) {}
	TreeNode(TreeNode<T, U> *n): key{n->key}, data{n->data}{}
	TreeNode() {}
	~TreeNode() { clean(left, right); }

	/*void insert(TreeNode<T> *newNode)
	{
	if (newNode)
	{
	if (newNode->data < data)
	{
	if (left) left->insert(newNode);
	else left = newNode;
	}
	else
	{
	if (right) right->insert(newNode);
	else right = newNode;
	}
	}
	return;
	}*/

	int getHeight() { return height; }
	int getLeftH() { return leftH; }
	int getRightH() { return rightH; }
	BalancedFactor getBlFt() { return b; }
	// update after rotation or insertion
	void updateH() // 1: height changed, 0: height non-changed
	{
		if (left) leftH = left->getHeight(); else leftH = -1;
		if (right) rightH = right->getHeight(); else rightH = -1;
		//int tmp{ height };
		height = (leftH < rightH) ? rightH + 1 : leftH + 1;
		b = static_cast<BalancedFactor>(leftH - rightH);
		//if (height != tmp) return 1; else return 0;
	}
	void clean() { clean(left, right); key = nullptr; data = nullptr; height = 0; leftH = rightH = -1; return; } // erase all children
	TreeNode<T, U>* getSuccessorParent() // return the parent node of the succcessor node
	{
		TreeNode<T, U> *pTmp{ right };
		if (pTmp)
		{
			if (pTmp->left)
				while (true)
				{
					if (!pTmp->left->left) break;
					pTmp = pTmp->left;
				}
			else return this;
		}
		return pTmp;
	}
	bool isBalanced()
	{
		return isBalanced(this);
	}

	/*TreeNode<T>* getPredecessor()
	{
	TreeNode<T> *pTmp{ left };
	if (pTmp)
	while (pTmp->right) pTmp = pTmp->right;
	return pTmp;
	}*/

private:
	//******* private attributes ********
	int height{ 0 };
	int leftH{ -1 }, rightH{ -1 };
	BalancedFactor b{ B_EH };
protected:
	//******* some hiden methods *******
	void clean(TreeNode<T, U> *&pL, TreeNode<T, U> *&pR) // erase all children
	{
		if (pL) pL->clean(pL->left, pL->right);
		delete pL;
		if (pR) pR->clean(pR->left, pR->right);
		delete pR;
		pL = pR = nullptr;
		return;
	}
	bool isBalanced(TreeNode<T, U> *pR)
	{
		if (!pR)
		{
			//cerr << "\nNo node to check!\n";
			return 1;
		}
		if (pR->getBlFt() >= B_RH && pR->getBlFt() <= B_LH)
			return isBalanced(pR->left) && isBalanced(pR->right);
		return 0;
	}
};

//******************
struct GData
{
	double minLong, maxLong, minLat, maxLat;
	GData(double miLo, double maLo, double miLa, double maLa): minLong{ miLo }, maxLong{ maLo }, minLat{ miLa }, maxLat{ maLa }
	{}
};
//#####################################
typedef void(*pFvoid_0)();

//using pFvoid_0 = void(*)();
//template <class T, class U>
//using pFvoid_1 = void(*)(T&);

template <class T, class U>
class AVLTree
{
	//***** attributes
	TreeNode<T, U> *root{ nullptr };
	size_t size{ 0 }; // number of nodes
protected:
	//###****** some hiden methods *******####
	TreeNode<T, U>*& getRefRoot() { return root; }
	void keepBalanced(TreeNode<T, U> *&pR) 
	{
		if (!pR)
		{
			cerr << "\nNot a node to be checked if it is balanced!\n";
			return;
		}
		switch (pR->getBlFt())
		{
		case B_EH:
		case B_LH:
		case B_RH:
			return;
		case UB_RH:
			switch (pR->right->getBlFt())
			{
			case B_RH:
			case UB_RH: 
			case B_EH: // when deleting a node on the left subtree decreases that subtree's height
				rotLeft(pR);
				return;
			case B_LH:
			case UB_LH:
				rotRL(pR);
				return;
			default:
				return;
			}
		case UB_LH:
			switch (pR->left->getBlFt())
			{
			case B_RH:
			case UB_RH:
				rotLR(pR);
				return;
			case B_LH:
			case UB_LH:
			case B_EH: // when deleting a node on the right subtree decreases that subtree's height
				rotRight(pR);
				return;
			default:
				return;
			}
		default:
			return;
		}
	}
	
	//*** Insert part *******

	// assuming newNode->left = newNode->right = nullptr, newNode->getHeight() = 0, newNode->key && newNode->data != nullptr
	// hiden insert method for recursively implementing
	void insert(TreeNode<T, U> *&pR, TreeNode<T, U> *newNode, bool &needChecked) 
	{
		if (!pR)
		{
			pR = newNode;
			size++;
			needChecked = 1;
		}
		else
		{
			if (*(newNode->key) < *(pR->key))
				insert(pR->left, newNode, needChecked);
			else
				insert(pR->right, newNode, needChecked);
			if (needChecked)
			{
				int tmpH{ pR->getHeight() };
				pR->updateH();
				keepBalanced(pR);
				if (pR->getHeight() == tmpH) needChecked = 0;
			}
		}
		return;
	}

	void insert(TreeNode<VM_Record, time_t> *newNode, VM_Record *d) {}
	//**** End insert part ******

	//******* Remove part ***********

	// find and replace a node by its successor if exist in the right subtree
	// a support function for function remove()
	bool findAndReplaceBySuccessor(TreeNode<T, U> *&pRmv, TreeNode<T, U> *&node, bool &needCheked) 
	{
		if (!node) return 0;
		if (node->left)
		{
			findAndReplaceBySuccessor(pRmv, node->left, needCheked);
			if (needCheked)
			{
				int tmpH{ node->getHeight() };
				node->updateH();
				keepBalanced(node);
				if (node->getHeight() == tmpH) needCheked = 0;
			}
		}
		else
		{
			pRmv->key = node->key;
			pRmv->data = node->data;
			TreeNode<T, U> *pTmp{ node };
			node = node->right; // update
			pTmp->right = nullptr; // if noit clean() will erase this
			delete pTmp;
			size--;
			needCheked = 1; // to check and fix balanced
			// node pRmv will be check in function remove()

			/*if (pRmv->right == node)
			{
				int tmpH{ pRmv->getHeight() };
				pRmv->updateH();
				keepBalanced(pRmv);
				if (pRmv->getHeight() == tmpH) needCheked = 0;
				else needCheked = 1;
			}*/
		}
		return 1;
	}
	// remove a node whose key is known
	// assuming current != nullptr
	// true: a node has been removed; false: no node has been removed
	bool remove(TreeNode<T, U> *&current, const U &d, bool &needChecked)
	{
		//if (!current) return false;
		if (d < *(current->key))
		{
			if (current->left)
			{
				if(remove(current->left, d, needChecked))
				{
					if (needChecked)
					{
						int tmpH{ current->getHeight() };
						current->updateH();
						keepBalanced(current);
						if (current->getHeight() == tmpH) needChecked = 0;
					}
					return 1;
				}
			}
		}
		else
		{
			if (d > *(current->key))
			{
				if (current->right)
				{
					if (remove(current->right, d, needChecked))
					{
						if (needChecked)
						{
							int tmpH{ current->getHeight() };
							current->updateH();
							keepBalanced(current);
							if (current->getHeight() == tmpH) needChecked = 0;
						}
						return 1;
					}
				}
			}
			else
			{
				if (!findAndReplaceBySuccessor(current, current->right, needChecked)) // removed node doesn't have right child
				{
					TreeNode<T, U> *p{ current };
					current = current->left;
					p->left = nullptr; // if not clean()  will erase this
					delete p;
					size--;
					needChecked = 1;
				}
				if (needChecked && current != nullptr)
				{
					int tmpH{ current->getHeight() };
					current->updateH();
					keepBalanced(current);
					if (current->getHeight() == tmpH) needChecked = 0;
				}
				return 1;
			}
		}
		return 0;
	}
	//********** End remove part ****************

	void printInOrdered(TreeNode<T, U> *pR)
	{
		if (pR->left) printInOrdered(pR->left);
		cout << *(pR->data) << " ";
		if (pR->right) printInOrdered(pR->right);
		return;
	}

	//****** Rotation part **********

	bool rotLeft(TreeNode<T, U> *&pR)
	{
		if (!pR || !pR->right)
		{
			cerr << "\nNot enoungh nodes to rotate!\n";
			return 0;
		}
		TreeNode<T, U> *pTmp{ pR->right };
		pR->right = pTmp->left;
		pR->updateH();
		pTmp->left = pR;
		pTmp->updateH();
		pR = pTmp;
		return 1;
	}
	bool rotRight(TreeNode<T, U> *&pR)
	{
		if (!pR || !pR->left)
		{
			cerr << "\nNot enoungh nodes to rotate!\n";
			return 0;
		}
		TreeNode<T, U> *pTmp{ pR->left };
		pR->left = pTmp->right;
		pR->updateH();
		pTmp->right = pR;
		pTmp->updateH();
		pR = pTmp;
		return 1;
	}
	bool rotLR(TreeNode<T, U> *&pR) // left right
	{
		if (pR && pR->left)
			if (rotLeft(pR->left))
				return rotRight(pR);
		return 0;
	}
	bool rotRL(TreeNode<T, U> *&pR)
	{
		if (pR && pR->right)
			if (rotRight(pR->right))
				return rotLeft(pR);
		return 0;
	}
	//***** End rotation part **********

	//***** Search part ************
	
	TreeNode<T, U>* search(TreeNode<T, U> *pR, const U &d) // search by key
	{
		if (!pR) { /*cout << "\nNot found!\n";*/ return nullptr; }
		if (d < *(pR->key))
			return search(pR->left, d);
		if (d > *(pR->key))
			return search(pR->right, d);
		if (d == *(pR->key)) return pR;
	}
	// search by key and return parent of the found node
	TreeNode<T, U>* getParent(TreeNode<T, U> *currParent, const U &d) 
	{
		if (!currParent) { /*cerr << "\nCannot search!\n";*/ return nullptr; }
		if (d < *(currParent->key))
		{
			if (currParent->left)
			{
				if (d == *(currParent->left->key)) return currParent;
				return getParent(currParent->left, d);
			}
		}
		else
		{
			if (d > *(currParent->key))
			{
				if (currParent->right)
				{
					if (d == *(currParent->right->key)) return currParent;
					return getParent(currParent->right, d);
				}
			}
		}
		return nullptr; // in case d == currParent->key or currParent->left or currParent->right is nullptr
	}
	//****** End search part ************

	//****** Traverse part ************
	void traverseLNR(TreeNode<T, U> *&pR, void(*ptrF)(T&))
	{
		if (!pR) return;
		traverseLNR(pR->left, ptrF);
		ptrF(*(pR->data));
		traverseLNR(pR->right, ptrF);
	}
	// for request 2
	size_t traverseLNR(TreeNode<T, U> *pR, const double &a, bool(*op)(TreeNode<T, U>*, const double&)){}
	// for request 4, ...
	bool traverseInAKeyValueInterval(TreeNode<T, U> *pR, const U &t1, const U &t2, const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<T, U>*, const double &, const double &, const double &), bool &isOutOfBound){}
	size_t traverse_UnderControlled(TreeNode<T, U> *pR, const double &Along, const double &Alat, const double &R, const time_t &t1, const time_t &t2, bool(*op)(TreeNode<VM_Record, time_t>*, const double &, const double &, const double &)) {}
	// for request 6
	void traverseInAKeyValueInterval(TreeNode<T, U> *pR, const double &Along, const double &Alat, const time_t &t1, const time_t &t2, bool &is2k, bool &is500, bool &is300, bool &isOutOfBound){}
	void traverseLNR_Overload(TreeNode<T, U> *pR, const double &Along, const double &Alat, const time_t &t, L1List<string> *l2000, L1List<string> *l500, int &l300, L1List<string> *lOnly2k){}
	// for request 7
	void traverseInAKeyValueInterval(TreeNode<T, U> *pR, const double &Along, const double &Alat, const time_t &t1, const time_t &t2, double &, bool &is2k, bool &is1k, bool &is500, bool &isOutOfBound) {}
	// for request 8, 9
	void traverseLNR_Remove(TreeNode<T, U> *&pR, const double &Along, const double &Alat, const double &R, const time_t &t, L1List<TreeNode<T, U>*> *rmvList){}
	// for request 5
	size_t traverseLNR(TreeNode<T, U> *pR, const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<T, U>*, const double &, const double &, const double &), bool &){}
	// for request 7
	void traverseRNL(TreeNode<T, U> *pR, AVLTree<U, T> *tree1, AVLTree<U, T> *tree2, const int &M, int &N)
	{	
		if (!pR) return;
		traverseRNL(pR->right, tree1, tree2, M, N);
		N++;
		if (N <= 0.75*M)
		{
			TreeNode<U, T> *newNode{ new TreeNode<U, T>(pR->key, pR->data) };
			tree1->insert(newNode);
		}
		else
		{
			TreeNode<U, T> *newNode{ new TreeNode<U, T>(pR->key, pR->data) };
			tree2->insert(newNode);
		}
		traverseRNL(pR->left, tree1, tree2, M, N);
	}
	void traverseLNR_Jam(TreeNode<T, U> *pR, const double &Along, const double &Alat, const time_t &t, AVLTree<string, double> *, L1List<double> *, int &l500, L1List<string> *l2000, L1List<string> *lOnly1000) {}
	void traverseLNR(TreeNode<T, U> *pR, void(*op)(U*), int &N)
	{
		if (!pR) return;
		traverseLNR(pR->left, op, N );
		N++;
		op(pR->key);
		if (N < size) cout << ' ';
		traverseLNR(pR->right, op, N);
	}
	//****** End traverse part ********
public:
	// should be manual deleted
	void *pGData{ nullptr };
	AVLTree() {}
	AVLTree(T *data, U *key) : root{ new TreeNode<T, U>(data, key) }, size{ 1 } {}
	//AVLTree(TreeNode<T, U> *newNode) {if (!newNode->left && !newNode->right) }
	//AVLTree(AVLTree<T> *r) { root = r->root; size = r->getSize(); }
	~AVLTree() { if (root) root->clean(); delete root; }
	//void create(size_t size) {} // randomly create a AVL tree

	//void renew() // clean all the subtree and keep the root for re generate a new tree
	//{
	//	root->clean();
	//	size = 0;
	//	return;
	//}

	size_t getSize() { return size; }
	TreeNode<T, U>* getRoot() { return root; }

	/*void copyTree(AVLTree<T, U> *r) { root = r->root; size = r->getSize(); return; }*/

	//void makeBalanced(TreeNode<T, U> *&pR) {}

	//******** Search a node by its key ********//
	TreeNode<T, U>* search(const U &k) // return the first found node
	{
		return search(root, k);
	}
	// should better check the key with the root's key before use this function
	TreeNode<T, U>* getParent(const U &k)
	{
		return getParent(root, k);
	}
	//******** Insert a given node **********//
	void insert(TreeNode<T, U> *newNode)
	{
		if (!newNode) { /*cerr << "\nNo new node to be inserted or the tree is empty!\n";*/ return; }
		if (!newNode->left && !newNode->right) // just permit to insert an external node to guarantee the balance
		{
			bool b;
			insert(root, newNode, b);
		}
		//else cerr << "\nThe node inserted mustn't have any child!\n";
		return;
	}
	//******** Create and insert a new node with given key and data to the tree ********
	void insert(T *d, U *k)
	{
		bool b;
		insert(root, new TreeNode<T, U>(d, k), b);
		return;
	}
	//****** insert function for AVLTree< AVLTree<T, U>, V> ********
	void insert(VM_Record *d){}
	//******** Remove a given node *********//
	bool remove(TreeNode<T, U> *rmvNode)
	{
		if (!rmvNode || !root) { /*cerr << "\nInput node is null!\n";*/ return 0; }
		/*if (search(rmvNode->data))
		{*/
		bool b;
		
		/*}*/
		//else cerr << "\nThis node isn't exist in the tree!\n";
		return remove(root, rmvNode->data, b);
	}
	//******* Remove a node having a given key *********
	bool remove(const U &k)
	{
		if (!root) return 0;
		bool b;
		//if (!) 
		//{
		//	/*cerr << "\nThis node isn't exist in the tree!\n";*/ return 0;
		//}
		return remove(root, k, b);
	}

	//******* Traverse **************

	// find the node which has minimum key larger than or equal the given one
	// assuming all the nodes have distinct keys
	TreeNode<T, U>* findSuccessorOfKeyValue(const U &k)
	{
		TreeNode<T, U> *pTmp{ root };
		TreeNode<T, U> *currSucc{ nullptr };
		while (pTmp)
		{
			if (k < *(pTmp->key))
			{
				currSucc = pTmp;
				pTmp = pTmp->left;
			}
			else // k >= *(pTmp->key)
			{
				if (k == *(pTmp->key))
					return pTmp;
				pTmp = pTmp->right;
			}
		}
		return currSucc;
	}

	void findNodesWithinAnInterval()
	{

	}

	// for request 4, ...
	bool traverseInAKeyValueInterval(const U &t1, const U &t2, const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<T, U>*, const double &, const double &, const double &)){}
	
	void traverseLNR(void(*op)(T&)) { traverseLNR(root, op); }
	// for request 2
	size_t traverseLNR(const double &a, bool(*op)(TreeNode<T, U>*, const double&)){}
	// for request 4, ...
	size_t traverse_UnderControlled(const double &Along, const double &Alat, const double &R, const time_t &t1, const time_t &t2, bool(*op)(TreeNode<VM_Record, time_t>*, const double &, const double &, const double &)){}

	// for request 5
	size_t traverseLNR(const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<VM_Record, time_t>*, const double &, const double &, const double &)){}

	// for request 8, 9
	void traverseLNR_Remove(const double &Along, const double &Alat, const double &R, const time_t &t, L1List<TreeNode<T, U>*> *l){}

	// for request 6
	void traverseInAKeyValueInterval(const double &Along, const double &Alat, const time_t &t, bool &is2k, bool &is500, bool &is300){}
	void traverseLNR_Overload(const double &Along, const double &Alat, const time_t &t, L1List<string> *l2000, L1List<string> *l500, int &l300, L1List<string> *lOnly2k){}
	
	// for request 7
	void traverseLNR_Jam(const double &Along, const double &Alat, const time_t &t, AVLTree<string, double> *, L1List<double> *, int &l500, L1List<string> *l2000, L1List<string> *lOnly1000){}
	void traverseInAKeyValueInterval(const double &Along, const double &Alat, const time_t &t, double &, bool &is2k, bool &is1k, bool &is500){}
	void traverseRNL(AVLTree<U, T> *tree1, AVLTree<U, T> *tree2, const int &M)
	{
		int N{ 0 };
		traverseRNL(root, tree1, tree2, M, N);
	}
	void traverseLNR(void(*op)(U*))
	{
		int N{ 0 };
		traverseLNR(root, op, N);
	}

	void printInOrdered()
	{
		if (!root) return;
		cout << '\n';
		printInOrdered(root);
		return;
	}
	
};

//#define AVL_VmR_Time   AVLTree<VM_Record, time_t>
//#define Node_VmR_Time  TreeNode<VM_Record, time_t>
//#define AVL_AVL        AVLTree< AVLTree<VM_Record, time_t>, string>
//#define Node_AVL_str   TreeNode<AVLTree<VM_Record, time_t>, string>

using AVL_VmR_Time  = AVLTree<VM_Record, time_t>;
using Node_VmR_Time = TreeNode<VM_Record, time_t>;
using AVL_AVL       = AVLTree< AVLTree<VM_Record, time_t>, string>;
using Node_AVL_str  = TreeNode<AVLTree<VM_Record, time_t>, string>;



//########################################################################################
#endif //A02_DSALIB_H
