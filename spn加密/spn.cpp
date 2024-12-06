#include<iostream>
#include<math.h>
#include<string>
using namespace std;
int S[16] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
int P[16] = { 1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16 };

int NR = 4;

void cal_k(int* K, int* k, int r) {
	int i = 4 * (r) - 4;
	int j = i + 16;
	int t = 0;
	for (; i < j; i++) {
		k[t] = K[i];
		t++;
	}
}

void intToBinary(int num, int* a) {
	// 用于存储二进制位的数组
	int index = 3; // 数组的索引，从最高位开始

	// 将整数转换为二进制
	for (; index >= 0; index--) {
		a[index] = num & 1;
		num >>= 1;
	}
}


void cal_v(int* U, int* v) {
	int i = 0;
	int num=0;
	for (; i < 4; i++) {
		num = 0;
		int t = 3;
		for (int j = i * 4; j < i * 4 + 4; j++) {
			num += pow(2, t) * U[j];
			
			t--;
			
		}
		
		num = S[num];
		int binary[4] = { 0 };
		intToBinary(num, binary);
		t = 0;
		for (int j = i * 4; j < i * 4 + 4; j++) {
			v[j] = binary[t];
			t++;
		}
	}
}


int main() {
	string s;
	cin >> s;
	string t;
	cin >> t;
	int x[16];
	int K[32];
	for (int i = 0; i < 32; i++) {
		K[i] = t[i] - '0';
	}
	for (int i = 0; i < 16; i++) {
		x[i]=s[i]-'0';
	}
	
	int w[16];
	int k[16];
	int U[16];
	int v[16];
	for (int i = 0; i < 16; i++) {
		w[i]=x[i];
	}
	for (int i = 1; i <= NR-1; i++) {
		cal_k(K, k, i);
		
		for (int i = 0; i < 16; i++) {
			if (k[i] == w[i]) {
				U[i] = 0;
				
			}
			else {
				U[i] = 1;
			}
		}
		
		
		cal_v(U, v);
		
		for (int j = 0; j < 16; j++) {
			w[j] = v[P[j]-1];
		}
		
	}
	
	cal_k(K, k, NR); 
	
	for (int i = 0; i < 16; i++) {
		if (k[i] == w[i]) {
			U[i] = 0;

		}
		else {
			U[i] = 1;
		}
	}
	
	cal_v(U, v);
	cal_k(K, k, NR+1);
	int result[16];
	for (int i = 0; i < 16; i++) {
		if (k[i] == v[i]) {
			result[i] = 0;

		}
		else {
			result[i] = 1;
		}
	}
	for (int i = 0; i < 16; i++) {
		cout << result[i];
	}

	system("pause");
	return 0;
}