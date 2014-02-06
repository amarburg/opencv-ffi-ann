

CXXFLAGS = -I$(TOP_DIR)/ext \
	   -I$(G2O_DIR)/build \
	   -I$(G2O_DIR) \
	   -I/usr/include/eigen3 \
	   -I/usr/include/suitesparse \
	   -I$(GTEST_DIR)/include \
	   -I$(shell rvm gemdir)/gems/rice-1.5.3/ruby/lib/include \
	   -I/home/aaron/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0 \
	   -I/home/aaron/.rvm/rubies/ruby-2.0.0-p353/include/ruby-2.0.0/x86_64-linux -I/home/aaron/.rvm/rubies/ruby-2.0.0-p353/lib/ruby/2.0.0/x86_64-linux -I/home/aaron/.rvm/rubies/ruby-2.0.0-p353/lib/ruby/site_ruby/2.0.0 \
	   -I$(GRAPH_MOSAIC_DIR)/lib \
	   -Wall -ggdb

LDFLAGS = 
LIBS    =-L$(G2O_DIR)/lib -lg2o_core -lg2o_stuff -lg2o_csparse_extension \
	 -lg2o_solver_csparse  \
	 -lcsparse -lcxsparse -lcholmod \
	 -L$(GTEST_DIR)/build -lgtest \
	 -lopencv_core -lopencv_features2d \
	 -lpthread -lm \
	 -lstdc++ \
	 -L/home/aaron/.rvm/rubies/ruby-2.0.0-p353/lib \
	 -lruby \
	 -L$(GRAPH_MOSAIC_DIR)/lib -lgraph_mosaic  \
	 -L$(TOP_DIR)/lib -lmosaic_c

SRCS = optimizer_test.cpp \
       bow_trainer_test.cpp \
       bow_classifier_test.cpp \
       random_descriptors.cpp \
       main.cpp

SCRATCH = /tmp/graph_mosaic

test_app: $(SRCS:.cpp=.o)
	gcc $(LDFLAGS) $^ $(LIBS) -o $@

%.o : %.cpp
	@mkdir -p $(SCRATCH)
	g++ $(CXXFLAGS) -c -o $@ $^

test: test_app
	LD_LIBRARY_PATH="$(G2O_DIR)/lib:$(TOP_DIR)/lib:/home/aaron/.rvm/rubies/ruby-2.0.0-p353/lib" ./test_app

gdb: test_app
	LD_LIBRARY_PATH="$(G2O_DIR)/lib:$(TOP_DIR)/lib:/home/aaron/.rvm/rubies/ruby-2.0.0-p353/lib" gdb ./test_app

.PHONY: clean

clean:
	rm -f *.o test_app
