/*
 * RSACodec.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Emilia Michalek
 */

#ifndef SRC_RSACODEC_H_
#define SRC_RSACODEC_H_

#include <boost/multiprecision/cpp_int.hpp>
#include <stdexcept>
#include "RSAAlgorithm.h"
#include "BlockReader.h"
#include "BlockWriter.h"
#include <iostream>
using namespace std;

// Function to calculate the maximum block length
unsigned int calculateBlockMaxLength(const boost::multiprecision::cpp_int& n, ostream& out){
	// declaring variable to return
	unsigned int blockMaxLength = 0;
	// calculating the maximum number of chunk bytes 0xFF so that 0xFFFF ... FFFF < n
	boost::multiprecision::cpp_int maxChunkValue = 0xFF;
	while(maxChunkValue < n){
		maxChunkValue <<= 8;
		++blockMaxLength;
	}
	// block size needs to be at least 1
	if(blockMaxLength < 1){
		throw runtime_error("calculateBlockMaxLength - Modulus " + n.str() + " is too small");
	}
	// returning the found value

	out << "Calculated Block length: " << blockMaxLength << endl;
	return blockMaxLength;
}


class RSACodec{
private:
	RSAAlgorithm rsaAlgorithm_;
	ostream& debugStream;

public:
	// Constructor
	RSACodec(const boost::multiprecision::cpp_int& p, const boost::multiprecision::cpp_int& q, const boost::multiprecision::cpp_int& e, ostream& debugStream) //.....
		: rsaAlgorithm_(p, q, e, debugStream), debugStream(debugStream){}

	void encryptStream(istream& fin, ostream& fout){
		// creating block reader object and passing input and debug streams
		BlockReader blockReader(fin, debugStream);
		// calculates maximum chunk length
		const int blockMaxLength = calculateBlockMaxLength(rsaAlgorithm_.getN(), debugStream);
		// while loop runs until eof and keeps track of block number via 'i' variable
		int i=0;
		while(true){
			// printing debug info
			debugStream << "--- RSACodec::encryptStream block #" << to_string(i) << ", max length " << to_string(blockMaxLength) << " ---\n\n";
			debugStream.flush();
			// declaring variables to hold both the plaintext and encrypted hexadecimal numbers
			boost::multiprecision::cpp_int plaintextNumber;
			boost::multiprecision::cpp_int ciphertextNumber;
			// tries to read data from input file, throws exception if there's a problem
			try{
				// variable to determine whether it has reached the eof
				int bytesRead = blockReader.readData(plaintextNumber, blockMaxLength);
				if(bytesRead == 0){
					// no more data to read, break out of the loop
					break;
				}
				// encrypting plaintextNumber to ciphertextNumber via rsaAlgorithm_
				ciphertextNumber = rsaAlgorithm_.encrypt(plaintextNumber);
				// writing to output stream in hexadecimal format
				fout << hex << uppercase << bytesRead << " " << hex << setw(2 * blockMaxLength) << setfill('0') << ciphertextNumber << "\n";

			}catch(const exception& e){
				cerr << "Error during encryption: " << e.what() << "\n";
				break;
			}
			// increments count to keep track of block number
			i++;
		}
	}

	void decryptStream(istream& fin, ostream& fout){
		// creating block writer object and passing output and debug streams
		BlockWriter blockWriter(fout, debugStream);
		// block length variable to be read from input file
		int blockLength;
		// creating ciphertext and plaintext variables
		boost::multiprecision::cpp_int ciphertextNumber, plaintextNumber;
		for(int i=0;;++i){
			// Read the blockLength and ciphertextNumber from fin
			fin >> hex >> blockLength >> ciphertextNumber;
			if(fin.eof()){
				// no more data
				break;
			}
			// printing debug info
			debugStream << "--- RSACodec::decryptStream block #" << to_string(i) << ", length " << to_string(blockLength) << " ---\n\n";
			debugStream.flush();
			// decrypting ciphertextNumber to plaintextNumber via rsaAlgorithm_
			plaintextNumber = rsaAlgorithm_.decrypt(ciphertextNumber);
			// writing plaintext to the output stream via BlockWriter
			blockWriter.writeData(plaintextNumber, blockLength);
		}
	}

};



#endif /* SRC_RSACODEC_H_ */
