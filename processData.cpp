/*
 * ==========================================================================================
 * Name        : processData.cpp
 * Description : student code for Assignment 2 - Data structures and Algorithms - Fall 2017
 * ==========================================================================================
 */
#include <iostream>
#include <vector>
#include <functional>
#include <math.h>
#include "requestLib.h"
#include "dbLib.h"

using namespace std; 

#define GPS_DISTANCE_ERROR 0.005

bool initVMGlobalData(void** pGData) {
    // TODO: allocate and initialize global data
    // return false if failed
    return true;
}

void releaseVMGlobalData(void* pGData) {
    // TODO: do your cleanup, left this empty if you don't have any dynamically allocated data

}

bool isUnderControlled(TreeNode<VM_Record, time_t> *pN, const double &Along, const double &Alat, const double &R)
{
	if (distanceEarth(pN->data->longitude, pN->data->latitude, Along, Alat) <= R) return 1;
	return 0;
}

//################################## template specialization #####################################################
template<>
TreeNode<VM_Record, time_t>::TreeNode(VM_Record *d, TreeNode<VM_Record, time_t> *pL, TreeNode<VM_Record, time_t> *pR) :data{ d }, left{ pL }, right{ pR }
{
	key = &(data->timestamp);
	if (left) leftH = left->getHeight();
	if (right) rightH = right->getHeight();
	height = (leftH < rightH) ? rightH + 1 : leftH + 1;
	b = static_cast<BalancedFactor>(leftH - rightH);
}

//********* AVL ***********

// assuming newNode is an external node
//template<>
//void AVLTree< AVLTree<VM_Record, time_t>, string>::insert(TreeNode<VM_Record, time_t> *newNode, VM_Record *d)
//{
//	string str{ (d->id) };
//	Node_AVL_str **pTmp{ &root };
//	//TreeNode< AVLTree<VM_Record, time_t>, string > **p1{ &root };
//	GData *pGtmp;
//	L1List< TreeNode<AVLTree<VM_Record, time_t>, string>** > lPath;
//	while (*pTmp)
//	{
//		if (str == *((*pTmp)->key)) // there exists a avl child with the same key
//		{
//			pGtmp = static_cast<GData*>((*pTmp)->data->pGData);
//			if (d->longitude < pGtmp->minLong) // find minLong
//			{
//				pGtmp->minLong = d->longitude;
//			}
//			else
//			{
//				if (d->longitude > pGtmp->maxLong) // find maxLong
//					pGtmp->maxLong = d->longitude;
//			}
//			if (d->latitude < pGtmp->minLat) // find minLat
//			{
//				pGtmp->minLat = d->latitude;
//			}
//			else
//			{
//				if (d->latitude > pGtmp->maxLat) // find maxLat
//					pGtmp->maxLat = d->latitude;
//			}
//			(*pTmp)->data->insert(newNode);
//			return;
//		}
//		else
//		{
//			lPath.insertHead(pTmp);
//			if (str < *((*pTmp)->key))
//			{
//				pTmp = &((*pTmp)->left);
//			}
//			else // str > *(pTmp->key)
//			{
//				pTmp = &((*pTmp)->right);
//			}
//		}
//	}
//	// create new Node_AVL_str
//	AVL_VmR_Time *p{ new AVL_VmR_Time() };
//	p->pGData = new GData(d->longitude, d->longitude, d->latitude, d->latitude);
//	p->insert(newNode);
//	*pTmp = new Node_AVL_str(p, &(d->id));
//	size++;
//	// fix balanced
//	L1Item< Node_AVL_str ** > *pt{ lPath.getHead() };
//	int tmpH;
//	while (pt) // null when *pTmp == root or all item in lPath have been removed
//	{
//		pTmp = pt->data;
//		tmpH = (*pTmp)->getHeight();
//		(*pTmp)->updateH();
//		keepBalanced(*pTmp);
//		if ((*pTmp)->getHeight() == tmpH) return; // check if it needs to fix balanced
//		lPath.removeHead();
//		pt = lPath.getHead();
//	}
//}

//template<>
//void AVL_AVL::insert(VM_Record *d)
//{
//	//TreeNode<VM_Record, time_t> *newNode{ new TreeNode<VM_Record, time_t>(d, &(d->timestamp)) };
//	insert(new TreeNode<VM_Record, time_t>(d, &(d->timestamp)), d);
//}

//***** traverse function for request 2 & 3 **********
template<>
size_t AVL_AVL::traverseLNR(Node_AVL_str *pR, const double &a, bool(*op)(Node_AVL_str*, const double&))
{
	if (!pR) return 0;
	if (op(pR, a)) return 1 + traverseLNR(pR->left, a, op) + traverseLNR(pR->right, a, op);
	else return traverseLNR(pR->left, a, op) + traverseLNR(pR->right, a, op);
}

template<>
size_t AVL_AVL::traverseLNR(const double &a, bool(*op)(Node_AVL_str*, const double&))
{
	return traverseLNR(root, a, op);
}
//**** end of function for request 2 & 3 **************

