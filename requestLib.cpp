/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 2 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "requestLib.h"

void loadRequests(char* fName, L1List<VM_Request> &rList) {
    // TODO: write your code to load requests. Each request is separated by a whitespace
	fstream inF(fName);
	if (!inF)
	{
		cerr << "\nCannot open the file for reading!\n";
		return;
	}
	else
	{
		string str;
		stringstream buf;
		getline(inF, str, ';');
		inF.close();
		buf << str;
		VM_Request n;
		while (!buf.eof())
		{
			buf >> str;
			n.code = str;
			str.clear();
			rList.push_back(n);
		}
	}
	return;
}
