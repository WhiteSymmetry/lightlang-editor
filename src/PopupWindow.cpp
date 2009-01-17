#include <QtGui/QToolButton>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QTextBrowser>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QCursor>
#include "PopupWindow.h"

PopupWindow::PopupWindow(QWidget *parent) : QDialog(parent) {

	setWindowFlags(Qt::Popup);

	header = new QLabel;

	browser = new QTextBrowser;

	cursor = new QCursor();
	
	closeButton = new QToolButton;
	closeButton->setAutoRaise(true);
	closeButton->setIcon(QIcon(":/icons/close.png"));
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));

	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(header);
	topLayout->addStretch();
	topLayout->addWidget(closeButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(browser);
	mainLayout->setMargin(3);
	mainLayout->setSpacing(3);

	setLayout(mainLayout);
	resize(300,200);
}

PopupWindow::~PopupWindow() {
	delete browser;
	delete closeButton;
	delete header;
	delete cursor;
}

void PopupWindow::setText(const QString &text) {
	browser->setHtml(text);
}

void PopupWindow::setHeaderText(const QString &headerText) {
	header->setText("<b>" + headerText + "</b>");
}

void PopupWindow::showPopup() {
	int x = 0;
	int y = 0;
	x = cursor->pos().x();
	y = cursor->pos().y();
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x + width() > QApplication::desktop()->width())
		x = QApplication::desktop()->width() - width() - 10;
	if (y + height() > QApplication::desktop()->height())
		y = QApplication::desktop()->height() - height() - 10;
	move(QPoint(x,y));
	show();
}