//****** for request 4, ... **********************
// t1 <= t2, isOutOfBound must be initialized with false value
template<>
bool AVL_VmR_Time::traverseInAKeyValueInterval(TreeNode<VM_Record, time_t> *pR, const time_t &t1, const time_t &t2, const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<VM_Record, time_t>*, const double &, const double &, const double &), bool &isOutOfBound)
{
	if (!pR) return 0;
	//if (t1 > t2) { isOutOfBound = 1; return 0; }
	if (t1 < *(pR->key))
	{
		if (!traverseInAKeyValueInterval(pR->left, t1, t2, Along, Alat, R, op, isOutOfBound))
		{
			if (isOutOfBound) return 0;
			else
			{
				if (t2 < *(pR->key)) { isOutOfBound = 1; return 0; }
				// else t2 >= *(pR->key)
				if (op(pR, Along, Alat, R)) return 1;
				else
				{
					if (t2 == *(pR->key)) { isOutOfBound = 1; return 0; }
					// *(pR->key) < t2
					return traverseInAKeyValueInterval(pR->right, *(pR->key), t2, Along, Alat, R, op, isOutOfBound);
				}
			}
		}
		else return 1;
	}
	else
	{
		if (t1 == *(pR->key))
		{
			return (op(pR, Along, Alat, R))? 1 : traverseInAKeyValueInterval(pR->right, t1, t2, Along, Alat, R, op, isOutOfBound);
		}
		else // t1 > *(pR->key)
		{
			return traverseInAKeyValueInterval(pR->right, t1, t2, Along, Alat, R, op, isOutOfBound);
		}
	}
}
template<>
bool AVL_VmR_Time::traverseInAKeyValueInterval(const time_t &t1, const time_t &t2, const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<VM_Record, time_t>*, const double &, const double &, const double &))
{
	if (t1 > t2) return false;
	bool b{ false };
	return traverseInAKeyValueInterval(root, t1, t2, Along, Alat, R, op, b);
}
template<>
size_t AVL_AVL::traverse_UnderControlled(Node_AVL_str *pR, const double &Along, const double &Alat, const double &R, const time_t &t1, const time_t &t2, bool(*op)(Node_VmR_Time*, const double &, const double &, const double &))
{
	if (!pR) return 0;
	return static_cast<size_t>(pR->data->traverseInAKeyValueInterval(t1, t2, Along, Alat, R, op)) + traverse_UnderControlled(pR->left, Along, Alat, R, t1, t2, op) + traverse_UnderControlled(pR->right, Along, Alat, R, t1, t2, op);
}
template<>
size_t AVL_AVL::traverse_UnderControlled(const double &Along, const double &Alat, const double &R, const time_t &t1, const time_t &t2, bool(*op)(Node_VmR_Time*, const double &, const double &, const double &))
{
	return traverse_UnderControlled(root, Along, Alat, R, t1, t2, op);
}

//******* for request 6 *************

// is2k = is500 = is300 = false, isOutOfBound = false
template<>
void AVL_VmR_Time::traverseInAKeyValueInterval(Node_VmR_Time *pR, const double &Along, const double &Alat, const time_t &t1, const time_t &t2, bool &is2k, bool &is500, bool &is300, bool &isOutOfBound)
{
	if (!pR) return;
	if (t1 < *(pR->key))
	{
		traverseInAKeyValueInterval(pR->left, Along, Alat, t1, t2, is2k, is500, is300, isOutOfBound);
		if ( isOutOfBound ) return;
		else
		{
			if (t2 < *(pR->key)) { isOutOfBound = 1; return; }
			// else t2 >= *(pR->key)
			if (is300) return;
			double d{ distanceEarth(pR->data->longitude, pR->data->latitude, Along, Alat) };
			if (!is2k)
			{
				if (d <= 0.3)
				{
					is2k = is500 = is300 = 1;
				}
				else if (d < 0.5)
				{
					is2k = is500 = 1;
				}
				else if (d <= 2)
				{
					is2k = 1;
				}
			}
			else // if is2k
			{
				if (d <= 0.3)
				{
					is500 = is300 = 1;
				}
				else if (d < 0.5)
				{
					is500 = 1;
				}
			}
			if (is300) return;
			if (t2 == *(pR->key)) { isOutOfBound = 1; return; }
			// t2 > *(pR->key)
			traverseInAKeyValueInterval(pR->right, Along, Alat, *(pR->key), t2, is2k, is500, is300, isOutOfBound);
		}
	}
	else
	{
		if (t1 == *(pR->key))
		{
			double d{ distanceEarth(pR->data->longitude, pR->data->latitude, Along, Alat) };
			if (!is2k)
			{
				if (d <= 0.3)
				{
					is2k = is500 = is300 = 1;
				}
				else if (d < 0.5)
				{
					is2k = is500 = 1;
				}
				else if (d <= 2)
				{
					is2k = 1;
				}
			}
			else // if is2k
			{
				if (d <= 0.3)
				{
					is500 = is300 = 1;
				}
				else if (d < 0.5)
				{
					is500 = 1;
				}
			}
			traverseInAKeyValueInterval(pR->right, Along, Alat, t1, t2, is2k, is500, is300, isOutOfBound);
		}
		else // t1 > *(pR->key)
		{
			traverseInAKeyValueInterval(pR->right, Along, Alat, t1, t2, is2k, is500, is300, isOutOfBound);
		}
	}
}
template<>
void AVL_VmR_Time::traverseInAKeyValueInterval(const double &Along, const double &Alat, const time_t &t, bool &is2k, bool &is500, bool &is300)
{
	bool b{ false };
	is2k = is500 = is300 = false;
	traverseInAKeyValueInterval(root, Along, Alat, t - 900, t, is2k, is500, is300, b);
}
template<>
void AVL_AVL::traverseLNR_Overload(Node_AVL_str *pR, const double &Along, const double &Alat, const time_t &t, L1List<string> *l2000, L1List<string> *l500, int &l300, L1List<string> *lOnly2k)
{
	if (!pR) return;
	traverseLNR_Overload(pR->left, Along, Alat, t, l2000, l500, l300, lOnly2k);
	bool is2k, is500, is300;
	pR->data->traverseInAKeyValueInterval(Along, Alat, t, is2k, is500, is300);
	if (is2k) l2000->push_back(*(pR->key));
	if (is2k && !is500) lOnly2k->push_back(*(pR->key));
	if (is500) l500->push_back(*(pR->key));
	if (is300) l300++;
	traverseLNR_Overload(pR->right, Along, Alat, t, l2000, l500, l300, lOnly2k);
}
template<>
void AVL_AVL::traverseLNR_Overload(const double &Along, const double &Alat, const time_t &t, L1List<string> *l2000, L1List<string> *l500, int &l300, L1List<string> *lOnly2k)
{
	l300 = 0;
	traverseLNR_Overload(root, Along, Alat, t, l2000, l500, l300, lOnly2k);
}

