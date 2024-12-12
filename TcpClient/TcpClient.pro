QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addmember.cpp \
    aescbc.cpp \
    book.cpp \
    creategroup.cpp \
    editinfo.cpp \
    friend.cpp \
    groupchat.cpp \
    main.cpp \
    online.cpp \
    opewidget.cpp \
    privatechat.cpp \
    protocol.cpp \
    sharefile.cpp \
    tcpclient.cpp \
    vip.cpp

HEADERS += \
    addmember.h \
    aescbc.h \
    book.h \
    creategroup.h \
    editinfo.h \
    friend.h \
    groupchat.h \
    online.h \
    opewidget.h \
    privatechat.h \
    protocol.h \
    sharefile.h \
    tcpclient.h \
    vip.h

FORMS += \
    editinfo.ui \
    groupchat.ui \
    online.ui \
    privatechat.ui \
    tcpclient.ui

INCLUDEPATH += $$quote(C:/Program Files/OpenSSL-Win64/include)
LIBS += $$quote(C:/Program Files/OpenSSL-Win64/lib/libssl.lib) \
        $$quote(C:/Program Files/OpenSSL-Win64/lib/libcrypto.lib)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    FileType.qrc \
    config.qrc

