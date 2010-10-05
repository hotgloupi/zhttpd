TEMPLATE = app
TARGET = rcon
CONFIG += warn_on qt moc uic debug
QT += network

RESOURCES += \

HEADERS += Rcon.hpp \
           Network.hpp \
           MainWindow.hpp \
           ConnectWindow.hpp \
           GraphWidget.hpp \

FORMS += ui/main.ui \
         ui/connect.ui \
         ui/graph.ui \

SOURCES += main.cpp \
           Rcon.cpp \
           Network.cpp \
           MainWindow.cpp \
           ConnectWindow.cpp \
           GraphWidget.cpp \

