//============================================================================
// Name        : RSA.cpp
// Author      : Emilia Michalek
//============================================================================

#include <boost/multiprecision/cpp_int.hpp>
#include "RSAAlgorithm.h"
#include "BlockWriter.h"
#include "RSACodec.h"
#include "FileComparator.h"

#include <iostream>
using namespace std;

RSACodec makeRSACodecFromFile(const string& keyMaterialFile, ostream& debugStream){
	// opening key material file
	ifstream fin(keyMaterialFile);
	if(!fin){
		throw runtime_error("Cannot open key material file " + keyMaterialFile + " for reading");
	}
	// declaring necessary variables
	boost::multiprecision::cpp_int p, q, e;
	// reading the file until eof
	while(fin){
		// reading in either "p", "q", or "e"
		string token;
		fin >> token;
		if(!fin){
			// reached eof
			break;
		}
		// save further data into the corresponding variable
		if(token == "p"){
			fin >> p;
		}else if(token == "q"){
			fin >> q;
		}else if(token == "e"){
			fin >> e;
		}else{
			throw runtime_error("Invalid token " + token + " in key file " + keyMaterialFile);
		}
	}
	// if p, q, or e values provided are not correct or not there
	if(p == 0){
		throw runtime_error("Missing or invalid p value in key file " + keyMaterialFile);
	}
	if(q == 0){
		throw runtime_error("Missing or invalid q value in key file " + keyMaterialFile);
	}
	if(e == 0){
		throw runtime_error("Missing or invalid e value in key file " + keyMaterialFile);
	}
	// closing key material file
	fin.close();
	// creating and returning the created RSACodec object with the found p, q, and e values
	return RSACodec(p, q, e, debugStream);
}

void testEncrypting(RSACodec& rsaCodec, const string& plainFile, const string& encryptedFile, ostream& debugStream){
	// opening original input file and output file to encrypt to:
	ifstream fin(plainFile, ios_base::binary);
	if(!fin){
		throw runtime_error("Cannot open file " + plainFile + " for reading");
	}
	ofstream fout(encryptedFile);
	if(!fout){
		throw runtime_error("Cannot open file " + encryptedFile + " for writing");
	}
	// printing debug statement
	debugStream << "*** Encrypting " << plainFile << " -> " << encryptedFile << " ***\n\n";
	debugStream.flush();
	// encrypting
	rsaCodec.encryptStream(fin, fout);
	// closing used files
	fin.close();
	fout.close();
	// printing final debug statements
	debugStream << "Encrypted file " << encryptedFile << "\n\n";
	debugStream.flush();
}

void testDecrypting(RSACodec& rsaCodec, const string& encryptedFile, const string& decryptedFile, ostream& debugStream){
	// opening encrypted input file and output file to print decrypted data to
	ifstream fin(encryptedFile, ios::binary);
	if(!fin){
		throw runtime_error("Cannot open file " + encryptedFile + " for reading");
	}
	ofstream fout(decryptedFile, ios::binary);
	if(!fout){
		throw runtime_error("Cannot open file " + decryptedFile + " for writing");
	}
	// printin debug statement
	debugStream << "*** Decrypting " << encryptedFile << " -> " << decryptedFile << " ***\n\n";
	debugStream.flush();
	// decrypting
	rsaCodec.decryptStream(fin, fout);
	// closing used files
	fin.close();
	fout.close();
	//  printing final debug statements
	debugStream << "Decrypted file " << decryptedFile << "\n\n";
	debugStream.flush();
}

void testFile(int fileNumber){
	const string numberString = to_string(fileNumber);
	// file names:
	const string debugFile = numberString + "_debug.txt";
	const string keyMaterialFile = numberString + "_keymat.txt";
	const string plainFile = numberString + "_in.dat";
	const string encryptedFile = numberString + "_encrypted.txt";
	const string decryptedFile = numberString + "_decrypted.dat";
	// opening corresponding debug file
	ofstream debugStream(debugFile);
	// printing console statement
	cout << "*** Testing input file " << plainFile << ", debug output file " << debugFile << " ***\n";
	cout.flush();
	// creating RSACodec object and reading in p, q, and e values
	RSACodec rsaCodec = makeRSACodecFromFile(keyMaterialFile, debugStream);
	// encrypting original input file
	testEncrypting(rsaCodec, plainFile, encryptedFile, debugStream);
	// decrypting the encrypted file
	testDecrypting(rsaCodec, encryptedFile, decryptedFile, debugStream);
	// comparing files for consistency
	FileComparator::compareFiles(plainFile, decryptedFile);  // !!! HAS TO THROW IF THE FILES ARE NOT EQUAL
	// printing final statements to debug and console:
	debugStream << ">>> Files " << plainFile << " and " << decryptedFile << " are equal.\n";
	debugStream.flush();
	debugStream.close();
	cout << "\nOK\n\n";
	cout.flush();
}


int main(){
	try{
		for(int i = 0; i <= 3; ++i){
			testFile(i);
		}

	}catch(const exception& ex){
		ofstream debugStream("debug_output.txt");
		if(debugStream.is_open()){
			debugStream << "Exception: " << ex.what() << "\n";
		}
		cout << "Exception: " << ex.what() << "\n";
	}

	return 0;
}



