INCLUDEPATH += ../src/
LIBS += -lsrc
unix{
LIBS += -L../src/
}
windows{
debug{
LIBS += -L../src/debug/
}
release{
LIBS += -L../src/release/
}
}