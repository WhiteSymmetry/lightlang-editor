TEMPLATE = app
TARGET = lileditor
CONFIG += release
DEPENDPATH += . src
INCLUDEPATH += . src

QT += sql

OBJECTS_DIR = build
MOC_DIR = build

# Input
HEADERS += src/const.h \
	src/MainWindow.h \
	src/TabsWidget.h \
	src/TabWidget.h \
	src/TranslationEditor.h \
	src/HighLighter.h \
	src/NewDictWidget.h \
	src/DatabaseCenter.h \
	src/Manual.h \
	src/About.h \
	src/Menu.h \
	src/TreeWidget.h \
	src/DictionariesManager.h \
	src/LoadDictionaryWidget.h \
	src/CentralWidget.h \
	src/BrowserWithWidgets.h \
	src/BorderPanelWithWidget.h \
	src/LoadDictionaryThread.h \
	src/ProgressBarWithWidgets.h \
	src/SettingsWidget.h \
	src/InfoButton.h \
	src/PopupWindow.h \
	src/StatusBarLabel.h \
	src/EditorTipsWidget.h \
	src/FindInTranslationPanel.h \
	src/SearchPanel.h
	
SOURCES += src/main.cpp \
	src/MainWindow.cpp \
	src/TabsWidget.cpp \
	src/TabWidget.cpp \
	src/TranslationEditor.cpp \
	src/HighLighter.cpp \
	src/NewDictWidget.cpp \
	src/DatabaseCenter.cpp \
	src/Manual.cpp \
	src/About.cpp \
	src/Menu.cpp \
	src/TreeWidget.cpp \
	src/DictionariesManager.cpp \
	src/LoadDictionaryWidget.cpp \
	src/CentralWidget.cpp \
	src/BrowserWithWidgets.cpp \
	src/BorderPanelWithWidget.cpp \
	src/LoadDictionaryThread.cpp \
	src/ProgressBarWithWidgets.cpp \
	src/SettingsWidget.cpp \
	src/InfoButton.cpp \
	src/PopupWindow.cpp \
	src/StatusBarLabel.cpp \
	src/EditorTipsWidget.cpp \
	src/FindInTranslationPanel.cpp \
	src/SearchPanel.cpp
	
RESOURCES += lileditor.qrc

unix {
		LIGHTLANG_PREFIX = $$system(pkg-config lightlang --variable=prefix)
		INSTALL_PREFIX=/usr
		target.path = $$INSTALL_PREFIX/bin
		
		main_icon.files += icons/lle.png
		main_icon.path = $$INSTALL_PREFIX/share/icons/

		xsl_ifa.files += ifa/lle.xml
		xsl_ifa.path = $$LIGHTLANG_PREFIX/lib/xsl/ifa
		
		INSTALLS += target
		INSTALLS += xsl_ifa
}
