QT += core
QT -= gui

TARGET = WutuDB
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app



INCLUDEPATH+=C:\DevLibrary\boost_1_57_0\src
INCLUDEPATH+=C:\Python34x86\include
INCLUDEPATH+=C:\DevLibrary\zlib1.2.8\vcall-win32\include
INCLUDEPATH+=C:\DevLibrary\poco-1.7.2-all\include
INCLUDEPATH+=C:\DevLibrary\Sockets-2.3.9.9\vc2013-win32\Include
INCLUDEPATH+=C:\DevLibrary\openssl-1.0.1k\vcall-win32-md\include
INCLUDEPATH+=C:\DevLibrary\libbson-1.4.0\src\bson
INCLUDEPATH+=C:\DevLibrary\mongo-c-driver-1.4.0\src\mongoc
INCLUDEPATH+=C:\DevLibrary\rocksdb4.9\vc2013x32md\include





LIBS+=-lUser32
LIBS+=-lws2_32
LIBS+=-lshlwapi
LIBS+=-lSecur32
LIBS+=-lCrypt32
LIBS+=-lBcrypt
LIBS+=-lAdvapi32
LIBS+=-lRpcrt4

LIBS+=-lC:\Python34x86\libs\python34
LIBS+=-lC:\Python34x86\libs\python34_d
LIBS+=-lC:\DevLibrary\openssl-1.0.1k\vcall-win32-md\lib\release\libeay32
LIBS+=-lC:\DevLibrary\openssl-1.0.1k\vcall-win32-md\lib\release\ssleay32
LIBS+=-lC:\DevLibrary\mongo-c-driver-1.4.0\vcall_static_dll_x32\md\Release\mongoc-static-1.0
LIBS+=-lC:\DevLibrary\libbson-1.4.0\vcall_static_dll_x32\mt\Release\bson-static-1.0

Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\boost_python-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\zlib1.2.8\vcall-win32\lib\Debug\zlibstat
Debug:LIBS+=-lC:\DevLibrary\poco-1.7.2-all\src\lib\PocoFoundationd
Debug:LIBS+=-lC:\DevLibrary\poco-1.7.2-all\src\lib\PocoNetd
Debug:LIBS+=-lC:\DevLibrary\Sockets-2.3.9.9\src\Project.net\lib\D\SocketsRMD
Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_serialization-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_regex-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_date_time-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_filesystem-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_system-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_thread-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_chrono-vc120-mt-gd-1_57
Debug:LIBS+=-lC:\DevLibrary\rocksdb4.9\vc2013x32md\Debug\rocksdblib

Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\boost_python-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\zlib1.2.8\vcall-win32\lib\Release\zlibstat
Release:LIBS+=-lC:\DevLibrary\poco-1.7.2-all\src\lib\PocoFoundation
Release:LIBS+=-lC:\DevLibrary\poco-1.7.2-all\src\lib\PocoNet
Release:LIBS+=-lC:\DevLibrary\Sockets-2.3.9.9\src\Project.net\lib\R\Sockets
Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_serialization-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_regex-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_date_time-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_filesystem-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_system-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_thread-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\boost_1_57_0\vc2013-win32\libdll\libboost_chrono-vc120-mt-1_57
Release:LIBS+=-lC:\DevLibrary\rocksdb4.9\vc2013x32md\Release\rocksdblib

SOURCES += main.cpp \
    lebbnativedb.cpp \
    rocksdbkeyvalue.cpp \
    CrossPlastformMutex.cpp \
    MultiIDSetGetSameID.cpp \
    SplitString.cpp

HEADERS += \
    lebbnativedb.h \
    rocksdbkeyvalue.h \
    CrossPlastformMutex.h \
    MultiIDSetGetSameID.h \
    SplitString.h

