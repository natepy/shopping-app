#pragma once
#include <iomanip>
#include <fstream>
#include <iostream>
using namespace std;
class CItem
{
private:
	/* Class attributes */
	int m_Code;// unique identifier for the item
	string m_Description;// the item name as well as any other relevent descriptions
	double m_Price, // item price
		m_DiscountRate;// discount percentage
public:
	/* Class constructors */
	CItem(void);// default (null) constructor
	CItem(int, string);// basic (code and description) constructor
	CItem(int, string, double);// price constructor
	CItem(int, string, double, double);// database constructor
	/* Class methods */
	void SetPrice(double);
	void SetDiscountRate(double);
	// Getters
	int GetCode(void);
	string GetDescription(void);
	double GetPrice(void);
	double GetDiscountRate(void);
	// Checks
	bool HasCode(int);
	bool HasDiscount(void);
	// Implementors
	void Display(void);
	double GetDiscount(void);// applys the discount rate
	void Delete(void);// this method will set all attribute values to empty in the event in which the user wants to remove the item form the list
};