#pragma once
#include <iomanip>
#include <iostream>
#include <fstream>
using namespace std;
class CUser
{
private:
	/* Class attributes */
	string m_name,
		m_password;
	char m_type;
public:
	/* Class constructors */
	CUser(void);
	CUser(string, string);
	CUser(string, string, char);
	/* Class methods */
	bool HasUser(string);
	bool passwordCheck(string);
	bool IsManager(void);
	string GetName(void);
	string SaveUser(void);
	char GetType(void);
	void SetName(string);
	void SetPassword(string);
	void SetType(char);
};