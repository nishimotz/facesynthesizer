#	 ���p�b�P�[�W�������s�t�@�C�����ɏ���������
PACKAGE	= ../bin/fsm
#	 ���\�[�X�t�@�C�����������ׂ�
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

#	 ���w�b�_�t�@�C�����������ׂ�
HEADS	= 

OBJS	= $(SRCS:.cpp=.o)

#	 ���p�b�P�[�W�Ɋ܂߂�t�@�C�������������ׂ�
FILES	= README Makefile $(HEADS) $(SRCS)
VER	= `date +%Y%m%d`


### command and flags ###
# uncomment when debugging
#DEBUG	= -ggdb -pg # -lefence

# common (*.o)
LD	= g++
LDFLAGS	= $(DEBUG)
LDLIBS	= -lglut -lGL -lGLU -lm -L/usr/X11R6/lib -L./glpng/lib -lglpng
LC++INCS = -I/usr/X11R6/include -I./glpng/include

# C (*.c)
CC	= gcc
CFLAGS	= -g -O2 $(DEBUG)
CPPFLAGS= -I.

# C++ (*.cpp *.c++)
CXX	= g++
CXXFLAGS= -O2 $(DEBUG)


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
#	 ���T���v���f�[�^�Ŏ��s����悤�ɏ�������
#	    $(PACKAGE) �͎��s�t�@�C�����ɓW�J�����
#	    �s���̓^�u�i�X�y�[�X�ł͂Ȃ��j
	./$(PACKAGE) < sample-data | less