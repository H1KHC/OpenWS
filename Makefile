PLATFORM = $(shell uname -s | perl -pe "s/(.*?)[0-9_].*/\$$1/" | tr '[:upper:]' '[:lower:]')

LDFLAGS = -m$(BIT)
DLIBFLAGS = -fvisibility=hidden

INCLUDE_DIR=include
SRC_DIR=src
LIB_DIR=lib
BIN_DIR=bin
TMP_DIR=tmp

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(TMP_DIR)/%.o, $(wildcard $(SRC_DIR)/*.cpp))

BIT = 32

CXXFLAGS += $(EXTRACXXFLAGS) -std=gnu++11 -I$(INCLUDE_DIR) -m$(BIT)

ifdef DEBUG
	CXXFLAGS += -D_DEBUG -g -Wall -Wextra
	LDFLAGS += -g
else
	CXXFLAGS += -O2
endif

ifdef STATIC
	CXXFLAGS += -DWS_STATIC
else
	CXXFLAGS += -DWS_DLL_IMPLEMENT
endif

include build/Makefile.$(PLATFORM)

.PHONY: all sample pre rebuild distclean clean platform

all: pre $(LIB_FULLNAME)

rebuild: clean all

pre: $(LIB_DIR) $(BIN_DIR) $(TMP_DIR)

ifdef STATIC
$(LIB_FULLNAME): $(OBJECTS)
	ar r $@ $^
else
$(LIB_FULLNAME): $(OBJECTS)
	$(CXX) -shared -o $@ $^ $(LDFLAGS) $(DLIBFLAGS)
endif

$(TMP_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(LIB_DIR):
	mkdir $(LIB_DIR)

$(BIN_DIR):
	mkdir $(BIN_DIR)

$(TMP_DIR):
	mkdir $(TMP_DIR)

distclean:
	rm -rf  $(TMP_DIR) $(OBJECTS)

clean: distclean
	rm -rf $(LIB_DIR) $(BIN_DIR)

include build/Makefile.sample

platform:
	@echo $(PLATFORM)
