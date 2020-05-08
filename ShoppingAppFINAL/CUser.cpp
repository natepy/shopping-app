#include "CUser.h"
CUser::CUser()
	:m_name(""),
	m_password(""),
	m_type(' ') {}
CUser::CUser(string name, string password)
	: m_name(name),
	m_password(password),
	m_type(' ') {}
CUser::CUser(string name, string password, char type)
	: m_name(name),
	m_password(password),
	m_type(type) {}
bool CUser::HasUser(string name)
{
	if (m_name == name)
		return true;
	return false;
}
bool CUser::IsManager(void)
{
	if (m_type == 'M')
		return true;
	return false;
}
bool CUser::passwordCheck(string password)
{
	if (m_password == password)
		return true;
	return false;
}
string CUser::GetName(void) { return m_name; }
string CUser::SaveUser(void) 
{ 
	return m_name + " " + m_password + " " + m_type + "\n";
}
char CUser::GetType(void) { return m_type; }
void CUser::SetName(string name) { m_name = name; }
void CUser::SetPassword(string password) { m_password = password; }
void CUser::SetType(char type) { m_type = type; }