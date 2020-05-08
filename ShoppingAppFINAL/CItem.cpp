#include "CItem.h"
/* Class constructors */
CItem::CItem(void)
	:m_Code(0),
	m_Description(""),
	m_Price(0),
	m_DiscountRate(0) {}
CItem::CItem(int code, string description)
	: m_Code(code),
	m_Description(description),
	m_Price(0),
	m_DiscountRate(0) {}
CItem::CItem(int code, string description, double price)
	: m_Code(code),
	m_Description(description),
	m_Price(price),
	m_DiscountRate(0) {}
CItem::CItem(int code, string description, double price, double discountrate)
	: m_Code(code),
	m_Description(description),
	m_Price(price),
	m_DiscountRate(discountrate) {}
/* Class methods */
// setters
void CItem::SetPrice(double price) { m_Price = price; }
void CItem::SetDiscountRate(double discountrate) { m_DiscountRate = discountrate; }
// getters
int CItem::GetCode(void) { return m_Code; }
string CItem::GetDescription(void) { return m_Description; }
double CItem::GetPrice(void) { return m_Price; }
double CItem::GetDiscountRate(void) { return m_DiscountRate; }
// checks
bool CItem::HasCode(int code)
{
	if (m_Code == code)
		return true;
	return false;
}
bool CItem::HasDiscount(void)
{
	if (m_DiscountRate != 0)
		return true;
	return false;
};
// implementors
void CItem::Display(void)
{
	cout << setw(30) << setfill(' ') << m_Code << setw(30) << m_Description << setw(30) << m_Price << setw(30) << m_DiscountRate << "\n";
}
double CItem::GetDiscount(void)
{
	return m_Price - (m_Price * (m_DiscountRate / 100));
}
void CItem::Delete(void)
{
	m_Code = 0;
	m_Description = "";
	m_Price = 0;
	m_DiscountRate = 0;
}