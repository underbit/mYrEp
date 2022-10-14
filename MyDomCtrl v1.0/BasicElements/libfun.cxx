#include "libfun.hh"

string IntToString(int i) {
	string appo = "";
	if(i == 0) appo = "0";
	else {
		while(i > 0) {
			appo = (char) ((i % 10) + 48) + appo;
			i = i / 10;
		}
	}
	return appo;
}

string FloatToString(float i) {
	string appo = IntToString(i);
	int a = i;
	float rest = i - a;
	if(i>0) appo += ".";
	while(i > 0) {
		rest *= 10;
		int intero = rest;
		appo += (char) ((intero % 10) + 48);
	}
	return appo;
}

float StringToFloat(string s) {
	float appo = 0;
	unsigned int i = 0, d = 0;
	bool dec = false;
	bool neg = false;
	while(i < s.length()) {
		if(s[i] == '-') { 
			neg = true;
			i++;
			continue;
		}
		if(s[i] == '.' || s[i] == ',') {
			dec = true;
			i++;
			continue;
		}
		if(s[i]>= '0' && s[i] <= '9') {
			if(dec) d++;
			appo = appo * 10 + (int)s[i] - 48;
		}
		i++;
	}
	if(dec) {
		int esp = 10;
		for(unsigned int ind = 1; ind < d; ind++)
			esp *= 10;
		appo /= esp;
	}
	if (neg) appo = -appo; 
	return appo;
}

int StringToInt(string s) {
	int appo = 0;
	unsigned int i = 0;
	bool neg = false;
	while(i < s.length()) {
		if(s[i] == '-') { 
			neg = true;
			i++;
			continue;
		}
		if(s[i] == '.' || s[i] == ',') break;
		if(s[i]>= '0' && s[i] <= '9') 			
			appo = appo * 10 + (int)s[i] - 48;
		i++;
	}
	if (neg) appo = -appo; 
	return appo;
}

bool StringToBool(string s) {
	if(s.length() > 0) {
		if(s[0] == '0') return false;
		else return true;
	} else return false;
}
