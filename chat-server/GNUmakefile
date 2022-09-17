PROG_CXX = chat-server
SRCS = chat-server.cpp

CXXFLAGS = -g -std=c++20 -Wall -Wextra -pthread -I../common

all: $(PROG_CXX)

clean:
	- rm $(PROG_CXX) $(SRCS:.cpp=.o)

$(PROG_CXX): $(SRCS:.cpp=.o)
	$(LINK.cpp) -o $@ $^ $(LDADD)

%.d: %.cpp
	$(CXX) -MM $(CXXFLAGS) $< | sed 's,[^:]*:,$*.o $@ :,g' > $@

include $(SRCS:.cpp=.d)
