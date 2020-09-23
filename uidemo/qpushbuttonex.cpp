#include "qpushbuttonex.h"

QPushButtonEx::QPushButtonEx(QWidget *parent)
	: QPushButton(parent)
{
	chValueBuf = NULL;
	nValueSize = 0;
}

QPushButtonEx::QPushButtonEx(const QString &text, QWidget *parent)
	: QPushButton(text,parent)
{
	chValueBuf = NULL;
	nValueSize = 0;

}

QPushButtonEx::~QPushButtonEx()
{
	if (chValueBuf != 0)
	{
		delete []chValueBuf;
		chValueBuf = NULL;
	}
}

bool QPushButtonEx::getValue(char *chBuf,  uint &nSize)
{
	if (nValueSize <= 0 || nValueSize > VALUE_SIZE_MAX ||
		chValueBuf == NULL || chBuf == NULL)
	{
		return false;
	}

	try
	{
		nSize = nValueSize;
		memcpy(chBuf,chValueBuf,nValueSize);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

bool QPushButtonEx::setValue(char *chBuf, uint nSize)
{
	if (nSize <= 0 || nSize > VALUE_SIZE_MAX)
	{
		return false;
	}

	if (chValueBuf != NULL)
	{
		delete []chValueBuf;
		chValueBuf = NULL;
	}

	try
	{
		nValueSize = nSize;
		chValueBuf = new char[nSize];
		memcpy(chValueBuf,chBuf,nSize);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

void QPushButtonEx::setExtUiInfo(QString extUiIdentity, QString extUiVersion, QString accessMode)
{
	strExtUiIdentity = extUiIdentity;
	strExtUiVersion = extUiVersion;
	strAccessMode = accessMode;
}
