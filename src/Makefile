CC= gcc
#CC= clang
# -Werror # -pg # -ggdb
CFLAGS= -Wall -std=c99 -lpthread -ggdb
OUT= matrixprod
PLAIN= matrixprod_plain
FILE= main.c
FILES= DLinkedList.o consumer.o producer.o matrix_reader.o compute_no_thread.o
MATRIX= matrix.o
MATRIX_PLAIN= matrix_plain.o

all: $(OUT) $(PLAIN)
	## You can launch the tests with: $ make run ##

sparce: $(OUT)

plain: $(PLAIN)

$(OUT): $(FILE) $(MATRIX) $(FILES)
	$(CC) $< $(MATRIX) $(FILES) -o $(OUT) $(CFLAGS)

$(PLAIN): $(FILE) $(MATRIX_PLAIN) $(FILES)
	$(CC) $< $(MATRIX_PLAIN) $(FILES) -o $(PLAIN) $(CFLAGS)

DLinkedList.o: DLinkedList.c
	$(CC) $(CFLAGS) -c $< -o $@
	
consumer.o: consumer.c
	$(CC) $(CFLAGS) -c $< -o $@
	
producer.o: producer.c
	$(CC) $(CFLAGS) -c $< -o $@

matrix.o: matrix.c
	$(CC) $(CFLAGS) -c $< -o $@

matrix_plain.o: matrix_plain.c
	$(CC) $(CFLAGS) -c $< -o $@

matrix_reader.o: matrix_reader.c
	$(CC) $(CFLAGS) -c $< -o $@

compute_no_thread.o: compute_no_thread.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(OUT)
	./$(OUT) matrixexemple

.PHONY: clean

clean:
	@rm -f $(OUT) $(FILES)
