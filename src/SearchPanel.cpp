#include <QtGui/QListWidget>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QSpinBox>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include "DatabaseCenter.h"
#include "PopupWindow.h"
#include "InfoButton.h"
#include "const.h"
#include "SearchPanel.h"

const int MAXIMUM_WIDTH = 200;

SearchPanel::SearchPanel(DatabaseCenter *dbCenter) {
	databaseCenter = dbCenter;
	
	timer = new QTimer;
	rollingToShow = false;
	timer->setInterval(5);
	connect(timer,SIGNAL(timeout()),this,SLOT(updateSize()));
	
	titleLabel = new QLabel(tr("Dictionary searching"));
	
	lineEdit = new QLineEdit;
	connect(lineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(textChanged(const QString&)));
	
	limitSpinBox = new QSpinBox;
	limitSpinBox->setMinimum(2);
	limitSpinBox->setMaximum(100);
	
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	limitSpinBox->setValue(settings.value("SearchPanel/MaximumWordsToShow",50).toInt());
	connect(limitSpinBox,SIGNAL(valueChanged(int)),this,SLOT(search()));
	
	closeButton = new QToolButton;
	closeButton->setAutoRaise(true);
	closeButton->setIcon(QIcon(":/icons/close.png"));
	closeButton->setIconSize(QSize(16,16));
	connect(closeButton,SIGNAL(clicked()),this,SLOT(hideWithRolling()));
	
	clearLineButton = new QToolButton;
	clearLineButton->setAutoRaise(true);
	clearLineButton->setIcon(QIcon(":/icons/clear.png"));
	clearLineButton->setIconSize(QSize(16,16));
	connect(clearLineButton,SIGNAL(clicked()),lineEdit,SLOT(clear()));
	connect(clearLineButton,SIGNAL(clicked()),lineEdit,SLOT(setFocus()));
	clearLineButton->setFocusPolicy(Qt::ClickFocus);
	
	searchButton = new QToolButton;
	searchButton->setIcon(QIcon(":/icons/search.png"));
	searchButton->setIconSize(QSize(16,16));
	searchButton->setShortcut(QKeySequence("Enter"));
	searchButton->setEnabled(false);
	connect(searchButton,SIGNAL(clicked()),this,SLOT(search()));
	connect(lineEdit,SIGNAL(returnPressed()),searchButton,SLOT(animateClick()));
	searchButton->setFocusPolicy(Qt::ClickFocus);
	
	popupWindow = new PopupWindow;
	
	infoButton = new InfoButton(popupWindow);
	infoButton->setPopupHeaderText(tr("Search panel"));
	infoButton->setPopupText(tr("If you want to find words starts with some substring, you should enter the substring into the top line and press Enter."));
	
	listWidget = new QListWidget;
	connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(emitSignalToEditTheWord(QListWidgetItem*)));
	
	QHBoxLayout *titleLayout = new QHBoxLayout;
	titleLayout->addWidget(titleLabel);
	titleLayout->addStretch();
	titleLayout->addWidget(closeButton);
	
	QHBoxLayout *lineEditLayout = new QHBoxLayout;
	lineEditLayout->addWidget(lineEdit,1);
	lineEditLayout->addWidget(clearLineButton);
	lineEditLayout->addWidget(searchButton);
	
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addWidget(new QLabel(tr("Show")));
	bottomLayout->addWidget(limitSpinBox);
	bottomLayout->addWidget(new QLabel(tr(" words")));
	bottomLayout->addStretch();
	bottomLayout->addWidget(infoButton);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(titleLayout);
	mainLayout->addLayout(lineEditLayout);
	mainLayout->addWidget(listWidget,1);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
	mainLayout->setContentsMargins(0,0,0,0);
	
	setMaximumWidth(0);
}

SearchPanel::~SearchPanel() {
	delete clearLineButton;
	delete searchButton;
	delete popupWindow;
	delete infoButton;
	delete limitSpinBox;
	delete lineEdit;
	delete listWidget;
	delete closeButton;
	delete titleLabel;
	delete timer;
}

void SearchPanel::showWithRolling() {
	show();
	rollingToShow = true;
	timer->start();
	lineEdit->setFocus();
}

void SearchPanel::hideWithRolling() {
	rollingToShow = false;
	timer->start();
}

void SearchPanel::updateSize() {
	if (rollingToShow) {
		if (width() + 30 >= MAXIMUM_WIDTH)
			setMaximumWidth(MAXIMUM_WIDTH);
		else
			setMaximumWidth(width() + 30);
		if (width() != MAXIMUM_WIDTH)
			timer->start();
		else {
			timer->stop();
		}
	} else {
		if (width() - 30 <= 0)
			setMaximumWidth(0);
		else
			setMaximumWidth(width() - 30);
		if (width() != 0)
			timer->start();
		else {
			timer->stop();
			hide();
			emit (closed());
		}
	}
	resize(MAXIMUM_WIDTH,height());
}

void SearchPanel::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Escape)
		hideWithRolling();
	if (event->key() == Qt::Key_Return && listWidget->currentItem() != 0)
		emitSignalToEditTheWord(listWidget->currentItem());
	QWidget::keyPressEvent(event);
}

void SearchPanel::search() {
	listWidget->clear();
	foreach (QString word,databaseCenter->getWordsStartsWith(lineEdit->text(),limitSpinBox->value()))
		listWidget->addItem(word);
}

void SearchPanel::textChanged(const QString& text) {
	searchButton->setEnabled(!text.isEmpty());
	if (text.isEmpty())
		listWidget->clear();
}

void SearchPanel::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.setValue("SearchPanel/MaximumWordsToShow",limitSpinBox->value());
}

void SearchPanel::emitSignalToEditTheWord(QListWidgetItem *item) {
	emit (wordChosen(item->text()));
}

void SearchPanel::setFocusAtLineEdit() {
	lineEdit->setFocus();
}
