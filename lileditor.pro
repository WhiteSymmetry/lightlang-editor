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
	src/StackedWidget.h \
	src/SettingsWidget.h \
	src/StatusBarLabel.h
	
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
	src/StackedWidget.cpp \
	src/SettingsWidget.cpp \
	src/StatusBarLabel.cpp
	
RESOURCES += lileditor.qrc
