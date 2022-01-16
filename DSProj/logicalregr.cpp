////#include <math.h>
//#include <iostream>
//#include <cmath>
//#include <vector>
//
//
//using namespace std;
//
//double sigmoidfunction(double elo) {
//	return 1 / (1 + exp(-elo));
//}
//
//double VectorMultiplication(vector<double>& parameters,vector<int>& ELO) {
//	return parameters[0] * ELO[0] + parameters[1] * ELO[1];
//}
//
////Logistic regression COST function 
//double ConstFunction(vector<vector<int>>& ELO,vector<int>& y,int m,vector<double>& parameters) {
//	int J = 0;
//	int J1 = 0;
//	for (int i = 0; i < m; i++) {
//		J1 = (y[i] * log(sigmoidfunction(VectorMultiplication(parameters, ELO[i]))));
//		J = J1 + (1 - y[i]) * log(sigmoidfunction(VectorMultiplication(parameters, ELO[i])));
//	}
//
//	J = (-1 / m) * J;
//
//	return J;
//}
//
//int main() {
//
//	vector<vector<int>> ELO;
//	vector<int>elotemp;
//
//	elotemp.push_back(1291);
//	elotemp.push_back(1234);
//	ELO.push_back(elotemp);
//	elotemp.clear();
//
//	elotemp.push_back(3245);
//	elotemp.push_back(2342);
//	ELO.push_back(elotemp);
//	elotemp.clear();
//
//	elotemp.push_back(3445);
//	elotemp.push_back(2642);
//	ELO.push_back(elotemp);
//	elotemp.clear();
//
//	elotemp.push_back(3465);
//	elotemp.push_back(2692);
//	ELO.push_back(elotemp);
//	elotemp.clear();
//
//	elotemp.push_back(3765);
//	elotemp.push_back(2792);
//	ELO.push_back(elotemp);
//	elotemp.clear();
//
//	for (auto x : ELO) {
//		for (auto y : x)
//			cout << y << ",";
//		cout << endl;
//	}
//
//	vector<int> y;
//	y.push_back(1);
//	y.push_back(0);
//	y.push_back(0);
//	y.push_back(1);
//	y.push_back(1);
//
//	vector<double> parameters;
//	parameters.push_back(0.5);
//	parameters.push_back(0.5);
//	parameters.push_back(0.5);
//	parameters.push_back(0.5);
//	parameters.push_back(0.5);
//
//	int m = 5;
//
//
//
//	cout << ConstFunction(ELO,y,m,parameters);
//}
