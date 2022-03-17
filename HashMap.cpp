#include <bits/stdc++.h>
#include "hashMap.hpp"
#include "hashNode.hpp"
#include <iostream>
#include <string.h>
#include <math.h>
using namespace std;

//Constructor for the hashmap. //when creating the
//map, make sure you initialize the values to
//NULL so you know whether that index has a key
//in it or is set to NULL
// ****here you’re just looping through the map of hashNodes (a pointer to an array of
//pointers to hashNodes) and initializing the array to NULL

hashMap::hashMap(bool hash1, bool coll1) {
	hashfn = hash1;
	collfn = coll1;
	numKeys = 0;
	hashcoll = 0;
	collisions = 0;
	mapSize = primes[0];
	map = new hashNode*[mapSize];
	for(int i = 0; i < mapSize; ++i){
		map[i] = NULL;
	}
}

//
int hashMap::hashFunc(string k){
	return k.length() % mapSize;
}

//Adds the string (v) while checking to see if the key is empty or not
void hashMap::addKeyValue(string k, string v) {
	int index = 0;
	//Checks if there are any hash functions to be called
	if(hashfn == true){
		index = calcHash1(k);
	}
	else{
		index = calcHash2(k);
	}
	//Check to see if the index is null
	if(map[index] == NULL){
		cout<<"The index is null thus we can put it on the map"<<endl;
		map[index] = new hashNode(k,v);
		numKeys++;
	}
	else if(map[index]->keyword == k){
		map[index]->addValue(v);
		cout<<"The keyword is found"<<endl;
	}
	//Calls collisions and checks the collision functions
	else{
		hashcoll++;
		if(collfn == true){
			index = coll1(index, hashcoll, k);
			cout<<"The collision happened at "<<index<<endl;
			//Checks to see if index is empty
			if(map[index] == NULL){
				map[index] = new hashNode(k,v);
				numKeys++;
			}
			//Checks to see if the keyword is found
			else if(map[index]->keyword ==k){
				map[index]->addValue(v);
			}
		}
		else{
			index = coll2(index, hashcoll, k);
			//Checks to see if index is empty
			if(map[index] == NULL){
				cout<<"Its empty"<<endl;
				map[index] = new hashNode(k,v);
				numKeys++;
			}
			//Checks to see if the keyword is found
			else if(map[index]->keyword == k){
				cout<<"Found a match"<<endl;
				map[index]->addValue(v);
			}
		}


	}

}

//Finds the key in the hashmap. No loops or 50% grade deduction. Besides, finding in hashing is O(1) not O(n) ya noob!
int hashMap::getIndex(string k) {
	cout<<"Testing getIndex and also calling functions inside it too"<<endl;
	int key = 0;
	if(hashfn == true){
		key = calcHash1(k);
	}
	else if(hashfn == false){
		key = calcHash2(k);
	}

	if(map[key] != NULL && map[key]->keyword == k){
		return key;
	}
	else{
		if(collfn == true){
			int collide = coll1(key, 0, k);
			cout<<"index found at :" << collide<<endl;
			cout<<"key found at :" << map[collide]->keyword<<endl;
			return collide;
		}
		else{
			int collide = coll2(key, 0, k);
			cout<<"index found at :" << collide<<endl;
			cout<<"key found at :" << map[collide]->keyword<<endl;
			return collide;
		}
	}
}

//This function also does linear probing but to the ith power.
int hashMap::calcHash2(string k) {
	cout << "calcHash2" << endl;
	unsigned long int hash = 0;
	unsigned long int len = k.length();
	for (int i = 0; i < len; i++) {
		hash += pow(k[i], i);
	}
	return hash % mapSize;
}


//Goes over each character, and performs quadratic probing on the characters with 2^i.
int hashMap::calcHash1(string k) {
	cout << "calcHash1" << endl;
	unsigned long int hash = 0;
	unsigned long int len = k.length();
	for (int i = 0; i < len; i++) {
		hash += k[i] * pow(2, i);
	}
	cout << "calcHash1 end k: "<<k<<endl;
	cout << "hash%mapSize: " << hash % mapSize<<endl;
	return hash % mapSize;
}

