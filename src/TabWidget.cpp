#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include "TranslationEditor.h"
#include "DatabaseCenter.h"
#include "TabWidget.h"

TabWidget::TabWidget(DatabaseCenter *dbCenter) {
	databaseCenter = dbCenter;
	
	textEdit = new TranslationEditor;
	
	searchButton = new QPushButton(tr("Find"));
	searchButton->setIcon(QIcon(":/icons/search.png"));
	
	lineEdit = new QLineEdit;
	
	clearLineEditButton = new QPushButton;
	clearLineEditButton->setFlat(true);
	clearLineEditButton->setIcon(QIcon(":/icons/clear.png"));
	
	QHBoxLayout *lineEditLayout = new QHBoxLayout;
	lineEditLayout->addWidget(new QLabel(tr("Phrase") + ":"));
	lineEditLayout->addWidget(lineEdit,1);
	lineEditLayout->addWidget(clearLineEditButton);
	lineEditLayout->addWidget(searchButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(lineEditLayout);
	mainLayout->addWidget(textEdit);
	mainLayout->setMargin(0);
	setLayout(mainLayout);
}

TabWidget::~TabWidget() {
	delete textEdit;
	delete lineEdit;
	delete searchButton;
	delete clearLineEditButton;
}

void TabWidget::setHtml(const QString& htmlText) {
	textEdit->setHtml(htmlText);
}

void TabWidget::setReadOnly(bool readOnly) {
	textEdit->setReadOnly(readOnly);
}
