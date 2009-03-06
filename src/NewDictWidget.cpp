#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QToolButton>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtCore/QTimer>
#include "NewDictWidget.h"

NewDictWidget::NewDictWidget() {
	timer = new QTimer;
	timer->setInterval(10);
	connect(timer,SIGNAL(timeout()),this,SLOT(updateSize()));
	rollingToShow = false;
	
	lineEdit = new QLineEdit;
	connect(lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(checkNameFormat(const QString&)));
	
	createButton = new QPushButton(tr("Create"));
	createButton->setShortcut(QKeySequence("Return"));
	createButton->setEnabled(false);
	connect(createButton,SIGNAL(clicked()),this,SLOT(create()));
	
	closeButton = new QToolButton;
	closeButton->setShortcut(QKeySequence("Esc"));
	closeButton->setIcon(QIcon(":/icons/close.png"));
	closeButton->setAutoRaise(true);
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hideWithRolling()));
	
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
	mainLayout->addStretch();
	setLayout(mainLayout);
	
	setMaximumHeight(0);
}

NewDictWidget::~NewDictWidget() {
	delete timer;
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

	QColor color;
	if (name.isEmpty())
		color = QColor(0,0,0);
	else if (createButton->isEnabled())
		color = QColor(0,255,0);
	else
		color = QColor(255,0,0);
	
	QPalette palette;
	palette.setColor(createButton->foregroundRole(),color);
	createButton->setPalette(palette);
}

void NewDictWidget::create() {
	emit (createDictionary(lineEdit->text()));
	hideWithRolling();
}

void NewDictWidget::setInvalidNames(const QStringList& list) {
	invalidNames = list;
}

void NewDictWidget::showWithRolling() {
	show();
	rollingToShow = true;
	timer->start();
}

void NewDictWidget::hideWithRolling() {
	rollingToShow = false;
	timer->start();
}

void NewDictWidget::updateSize() {
	if (rollingToShow) {
		if (height() + 10 >= sizeHint().height())
			setMaximumHeight(sizeHint().height());
		else
			setMaximumHeight(height() + 10);
		if (height() != sizeHint().height())
			timer->start();
		else {
			timer->stop();
		}
	} else {
		if (height() - 10 <= 0)
			setMaximumHeight(0);
		else
			setMaximumHeight(height() - 10);
		if (height() != 0)
			timer->start();
		else {
			timer->stop();
			hide();
		}
	}
	resize(width(),sizeHint().height());
	lineEdit->setFocus();
}

