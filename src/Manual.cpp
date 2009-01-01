#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QToolButton>
#include <QtGui/QTextBrowser>
#include <QtGui/QIcon>
#include <QtGui/QApplication>
#include <QtCore/QDir>
#include <QtCore/QLocale>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include "const.h"
#include "Manual.h"

const int SPEED = 15;

ManualBrowserWithWidgets::ManualBrowserWithWidgets() {
	timer = new QTimer;
	timer->setInterval(5);
	rollToDown = false;
	
	setOpenExternalLinks(true);
	
	buttonsFrame = new QFrame;
	buttonsFrame->setFrameShape(QFrame::Box);
	buttonsFrame->setFrameShadow(QFrame::Raised);
	buttonsFrame->setStyleSheet("QFrame {border: 1px solid gray; border-radius: 4px; background-color: rgb(200, 200, 200, 180)}");
	
	buttonsFrameLayout = new QHBoxLayout;
	buttonsFrame->setLayout(buttonsFrameLayout);
	
	showLinksButton = new QToolButton;
	showLinksButton->setIcon(QIcon(":/icons/manual.png"));
	showLinksButton->setIconSize(QSize(22, 22));
	showLinksButton->setCursor(Qt::ArrowCursor);
	showLinksButton->setAutoRaise(true);
	
	backwardButton = new QToolButton;
	backwardButton->setIcon(QIcon(":/icons/backward.png"));
	backwardButton->setIconSize(QSize(22, 22));
	backwardButton->setCursor(Qt::ArrowCursor);
	backwardButton->setAutoRaise(true);
	backwardButton->setEnabled(false);
	
	forwardButton = new QToolButton;
	forwardButton->setIcon(QIcon(":/icons/forward.png"));
	forwardButton->setIconSize(QSize(22, 22));
	forwardButton->setCursor(Qt::ArrowCursor);
	forwardButton->setAutoRaise(true);
	forwardButton->setEnabled(false);
	
	QFrame *verticalFrame = new QFrame;
	verticalFrame->setFrameStyle(QFrame::VLine | QFrame::Sunken);
	
	buttonsFrameLayout->addWidget(showLinksButton);
	buttonsFrameLayout->addWidget(verticalFrame);
	buttonsFrameLayout->addWidget(backwardButton);
	buttonsFrameLayout->addWidget(forwardButton);
	buttonsFrameLayout->setContentsMargins(0,0,0,0);
	
	buttonsFrame->setFixedSize(buttonsFrameLayout->minimumSize());
	
	linksFrame = new QFrame;
	linksFrame->setFrameShape(QFrame::Box);
	linksFrame->setFrameShadow(QFrame::Raised);
	QColor color = QApplication::palette().color(QPalette::Window);
	QString r = QVariant(color.red()).toString(); 
	QString g = QVariant(color.green()).toString();
	QString b = QVariant(color.blue()).toString();
	linksFrame->setStyleSheet(QString("QFrame {border: 1px solid gray; border-radius: 4px; background-color: rgb(%1, %2, %3, 180)}").arg(r,g,b));
	linksFrame->setMaximumHeight(0);
	
	linksFrameLayout = new QVBoxLayout;
	linksFrame->setLayout(linksFrameLayout);
	linksFrameLayout->setContentsMargins(0,0,0,0);
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addStretch();
	topLayout->addWidget(buttonsFrame);
	topLayout->setContentsMargins(0,0,0,0);
	
	QHBoxLayout *bottomLayout = new QHBoxLayout;
	bottomLayout->addStretch();
	bottomLayout->addWidget(linksFrame);
	bottomLayout->setContentsMargins(0,0,0,0);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addLayout(bottomLayout);
	mainLayout->addStretch();
	mainLayout->setContentsMargins(0,6,18,0);
	setLayout(mainLayout);
	
	connect(showLinksButton,SIGNAL(clicked()),this,SLOT(showLinks()));
	connect(timer,SIGNAL(timeout()),this,SLOT(updateLinksSize()));
	connect(backwardButton,SIGNAL(clicked()),this,SLOT(backward()));
	connect(forwardButton,SIGNAL(clicked()),this,SLOT(forward()));
	connect(this,SIGNAL(backwardAvailable(bool)),backwardButton,SLOT(setEnabled(bool)));
	connect(this,SIGNAL(forwardAvailable(bool)),forwardButton,SLOT(setEnabled(bool)));
}