//I opted to do binary search
void hashMap::getClosestPrime() {
	cout<<"Testing getClosestPrime()"<<endl;
	int start = 0;
	int replacedSize = mapSize * 2;
	int end = 1204;

	//Binary search from the primes
	while(start <= end){
		int middle = start + (start + end) / 2;
		if(primes[middle] > replacedSize && primes[middle - 1] < replacedSize){
			mapSize = primes[middle];
			break;
		}
		//If middle index < doubledSize, increment start by 1
		if(primes[middle] < replacedSize){
			start = middle + 1;
		}
		//If middle index > doubledSize, increment end by 1
		else{
			end = middle - 1;
		}
	}

	map = new hashNode*[mapSize];
	for(int i = 0; i < mapSize; i++){
			map[i] = NULL;
	}
}

//Rehashes the array via copying the old values in the new data then deleting the old value
//1) Create new hashmap double the size
//2) Collect nodes of current hashmap
//3)Place collected nodes in newhashmap
//4)Set map global variable to newHashmap

void hashMap::reHash() {
	cout<<"Testing rehash"<<endl;
	int existSize = mapSize;
	//Integer to access the index and the string, target
	int index;
	hashNode **old = map;
	hashNode **temp = new hashNode*[existSize];
	string target;
	getClosestPrime();
	for(int i = 0; i < existSize; ++i){
		if(old[i] != NULL){
			target = old[i]->keyword;
			index = getIndex(target);
			map[index] = old[i];
		}

	}
	delete old;
	int hashing;
	for(int i = 0; i < existSize; ++i){
		if(&temp[i] == NULL){
			continue;
			hashing = 0;
		}
		if(hashfn == true){
			hashing = calcHash1(temp[i]->keyword);
			if(map[hashing] != NULL){
					if(collfn == true){
						hashing = coll1(hashing, 0, temp[i]->keyword);
					}
					else{
						hashing = coll2(hashing, 0, temp[i]->keyword);
					}
			}
			else{
				cout<<"Testing hashing2"<<endl;
				hashing = calcHash2(temp[i]->keyword);
				if(map[hashing] != NULL){
					if(collfn == true){
						hashing = coll1(hashing, 0, temp[i]->keyword);
					}
					else{
						hashing = coll2(hashing, 0, temp[i]->keyword);
						}
				}
			}


		}

	}

}

//Probing starts here, this one is recursion and does chaining
int hashMap::coll1(int h, int i, string k) {
	cout << "Testing coll1" << endl;
	h++;
	if (h >= mapSize) {
		//cout << "if1"<<endl;
		h = 0;
	}
	if (map[h] == NULL) {
		//cout << "if2"<<endl;
		cout << "coll1 ret Null: " << h << endl;
		return h;
	} else if (map[h]->keyword == k) {
		//cout << "if3"<<endl;
		cout << "coll1 ret Mathc: " << h << endl;
		return h;
	} else {
		//cout << "if4"<<endl;
		return coll1(h, i, k);
	}
	//cout<<"end"<<endl;
	cout << "coll1 ret: " << h << endl;
	return h;


}
//Other methods for probing collisions, in this case quadratic probing is used
int hashMap::coll2(int h, int i, string k) {
	cout << "Testing coll2" << endl;
	h = h + pow(i, 2);
	if (h >= mapSize) {
		h = h%mapSize;
	}
	if (map[h] == NULL) {
		return h;
	} else if (map[h]->keyword == k) {
		return h;
	} else {
		i++;
		return coll2(h, i, k);
	}
	return h;
}



//Prints out the map
void hashMap::printMap() {
	cout << "In printMap()" << endl;
	for (int i = 0; i < mapSize; i++) {
		//cout << "In loop" << endl;
		if (map[i] != NULL) {
			cout << map[i]->keyword << ": ";
			for (int j = 0; j < map[i]->currSize;j++) {
				cout << map[i]->values[j] << ", ";
			}
			cout << endl;
		}
	}
}


