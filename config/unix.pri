unix{
    LIBS += -lgpgmepp -lqgpgme
}

macx{
LIBS += -stdlib=libc++
QMAKE_CXXFLAGS += -stdlib=libc++
QMAKE_CXXFLAGS += -mmacosx-version-min=10.7
QMAKE_LFLAGS += -mmacosx-version-min=10.7
}
