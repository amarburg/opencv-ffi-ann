

CXXFLAGS = -I$(TOP_DIR)/ext \
	   -I$(GTEST_DIR)/include \
	   -I$(RICE_DIR)/include \
	   -I$(RB_INCLUDE_ARGS) \
	   -Wall -ggdb

LDFLAGS = 

LIBS    = -L$(GTEST_DIR)/build -lgtest \
	  -lopencv_core -lopencv_features2d \
	  -lpthread -lm \
	  -lstdc++ \
	  $(RB_LIB_ARGS) \
	  -L$(TOP_DIR)/lib -lcvffi_ann_c

SRCS = main.cpp

test_app: $(SRCS:.cpp=.o)
	gcc $(LDFLAGS) $^ $(LIBS) -o $@

%.o : %.cpp
	g++ $(CXXFLAGS) -c -o $@ $^

test: test_app
	LD_LIBRARY_PATH="$(G2O_DIR)/lib:$(TOP_DIR)/lib:/home/aaron/.rvm/rubies/ruby-2.0.0-p353/lib" ./test_app

gdb: test_app
	LD_LIBRARY_PATH="$(G2O_DIR)/lib:$(TOP_DIR)/lib:/home/aaron/.rvm/rubies/ruby-2.0.0-p353/lib" gdb ./test_app

.PHONY: clean

clean:
	rm -f *.o test_app