//***** for request 7 ***********
template<>
void AVL_VmR_Time::traverseInAKeyValueInterval(Node_VmR_Time *pR, const double &Along, const double &Alat, const time_t &t1, const time_t &t2, double &max, bool &is2k, bool &is1k, bool &is500, bool &isOutOfBound)
{
	if (!pR) return;
	if (t1 < *(pR->key))
	{
		traverseInAKeyValueInterval(pR->left, Along, Alat, t1, t2, max, is2k, is1k, is500, isOutOfBound);
		if (isOutOfBound) return;
		else
		{
			if (t2 < *(pR->key)) { isOutOfBound = 1; return; }
			// else t2 >= *(pR->key)
			if (is500) return;
			double d{ distanceEarth(pR->data->longitude, pR->data->latitude, Along, Alat) };
			if (d <= 0.5)
			{
				is1k = is500 = 1; is2k = 0;
			}
			else if (d < 1)
			{
				is1k = 1; is2k = 0;
			}
			else if (d <= 2)
			{
				if (!is1k)
				{
					if (is2k)
					{
						if (d > max) max = d;
					}
					else
					{
						is2k = 1;
						max = d;
					}
				}
			}
			if (is500) return;
			if (t2 == *(pR->key)) { isOutOfBound = 1; return; }
			// t2 > *(pR->key)
			traverseInAKeyValueInterval(pR->right, Along, Alat, *(pR->key), t2, max, is2k, is1k, is500, isOutOfBound);
		}
	}
	else
	{
		if (t1 == *(pR->key))
		{
			double d{ distanceEarth(pR->data->longitude, pR->data->latitude, Along, Alat) };
			if (d <= 0.5)
			{
				is1k = is500 = 1; is2k = 0;
			}
			else if (d < 1)
			{
				is1k = 1; is2k = 0;
			}
			else if (d <= 2)
			{
				if (!is1k)
				{
					if (is2k)
					{
						if (d > max) max = d;
					}
					else
					{
						is2k = 1;
						max = d;
					}
				}
			}
			if (is500) return;
			traverseInAKeyValueInterval(pR->right, Along, Alat, t1, t2, max, is2k, is1k, is500, isOutOfBound);
		}
		else // t1 > *(pR->key)
		{
			traverseInAKeyValueInterval(pR->right, Along, Alat, t1, t2, max, is2k, is1k, is500, isOutOfBound);
		}
	}
}
template<>
void AVL_VmR_Time::traverseInAKeyValueInterval(const double &Along, const double &Alat, const time_t &t, double &max, bool &is2k, bool &is1k, bool &is500)
{
	bool b{ false }; is2k = is1k = is500 = false;
	traverseInAKeyValueInterval(root, Along, Alat, t, t + 1800, max, is2k, is1k, is500, b);
}
template<>
void AVL_AVL::traverseLNR_Jam(Node_AVL_str *pR, const double &Along, const double &Alat, const time_t &t, AVLTree<string, double> *Tree12, L1List<double> *dataL12, int &l500, L1List<string> *l2000, L1List<string> *lOnly1000) 
{
	if (!pR) return;
	traverseLNR_Jam(pR->left, Along, Alat, t, Tree12, dataL12, l500, l2000, lOnly1000);
	bool is2k, is1k, is500; double max;
	pR->data->traverseInAKeyValueInterval(Along, Alat, t, max, is2k, is1k, is500);
	if (is500) l500++;
	if (is2k)
	{
		l2000->push_back(*(pR->key));
		dataL12->insertHead(max);
		Tree12->insert(pR->key, &(dataL12->getHead()->data));
		/*if (!is1k)
		{
			
		}*/
	}
	else
	{
		if (is1k)
		{
			lOnly1000->push_back(*(pR->key));
			l2000->push_back(*(pR->key));
		}
	}
	traverseLNR_Jam(pR->right, Along, Alat, t, Tree12, dataL12, l500, l2000, lOnly1000);
}
template<>
void AVL_AVL::traverseLNR_Jam(const double &Along, const double &Alat, const time_t &t, AVLTree<string, double> *Tree12, L1List<double> *dataL12, int &l500, L1List<string> *l2000, L1List<string> *lOnly1000) 
{
	l500 = 0;
	traverseLNR_Jam(root, Along, Alat, t, Tree12, dataL12, l500, l2000, lOnly1000);
}

