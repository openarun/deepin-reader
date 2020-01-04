QT += core gui svg sql printsupport
QT += dtkwidget
QT += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#private
QT       += gui-private

TARGET = deepin-reader
TEMPLATE = app
CONFIG += c++11 link_pkgconfig

include (frame/frame.pri)
include (utils/utils.pri)
include (controller/controller.pri)
include (pdfControl/pdfControl.pri)
include (docview/docview.pri)
include (subjectObserver/subjectObserver.pri)
include (mainShow/mainShow.pri)
include (CustomControl/CustomControl.pri)

SOURCES += \
        main.cpp \
    application.cpp\

RESOURCES +=         \
    icons.qrc

HEADERS +=\
    application.h

TRANSLATIONS += \
    translations/deepin-reader_en_US.ts\
    translations/deepin-reader_zh_CN.ts


DISTFILES += \
    themes/solarized_light.theme \
    themes/atom_dark.theme \
    themes/deepin_dark.theme \
    themes/solarized_dark.theme \
    themes/deepin.theme

APPICONDIR = $$PREFIX/share/icons/deepin/apps/scalable

isEmpty(BINDIR):BINDIR=/usr/bin
isEmpty(APPDIR):APPDIR=/usr/share/applications
isEmpty(DSRDIR):DSRDIR=/usr/share/deepin-reader

target.path = $$INSTROOT$$BINDIR

desktop.path = $$INSTROOT$$APPDIR
desktop.files = $$PWD/deepin-reader.desktop

icon_files.path = /usr/share/icons/hicolor/scalable/apps
icon_files.files = $$PWD/icons/deepin/builtin/deepin-reader.svg

CONFIG(release, debug|release) {
    #遍历目录中的ts文件，调用lrelease将其生成为qm文件
    for(tsfile, TRANSLATIONS) {
        qmfile = $$replace(tsfile, .ts$, .qm)
        system(lrelease $$tsfile -qm $$qmfile) | error("Failed to lrelease")
    }
    #将qm文件添加到安装包
    dtk_translations.path = /usr/share/$$TARGET/translations
    dtk_translations.files = $$PWD/translations/*.qm
    INSTALLS += dtk_translations
}

INSTALLS += target desktop icon_files





