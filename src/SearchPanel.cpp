#include <QtGui/QListWidget>
#include <QtGui/QLabel>
#include <QtGui/QToolButton>
#include <QtGui/QLineEdit>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QKeyEvent>
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
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
	
	warningLabel = new QLabel("<i>" + tr("The word wasn't found") + "</i>");
	warningLabel->hide();
	
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
	
	showMarkedWordsCheckBox = new QCheckBox(tr("Show all marked words"));
	connect(showMarkedWordsCheckBox,SIGNAL(toggled(bool)),this,SLOT(showAllMarkedWords(bool)));
	
	showAllWordsCheckBox = new QCheckBox(tr("all words"));
	connect(showAllWordsCheckBox,SIGNAL(toggled(bool)),this,SLOT(updateList()));
	
	popupWindow = new PopupWindow;
	
	infoButton = new InfoButton(popupWindow);
	infoButton->setPopupHeaderText(tr("Search panel"));
	infoButton->setPopupText(tr("If you want to find words starts with some substring, you should enter the substring into the top line and press Enter."));
	
	listWidget = new QListWidget;
	connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem *)),this,SLOT(emitSignalToEditTheWord(QListWidgetItem*)));
	
	sortComboBox = new QComboBox;
	sortComboBox->addItem(tr("Unsorted"));
	sortComboBox->addItem(tr("Ascending"));
	sortComboBox->addItem(tr("Descending"));
	sortComboBox->setCurrentIndex(0);
	connect(sortComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateList()));
	
	sortInfoButton = new InfoButton(popupWindow);
	sortInfoButton->setPopupHeaderText(tr("Sorting"));
	sortInfoButton->setPopupText(tr("You can sort the search results in three different ways. If you veberete way \"Unsorted\", then all the words will appear in the order in which you add them to the dictionary. Also you can sort the list in ascending and descending."));
	
	QHBoxLayout *sortLayout = new QHBoxLayout;
	sortLayout->addWidget(sortInfoButton);
	sortLayout->addWidget(new QLabel(tr("Sorting") + ":"));
	sortLayout->addWidget(sortComboBox);
	sortLayout->addStretch();
	
	QHBoxLayout *titleLayout = new QHBoxLayout;
	titleLayout->addWidget(infoButton);
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
	bottomLayout->addWidget(new QLabel(tr("or")));
	bottomLayout->addWidget(showAllWordsCheckBox);
	bottomLayout->addStretch();
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(titleLayout);
	mainLayout->addLayout(lineEditLayout);
	mainLayout->addWidget(showMarkedWordsCheckBox);
	mainLayout->addWidget(warningLabel);
	mainLayout->addWidget(listWidget,1);
	mainLayout->addLayout(sortLayout);
	mainLayout->addLayout(bottomLayout);
	setLayout(mainLayout);
	mainLayout->setContentsMargins(0,0,0,0);
	
	setMaximumWidth(0);
	hide();
}

SearchPanel::~SearchPanel() {
	delete showMarkedWordsCheckBox;
	delete clearLineButton;
	delete searchButton;
	delete popupWindow;
	delete infoButton;
	delete limitSpinBox;
	delete sortComboBox;
	delete sortInfoButton;
	delete showAllWordsCheckBox;
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
	if (!lineEdit->text().isEmpty()) {
		listWidget->clear();
		limitSpinBox->setEnabled(!showAllWordsCheckBox->isChecked());
		if (showAllWordsCheckBox->isChecked()) {
			foreach (QString word,databaseCenter->getWordsStartsWith(lineEdit->text(),0))
				listWidget->addItem(word);
		} else {
			foreach (QString word,databaseCenter->getWordsStartsWith(lineEdit->text(),limitSpinBox->value()))
				listWidget->addItem(word);
		}
		warningLabel->setVisible(listWidget->count() == 0);
		
		if (sortComboBox->currentIndex() == 1)
			listWidget->sortItems();
		else if (sortComboBox->currentIndex() == 2)
			listWidget->sortItems(Qt::DescendingOrder);
	}
}

void SearchPanel::textChanged(const QString& text) {
	searchButton->setEnabled(!text.isEmpty());
	if (text.isEmpty()) {
		warningLabel->hide();
		listWidget->clear();
	}
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

void SearchPanel::showAllMarkedWords(bool show) {
	listWidget->clear();
	limitSpinBox->setEnabled(!show);
	lineEdit->setEnabled(!show);
	searchButton->setEnabled(!show);
	clearLineButton->setEnabled(!show);
	showAllWordsCheckBox->setEnabled(!show);
	if (show) {
		foreach (QString word,databaseCenter->getAllMarkedWords()) {
			QListWidgetItem *markedItem = new QListWidgetItem(QIcon(":/icons/mark.png"),word);
			listWidget->addItem(markedItem);
		}
		
		if (sortComboBox->currentIndex() == 1)
			listWidget->sortItems();
		else if (sortComboBox->currentIndex() == 2)
			listWidget->sortItems(Qt::DescendingOrder);
		
		warningLabel->setText("<i>" + tr("There aren't marked words") + "</i>");
		warningLabel->setVisible(listWidget->count() == 0);
	} else {
		warningLabel->setText("<i>" + tr("The word wasn't found") + "</i>");
		textChanged(lineEdit->text());
		search();
	}
}

void SearchPanel::updateList() {
	showAllMarkedWords(showMarkedWordsCheckBox->isChecked());
}
