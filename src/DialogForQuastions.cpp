#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include "DialogForQuastions.h"

DialogForQuastions::DialogForQuastions(QWidget *parent) : QDialog(parent) {
	textLabel = new QLabel;
	textLabel->setWordWrap(true);
	
	headerLabel = new QLabel;
	headerLabel->setWordWrap(true);
	
	iconLabel = new QLabel;
	iconLabel->hide();
	
	widgetsLayout = new QHBoxLayout;
		
	QVBoxLayout *iconLabelLayout = new QVBoxLayout;
	iconLabelLayout->addWidget(iconLabel);
	iconLabelLayout->addStretch();
	
	QVBoxLayout *textLabelLayout = new QVBoxLayout;
	textLabelLayout->addWidget(headerLabel);
	textLabelLayout->addWidget(textLabel);
	textLabelLayout->addStretch();
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addLayout(iconLabelLayout);
	topLayout->addLayout(textLabelLayout,1);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(widgetsLayout);
	setLayout(mainLayout);
}

DialogForQuastions::~DialogForQuastions() {
	delete textLabel;
	delete iconLabel;
	delete headerLabel;
}


void DialogForQuastions::setIcon(const QIcon& icon) {
	setIconPixmap(icon.pixmap(64,64));
}
