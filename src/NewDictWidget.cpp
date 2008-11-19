#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include "NewDictWidget.h"

NewDictWidget::NewDictWidget() {
	lineEdit = new QLineEdit;
	connect(lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(checkNameFormat(const QString&)));
	
	createButton = new QPushButton(tr("Create"));
	createButton->setShortcut(QKeySequence("Return"));
	createButton->setEnabled(false);
	connect(createButton,SIGNAL(clicked()),this,SLOT(create()));
	
	closeButton = new QPushButton;
	closeButton->setShortcut(QKeySequence("Esc"));
	closeButton->setIcon(QIcon(":/icons/cancel.png"));
	closeButton->setFlat(true);
	
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hide()));
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(new QLabel("<b><font size='4'>" + tr("New dictionary creation") + "</font></b>"));
	topLayout->addStretch();
	topLayout->addWidget(closeButton);
	
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(new QLabel(tr("Dictionary name") + ":"));
	bottomLayout->addWidget(lineEdit,1);
	bottomLayout->addWidget(createButton);
	
	QLabel *formatLabel = new QLabel;
	formatLabel->setWordWrap(true);
	formatLabel->setText("<font color='grey'>" + tr("Dictionary name must have format: &lt;name&gt;.&lt;from language&gt;-&lt;to language&gt;") + "</font>");
	
	warningLabel = new QLabel("<font color='red'>" + tr("The dictionary with such name already exists") + "</font>");
	warningLabel->hide();

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(formatLabel);
	mainLayout->addLayout(bottomLayout);
	mainLayout->addWidget(warningLabel);
	setLayout(mainLayout);
}

NewDictWidget::~NewDictWidget() {
	delete createButton;
	delete closeButton;
	delete lineEdit;
	delete warningLabel;
}

void NewDictWidget::showEvent(QShowEvent *) {
	lineEdit->clear();
	lineEdit->setFocus();
}

void NewDictWidget::checkNameFormat(const QString& name) {
	if (name.isEmpty())
		createButton->setEnabled(false);
	QRegExp expression("^(.*\\.[a-z][a-z]\\-[a-z][a-z])$");
	createButton->setEnabled(name.contains(expression) && !invalidNames.contains(name));
	warningLabel->setVisible(invalidNames.contains(name));
}

void NewDictWidget::create() {
	emit (createDictionary(lineEdit->text()));
	hide();
}

void NewDictWidget::setInvalidNames(const QStringList& list) {
	invalidNames = list;
}
