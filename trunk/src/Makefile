
OBJS = main.o testcase.o bb_node.o model.o order_width.o pattern.o knapsack.o cmdline.o reports.o
CFLAGS = -Wall -g
INC = ../include
GLPKINC = /usr/local/include

cspsol: $(OBJS)
	g++ -o cspsol $(OBJS) -lglpk 

datagen: datagen.cpp
	g++ -o datagen datagen.cpp

$(OBJS): $(INC)/*.h

main.o: main.cpp
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) main.cpp

testcase.o: testcase.cpp 
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) testcase.cpp

reports.o: reports.cpp 
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) reports.cpp
	
cmdline.o: cmdline.cpp 
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) cmdline.cpp

knapsack.o: knapsack.cpp 
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) knapsack.cpp

bb_node.o: bb_node.cpp
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) bb_node.cpp

model.o: model.cpp
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) model.cpp

order_width.o: order_width.cpp
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) order_width.cpp

pattern.o: pattern.cpp
	g++ -c $(CFLAGS) -I$(GLPKINC) -I$(INC) pattern.cpp

.PHONY: 
clean:
	rm $(OBJS) cspsol datagen *.lp bb_log.txt
