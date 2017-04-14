#include <iostream>
#include "../include/matrix.hpp"

using namespace std;

int main(void) {

	Matrix<float> matA(5,5);
	Matrix<float> matB;
	Matrix<float> matC;

	cout << "\nmatA\n" << matA;
	cout << "\nmatB\n" << matB;
	cout << "\nmatC\n" << matC;
	cout << "\n------\n";

	matA = 1.1;
	cout << "\nmatA = 1.1\n";
	cout << "\nmatA\n" << matA;

	matB = matA;
	cout << "\nmatB = matA\n";
	cout << "\nmatB\n" << matB;

	matC = matA + matB;
	cout << "\nmatC = matA + matB\n";
	cout << "\nmatC\n" << matC;

	matC += matC;
	cout << "\nmatC += matC\n";
	cout << "\nmatC\n" << matC;

	return 0;
}
