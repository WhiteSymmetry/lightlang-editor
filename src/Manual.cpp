#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QTextBrowser>
#include <QtGui/QListWidget>
#include <QtGui/QSplitter>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QLocale>
#include <QtCore/QSettings>
#include <QtCore/QFileInfo>
#include "const.h"
#include "Manual.h"

Manual::Manual(QWidget *parent) : QDialog(parent)
{
	language = QLocale().name();
	language = language.remove(language.indexOf("_"),language.length());

	headerLabel = new QLabel;
	headerLabel->setText("<center><b><font size=\'4\'>" + tr("LightLang Editor\'s manual") + "</font></b></center>");
	// Create push buttons
	// backward button
	backwardButton = new QPushButton;
	backwardButton->setIcon(QIcon(":/icons/backward.png"));
	backwardButton->setIconSize(QSize(22,22));
	backwardButton->setFlat(true);
	backwardButton->setEnabled(false);
	backwardButton->adjustSize();
	backwardButton->setFixedSize(backwardButton->size());
	// Next button
	forwardButton = new QPushButton;
	forwardButton->setIcon(QIcon(":/icons/forward.png"));
	forwardButton->setIconSize(QSize(22,22));
	forwardButton->setFlat(true);
	forwardButton->setEnabled(false);
	forwardButton->adjustSize();
	forwardButton->setFixedSize(forwardButton->size());
	//=========================
	// Create list widget
	listWidget = new QListWidget;
	listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	connect(listWidget,SIGNAL(currentRowChanged(int)),this,SLOT(changePage(int)));
	// Create text browser
	browser = new QTextBrowser;
	
	splitter = new QSplitter;
	splitter->insertWidget(0,listWidget);
	splitter->insertWidget(1,browser);
	splitter->setChildrenCollapsible(false);
	splitter->setStretchFactor(1,1);
	
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.beginGroup("GeneralSettings");
	splitter->restoreState(settings.value("ManualState").toByteArray());
	settings.endGroup();
	
	addItem(tr("About LightLang Editor"),"about.html");
	addItem(tr("Tags and dictionary's format"),"tags.html");
	addItem(tr("How to use the program"),"howtouse.html");
	addItem(tr("Interaction of Editor and SL"),"interaction.html");
	addItem(tr("Integradable friend applications"),"ifa.html");
	addItem(tr("Bugs and offers"),"bugs.html");
	addItem(tr("Authors and thanks"),"authors.html");
	addItem(tr("Internet links"),"links.html");
	addItem(tr("List of changes"),"changelog.html");
	addItem(tr("License"),"license.html");
	listWidget->setCurrentRow(0);
	
	// Create connections 
	connect(backwardButton,SIGNAL(clicked()),this,SLOT(backward()));
	connect(forwardButton,SIGNAL(clicked()),this,SLOT(forward()));
	connect(browser,SIGNAL(backwardAvailable(bool)),backwardButton,SLOT(setEnabled(bool)));
	connect(browser,SIGNAL(forwardAvailable(bool)),forwardButton,SLOT(setEnabled(bool)));
	connect(browser,SIGNAL(anchorClicked(const QUrl&)),this,SLOT(changePage(const QUrl&)));
	
	// Create top Layout with home, backward and forward buttons
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addWidget(backwardButton);
	topLayout->addWidget(forwardButton);
	topLayout->addWidget(headerLabel,1);
	
	//Create Main Layout
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(splitter,1);
	mainLayout->setMargin(0);

	setLayout(mainLayout);
	setWindowTitle(tr("Manual"));
	setWindowIcon(QIcon(":/icons/manual.png"));
	resize(600,300);
}

Manual::~Manual() {
	delete backwardButton;
	delete forwardButton;
	delete browser;
	delete listWidget;
	delete splitter;
	delete headerLabel;
}

QByteArray Manual::getState()
{
	return splitter->saveState();
}

void Manual::addItem(const QString title,const QString url)
{
	QListWidgetItem *item = new QListWidgetItem;
	item->setData(1,url);
	item->setText(title);
	listWidget->addItem(item);
}

void Manual::changePage(int index)
{
	browser->setSource(QUrl(DOC_PATH + language + '/' + listWidget->item(index)->data(1).toString()));
}

void Manual::changePage(const QUrl& url)
{
	QString name = QFileInfo(url.toString()).fileName();
	for (int i = 0; i < listWidget->count(); i++)
		if (name == listWidget->item(i)->data(1).toString())
		{
			listWidget->blockSignals(true);
			listWidget->setCurrentRow(i);
			listWidget->blockSignals(false);
		}
}

void Manual::backward()
{
	browser->backward();
	QString name = QFileInfo(browser->source().toString()).fileName();
	for ( int i = 0; i < listWidget->count(); i++ )
		if ( name == listWidget->item(i)->data(1).toString() )
		{
			listWidget->blockSignals(true);
			listWidget->setCurrentRow(i);
			listWidget->blockSignals(false);
		}
}

void Manual::forward()
{
	browser->forward();
	QString name = QFileInfo(browser->source().toString()).fileName();
	for ( int i = 0; i < listWidget->count(); i++ )
		if ( name == listWidget->item(i)->data(1).toString() )
		{
			listWidget->blockSignals(true);
			listWidget->setCurrentRow(i);
			listWidget->blockSignals(false);
		}
}
