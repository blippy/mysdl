#AM_CXXFLAGS = -std=c++14 @SDL2_CFLAGS@
AM_CXXFLAGS = -std=c++14 $(SDL2_CFLAGS)
AM_CXXFLAGS += -DSHAREDIR="\"$(datadir)/mysdl\""


bin_PROGRAMS = mysdl pink

mysdl_LDADD = $(SDL2_LIBS) -lSDL2_mixer -lpthread
mysdl_SOURCES = mysdl.cc

pink_LDADD = $(SDL2_LIBS) -lSDL2_mixer
pink_SOURCES = pink.cc


sharedir = $(datadir)/mysdl
EXTRA_DIST = pink.wav pluck.wav
dist_share_DATA = $(EXTRA_DIST)
