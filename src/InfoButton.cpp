#include "InfoButton.h"
#include "PopupWindow.h"

InfoButton::InfoButton(PopupWindow *popup,const QString& text, const QString& header) {
	setIcon(QIcon(":/icons/info.png"));
	setIconSize(QSize(16,16));
	setAutoRaise(true);
	popupWindow = popup;
	popupText = text;
	popupHeader = header;
	connect(this,SIGNAL(clicked()),this,SLOT(setPopupData()));
}

void InfoButton::setPopupText(const QString& text) {
	popupText = text;
}

void InfoButton::setPopupHeaderText(const QString& header) {
	popupHeader = header;
}

void InfoButton::setPopupData() {
	if (popupText.isEmpty())
		popupWindow->setText(tr("Cannot find documentation for it. Try to install new version of MountManager or report about it to developer."));
	else
		popupWindow->setText(popupText);
	if (popupHeader.isEmpty())
		popupWindow->setHeaderText(tr("Popup window"));
	else
		popupWindow->setHeaderText(popupHeader);
	popupWindow->showPopup();
}
