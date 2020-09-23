#ifndef QPUSHBUTTONEX_H
#define QPUSHBUTTONEX_H

#define VALUE_SIZE_MAX 4*1024

#include <QtWidgets/QPushButton>

class QPushButtonEx : public QPushButton
{
	Q_OBJECT

public:
	QPushButtonEx(QWidget *parent);
	QPushButtonEx(const QString &text, QWidget *parent=0);
	~QPushButtonEx();

	bool getValue(char *chBuf,  uint &nSize);
	bool setValue(char *chBuf, const uint nSize);
	void setExtUiInfo(QString extUiIdentity, QString extUiVersion, QString accessMode);

private:

	QString strExtUiIdentity;
	QString strExtUiVersion;
	QString strAccessMode;

	char *chValueBuf;
	uint nValueSize;
	
};

#endif // QPUSHBUTTONEX_H
