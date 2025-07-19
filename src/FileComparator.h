/*
 * FileComparator.h
 *
 *  Created on: Nov 28, 2023
 *      Author: Emilia Michalek
 */

#ifndef SRC_FILECOMPARATOR_H_
#define SRC_FILECOMPARATOR_H_

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

class FileComparator{

public:
	static void compareFiles(const string& firstFile, const string& secondFile){
		// opening both files in binary and checking if both opened:
		ifstream fin1(firstFile, ios::binary);
		ifstream fin2(secondFile, ios::binary);
		if(!fin1.is_open()){
			throw runtime_error("Cannot open file " + firstFile + " for reading");
		}
		if(!fin2.is_open()){
			throw runtime_error("Cannot open file " + secondFile + " for reading");
		}

		int blockCount = 0;
		const int BLOCKSIZE = 1024;
		char buffer1[BLOCKSIZE];
		char buffer2[BLOCKSIZE];
		// reading until eof
		while(true){
			fin1.read(buffer1, BLOCKSIZE);
			fin2.read(buffer2, BLOCKSIZE);

			const int length1 = fin1.gcount();
			const int length2 = fin2.gcount();

			if(length1 != length2){
				cout << "Error: Sizes of the files are different, file " << firstFile << " has size "
						  << (length1 > length2 ? ">=" : "==") << length1 + BLOCKSIZE * blockCount
						  << ", file " << secondFile << " has size " << (length1 < length2 ? ">=" : "==") << length2 + BLOCKSIZE * blockCount << endl;

				throw runtime_error("Sizes of the files are different");
			}
			for(int i = 0; i < length1; ++i){
				if(buffer1[i] != buffer2[i]){
					cout << "Error: Files are different at position " << i + BLOCKSIZE * blockCount
							  << ", file " << firstFile << " has character " << static_cast<int>(buffer1[i])
							  << ", file " << secondFile << " has character " << static_cast<int>(buffer2[i]) << endl;

					throw runtime_error("Files are different at position " + to_string(i + BLOCKSIZE * blockCount));
				}
			}
			if(length1 == 0){
				break;
			}

			++blockCount;
		}

		fin1.close();
		fin2.close();
	}
};




#endif /* SRC_FILECOMPARATOR_H_ */