ManualBrowserWithWidgets::~ManualBrowserWithWidgets() {
	foreach (Link link,links)
		delete link.linkView;
	
	delete timer;
	
	delete showLinksButton;
	delete backwardButton;
	delete forwardButton;
	
	delete buttonsFrameLayout;
	delete buttonsFrame;
	
	delete linksFrameLayout;
	delete linksFrame;
}

void ManualBrowserWithWidgets::addLink(const QString& linkTitle,const QString& sourcePath) {
	QToolButton *linkView = new QToolButton;
	linkView->setToolButtonStyle(Qt::ToolButtonTextOnly);
	linkView->setText(linkTitle);
	linkView->setAutoRaise(true);
	linkView->setCheckable(true);
	linkView->setMaximumHeight(20);
	linksFrameLayout->addWidget(linkView);
	connect(linkView,SIGNAL(clicked()),this,SLOT(changeSource()));
	
	links << Link(linkView,sourcePath);
	if (links.count() == 1)
		setSource(sourcePath);
}

void ManualBrowserWithWidgets::changeSource() {
	foreach (Link link,links) {
		if (link.linkView->isChecked()) {
			link.linkView->setChecked(false);
			setSource(link.linkSource);
		}
	}
	//showLinks();
}

void ManualBrowserWithWidgets::showLinks() {
	rollToDown = !rollToDown;
	timer->start();
}

void ManualBrowserWithWidgets::updateLinksSize() {
	if (rollToDown) {
		if (linksFrame->height() + SPEED >= linksFrame->sizeHint().height())
			linksFrame->setMaximumHeight(linksFrame->sizeHint().height());
		else
			linksFrame->setMaximumHeight(linksFrame->height() + SPEED);
		if (linksFrame->height() != linksFrame->sizeHint().height())
			timer->start();
	} else {
		if (linksFrame->height() - SPEED <= 0)
			linksFrame->setMaximumHeight(0);
		else
			linksFrame->setMaximumHeight(linksFrame->height() - SPEED);
		if (linksFrame->height() != 0)
			timer->start();
	}
	linksFrame->resize(linksFrame->sizeHint());
}

Manual::Manual(QWidget *parent) : QDialog(parent) {
	
	language = QLocale().name();
	language = language.remove(language.indexOf("_"),language.length());
	
	browser = new ManualBrowserWithWidgets;
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addWidget(browser);
	mainLayout->setContentsMargins(0,0,0,0);
	
	setLayout(mainLayout);
	setWindowIcon(QIcon(":/icons/manual.png"));
	setWindowTitle(tr("Documentation"));
	
	// If documentation with user language doesn't exist - show english
	if (!QDir(QDir::toNativeSeparators(QDir::currentPath() + "/doc/" + language)).exists())
		language = "en";
	
	QString documenationPath = QDir::toNativeSeparators(QDir::currentPath() + "/doc/" + language + "/html/");
	
	browser->addLink(tr("About the program"),documenationPath + "about.html");
	browser->addLink(tr("Tags and dictionary's format"),documenationPath + "tags.html");
	browser->addLink(tr("How to use the program"),documenationPath + "howtouse.html");
	browser->addLink(tr("Interaction with SL"),documenationPath + "interaction.html");
	browser->addLink(tr("IFA information"),documenationPath + "ifa.html");
	browser->addLink(tr("Bugs in the program"),documenationPath + "bugs.html");
	browser->addLink(tr("Authors"),documenationPath + "authors.html");
	browser->addLink(tr("Web links"),documenationPath + "links.html");
	browser->addLink(tr("Changelogs"),documenationPath + "changelog.html");
	browser->addLink(tr("License"),documenationPath + "license.html");
	
	setMinimumSize(sizeHint());
}

Manual::~Manual() {
	delete browser;
}

void Manual::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.setValue("Manual/size",size());
}

void Manual::loadSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	resize(settings.value("Manual/size",QSize(500,300)).toSize());
}