//***** for request 8, 9 *************
// assuming rmvList != null
template<>
void AVL_AVL::traverseLNR_Remove(Node_AVL_str *&pR, const double &Along, const double &Alat, const double &R, const time_t &t, L1List<Node_AVL_str*> *rmvList)
{
	if (!pR) return;
	traverseLNR_Remove(pR->left, Along, Alat, R, t, rmvList);
	if (pR->data->traverseInAKeyValueInterval(t, t + 59, Along, Alat, R, isUnderControlled))
	{
		Node_AVL_str * newNode{ new Node_AVL_str(pR) };
		rmvList->push_back(newNode);
	}
	traverseLNR_Remove(pR->right, Along, Alat, R, t, rmvList);
	if (pR == root) // assuming root != null
	{
		L1Item<Node_AVL_str*> *pTmp{ rmvList->getHead() }; bool b;
		while (pTmp)
		{
			remove(root, *(pTmp->data->key), b);

			pTmp = pTmp->pNext;
		}
	}
	return;
}
template<>
void AVL_AVL::traverseLNR_Remove(const double &Along, const double &Alat, const double &R, const time_t &t, L1List<Node_AVL_str*> *rmvL)
{
	traverseLNR_Remove(root, Along, Alat, R, t, rmvL);
}

//****** for request 5 *************

// isUnderControlled must be initialized with false value
template<>
size_t AVL_VmR_Time::traverseLNR(TreeNode<VM_Record, time_t> *pR, const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<VM_Record, time_t>*, const double &, const double &, const double &), bool &isUnderControlled) 
{
	if (!pR) return 0;
	size_t l{ traverseLNR(pR->left, Along, Alat, R, op, isUnderControlled) };
	if (op(pR, Along, Alat, R))
	{
		if (isUnderControlled)
		{
			return l + traverseLNR(pR->right, Along, Alat, R, op, isUnderControlled);
		}
		else
		{
			isUnderControlled = 1;
			return l + 1 + traverseLNR(pR->right, Along, Alat, R, op, isUnderControlled);
		}
	}
	else
	{
		isUnderControlled = 0;
		return l + traverseLNR(pR->right, Along, Alat, R, op, isUnderControlled);
	}
}
template<>
size_t AVL_VmR_Time::traverseLNR(const double &Along, const double &Alat, const double &R, bool(*op)(TreeNode<VM_Record, time_t>*, const double &, const double &, const double &))
{
	bool b{ false };
	return traverseLNR(root, Along, Alat, R, op, b);
}
//*******************************************************************************************************

extern AVL_AVL dbAVLTree;

//***** Request 1 **********************
bool checkValidRequest1(string &code, string &x, string &y, time_t &t)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() == '_') return 0;
	string tmp{ code };
	stringstream buf{ code };
	int i{ 0 };
	while (i != 3)
	{
		switch (i++)
		{
		case 0:
			getline(buf, code, '_');
			break;
		case 1:
			getline(buf, x, '_');
			break;
		case 2:
			getline(buf, y, '_');
			break;

		/*case 3:
			getline(buf, tmp, '_');
			continue;
		default:
			return 0;*/

		}
		if (buf.eof()) return 0;
	}
	tmp = tmp.substr(tmp.find_last_of('_') + 1, 6);
	tmp.resize(6, '\0');
	tm tM; tM.tm_mday = 7; tM.tm_mon = 6; tM.tm_year = 70;

	if (isdigit(tmp[0]))
	{
		if (isdigit(tmp[1])) 
		{
			tM.tm_hour = stoi(tmp.substr(0, 2)); if (tM.tm_hour > 23) tM.tm_hour -= 24;
		}
		else
		{
			tM.tm_hour = stoi(tmp.substr(0, 1));
			tM.tm_min = 0;
			tM.tm_sec = 0;
			t = mktime(&tM);
			return 1;
		}
		if (isdigit(tmp[2]))
		{
			if (isdigit(tmp[3]))
			{
				tM.tm_min = stoi(tmp.substr(2, 2));
				if (tM.tm_min > 59) tM.tm_min -= 60;
			}
			else
			{
				tM.tm_min = stoi(tmp.substr(2, 1));
				tM.tm_sec = 0;
				t = mktime(&tM);
				return 1;
			}
		}
		else
		{
			tM.tm_min = 0; tM.tm_sec = 0; t = mktime(&tM); return 1;
		}
		if (isdigit(tmp[4]))
		{
			if (isdigit(tmp[5]))
			{
				tM.tm_sec = stoi(tmp.substr(4, 2));
				if (tM.tm_sec > 59) tM.tm_sec -= 60;
			}
			else
			{
				tM.tm_sec = stoi(tmp.substr(4, 1)); t = mktime(&tM);
				return 1;
			}
		}
		else
		{
			tM.tm_sec = 0; t = mktime(&tM); return 1;
		}
	}
	else
	{
		tM.tm_hour = tM.tm_min = tM.tm_sec = 0;
		t = mktime(&tM);
		return 1;
	}

	/*try
	{
		tM.tm_hour = stoi(tmp.substr(0, 2));
		tM.tm_min = stoi(tmp.substr(2, 2));
		tM.tm_sec = stoi(tmp.substr(4, 2));
	}
	catch ()
	{

	}*/

	t = mktime(&tM);
	return 1;
}
void handleRequest1(const string &code, const string &x, const string &y, const time_t &t)
{
	Node_AVL_str *X{ dbAVLTree.search(x) };
	if (!X)
	{
		cout << code << ": -1\n";
		return;
	}
	Node_AVL_str *Y{ dbAVLTree.search(y) };
	if (!Y)
	{
		cout << code << ": -1\n";
		return;
	}
	Node_VmR_Time *xX{ X->data->search(t) };
	if (!xX)
	{
		cout << code << ": -1\n";
		return;
	}
	Node_VmR_Time *yY{ Y->data->search(t) };
	if (!yY)
	{
		cout << code << ": -1\n";
		return;
	}
	cout << code << ": ";
	if (xX->data->longitude >= yY->data->longitude) cout << "E ";
	else cout << "W ";
	if (xX->data->latitude >= yY->data->latitude) cout << "N " << distanceEarth(xX->data->longitude, xX->data->latitude, yY->data->longitude, yY->data->latitude) << '\n';
	else cout << "S " << distanceEarth(xX->data->longitude, xX->data->latitude, yY->data->longitude, yY->data->latitude) << '\n';
}

