/*
 * BlockReader.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Emilia Michalek
 */

#ifndef SRC_BLOCKREADER_H_
#define SRC_BLOCKREADER_H_

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
using namespace std;

class BlockReader{
private:
	istream& is_;
	ostream& debugStream;

	string bytePresentation(const char byte){
		if(byte == '\n') return "\\n";
		if(byte == '\r') return "\\r";
		if(byte == '\t') return "\\t";

		if(byte >= ' ' && byte <= '~'){
			ostringstream oss;
			oss << "'" << byte << "'";
			return oss.str();
		}

		ostringstream oss;
		oss << "0x" << hex << setw(2) << setfill('0') << uppercase << (static_cast<int>(byte) & 0xFF);
		return oss.str();
	}

public:
	// Constructor
	BlockReader(istream& is, ostream& debugStream) : is_(is), debugStream(debugStream){}

	// Function to read data of one block and convert it to a BigNumber
	int readData(boost::multiprecision::cpp_int& bigNumber, const int count){
		// creating separate count variable because of const. Represents the block length
		int Count = count;
		// if block size is 0 throw error
		if(count == 0){
			throw runtime_error("BlockReader::readData - Invalid parameter count = " + to_string(count));
		}
		// printing debug output with block size
		debugStream << "BlockReader::readData - Requested count " << to_string(count) << " bytes, got\n";
		// setting initial bigNumber and width to 0
		int width = 0;
		bigNumber = 0;
		// looping until it reaches the block size
		while(Count != 0){
			// variable to hold the read-in byte
			char byte;
			// Binary read operation to read one byte
			is_.read(&byte, 1);
			// Check if there was no more data (EOF)
			if(is_.eof()){
				debugStream << "\n";
				break;
			}
			// print debug info
			debugStream << " [" << to_string(width) << "] " << bytePresentation(byte) << " ";
			// increment width
			++width;
			bigNumber <<= 8;
			bigNumber |= static_cast<unsigned char>(byte);  // Cast byte to unsigned char before using in BigNumber
			// update count variable
			Count --;
		}
		// print debug info for end of reading in block
		debugStream << endl;
		debugStream << "BlockReader::readData - Read 0x" << hex << bigNumber << " as " << to_string(width) << " bytes" << "\n\n";
		// returns how may bytes were read
		return width;
	}

};



#endif /* SRC_BLOCKREADER_H_ */
