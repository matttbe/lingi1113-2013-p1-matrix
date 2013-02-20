CC= gcc
#CC= clang
# -Werror # -pg
CFLAGS= -Wall -std=c99 -lpthread
OUT= matrixprod
FILE= main.c
FILES= matrix.o DLinkedList.o consumer.o producer.o matrix_reader.o

all: $(OUT)
	## You can launch the tests with: $ make run ##

$(OUT): $(FILE) $(FILES)
	$(CC) $< $(FILES) -o $(OUT) $(CFLAGS)

DLinkedList.o: DLinkedList.c
	$(CC) $(CFLAGS) -c $< -o $@
	
consumer.o: consumer.c
	$(CC) $(CFLAGS) -c $< -o $@
	
producer.o: producer.c
	$(CC) $(CFLAGS) -c $< -o $@

matrix.o: matrix.c
	$(CC) $(CFLAGS) -c $< -o $@

matrix_reader.o: matrix_reader.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(OUT)
	./$(OUT) matrixexemple

.PHONY: clean

clean:
	@rm -f $(OUT) $(FILES)