//***** Request 2 ********************
bool checkValidRequest2(string &code, double &Along, char &ch)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() != 'E')
	{
		if (code.back() != 'W') return 0;
		else ch = 'W';
	}
	else ch = 'E';
	stringstream buf{ code };
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	string tmp;
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	buf.str(""); buf.clear();
	buf << tmp;
	Along = 0;
	buf >> Along;
	return 1;
}
// check if this vehicle always travels to the East of Along
bool checkE(Node_AVL_str *pN, const double &a)
{
	if (static_cast<GData*>(pN->data->pGData)->minLong >= a) return 1;
	else return 0;
}
// check if this vehicle always travels to the West of Along
bool checkW(Node_AVL_str *pN, const double &a)
{
	if (static_cast<GData*>(pN->data->pGData)->maxLong <= a) return 1;
	else return 0;
}
void handleRequest2(const string &code, const double &Along, const char &ch)
{
	switch (ch)
	{
	case'E':
		cout << code << ": " << dbAVLTree.traverseLNR(Along, checkE) << '\n';
		return;
	case'W':
		cout << code << ": " << dbAVLTree.traverseLNR(Along, checkW) << '\n';
		return;
	default:
		return;
	}
}

//********* Request 3 **************
bool checkValidRequest3(string &code, double &Alat, char &ch)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() != 'N')
	{
		if (code.back() != 'S') return 0;
		else ch = 'S';
	}
	else ch = 'N';
	stringstream buf{ code };
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	string tmp;
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	buf.str(""); buf.clear();
	buf << tmp;
	Alat = 0;
	buf >> Alat;
	return 1;
}
// check if this vehicle always travels to the North of Alat
bool checkN(Node_AVL_str *pN, const double &a)
{
	if (static_cast<GData*>(pN->data->pGData)->minLat >= a) return 1;
	else return 0;
}
// check if this vehicle always travels to the South of Alat
bool checkS(Node_AVL_str *pN, const double &a)
{
	if (static_cast<GData*>(pN->data->pGData)->maxLat <= a) return 1;
	else return 0;
}
void handleRequest3(const string &code, const double &Alat, const char &ch)
{
	switch (ch)
	{
	case 'N':
		cout << code << ": " << dbAVLTree.traverseLNR(Alat, checkN) << '\n';
		return;
	case 'S':
		cout << code << ": " << dbAVLTree.traverseLNR(Alat, checkS) << '\n';
		return;
	default:
		return;
	}
}

