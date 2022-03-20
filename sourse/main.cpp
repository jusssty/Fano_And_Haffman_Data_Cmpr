#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map> 
#include <iomanip>
#include <cmath>
#include "windows.h"
#define elif else if
#define CRED SetConsoleTextAttribute(hConsole, 4);
#define CBLUE SetConsoleTextAttribute(hConsole, 9);
#define CYELLOW SetConsoleTextAttribute(hConsole, 6);
#define CGREEN SetConsoleTextAttribute(hConsole, 2);
#define CCLASSIC SetConsoleTextAttribute(hConsole, 14);
#define CBRIGHT_BLUE SetConsoleTextAttribute(hConsole, 11);

using namespace std;

vector <pair <char, int> > alph;
vector <pair <string, int> > alph_hf;
map <char, string> code;
map <string, char> decode;
const int INT_MAX = 2147483647;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

bool comp(pair <char, int> a, pair <char, int> b) {
	return a.second > b.second;
}

bool comp_hf(pair <string, int> a, pair <string, int> b) {
	return a.second < b.second;
}

void find_l(char a) {
	for (int i = 0; i < (int)alph.size(); i++) {
		if (alph[i].first == a) {
			alph[i].second++;
			return;
		}
	}
	alph.push_back(make_pair(a, 1));
	return;
}

void find_l_hf(char a) {
	for (int i = 0; i < (int)alph_hf.size(); i++) {
		if (alph_hf[i].first[0] == a) {
			alph_hf[i].second++;
			return;
		}
	}
	string op;
	op.push_back(a);
	alph_hf.push_back(make_pair(op, 1));
	return;
}

void min_srch(int l, int r) {
	if (r - l <= 1) return;
	int suml = alph[l].second, sumr = 0, rpr, rr;
	for (int i = l + 1; i < r; i++) sumr += alph[i].second;
	rpr = abs(sumr - suml);
	for (int i = l + 1; i < r; i++) {
		suml += alph[i].second; sumr -= alph[i].second;
		rr = i;
		if (rpr < abs(sumr - suml)) break;
		rpr = abs(sumr - suml);
	}
    	for (int i = l; i < rr; i++) code[alph[i].first].push_back('1');
	for (int i = rr; i < r; i++) code[alph[i].first].push_back('0');
	min_srch(l, rr);
	min_srch(rr, r);
	return;
}

void min_srch_hf() {
	if (alph_hf.size() == 1) return;
	int minn = INT_MAX, min1 = INT_MAX, min_it = -1, min1_it = -1;
	for (int i = 0; i < (int)alph_hf.size(); i++) {
		if (alph_hf[i].second < minn) {
			minn = alph_hf[i].second;
			min_it = i;
		}
	}
	for (int i = 0; i < (int)alph_hf.size(); i++) {
		if (alph_hf[i].second < min1 && i != min_it) {
			min1 = alph_hf[i].second;
			min1_it = i;
		}
	}
	for (int i = 0; i < (int)alph_hf[max(min1_it, min_it)].first.size(); i++)
		code[alph_hf[max(min1_it, min_it)].first[i]].push_back('0');
	for (int i = 0; i < (int)alph_hf[min(min1_it, min_it)].first.size(); i++) 
		code[alph_hf[min(min1_it, min_it)].first[i]].push_back('1');
	alph_hf[min1_it].first += alph_hf[min_it].first;
	alph_hf[min1_it].second += alph_hf[min_it].second;
	alph_hf.erase(alph_hf.begin() + min_it);
	min_srch_hf();
	return;
}

