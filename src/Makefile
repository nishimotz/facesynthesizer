#	 ↓パッケージ名かつ実行ファイル名に書き換える
PACKAGE	= ../bin/fsm
#	 ↓ソースファイルを書き並べる
SRCS	= ImageData.cpp\
	SgiImageFormat.cpp\
	Texture.cpp\
	WinBitmapFormat.cpp\
	Wfm.cpp\
	UnitMove.cpp\
	FaceModel.cpp\
	Trans.cpp\
	ObjModel.cpp\
	ObjectModel.cpp\
	SetViseme.cpp\
	FaceView.cpp\
	FaceEditView.cpp\
	FaceEditApp.cpp\
	CommandSet.cpp\
	ReadCommand.cpp\
	Configuration.cpp\
	ControlTime.cpp\
	Main.cpp

#	 ↓ヘッダファイルを書き並べる
HEADS	= 

OBJS	= $(SRCS:.cpp=.o)

#	 ↓パッケージに含めるファイル名を書き並べる
FILES	= README Makefile $(HEADS) $(SRCS)
VER	= `date +%Y%m%d`


### command and flags ###
# uncomment when debugging
#DEBUG	= -ggdb -pg # -lefence

ARCH	= -DMACOSX -arch i386

# common (*.o)
LD	= g++ $(ARCH)
LDFLAGS	= $(DEBUG)
LDLIBS	= -lglut -lGL -lGLU -lm -L/usr/X11R6/lib -L./glpng/lib -lglpng
LC++INCS = -I/usr/X11R6/include -I./glpng/include

# C (*.c)
CC	= gcc
CFLAGS	= -g -O2 $(DEBUG) $(ARCH)
CPPFLAGS= -I.

# C++ (*.cpp *.c++)
CXX	= g++
CXXFLAGS= -O2 $(DEBUG) $(ARCH)


# etc
SHELL	= /bin/sh
RM	= rm -f
PROF	= gprof


### rules ###

.SUFFIXES:
.SUFFIXES: .o .c .cc .c++ .cpp .f .p

all: $(PACKAGE)

$(PACKAGE): $(OBJS)
	$(LD) $(LC++INCS) $(OBJS) $(LDFLAGS) $(LDLIBS)  -o $@

$(OBJS): $(HEADS) Makefile

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LC++INCS) -c $< -o $@
.cc.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LC++INCS) -c $< -o $@
.c++.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LC++INCS) -c $< -o $@
.cpp.o:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LC++INCS) -c $< -o $@

.f.o:
	$(FC) $(FFLAGS) -c $< -o $@
.p.o:
	$(PC) $(PFLAGS) $(CPPFLAGS) -c $< -o $@


### useful commands ###

clean:
#	$(RM) $(PACKAGE) $(OBJS)
	$(RM) core gmon.out *~ #*#
	$(RM) *.o
tar:
	@echo $(PACKAGE)-$(VER) > .package
	@$(RM) -r `cat .package`
	@mkdir `cat .package`
	@ln $(FILES) `cat .package`
	tar cvf - `cat .package` | gzip -9 > `cat .package`.tar.gz
	@$(RM) -r `cat .package` .package

zip:
	zip -9 $(PACKAGE)-$(VER).zip $(FILES)


prof: run
	$(PROF) $(PACKAGE) | less

run: all
#	 ↓サンプルデータで実行するように書き直す
#	    $(PACKAGE) は実行ファイル名に展開される
#	    行頭はタブ（スペースではない）
	./$(PACKAGE) < sample-data | less
