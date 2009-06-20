#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QTextBrowser>
#include <QtGui/QIcon>
#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtCore/QDir>
#include <QtCore/QLocale>
#include <QtCore/QSettings>
#include "const.h"
#include "Manual.h"

Manual::Manual(QWidget *parent) : QDialog(parent) {
	
	language = QLocale().name();
	language = language.remove(language.indexOf("_"),language.length());
	
	titleLabel = new QLabel;
	
	textBrowser = new QTextBrowser;
	contentComboBox = new QComboBox;
	
	QHBoxLayout *topLayout = new QHBoxLayout;
	topLayout->addStretch();
	topLayout->addWidget(titleLabel);
	topLayout->addStretch();
	topLayout->addWidget(new QLabel(tr("Content") + ":"));
	topLayout->addWidget(contentComboBox);
	
	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(textBrowser);
	mainLayout->setContentsMargins(1,1,1,1);
	
	setLayout(mainLayout);
	setWindowIcon(QIcon(":/icons/manual.png"));
	setWindowTitle(tr("Documentation"));
	
	// If documentation with user language doesn't exist - show english
	if (!QDir(QDir::toNativeSeparators(QDir::currentPath() + "/doc/" + language)).exists())
		language = "en";
	
	QString documenationPath = QDir::toNativeSeparators(QDir::currentPath() + "/doc/" + language + "/html/");
	
	contentComboBox->addItem(tr("About the program"),documenationPath + "about.html");
	contentComboBox->addItem(tr("Tags and dictionary's format"),documenationPath + "tags.html");
	contentComboBox->addItem(tr("How to use the program"),documenationPath + "howtouse.html");
	contentComboBox->addItem(tr("Interaction with SL"),documenationPath + "interaction.html");
	contentComboBox->addItem(tr("IFA information"),documenationPath + "ifa.html");
	contentComboBox->addItem(tr("Bugs in the program"),documenationPath + "bugs.html");
	contentComboBox->addItem(tr("Authors"),documenationPath + "authors.html");
	contentComboBox->addItem(tr("Web links"),documenationPath + "links.html");
	contentComboBox->addItem(tr("Changelogs"),documenationPath + "changelog.html");
	contentComboBox->addItem(tr("License"),documenationPath + "license.html");
	
	setMinimumSize(sizeHint());
	
	connect(contentComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(showContentNumber(int)));
	
	showContentNumber(0);
}

Manual::~Manual() {
	delete titleLabel;
	delete textBrowser;
	delete contentComboBox;
}

void Manual::saveSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	settings.setValue("Manual/size",size());
}

void Manual::loadSettings() {
	QSettings settings(ORGANIZATION,PROGRAM_NAME);
	resize(settings.value("Manual/size",QSize(500,300)).toSize());
}

void Manual::showContentNumber(int index) {
	textBrowser->setSource(QUrl(contentComboBox->itemData(index).toString()));
	titleLabel->setText("<b><font size='4'>" + contentComboBox->itemText(index) + "</font></b>");
}