void haffman_encrypt() {
	alph_hf.clear();
	alph.clear();
	code.clear();
	string s;
	CGREEN;
	cout << "Enter your message: ";
	CYELLOW;
	fflush(stdin);
	getline(cin, s);
	for (auto i : s) find_l_hf(i);
	for (auto i : s) find_l(i);
	string non;
	for (int i = 0; i < (int)alph_hf.size(); i++) code.insert(make_pair(alph_hf[i].first[0], non));
	sort(alph_hf.begin(), alph_hf.end(), comp_hf);
	sort(alph.begin(), alph.end(), comp);
	min_srch_hf();
	CBRIGHT_BLUE;
	cout << "Code for each letter and frequency:\n";
	for (int i = 0; i < (int)code.size(); i++) {
		reverse(code[alph[i].first].begin(), code[alph[i].first].end());
		cout << alph[i].first << " = " << code[alph[i].first] << ", met " << alph[i].second << " times, code length: "
 		 	<< code[alph[i].first].size() << ", takes " << alph[i].second * code[alph[i].first].size() 
 		 	<< (alph[i].second * code[alph[i].first].size() == 1 ? " bit\n" : " bits\n");
	}
	CBLUE;
	cout << "Fano-code:\nC = \"";
	double len = 0;
	for (int i = 0; i < (int)s.size(); i++) {
		cout << code[s[i]];
		len += code[s[i]].size();
	}
	cout << "\", code length: " << len << ", bit/symbol: " << fixed << setprecision(2) << len / s.size() << endl;
	CRED;
	cout << "END\n\n";
 	return;
}

void fano_encrypt() {
	alph.clear();
	code.clear();
	string s, op;
	CGREEN;
	cout << "Enter your message: ";
	CYELLOW;
	fflush(stdin);
	getline(cin, s);
	for (auto i : s) find_l(i);
	sort(alph.begin(), alph.end(), comp);
	for (int i = 0; i < (int)alph.size(); i++) code.insert(make_pair(alph[i].first, op));
	min_srch(0, alph.size());
	CBRIGHT_BLUE;
	cout << "Code for each letter and frequency:\n";
	for (int i = 0; i < (int)code.size(); i++) {
		cout << alph[i].first << " = " << code[alph[i].first] << ", met " << alph[i].second << " times, code length: "
 		 	<< code[alph[i].first].size() << ", takes " << alph[i].second * code[alph[i].first].size() 
 		 	<< (alph[i].second * code[alph[i].first].size() == 1 ? " bit\n" : " bits\n");
	}
	CBLUE;
	cout << "Fano-code:\nC = \"";
	double len = 0;
	for (int i = 0; i < (int)s.size(); i++) {
		cout << code[s[i]];
		len += code[s[i]].size();
	}
	cout << "\", code length: " << len << ", bit/symbol: " << fixed << setprecision(2) << len / s.size() << endl;
	CRED;
	cout << "END\n\n";
	return;
}

void decrypt() {
	decode.clear();
	string let, cd;
	CBLUE;
	cout << "Write letters and their codes. Mark the space as \"_\". Example: \nA 01\nB 00\n_ 111.\nTo stop typing, enter \"END\";\n";
	fflush(stdin);
	while (true) {
		CYELLOW;
		cin >> let;
		if (let == "END") break;
		cin >> cd;
		if (let.size() != 1) {
			CRED;
			cout << "Invalid format!\n";
			CYELLOW;
			continue;
		}
		decode.insert(make_pair(cd, let[0]));
	}
	if (!decode.size()) {
		CRED;
		cout << "You typed nothing!\n";
		return;
	}
	CGREEN;
	cout << "Enter code:\n";
	string cdd, v_let;
	CYELLOW;
	cin >> cdd;
	CBRIGHT_BLUE;
	cout << "S = \"";
	for (int i = 0; i < (int)cdd.size(); i++) {
		v_let.push_back(cdd[i]);
		if (decode.find(v_let) != decode.end()) {
			cout << decode[v_let];
			v_let.clear();
		}
	}
	cout << "\";\n\n";
	return;
}

int main() {
	int d;
	while (true) {
		CCLASSIC;
		cout << "1) message -> fano-code; 2) message -> haffman-code; 3)decode; 4) clear console; 5) exit;\n";
		cin >> d;
		if (d == 1) fano_encrypt();
		elif (d == 2) haffman_encrypt();
		elif (d == 3) decrypt();
		elif (d == 4) system("cls");
		elif (d == 5) {
			cout << "Goodbye!\n";
			goto the_end;
		}
		else {
			cout << "I have nothing to do...\n";
			the_end:
				system("pause");
				CloseHandle(hConsole);
				return 0;
		}
	}
}