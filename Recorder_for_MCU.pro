QT += charts
QT += serialport
QT += core gui widgets
TEMPLATE = app

TARGET = Recorder_for_MCU

VERSION = 1.0.0

QMAKE_TARGET_COPYRIGHT = (c) Ryabov Evgeniy

win32 {
    QMAKE_TARGET_PRODUCT = Recorder_for_MCU
    QMAKE_TARGET_DESCRIPTION = Recorder_for_MCU
}

PROJECT_ROOT_PATH = $${PWD}/

win32: OS_SUFFIX = win32
linux-g++: OS_SUFFIX = linux

CONFIG(debug, debug|release){
    BUILD_FLAG = debug
    LIB_SUFFIX = d
} else {
    BUILD_FLAG = release
}

LIBS_PATH = $${PROJECT_ROOT_PATH}/lib.$${OS_SUFFIX}/
INC_PATH = $${PROJECT_ROOT_PATH}/include/
IMPORT_PATH = $${PROJECT_ROOT_PATH}/import/
BIN_PATH = $${PROJECT_ROOT_PATH}/bin/$${BUILD_FLAG}/

BUILD_PATH = $${PROJECT_ROOT_PATH}/build/$${BUILD_FLAG}/$${TARGET}/
RCC_DIR = $${BUILD_PATH}/rcc/
UI_DIR = $${BUILD_PATH}/ui/
MOC_DIR = $${BUILD_PATH}/moc/
OBJECTS_DIR = $${BUILD_PATH}/obj/

LIBS += -L$${LIBS_PATH}/
INCLUDEPATH += $${INC_PATH}/
INCLUDEPATH += $${IMPORT_PATH}/

HEADERS += chart.h \
    info_dialog.h \
    file_item_model_dialog.h \
    serialport.h \
    graphwindows.h

SOURCES += main.cpp chart.cpp \
    info_dialog.cpp \
    file_item_model_dialog.cpp \
    serialport.cpp \
    graphwindows.cpp

FORMS += \
    info_dialog.ui \
    file_item_model_dialog.ui