//********* Request 4 **************
bool checkValidRequest4(string &code, double &Along, double &Alat, double &R, time_t &H1, time_t &H2)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() == '_') return 0;
	stringstream buf{ code };
	string tmp, tmpCode{ code };
	// code
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	// Along
	buf >> Along;
	if (buf.fail())
	{
		Along = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// Alat
	buf >> Alat;
	if (buf.fail())
	{
		Alat = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// R
	buf >> R;
	if (buf.fail())
	{
		R = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// H1
	tm t; t.tm_mday = 7; t.tm_mon = 6; t.tm_year = 70; t.tm_min = 0; t.tm_sec = 0;
	buf >> t.tm_hour;
	if (buf.fail())
	{
		t.tm_hour = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	/*if (t.tm_hour > 23)
	{
		t.tm_hour = 23; t.tm_min = 59; t.tm_sec = 59;
	}*/
	H1 = mktime(&t);
	// H2
	tmpCode = tmpCode.substr(tmpCode.find_last_of('_') + 1);
	buf.str(""); buf.clear();
	buf << tmpCode;
	tm t1; t1.tm_mday = 7; t1.tm_mon = 6; t1.tm_year = 70; t1.tm_min = 0; t1.tm_sec = 0;
	buf >> t1.tm_hour;
	if (buf.fail())
	{
		t1.tm_hour = 0; //buf.clear();
		H2 = mktime(&t1);
		return 1;
	}
	/*if (t1.tm_hour > 23)
	{
		t1.tm_hour = 23; t1.tm_min = 59; t1.tm_sec = 59;
	}*/
	H2 = mktime(&t1);
	return 1;
}
void handleRequest4(const string &code, const double &Along, const double &Alat, const double &R, const time_t &H1, const time_t &H2)
{
	cout << code << ": " << dbAVLTree.traverse_UnderControlled(Along, Alat, R, H1, H2, isUnderControlled) << '\n';
}

//******** Request 5 *****************
bool checkValidRequest5(string &code, string &X, double &Along, double &Alat, double &R)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() == '_') return 0;
	stringstream buf{ code };
	string tmp, tmpCode{ code };
	// code
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	// X
	getline(buf, X, '_');
	if (buf.eof()) return 0;
	// Along
	buf >> Along;
	if (buf.fail())
	{
		Along = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// Alat
	buf >> Alat;
	if (buf.fail())
	{
		Alat = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// R
	tmpCode = tmpCode.substr(tmpCode.find_last_of('_') + 1);
	buf.str(""); buf.clear();
	buf << tmpCode;
	buf >> R;
	if (buf.fail())
	{
		R = 0;
	}
	return 1;
}
void handleRequest5(const string &code, const string &X, const double &Along, const double &Alat, const double &R)
{
	Node_AVL_str *foundNode{ dbAVLTree.search(X) };
	if (foundNode)
		cout << code << ": " << foundNode->data->traverseLNR(Along, Alat, R, isUnderControlled) << '\n';
	else
		cout << code << ": -1\n";
}

//********* Request 6 *****************
bool checkValidRequest6(string &code, double &Along, double &Alat, int &M, time_t &t)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() == '_') return 0;
	stringstream buf{ code }; string tmp, tmpCode{ code };
	// code
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	// Along
	buf >> Along;
	if (buf.fail())
	{
		Along = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// Alat
	buf >> Alat;
	if (buf.fail())
	{
		Alat = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// M
	buf >> M;
	if (buf.fail())
	{
		M = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	if (M < 0) M = 0;
	// hhmm
	tmpCode = tmpCode.substr(tmpCode.find_last_of('_') + 1, 4);
	tmpCode.resize(4, '\0');
	tm tM; tM.tm_mday = 7; tM.tm_mon = 6; tM.tm_year = 70; tM.tm_sec = 0;
	if (isdigit(tmpCode[0]))
	{
		if (isdigit(tmpCode[1]))
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 2));
			if (tM.tm_hour > 23) tM.tm_hour -= 24;
		}
		else
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 1));
			tM.tm_min = 0;
			t = mktime(&tM);
			return 1;
		}
		if (isdigit(tmpCode[2]))
		{
			if (isdigit(tmpCode[3]))
			{
				tM.tm_min = stoi(tmpCode.substr(2, 2));
				if (tM.tm_min > 59) tM.tm_min -= 60;
			}
			else
			{
				tM.tm_min = stoi(tmpCode.substr(2, 1));
				t = mktime(&tM);
				return 1;
			}
		}
		else
		{
			tM.tm_min = 0; t = mktime(&tM); return 1;
		}
	}
	else
	{
		tM.tm_hour = tM.tm_min = 0;
		t = mktime(&tM);
		return 1;
	}
	t = mktime(&tM);
	return 1;
}
void handleRequest6(const string &code, const double &Along, const double &Alat, const int &M, const time_t &t)
{
	L1List<string> l2000, l500, lOnly2k; int l300;
	dbAVLTree.traverseLNR_Overload(Along, Alat, t, &l2000, &l500, l300, &lOnly2k);
	cout << code << ": ";
	if (l2000.getSize() < M) // In
	{
		L1Item<string> *pTmp{ l2000.getHead() };
		if (!pTmp)
		{
			cout << "- -1\n"; 
			return;
		}
		while (true)
		{
			if (pTmp->pNext)
			{
				cout << pTmp->data << ' ';
				pTmp = pTmp->pNext;
			}
			else
			{
				cout << pTmp->data << " - -1\n";
				return;
			}
		}
	}
	else
	{
		if (l300 > 0.75 * M)
		{
			L1Item<string> *pTmp{ l2000.getHead() };
			if (!pTmp)
			{
				cout << "-1 - \n";
				return;
			}
			cout << "-1 - ";
			while (true)
			{
				if (pTmp->pNext)
				{
					cout << pTmp->data << ' ';
					pTmp = pTmp->pNext;
				}
				else
				{
					cout << pTmp->data << '\n';
					return;
				}
			}
		}
		else
		{
			L1Item<string> *pTmp1{ lOnly2k.getHead() };
			L1Item<string> *pTmp{ l500.getHead() };
			if (!pTmp)
			{
				if (!pTmp1)
				{
					cout << "- \n"; return;
				}
				else
				{
					cout << "- ";
					while (true)
					{
						if (pTmp1->pNext)
						{
							cout << pTmp1->data << ' ';
							pTmp1 = pTmp1->pNext;
						}
						else
						{
							cout << pTmp1->data << '\n';
							return;
						}
					}
				}
			}
			while (true)
			{
				if (pTmp->pNext)
				{
					cout << pTmp->data << ' ';
					pTmp = pTmp->pNext;
				}
				else
				{
					cout << pTmp->data << " - ";
					break;
				}
			}
			if (!pTmp1)
			{
				cout << "\n"; return;
			}
			while (true)
			{
				if (pTmp1->pNext)
				{
					cout << pTmp1->data << ' ';
					pTmp1 = pTmp1->pNext;
				}
				else
				{
					cout << pTmp1->data << '\n';
					return;
				}
			}
		}
	}
}

