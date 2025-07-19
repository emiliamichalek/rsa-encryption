/*
 * RSAAlgorithm.h
 *
 *  Created on: Nov 26, 2023
 *      Author: Emilia Michalek
 */

#ifndef RSAALGORITHM_H_
#define RSAALGORITHM_H_
#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <fstream>
using namespace std;

class RSAAlgorithm{
	private:
	ostream& debugStream;

	boost::multiprecision::cpp_int n;  // n is part of the private key
	boost::multiprecision::cpp_int d;  // d is the private exponent/decryption exponent
	boost::multiprecision::cpp_int e;  // e is the encryption exponent

	// Function to calculate the modular multiplicative inverse, d
	boost::multiprecision::cpp_int calculateD(boost::multiprecision::cpp_int& a, boost::multiprecision::cpp_int& m){
		boost::multiprecision::cpp_int m0 = m, t, q;
		boost::multiprecision::cpp_int x0 = 0, x1 = 1;
		if(m == 1){
			return 0;
		}
		while(a > 1){
			q = a / m;
			t = m;
			m = a % m, a = t;
			t = x0;
			x0 = x1 - q * x0;
			x1 = t;
		}
		if(x1 < 0){
			x1 += m0;
		}
		return x1;
	}

	// funciton to manually calculate pow and mod n
	boost::multiprecision::cpp_int modexp(boost::multiprecision::cpp_int& base, boost::multiprecision::cpp_int& exponent, const boost::multiprecision::cpp_int& modulus){
		// initializing necessary variables
		boost::multiprecision::cpp_int result = 1;
		base = base % modulus;
		boost::multiprecision::cpp_int Exponent = exponent;
		// raising to the power of 'exponent' and moding by 'modulus' at each step
		while(Exponent > 0){
			if(Exponent % 2 == 1){
				result = (result * base) % modulus;
			}

			Exponent >>= 1;
			base = (base * base) % modulus;
		}
		// returning final value
		return result;
	}

public:
	// Constructor
	RSAAlgorithm(const boost::multiprecision::cpp_int& p, const boost::multiprecision::cpp_int& q, const boost::multiprecision::cpp_int& passedE, ostream& debugStream) : debugStream(debugStream){ //.....
		// setting local e variable to the passed value for e
		e = passedE;
		// creating e variable that will be used and changed in the calculateD function
		boost::multiprecision::cpp_int tempe = passedE;
		// calculating n, phin, and d
		n = p * q;
		boost::multiprecision::cpp_int phin = (p - 1) * (q - 1);
		d = calculateD(tempe, phin);
		// printing debugging information
		debugStream << "--- RSAAlgorithm"
				  << "\n p " << p
				  << "\n q " << q
				  << "\n e " << e
				  << "\n n " << n
				  << "\n d " << d
				  << "\n\n";
	}

	// encrypts the plaintext number
	boost::multiprecision::cpp_int encrypt(boost::multiprecision::cpp_int& plaintextNumber){
		// creating copy variable to not change the passed plaintext number
		boost::multiprecision::cpp_int plaintxtNum = plaintextNumber;
		// performing the core encryption operation: ciphertext = (plaintext ^ e) mod n
		boost::multiprecision::cpp_int ciphertextNumber = modexp(plaintxtNum, e, n);
		// printing debugging information
		debugStream << "RSAAlgorithm::encrypt 0x" << hex << plaintextNumber << " -> 0x" << ciphertextNumber << "\n\n";
		// returning the ciphertext number
		return ciphertextNumber;
	}

	boost::multiprecision::cpp_int decrypt(boost::multiprecision::cpp_int& ciphertextNumber){
		// creating copy variable to not change the passed ciphertext number
		boost::multiprecision::cpp_int ciphertxtNum = ciphertextNumber;
		// performing the decryption operation: plaintext = (ciphertext ^ d) mod n
		boost::multiprecision::cpp_int plaintextNumber = modexp(ciphertxtNum, d, n);
		// printing debugging information
		debugStream << "RSAAlgorithm::decrypt 0x" << hex << ciphertextNumber << " -> 0x" << plaintextNumber << "\n\n";
		// returning the plaintext number
		return plaintextNumber;
	}

	// returns the value of 'n'
	boost::multiprecision::cpp_int getN() const{
		return n;
	}

};



#endif /* RSAALGORITHM_H_ */
