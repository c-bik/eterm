ERLOCI_LIB_PATH = c_src/erloci_lib
ERLOCI_DRV_PATH = c_src/erloci_drv
OCI_DIR = c_src/lib/instantclient
ERL_INTERFACE_DIR = $(OTPROOT)/lib/erl_interface-3.7.11

ERLOCI_LIB_SRCS = 	$(ERLOCI_LIB_PATH)/ocisession.cpp	\
				  	$(ERLOCI_LIB_PATH)/ocistmt.cpp		\
					$(ERLOCI_LIB_PATH)/oci_lib_intf.cpp

ERLOCI_LIB_OBJS =	$(ERLOCI_LIB_SRCS:.cpp=.o)

ERLOCI_DRV_SRCS =	$(ERLOCI_DRV_PATH)/cmd_processors.cpp	\
					$(ERLOCI_DRV_PATH)/cmd_thread_pool.cpp 	\
					$(ERLOCI_DRV_PATH)/erloci_drv.cpp 		\
					$(ERLOCI_DRV_PATH)/oci_logger.cpp 		\
					$(ERLOCI_DRV_PATH)/oci_marshal.cpp 		\
					$(ERLOCI_DRV_PATH)/stdafx.cpp 			\
					$(ERLOCI_DRV_PATH)/threadpool.cpp

ERLOCI_DRV_OBJS = $(ERLOCI_DRV_SRCS:.cpp=.o)

CXXFLAGS = -ggdb -Wall -I$(ERL_INTERFACE_DIR)/include  -I$(ERLOCI_DRV_PATH) -I$(ERLOCI_LIB_PATH) -I$(OCI_DIR)/sdk/include/
LINKDIRS = -L$(ERL_INTERFACE_DIR)/lib -Lpriv -L$(OCI_DIR)
LINKFLAGS = -levent -lpthread -lerl_interface -lei -lerloci -locci -lons -lclntshcore -lclntsh -lnnz12

all: priv/erloci
	cp -u $(OCI_DIR)/*.* priv/
	@echo "erloci compiled"

priv/erloci: priv/liberloci.a $(ERLOCI_DRV_OBJS)
	g++ -Wall $(LINKDIRS) $^ $(LINKFLAGS) -o $@

.cpp.o:
	g++ $(CXXFLAGS) -c $< -o $@

# Compiling the common library
$(ERLOCI_LIB_OBJS): %.o : %.h

priv/liberloci.a: $(ERLOCI_LIB_OBJS)
	ar rcs priv/liberloci.a $(ERLOCI_LIB_OBJS)

# Compiling the driver
$(ERLOCI_DRV_PATH)/stdafx.cpp : $(ERLOCI_DRV_PATH)/stdafx.h
$(ERLOCI_DRV_PATH)/threadpool.cpp : $(ERLOCI_DRV_PATH)/threadpool.h
$(ERLOCI_DRV_PATH)/erloci_drv.cpp : $(ERLOCI_DRV_PATH)/stdafx.h $(ERLOCI_DRV_PATH)/cmd_processors.h $(ERLOCI_DRV_PATH)/oci_marshal.h
$(ERLOCI_DRV_PATH)/oci_marshal.cpp : $(ERLOCI_DRV_PATH)/stdafx.h $(ERLOCI_DRV_PATH)/oci_marshal.h
$(ERLOCI_DRV_PATH)/cmd_processors.cpp : $(ERLOCI_DRV_PATH)/cmd_processors.h $(ERLOCI_DRV_PATH)/oci_marshal.h $(ERLOCI_LIB_PATH)/ocisession.h $(ERLOCI_LIB_PATH)/ocistmt.h $(ERLOCI_LIB_PATH)/oci_lib_intf.h
$(ERLOCI_DRV_PATH)/cmd_thread_pool.cpp : $(ERLOCI_DRV_PATH)/stdafx.h $(ERLOCI_DRV_PATH)/cmd_processors.h $(ERLOCI_DRV_PATH)/oci_marshal.h $(ERLOCI_DRV_PATH)/threadpool.h

clean:
	rm -rf c_src/erloci_drv/*.o
	rm -rf c_src/erloci_lib/*.o
	rm -rf priv/*.a
	rm -rf priv/erloci