//********** Request 7 ****************
bool checkValidRequest7(string &code, double &Along, double &Alat, int &M, time_t &t)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() == '_') return 0;
	stringstream buf{ code }; string tmp, tmpCode{ code };
	// code
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	// Along
	buf >> Along;
	if (buf.fail())
	{
		Along = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// Alat
	buf >> Alat;
	if (buf.fail())
	{
		Alat = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// M
	buf >> M;
	if (buf.fail())
	{
		M = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	if (M < 0) M = 0;
	// R
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// hhmm
	tmpCode = tmpCode.substr(tmpCode.find_last_of('_') + 1, 4);
	tmpCode.resize(4, '\0');
	tm tM; tM.tm_mday = 7; tM.tm_mon = 6; tM.tm_year = 70; tM.tm_sec = 0;
	if (isdigit(tmpCode[0]))
	{
		if (isdigit(tmpCode[1]))
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 2));
			if (tM.tm_hour > 23) tM.tm_hour -= 24;
		}
		else
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 1));
			tM.tm_min = 0;
			t = mktime(&tM);
			return 1;
		}
		if (isdigit(tmpCode[2]))
		{
			if (isdigit(tmpCode[3]))
			{
				tM.tm_min = stoi(tmpCode.substr(2, 2));
				if (tM.tm_min > 59) tM.tm_min -= 60;
			}
			else
			{
				tM.tm_min = stoi(tmpCode.substr(2, 1));
				t = mktime(&tM);
				return 1;
			}
		}
		else
		{
			tM.tm_min = 0; t = mktime(&tM); return 1;
		}
	}
	else
	{
		tM.tm_hour = tM.tm_min = 0;
		t = mktime(&tM);
		return 1;
	}
	t = mktime(&tM);
	return 1;
}
void print(string *s)
{
	cout << *s;
}
void handleRequest7(const string &code, const double &Along, const double &Alat, const int &M, const time_t &t)
{
	AVLTree<string, double> Tree12; L1List<double> data12; 
	L1List<string> l2000, lOnly1k; int l500;
	dbAVLTree.traverseLNR_Jam(Along, Alat, t, &Tree12, &data12, l500, &l2000, &lOnly1k);
	cout << code << ": ";
	if (l500 < 0.7 * M)
	{
		L1Item<string> *pTmp{ l2000.getHead() };
		if (!pTmp)
		{
			cout << "-1 - -1\n"; return;
		}
		cout << "-1 - ";
		while (true)
		{
			if (pTmp->pNext)
			{
				cout << pTmp->data << ' ';
				pTmp = pTmp->pNext;
			}
			else
			{
				cout << pTmp->data << '\n';
				return;
			}
		}
	}
	else // traffic jam
	{
		AVLTree<double, string> Tree12_1, Tree12_2;
		Tree12.traverseRNL(&Tree12_1, &Tree12_2, Tree12.getSize());
		L1Item<string> *pTmp{ lOnly1k.getHead() };
		double *d{ &(data12.getHead()->data) };
		while (pTmp)
		{
			Tree12_2.insert(d, &(pTmp->data));
			pTmp = pTmp->pNext;
		}
		if (Tree12_2.getRoot())
			Tree12_2.traverseLNR(print);
		else cout << "-1";
		cout << " - ";
		if (Tree12_1.getRoot())
			Tree12_1.traverseLNR(print);
		else cout << "-1";
		cout << '\n';
	}
}

L1List<Node_AVL_str*> removedList;
//********** Request 8 ***************
bool checkValidRequest8(string &code, double &Along, double &Alat, double &R, time_t &t)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() == '_') return 0;
	stringstream buf{ code }; string tmp, tmpCode{ code };
	// code
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	// Along
	buf >> Along;
	if (buf.fail())
	{
		Along = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// Alat
	buf >> Alat;
	if (buf.fail())
	{
		Alat = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// R
	buf >> R;
	if (buf.fail())
	{
		R = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// hhmm
	tmpCode = tmpCode.substr(tmpCode.find_last_of('_') + 1, 4);
	tmpCode.resize(4, '\0');
	tm tM; tM.tm_mday = 7; tM.tm_mon = 6; tM.tm_year = 70; tM.tm_sec = 0;
	if (isdigit(tmpCode[0]))
	{
		if (isdigit(tmpCode[1]))
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 2));
			if (tM.tm_hour > 23) tM.tm_hour -= 24;
		}
		else
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 1)); // < 23
			tM.tm_min = 0;
			t = mktime(&tM);
			return 1;
		}
		if (isdigit(tmpCode[2]))
		{
			/*if (tmpCode[2] > '5')
			{
				tM.tm_min = stoi(tmpCode.substr(2, 1));
				t = mktime(&tM);
				return 1;
			}*/
			if (isdigit(tmpCode[3]))
			{
				tM.tm_min = stoi(tmpCode.substr(2, 2));
				if (tM.tm_min > 59) tM.tm_min -= 60;
			}
			else
			{
				tM.tm_min = stoi(tmpCode.substr(2, 1));
				t = mktime(&tM);
				return 1;
			}
		}
		else
		{
			tM.tm_min = 0; t = mktime(&tM); return 1;
		}
	}
	else
	{
		tM.tm_hour = tM.tm_min = 0;
		t = mktime(&tM);
		return 1;
	}
	t = mktime(&tM);
	return 1;
}
void handleRequest8(const string &code, const double &Along, const double &Alat, const double &R, const time_t &t)
{
	L1List<Node_AVL_str*> newRmvL;
	dbAVLTree.traverseLNR_Remove(Along, Alat, R, t, &newRmvL);
	//removedList.getTail()->pNext = newRmvL->getHead(); // add to global list

	L1Item<Node_AVL_str*> *pTmp{ newRmvL.getHead() };
	cout << code << ": ";
	if (!pTmp) 
	{
		cout << "0\n"; return;
	}
	while (true)
	{
		removedList.push_back(pTmp->data);
		if (pTmp->pNext)
		{
			cout << *(pTmp->data->key) << ' ';
			pTmp = pTmp->pNext;
		}
		else
		{
			cout << *(pTmp->data->key) << '\n';
			return;
		}
	}
}

