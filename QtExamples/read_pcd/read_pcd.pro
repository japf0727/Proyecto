QT += core
QT -= gui

CONFIG += c++11

TARGET = read_pcd
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L"usr/include/pcl-1.7/"
