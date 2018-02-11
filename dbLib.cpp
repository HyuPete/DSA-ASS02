/*
 * =========================================================================================
 * Name        : dbLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This file implements functions used for database management
 * =========================================================================================
 */

#include "dbLib.h"

#include <time.h>
#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#define __PI 3.14159265358979323846
#define earthRadiusKm 6371.0

using namespace std;

void    strPrintTime(char* des, time_t& t) {
    tm *pTime = gmtime(&t);
    strftime(des, 26, "%Y-%m-%d %H:%M:%S", pTime);
}

AVL_AVL dbAVLTree;
// assuming newNode is an external node
template<>
void AVLTree< AVLTree<VM_Record, time_t>, string>::insert(TreeNode<VM_Record, time_t> *newNode, VM_Record *d)
{
	string str{ (d->id) };
	Node_AVL_str **pTmp{ &root };
	//TreeNode< AVLTree<VM_Record, time_t>, string > **p1{ &root };
	GData *pGtmp;
	L1List< TreeNode<AVLTree<VM_Record, time_t>, string>** > lPath;
	while (*pTmp)
	{
		if (str == *((*pTmp)->key)) // there exists a avl child with the same key
		{
			pGtmp = static_cast<GData*>((*pTmp)->data->pGData);
			if (d->longitude < pGtmp->minLong) // find minLong
			{
				pGtmp->minLong = d->longitude;
			}
			else
			{
				if (d->longitude > pGtmp->maxLong) // find maxLong
					pGtmp->maxLong = d->longitude;
			}
			if (d->latitude < pGtmp->minLat) // find minLat
			{
				pGtmp->minLat = d->latitude;
			}
			else
			{
				if (d->latitude > pGtmp->maxLat) // find maxLat
					pGtmp->maxLat = d->latitude;
			}
			(*pTmp)->data->insert(newNode);
			return;
		}
		else
		{
			lPath.insertHead(pTmp);
			if (str < *((*pTmp)->key))
			{
				pTmp = &((*pTmp)->left);
			}
			else // str > *(pTmp->key)
			{
				pTmp = &((*pTmp)->right);
			}
		}
	}
	// create new Node_AVL_str
	AVL_VmR_Time *p{ new AVL_VmR_Time() };
	p->pGData = new GData(d->longitude, d->longitude, d->latitude, d->latitude);
	p->insert(newNode);
	*pTmp = new Node_AVL_str(p, &(d->id));
	size++;
	// fix balanced
	L1Item< Node_AVL_str ** > *pt{ lPath.getHead() };
	int tmpH;
	while (pt) // null when *pTmp == root or all item in lPath have been removed
	{
		pTmp = pt->data;
		tmpH = (*pTmp)->getHeight();
		(*pTmp)->updateH();
		keepBalanced(*pTmp);
		if ((*pTmp)->getHeight() == tmpH) return; // check if it needs to fix balanced
		lPath.removeHead();
		pt = lPath.getHead();
	}
}
template<>
void AVL_AVL::insert(VM_Record *d)
{
	//TreeNode<VM_Record, time_t> *newNode{ new TreeNode<VM_Record, time_t>(d, &(d->timestamp)) };
	insert(new TreeNode<VM_Record, time_t>(d, &(d->timestamp)), d);
}
void loadVMDB(char* fName, L1List<VM_Record> &db) {
    //ifstream inFile(fName);

	ifstream dbFile(fName);

    //if (inFile) {
    //    string line;
    //    getline(inFile , line);// skip the first line
    //    VM_Record record;
    //    db.insertHead(record);/// add dummy object
    //    while (getline(inFile , line)) {
    //        /// On Windows, lines on file ends with \r\n. So you have to remove \r
    //        if (line[line.length() - 1] == '\r')
    //            line.erase(line.length() - 1);
    //        if (line.length() > 0) {
    //            if (parseVMRecord((char*)line.data(), db[0]))/// parse and store data directly
    //                db.insertHead(record);/// add dummy object for next turn
    //        }
    //    }
    //    db.removeHead();/// remove the first dummy
    //    db.reverse();
    //    inFile.close();
    //}
    //else {
    //    cout << "The file is not found!";
    //}

	if (!dbFile)
	{
		db.clean();
		cerr << "\nCannot open the database file for reading!\n";
		return;
	}
	else
	{
		stringstream buf;
		string str;
		tm t;
		//int in;
		VM_Record n;
		getline(dbFile, str); // discard the first line
		while (!dbFile.eof())
		{
			getline(dbFile, str, ','); // discard the 1st item on each line
			// handle time data
			if (!str.empty() && (str.back() > ' ')) // 
			{
				getline(dbFile, str, ' '); // "day/mon/year"
				getline(dbFile, str, ',');
				buf << str;
				buf >> t.tm_hour;
				buf.ignore(); // discard ':'
				buf >> t.tm_min;
				buf.ignore(); // discard ':'
				buf >> t.tm_sec;
				t.tm_mday = 7; t.tm_mon = 6; t.tm_year = 70;
				//buf >> get_time(&t, "%m/%d/%Y %H:%M:%S");
				n.timestamp = mktime(&t); // convert from tm to time_t
				buf.str("");
				buf.clear();
				// handle vehicle id
				getline(dbFile, str, ','); // vehicle tag
				n.id = str;
				// handle longitude
				getline(dbFile, str, ','); // Longitude
				n.longitude = stod(str);
				// handle latitude
				getline(dbFile, str, ','); // Latitude
				n.latitude = stod(str);
				getline(dbFile, str); // read the rest of the line
				db.push_back(n);
				dbAVLTree.insert(&(db.getTail()->data));
			}
		}
		dbFile.close();
	}
	return;
}

bool parseVMRecord(char *pBuf, VM_Record &bInfo) {
    // TODO: write code to parse a record from given line
	
	return 1;
}

void process(L1List<VM_Request>& requestList, L1List<VM_Record>& rList) {
    void*   pGData = NULL;
    initVMGlobalData(&pGData);

    while (!requestList.isEmpty()) {
        if(!processRequest(requestList[0], rList, pGData))
            cout << requestList[0].code << " is an invalid event\n";
        requestList.removeHead();
    }

    releaseVMGlobalData(pGData);
}

void printVMRecord(VM_Record &b) {
   // printf("%s: (%0.5f, %0.5f), %s\n", b.id, b.longitude, b.latitude, ctime(&b.timestamp));
}

/// This function converts decimal degrees to radians
inline double deg2rad(double deg) {
    return (deg * __PI / 180);
}

///  This function converts radians to decimal degrees
inline double rad2deg(double rad) {
    return (rad * 180 / __PI);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double distanceEarth(double lon1d, double lat1d, double lon2d, double lat2d) {
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}