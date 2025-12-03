# 基础配置
QT += core gui widgets svg
TEMPLATE = lib
CONFIG += plugin c++11 debug
TARGET = peony-emblem-plugin
DESTDIR = $$OUT_PWD/../plugins
MOC_DIR = $$OUT_PWD/moc
OBJECTS_DIR = $$OUT_PWD/obj

# 仅保留 Peony 和 Qt 基础依赖（移除 ExtensionSystem）
INCLUDEPATH += /usr/include/peony \
               $$PWD \
               plugin-iface

LIBS += -lQt5Core \
        -lQt5Widgets \
        -lQt5Svg 

# 安装配置
target.path = /usr/lib/x86_64-linux-gnu/peony-extensions/
INSTALLS += target

HEADERS += \
    plugin-iface/emblem-plugin-iface.h \
    plugin-iface/emblem-provider.h \
    plugin-iface/peony-core_global.h \
    plugin-iface/plugin-iface.h \
    emblem_global.h \
    emblemconstants.h \
    emblemplugin.h

SOURCES += \
    emblemplugin.cpp
