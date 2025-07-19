/*
 * BlockWriter.h
 *
 *  Created on: Nov 28, 2023
 *      Author: Emilia Michalek
 */

#ifndef SRC_BLOCKWRITER_H_
#define SRC_BLOCKWRITER_H_

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
using namespace std;

class BlockWriter{
private:
	ostream& os_;
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
	BlockWriter(ostream& os, ostream& debugStream) : os_(os), debugStream(debugStream){}

	// Function to write a value to the output stream with a specified width
	void writeData(const boost::multiprecision::cpp_int& number, const int count){
		// throw error if block size is 0
		if(count == 0){
			throw runtime_error("BlockWriter::writeData - Invalid parameter count " + to_string(count));
		}
		// printing debug info
		debugStream << "BlockWriter::writeData - Writing 0x" << hex << number << " as " << to_string(count) << " bytes" << endl; //....
		// Convert number to a byte sequence
		for(int i = 0, bitOffset = count * 8 - 8; bitOffset >= 0; ++i, bitOffset -= 8){
			const unsigned char byte = static_cast<unsigned char>((number >> bitOffset) & 0xFF);
			// Use binary write operation to write one byte to os_
			os_.write(reinterpret_cast<const char*>(&byte), 1);
			// printing debug info
			debugStream << " [" << to_string(i) << "] " << setfill('0') << bytePresentation(byte) << " ";
		}
		debugStream << "\n\n";
		// Check if writing failed
		if(!os_){
			throw runtime_error("BlockWriter::writeData - Failed to write " + to_string(count) + " bytes");
		}

	}

};



#endif /* SRC_BLOCKWRITER_H_ */
