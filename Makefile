CXXFLAGS=-std=c++11 `pkg-config --cflags libssh`
LDFLAGS=-std=c++11 `pkg-config --libs libssh`

all: sshtool

sshtool: main.o sshsession.o picommand.o
	clang++ $(LDFLAGS) $^ -o $@

main.o: main.cpp
	clang++ $(CXXFLAGS) -c $< -o $@

sshsession.o: sshsession.cpp
	clang++ $(CXXFLAGS) -c $< -o $@

picommand.o: picommand.cpp
	clang++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *o sshtool