//*********** Request 9 *************
bool checkValidRequest9(string &code, double &Along, double &Alat, double &R, time_t &t)
{
	if (code.size() > REQUEST_CODE_SIZE - 1) code.resize(REQUEST_CODE_SIZE - 1);
	if (code.back() == '_') return 0;
	stringstream buf{ code }; string tmp, tmpCode{ code };
	// code
	getline(buf, code, '_');
	if (buf.eof()) return 0;
	// Along
	buf >> Along;
	if (buf.fail())
	{
		Along = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// Alat
	buf >> Alat;
	if (buf.fail())
	{
		Alat = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// R
	buf >> R;
	if (buf.fail())
	{
		R = 0; buf.clear();
	}
	getline(buf, tmp, '_');
	if (buf.eof()) return 0;
	// hhmm
	tmpCode = tmpCode.substr(tmpCode.find_last_of('_') + 1, 4);
	tmpCode.resize(4, '\0');
	tm tM; tM.tm_mday = 7; tM.tm_mon = 6; tM.tm_year = 70; tM.tm_sec = 0;
	if (isdigit(tmpCode[0]))
	{
		if (isdigit(tmpCode[1]))
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 2));
			if (tM.tm_hour > 23) tM.tm_hour -= 24;
		}
		else
		{
			tM.tm_hour = stoi(tmpCode.substr(0, 1));
			tM.tm_min = 0;
			t = mktime(&tM);
			return 1;
		}
		if (isdigit(tmpCode[2]))
		{
			if (isdigit(tmpCode[3]))
			{
				tM.tm_min = stoi(tmpCode.substr(2, 2));
				if (tM.tm_min > 59) tM.tm_min -= 60;
			}
			else
			{
				tM.tm_min = stoi(tmpCode.substr(2, 1));
				t = mktime(&tM);
				return 1;
			}
		}
		else
		{
			tM.tm_min = 0; t = mktime(&tM); return 1;
		}
	}
	else
	{
		tM.tm_hour = tM.tm_min = 0;
		t = mktime(&tM);
		return 1;
	}
	t = mktime(&tM);
	return 1;
}
void handleRequest9(const string &code, const double &Along, const double &Alat, const double &R, const time_t &t)
{
	L1Item<Node_AVL_str*> *pTmp{ removedList.getHead() };
	cout << code << ": ";
	if (!pTmp) { cout << "0\n"; return; }
	L1Item<Node_AVL_str*> *pPrev{ pTmp };
	size_t bef{ dbAVLTree.getSize() };
	while (true)
	{
		if (pTmp->pNext)
		{
			if (pTmp->data->data->traverseInAKeyValueInterval(t, t + 59, Along, Alat, R, isUnderControlled))
			{
				cout << *(pTmp->data->key) << ' ';
				dbAVLTree.insert(pTmp->data);
				// erase from the list
				if (pPrev == pTmp)
				{
					pPrev = pPrev->pNext;
					removedList.removeHead();
					pTmp = pPrev;
				}
				else
				{
					pPrev->pNext = pTmp->pNext;
					delete pTmp;
					removedList.decreaseSizeBy1();
					pTmp = pPrev->pNext;
				}
			}
			else
			{
				pPrev = pTmp;
				pTmp = pTmp->pNext;
			}
		}
		else // pTmp == removedList.getTail()
		{
			if (pTmp->data->data->traverseInAKeyValueInterval(t, t + 59, Along, Alat, R, isUnderControlled))
			{
				cout << *(pTmp->data->key) << '\n';
				dbAVLTree.insert(pTmp->data);
				// erase from the list
				removedList.removeLast();
			}
			else
			{
				if (dbAVLTree.getSize() != bef) cout << "0\n";
			}
		}
	}
}

//********** process requests **********
bool processRequest(VM_Request &request, L1List<VM_Record> &recordList, void *pGData) 
{
    // TODO: Your code goes here
	char c{ request.code[0] };
	string code{ request.code };
	switch (c)
	{
	case '1':
	{
		string x, y; time_t t;
		if (checkValidRequest1(code, x, y, t))
		{
			handleRequest1(code, x, y, t);
			return 1;
		}
		else return 0;
	}
	case '2':
	{
		double a; 
		if (checkValidRequest2(code, a, c))
		{
			handleRequest2(code, a, c);
			return 1;
		}
		return 0;
	}
	case '3':
	{
		double a;
		if (checkValidRequest3(code, a, c))
		{
			handleRequest3(code, a, c);
			return 1;
		}
		return 0;
	}
	case '4':
	{
		double Along, Alat, R; time_t H1, H2;
		if (checkValidRequest4(code, Along, Alat, R, H1, H2))
		{
			handleRequest4(code, Along, Alat, R, H1, H2);
			return 1;
		}
		return 0;
	}
	case '5':
	{
		string X; double Along, Alat, R;
		if (checkValidRequest5(code, X, Along, Alat, R))
		{
			handleRequest5(code, X, Along, Alat, R);
			return 1;
		}
		return 0;
	}
	case '6':
	{
		double Along, Alat; int M; time_t t;
		if (checkValidRequest6(code, Along, Alat, M, t))
		{
			handleRequest6(code, Along, Alat, M, t);
			return 1;
		}
		return 0;
	}
	case '7':
	{
		double Along, Alat; int M; time_t t;
		if (checkValidRequest7(code, Along, Alat, M, t))
		{
			handleRequest7(code, Along, Alat, M, t);
			return 1;
		}
		return 0;
	}
	case '8':
	{
		double Along, Alat, R; time_t t;
		if (checkValidRequest8(code, Along, Alat, R, t))
		{
			handleRequest8(code, Along, Alat, R, t);
			return 1;
		}
		return 0;
	}
	case '9':
	{
		double Along, Alat, R; time_t t;
		if (checkValidRequest9(code, Along, Alat, R, t))
		{
			handleRequest9(code, Along, Alat, R, t);
			return 1;
		}
		return 0;
	}
	default:
		return 0;
	}
    // return false for invlaid events
    //return true;
}


