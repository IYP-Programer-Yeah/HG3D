#ifndef _hstring_h
#define _hstring_h
#include <math.h>
#include <iostream>
#include <Windows.h>
#define pi 3.1415926535897932384626433832795
//string
static bool rad = 0;
unsigned int hstrlen(const char *string3);
class string
{
public:
	char string1[1000];
	//EQ
	void operator=(const char *string2)
	{
		for (register unsigned int i = 0; i<1000; i++)
		{
			string1[i] = string2[i];
			if (string2[i] == '\0')
				break;
		}
	}
	void operator=(const char string2)
	{
		string1[0] = string2;
		string1[1] = '\0';
	}
	void operator=(string string)
	{
		for (register unsigned int i = 0; i<1000; i++)
		{
			string1[i] = string.string1[i];
			if (string.string1[i] == '\0')
				break;
		}
	}
	//SUM
	string operator+(const char *string2)
	{
		string v;
		unsigned int len1 = strlen(string1);
		v = string1;
		for (register unsigned int i = 0; i <= 1000; i++)
		{
			v.string1[len1 + i] = string2[i];
			if (string2[i] == '\0')
				break;
		}
		return v;
	}
	string operator+(const char string2)
	{
		string c;
		c = string1;
		c.string1[strlen(c.string1)] = string2;
		c.string1[strlen(string1) + 1] = '\0';
		return c;
	}
	string operator+(string strings)
	{
		string v;
		unsigned int len1 = strlen(string1);
		v = string1;
		for (register unsigned int i = 0; i <= 1000; i++)
		{
			v.string1[len1 + i] = strings.string1[i];
			if (strings.string1[i] == '\0')
				break;
		}
		return v;
	}
	//EQTST
	int operator==(const char *string2)
	{
		int x = 0;
		unsigned int slen1 = strlen(string1);
		if (slen1 == strlen(string2))
		{
			for (register unsigned int i = 0; i<slen1; i++)
			{
				if (string1[i] == string2[i])
				{
					x = 1;
				}
				else
				{
					x = 0;
					break;
				}

			}
		}
		return x;
	}
	int operator==(const char string2)
	{
		int x;
		if ((string1[0] == string2) && (string1[1] == '\0'))
		{
			x = 1;
		}
		else
		{
			x = 0;
		}
		return x;
	}
	int operator==(string string)
	{
		int x = 0;
		unsigned int slen1 = strlen(string1);
		if (slen1 == strlen(string.string1))
		{
			for (register unsigned int i = 0; i<slen1; i++)
			{
				if (string1[i] == string.string1[i])
				{
					x = 1;
				}
				else
				{
					x = 0;
					break;
				}

			}
		}
		return x;
	}
};
//strlen
unsigned int hstrlen(const char *string3)
{
	for (register int unsigned i = 0; i<1000; i++)
		if (string3[i] == '\0')
			return i;
	return 1000;
}
//strlen(string)
unsigned int strlen(string string)
{
	return strlen(string.string1);
}
//instr
long int instr(const char *string1, const char *string2, char type = 't', int number = 0)
{
	unsigned int a;
	unsigned int len1;
	unsigned int len2;
	if (type == 'b' || type == 'B')
	{
		len1 = strlen(string1);
		len2 = strlen(string2);
		for (register unsigned int j = number; j<len1; j++){
			for (register unsigned int i = 0; i<len2; i++){
				if (string1[j + i] == string2[i])
					a = i + 1;
				else
				{
					break;
				}
				if (a == len2)
					return j;
			}
		}
		return -1;
	}
	else if (type == 't' || type == 'T')
	{
		len1 = strlen(string1);
		len2 = strlen(string2);
		for (register unsigned int j = number; j<len1; j++){
			for (register unsigned int i = 0; i<len2; i++){
				if (string1[j + i] == string2[i] || ((int)string1[j + i] + 32) == (int)string2[i] || ((int)string1[j + i]) == ((int)string2[i] + 32))
					a = i + 1;
				else
				{
					break;
				}
				if (a == len2)
					return j;
			}
		}
		return -1;
	}
	else
		return -2;
}
//instr(string,string)
long int instr(string string1, string string2, char type = 't', int number = 0)
{
	return instr(string1.string1, string2.string1, type, number);
}
//instr(*char,string)
long int instr(const char *string1, string string2, char type = 't', int number = 0)
{
	return instr(string1, string2.string1, type, number);
}
//instr(string,*char)
int instr(string string1, const char *string2, char type = 't', int number = 0)
{
	return instr(string1.string1, string2, type, number);
}
//strcpy
void strcpy(char *string1, char *string2)
{
	int a;
	a = strlen(string2);
	for (register int i = 0; i<a; i++)
	{
		string1[i] = string2[i];
	}
	string1[a] = '\0';
	string1[a] = '\0';
}
//strcpy(*char,string)
void strcpy(char *string3, string string2)
{
	string2 = string3;
}
//strcpy(string,*char)
void strcpy(string string2, char *string1)
{
	string2 = string1;
}
//strcpy(string,string)
void strcpy(string string2, string string1)
{
	string2 = string1;
}
//strreverse
void strreverse(char *string)
{
	_strrev(string);
}
//strreverse(string)
void strreverse(string string)
{
	_strrev(string.string1);
}
//val
long double val(char *string)
{
	long int b;
	long double c;
	char g;
	unsigned int d = 0;
	long double number = 0;
	unsigned int len;
	int z = 0;
	len = strlen(string);
	if (string[0] == '-')
	{
		z = 1;
		for (register unsigned int i = 1; i <= len; i++)
		{
			string[i - 1] = string[i];
		}
	}
	_strrev(string);
	len = strlen(string);
	for (register unsigned int i = 1; i <= len; i++)
	{
		g = string[d];
		b = (int)g - 48;
		c = (long int)pow(10.0, (long int)d);
		number = b*c + number;
		d++;
		if (b>9 || b<0)
		{
			std::cout << "Val ERROR .The string that you had insert is NOT a NUMBER ." << std::endl << "ERROR code 1::The function will RETURN 0 ." << std::endl;
			return 0;
		}
	}
	if (z == 1)
	{
		number = number*(-1);
	}
	return number;
}
//val(string)
long double val(string string)
{
	long int b;
	long double c;
	char g;
	unsigned int d = 0;
	long double number = 0;
	unsigned int len;
	int z = 0;
	len = strlen(string.string1);
	if (string.string1[0] == '-')
	{
		z = 1;
		for (register unsigned int i = 1; i <= len; i++)
		{
			string.string1[i - 1] = string.string1[i];
		}
	}
	_strrev(string.string1);
	len = strlen(string.string1);
	for (register unsigned int i = 1; i <= len; i++)
	{
		g = string.string1[d];
		b = (int)g - 48;
		c = (long int)pow(10.0, (long int)d);
		number = b*c + number;
		d++;
		if (b>9 || b<0)
		{
			std::cout << "Val ERROR .The string that you had insert is NOT a NUMBER ." << std::endl << "ERROR code 1::The function will RETURN 0 ." << std::endl;
			return 0;
		}
	}
	if (z == 1)
	{
		number = number*(-1);
	}
	return number;
}
void show()
{
	std::cout << "Testing Screen .";
}
int intlength(double a)
{
	int counter = 0;
	while (pow(10.0, counter) <= a)
		counter++;
	return counter;
}
string inttostring(long int num)
{
	string a;
	bool t = 0;
	a = '\0';
	if (num<0)
	{
		num = num*-1;
		t = 1;
	}
	if (num == 0)
		a = "0";
	while (num>0)
	{
		a = a + (char)(num % 10 + 48);
		num = num / 10;
	}
	if (t)
		a = a + '-';
	a = _strrev(a.string1);
	return a;
}
string doubletostring(long double num)
{
	string ret;
	ret = '-';
	if (abs(num)<1 && num<0)
		ret = ret + inttostring((long int)num);
	else
		ret = inttostring((long int)num);
	if (num != int(num))
	{
		long double temp1;
		temp1 = num - int(num);
		ret = ret + '.';
		while (temp1 != int(temp1))
		{
			temp1 = temp1 * 10;
			ret = ret + inttostring(abs((long int)temp1));
			temp1 = temp1 - int(temp1);
		}
	}
	return ret;
}
string Lshift(const char *str, unsigned int zero)
{
	string ret;
	unsigned int len = strlen(str) + 1;
	for (register unsigned int i = zero; i<len; i++)
		ret.string1[i - zero] = str[i];
	return ret;
}
string Lshift(string str, unsigned int zero)
{
	return Lshift(str.string1, zero);
}
static int CSTRErorr, SCSTRErorr, sctcaaoaErorr;
int getCSTRErorr()
{
	int temp = CSTRErorr;
	CSTRErorr = 0;
	return temp;
}
int getSCSTRErorr()
{
	int temp = SCSTRErorr;
	SCSTRErorr = 0;
	return temp;
}
int getsctcaaoaErorr()
{
	int temp = sctcaaoaErorr;
	sctcaaoaErorr = 0;
	return temp;
}
string marknumbers(string input)
{
	string ret;
	unsigned int len = strlen(input.string1);
	ret = "\"";
	for (register unsigned int i = 0; i<len; i++)
	{
		if (input.string1[i] == '*' || input.string1[i] == '/' || input.string1[i] == '^')
		{
			ret = ret + '\"';
			ret = ret + input.string1[i];
			ret = ret + '\"';
		}
		else if (input.string1[i] == '+' || input.string1[i] == '-')
		{
			if (i)
			{
				if (!(input.string1[i - 1] == '*' || input.string1[i - 1] == '/' || input.string1[i - 1] == '^' || input.string1[i - 1] == '+' || input.string1[i - 1] == '-'))
				{
					ret = ret + '\"';
					ret = ret + input.string1[i];
					ret = ret + '\"';
				}
				else
					ret = ret + input.string1[i];
			}
			else
				ret = ret + input.string1[i];
		}
		else
			ret = ret + input.string1[i];
	}
	ret = ret + "\"";
	return ret;
}
string getlast_number(string input)
{
	string ret;
	ret = input;
	_strrev(ret.string1);
	ret = Lshift(ret, instr(ret, "\"") + 1);
	ret.string1[instr(ret, "\"")] = '\0';
	_strrev(ret.string1);
	return ret;
}
string CSTR(string intery)
{
	string ret, spart;
	ret = intery;
	int firstfa;
	firstfa = instr(intery, "!");
	if (firstfa != -1)
	{
		string temp1;
		temp1 = intery;
		temp1.string1[firstfa] = '\0';
		_strrev(temp1.string1);
		int temp2 = strlen(temp1.string1);
		long double temp3 = 1;
		if (instr(temp1, "^")<temp2&&instr(temp1, "^") != -1)
			temp2 = instr(temp1, "^");
		if (instr(temp1, "*")<temp2&&instr(temp1, "*") != -1)
			temp2 = instr(temp1, "*");
		if (instr(temp1, "/")<temp2&&instr(temp1, "/") != -1)
			temp2 = instr(temp1, "/");
		if (instr(temp1, "+")<temp2&&instr(temp1, "+") != -1)
			temp2 = instr(temp1, "+");
		if (instr(temp1, "-")<temp2&&instr(temp1, "-") != -1)
			temp2 = instr(temp1, "-");
		temp2 = strlen(temp1) - 1 - temp2;
		temp1 = intery;
		temp1.string1[firstfa] = '\0';
		temp1 = Lshift(temp1, temp2 + 1);
		for (register long double i = 1; i<atof(temp1.string1) + 1; i++)
			temp3 = temp3*i;
		if (strlen(temp1.string1) == 0)
		{
			CSTRErorr = 11;
			return temp1;
		}
		ret.string1[temp2 + 1] = '\0';
		return (ret + doubletostring(temp3)) + Lshift(intery, firstfa + 1);
	}
	string numbers_marked, temp3;
	numbers_marked = marknumbers(intery);
	int firstpower;
	firstpower = instr(numbers_marked, "\"^\"");
	if (firstpower != -1)
	{
		firstpower++;
		temp3 = numbers_marked;
		temp3.string1[firstpower] = '\0';
		if (strlen((getlast_number(temp3)).string1) == 0)
		{
			CSTRErorr = 1;
			return getlast_number(temp3);
		}
		long double num = atof((getlast_number(temp3)).string1);
		temp3 = numbers_marked;
		temp3 = Lshift(temp3, firstpower + 1);
		_strrev(temp3.string1);
		temp3 = getlast_number(temp3);
		_strrev(temp3.string1);
		if (strlen(temp3.string1) == 0)
		{
			CSTRErorr = 2;
			return temp3;
		}
		long double power = atof(temp3.string1);
		spart = numbers_marked;
		spart = Lshift(spart, firstpower + 2);
		spart = Lshift(spart, instr(spart, "\""));
		ret = numbers_marked;
		_strrev(ret.string1);
		ret = Lshift(ret, strlen(ret.string1) - firstpower + 1);
		ret = Lshift(ret, instr(ret, "\""));
		_strrev(ret.string1);
		temp3 = (ret + doubletostring(pow(num, power))) + spart;
		ret = "";
		for (register unsigned int i = 0; i<strlen(temp3.string1); i++)
			if (temp3.string1[i] != '\"')
				ret = ret + temp3.string1[i];
		return ret;
	}
	int firstmult;
	firstmult = instr(numbers_marked, "\"*\"");
	if (firstmult != -1)
	{
		firstmult++;
		temp3 = numbers_marked;
		temp3.string1[firstmult] = '\0';
		if (strlen((getlast_number(temp3)).string1) == 0)
		{
			CSTRErorr = 3;
			return getlast_number(temp3);
		}
		long double fnum = atof((getlast_number(temp3)).string1);
		temp3 = numbers_marked;
		temp3 = Lshift(temp3, firstmult + 1);
		_strrev(temp3.string1);
		temp3 = getlast_number(temp3);
		_strrev(temp3.string1);
		if (strlen(temp3.string1) == 0)
		{
			CSTRErorr = 4;
			return temp3;
		}
		long double snum = atof(temp3.string1);
		spart = numbers_marked;
		spart = Lshift(spart, firstmult + 2);
		spart = Lshift(spart, instr(spart, "\""));
		ret = numbers_marked;
		_strrev(ret.string1);
		ret = Lshift(ret, strlen(ret.string1) - firstmult + 1);
		ret = Lshift(ret, instr(ret, "\""));
		_strrev(ret.string1);
		temp3 = (ret + doubletostring(fnum*snum)) + spart;
		ret = "";
		for (register unsigned int i = 0; i<strlen(temp3.string1); i++)
			if (temp3.string1[i] != '\"')
				ret = ret + temp3.string1[i];
		return ret;
	}
	int firstdivid;
	firstdivid = instr(numbers_marked, "\"/\"");
	if (firstdivid != -1)
	{
		firstdivid++;
		temp3 = numbers_marked;
		temp3.string1[firstdivid] = '\0';
		if (strlen((getlast_number(temp3)).string1) == 0)
		{
			CSTRErorr = 5;
			return getlast_number(temp3);
		}
		long double fnum = atof((getlast_number(temp3)).string1);
		temp3 = numbers_marked;
		temp3 = Lshift(temp3, firstdivid + 1);
		_strrev(temp3.string1);
		temp3 = getlast_number(temp3);
		_strrev(temp3.string1);
		if (strlen(temp3.string1) == 0)
		{
			CSTRErorr = 6;
			return temp3;
		}
		long double snum = atof(temp3.string1);
		spart = numbers_marked;
		spart = Lshift(spart, firstdivid + 2);
		spart = Lshift(spart, instr(spart, "\""));
		ret = numbers_marked;
		_strrev(ret.string1);
		ret = Lshift(ret, strlen(ret.string1) - firstdivid + 1);
		ret = Lshift(ret, instr(ret, "\""));
		_strrev(ret.string1);
		temp3 = (ret + doubletostring(fnum / snum)) + spart;
		ret = "";
		for (register unsigned int i = 0; i<strlen(temp3.string1); i++)
			if (temp3.string1[i] != '\"')
				ret = ret + temp3.string1[i];
		return ret;
	}
	int firstadd;
	firstadd = instr(numbers_marked, "\"+\"");
	if (firstadd != -1)
	{
		firstadd++;
		temp3 = numbers_marked;
		temp3.string1[firstadd] = '\0';
		if (strlen((getlast_number(temp3)).string1) == 0)
		{
			CSTRErorr = 7;
			return getlast_number(temp3);
		}
		long double fnum = atof((getlast_number(temp3)).string1);
		temp3 = numbers_marked;
		temp3 = Lshift(temp3, firstadd + 1);
		_strrev(temp3.string1);
		temp3 = getlast_number(temp3);
		_strrev(temp3.string1);
		if (strlen(temp3.string1) == 0)
		{
			CSTRErorr = 8;
			return temp3;
		}
		long double snum = atof(temp3.string1);
		spart = numbers_marked;
		spart = Lshift(spart, firstadd + 2);
		spart = Lshift(spart, instr(spart, "\""));
		ret = numbers_marked;
		_strrev(ret.string1);
		ret = Lshift(ret, strlen(ret.string1) - firstadd + 1);
		ret = Lshift(ret, instr(ret, "\""));
		_strrev(ret.string1);
		temp3 = (ret + doubletostring(fnum + snum)) + spart;
		ret = "";
		for (register unsigned int i = 0; i<strlen(temp3.string1); i++)
			if (temp3.string1[i] != '\"')
				ret = ret + temp3.string1[i];
		return ret;
	}
	int firstneg;
	firstneg = instr(numbers_marked, "\"-\"");
	if (firstneg != -1)
	{
		firstneg++;
		temp3 = numbers_marked;
		temp3.string1[firstneg] = '\0';
		if (strlen((getlast_number(temp3)).string1) == 0)
		{
			CSTRErorr = 9;
			return getlast_number(temp3);
		}
		long double fnum = atof((getlast_number(temp3)).string1);
		temp3 = numbers_marked;
		temp3 = Lshift(temp3, firstneg + 1);
		_strrev(temp3.string1);
		temp3 = getlast_number(temp3);
		_strrev(temp3.string1);
		if (strlen(temp3.string1) == 0)
		{
			CSTRErorr = 10;
			return temp3;
		}
		long double snum = atof(temp3.string1);
		spart = numbers_marked;
		spart = Lshift(spart, firstneg + 2);
		spart = Lshift(spart, instr(spart, "\""));
		ret = numbers_marked;
		_strrev(ret.string1);
		ret = Lshift(ret, strlen(ret.string1) - firstneg + 1);
		ret = Lshift(ret, instr(ret, "\""));
		_strrev(ret.string1);
		temp3 = (ret + doubletostring(fnum - snum)) + spart;
		ret = "";
		for (register unsigned int i = 0; i<strlen(temp3.string1); i++)
			if (temp3.string1[i] != '\"')
				ret = ret + temp3.string1[i];
		return ret;
	}
	return intery;
}
//////////////////////////////*******************///////////////////////////////
string sctcaaoa(string intery);
string SCSTR(string intery)
{
	int oppn = 0, clpn = 0, ineropp, inerclp;
	string ret;
	ret = "";
	while (!(ret == intery))
	{
		ret = intery;
		intery = sctcaaoa(intery);
	}
	ret = intery;
	if (strlen(intery.string1) == 0)
	{
		SCSTRErorr = 1;
		return intery;
	}
	for (unsigned int i = 0; i<strlen(intery.string1); i++)
	{
		if (intery.string1[i] == '(')
			oppn++;
		if (intery.string1[i] == ')')
			clpn++;
	}
	if (oppn - clpn>0)
	{
		for (int i = 0; i<oppn - clpn; i++)
			intery = intery + ')';
		oppn = clpn;
	}
	if (oppn - clpn<0)
	{
		string temp;
		temp = "";
		for (int i = 0; i<clpn - oppn; i++)
			temp = temp + '(';
		intery = temp + intery;
	}
	string temp2;
	temp2 = "";
	if (oppn)
	{
		inerclp = instr(intery, ")");
		string temp1;
		temp1 = intery;
		temp1.string1[inerclp] = '\0';
		_strrev(temp1.string1);
		ineropp = strlen(temp1) - 1 - instr(temp1, "(");
		temp1 = intery;
		temp1.string1[inerclp] = '\0';
		temp1 = Lshift(temp1, ineropp + 1);
		ret.string1[ineropp] = '\0';
		while (!(temp1 == temp2))
		{
			temp2 = temp1;
			if (strlen(temp1.string1) != 0)
				temp1 = CSTR(temp1);
			else
				temp1 = "\"0\"";
		}
		ret = ret + temp1;
		ret = ret + Lshift(intery, inerclp + 1);
	}
	else
	{
		while (!(ret == temp2))
		{
			temp2 = ret;
			ret = CSTR(ret);
		}
	}
	return ret;
}
////////////////////////////*******************////////////////////////////////
////////////////////////////*******************/////////////////////////////////
string sctcaaoa(string intery)
{
	int mode, fsctcaaoa, fp = -1, sp = -1, tp = -1;
	string temp1, ret;
	if (instr(intery, "arcsin")>-1)
	{
		mode = 5;
		fsctcaaoa = instr(intery, "arcsin");
	}
	else if (instr(intery, "arccos")>-1)
	{
		mode = 6;
		fsctcaaoa = instr(intery, "arccos");
	}
	else if (instr(intery, "arctan")>-1)
	{
		mode = 7;
		fsctcaaoa = instr(intery, "arctan");
	}
	else if (instr(intery, "arccot")>-1)
	{
		mode = 8;
		fsctcaaoa = instr(intery, "arccot");
	}
	else if (instr(intery, "sin")>-1)
	{
		mode = 1;
		fsctcaaoa = instr(intery, "sin");
	}
	else if (instr(intery, "cos")>-1)
	{
		mode = 2;
		fsctcaaoa = instr(intery, "cos");
	}
	else if (instr(intery, "tan")>-1)
	{
		mode = 3;
		fsctcaaoa = instr(intery, "tan");
	}
	else if (instr(intery, "cot")>-1)
	{
		mode = 4;
		fsctcaaoa = instr(intery, "cot");
	}
	else if (instr(intery, "abs")>-1)
	{
		mode = 9;
		fsctcaaoa = instr(intery, "abs");
	}
	else if (instr(intery, "int")>-1)
	{
		mode = 10;
		fsctcaaoa = instr(intery, "int");
	}
	else if (instr(intery, "sqr")>-1)
	{
		mode = 11;
		fsctcaaoa = instr(intery, "sqr");
	}
	else if (instr(intery, "log")>-1)
	{
		mode = 12;
		fsctcaaoa = instr(intery, "log");
	}
	else if (instr(intery, "ln")>-1)
	{
		mode = 13;
		fsctcaaoa = instr(intery, "ln");
	}
	else
	{
		return intery;
	}
	temp1 = Lshift(intery, fsctcaaoa);
	if (instr(temp1, "(")>-1)
		fp = instr(temp1, "(") + fsctcaaoa;
	int tempint = 0;
	for (unsigned int i = fp - fsctcaaoa; i<strlen(temp1.string1); i++)
	{
		if (temp1.string1[i] == '(')
			tempint++;
		if (temp1.string1[i] == ')')
			tempint--;
		if (tempint == 0)
		{
			sp = i + fsctcaaoa;
			break;
		}
		if (tempint == 1 && mode == 12 && temp1.string1[i] == ',')
			tp = i + fsctcaaoa;
	}
	if (fp != -1 && sp != -1)
	{
		long double temp2;
		temp1 = intery;
		temp1.string1[sp] = '\0';
		temp1 = Lshift(temp1, fp + 1);
		string tempstr;
		if (tp == -1)
		{
			tempstr = "";
			while (!(tempstr == temp1))
			{
				tempstr = temp1;
				temp1 = SCSTR(temp1);
			}
		}
		else
		{
			tempstr = "";
			string sptemp;
			sptemp = temp1;
			temp1.string1[tp - fp - 1] = 0;
			while (!(tempstr == temp1))
			{
				tempstr = temp1;
				temp1 = SCSTR(temp1);
			}
			tempstr = "";
			sptemp = Lshift(sptemp, tp - fp);
			while (!(tempstr == sptemp))
			{
				tempstr = sptemp;
				sptemp = SCSTR(sptemp);
			}
		}
		if (strlen(temp1.string1) == 0 || (strlen(tempstr.string1) == 0 && tp != -1))
		{
			sctcaaoaErorr = 2;
			return temp1;
		}
		switch (mode)
		{
		case 1:
			temp2 = sin(rad ? atof(temp1.string1) : (atof(temp1.string1)*pi) / 180);
			break;
		case 2:
			temp2 = cos(rad ? atof(temp1.string1) : (atof(temp1.string1)*pi) / 180);
			break;
		case 3:
			temp2 = tan(rad ? atof(temp1.string1) : (atof(temp1.string1)*pi) / 180);
			break;
		case 4:
			temp2 = 1 / (tan(rad ? atof(temp1.string1) : (atof(temp1.string1)*pi) / 180));
			break;
		case 5:
			temp2 = rad ? asin(atof(temp1.string1)) : (asin(atof(temp1.string1)) * 180) / pi;
			break;
		case 6:
			temp2 = rad ? acos(atof(temp1.string1)) : (acos(atof(temp1.string1)) * 180) / pi;
			break;
		case 7:
			temp2 = rad ? atan(atof(temp1.string1)) : (atan(atof(temp1.string1)) * 180) / pi;
			break;
		case 8:
			temp2 = rad ? atan(1 / atof(temp1.string1)) : (atan(1 / atof(temp1.string1)) * 180) / pi;
			break;
		case 9:
			temp2 = abs(atof(temp1.string1));
			break;
		case 10:
			temp2 = int(atof(temp1.string1));
			break;
		case 11:
			temp2 = sqrt(atof(temp1.string1));
			break;
		case 12:
			if (tp == -1)
				temp2 = log((long double)atof(temp1.string1)) / log((long double)10);
			else
				temp2 = log((long double)atof(temp1.string1)) / log((long double)atof(tempstr.string1));
			break;
		case 13:
			temp2 = log((long double)atof(temp1.string1));
			break;
		}
		ret = intery;
		ret.string1[fsctcaaoa] = '\0';
		return (ret + doubletostring(temp2)) + Lshift(intery, sp + 1);
	}
	else
	{
		sctcaaoaErorr = 1;
		ret = "";
		return ret;
	}
}
long double FCSTR(string iintery)
{
	string intery, tmp;
	intery = iintery;
	tmp = "0";
	if (strlen(intery) == 0)
		return 0;
	while (!(intery == tmp))
	{
		tmp = intery;
		intery = SCSTR(intery);
	}
	return atof(intery.string1);
}
void GetAppPath(char APP_PATH[MAX_PATH])
{
	GetModuleFileNameA(NULL, APP_PATH, MAX_PATH);
}
#endif
