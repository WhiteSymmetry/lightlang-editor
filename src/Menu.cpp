#include <QtGui/QWidgetAction>
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>
#include <QtGui/QFont>
#include <QtGui/QLabel>
#include <QtGui/QStyle>
#include <QtGui/QIcon>
#include "Menu.h"

Menu::Menu(bool useSpecialHeader) {
	// Init
	headerAction = 0;
	headerActionFrame = 0;
	headerLabel = 0;
	iconLabel = 0;
	// Create header action if useSpecialHeader == true
	if (useSpecialHeader) {
		headerAction = new QWidgetAction(this);
		headerAction->setEnabled(false);
	
		headerActionFrame = new QFrame;
		headerActionFrame->setFrameShape(QFrame::Box);
	
		headerAction->setDefaultWidget(headerActionFrame);
	
		QHBoxLayout *headerActionFrameLayout = new QHBoxLayout();
		headerActionFrameLayout->setMargin(3);
		headerActionFrameLayout->setSpacing(3);
		headerActionFrame->setLayout(headerActionFrameLayout);

		iconLabel = new QLabel;
		headerLabel = new QLabel;
		
		iconSize = style()->pixelMetric(QStyle::PM_SmallIconSize);
		headerActionFrameLayout->insertWidget(-1,iconLabel,0);
		headerActionFrameLayout->insertWidget(-1,headerLabel,20);
		QFont font = headerLabel->font();
		font.setBold(true);
		headerLabel->setFont(font);
	
	
		addAction(headerAction);
	}
}

Menu::~Menu() {
	if (headerAction != 0) {
		delete headerLabel;
		delete iconLabel;
		delete headerActionFrame;
		delete headerAction;
	}
}

void Menu::setHeaderText(const QString &headerText) {
	if (headerLabel != 0)
		headerLabel->setText(headerText);
}

void Menu::setHeaderIcon(const QIcon &icon) {
	if (iconLabel != 0)
		iconLabel->setPixmap(icon.pixmap(iconSize,iconSize));
}