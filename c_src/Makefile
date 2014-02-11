ETERM_PROJECT_PATH = c_src/eterm
ERLOCI_DRV_PATH = c_src/erloci_drv
ERL_INTERFACE_DIR = $(OTPROOT)/lib/erl_interface-3.7.11

ETERM_SRCS = 	$(wildcard $(ETERM_PROJECT_PATH)/*.cpp)

ETERM_OBJS =	$(ETERM_SRCS:.cpp=.o)

CXXFLAGS = -ggdb -Wall -I$(ERL_INTERFACE_DIR)/include -I$(ETERM_PROJECT_PATH)
LINKDIRS = -L$(ERL_INTERFACE_DIR)/lib -Lpriv
LINKFLAGS = -levent -lpthread -lerl_interface -lei

all: priv/eterm
	@echo "erterm compiled"

priv/eterm: $(ETERM_OBJS)
	g++ -Wall $(LINKDIRS) $^ $(LINKFLAGS) -o $@

.cpp.o:
	g++ $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf c_src/eterm/*.o
	rm -rf priv/eterm
