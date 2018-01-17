CUR_PATH = ./

CC = gcc
SRC_PATH = ${CUR_PATH}src/
INC_PATH = ${CUR_PATH}inc/
OUTPUT_PATH = ${CUR_PATH}output/

CFLAGS =
IFLAGS = -I${CUR_PATH}inc/

main: ${OUTPUT_PATH}main.o ${OUTPUT_PATH}interface.o ${OUTPUT_PATH}capture.o ${OUTPUT_PATH}common.o ${OUTPUT_PATH}comm.o ${OUTPUT_PATH}parser.o
	${CC} ${OUTPUT_PATH}main.o ${OUTPUT_PATH}interface.o ${OUTPUT_PATH}capture.o ${OUTPUT_PATH}common.o ${OUTPUT_PATH}comm.o ${OUTPUT_PATH}parser.o -o main
	mv main ${OUTPUT_PATH}

test: ${OUTPUT_PATH}test.o ${OUTPUT_PATH}common.o ${OUTPUT_PATH}parser.o ${OUTPUT_PATH}comm.o ${OUTPUT_PATH}interface.o
	${CC} ${OUTPUT_PATH}test.o ${OUTPUT_PATH}common.o ${OUTPUT_PATH}parser.o ${OUTPUT_PATH}comm.o ${OUTPUT_PATH}interface.o -o test
	mv test ${OUTPUT_PATH}

${OUTPUT_PATH}main.o: ${SRC_PATH}main.c
	${CC} ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}main.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}interface.o: ${SRC_PATH}interface.c ${INC_PATH}interface.h
	${CC} ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}interface.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}capture.o: ${SRC_PATH}capture.c ${INC_PATH}capture.h
	${CC} ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}capture.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}common.o: ${SRC_PATH}common.c ${INC_PATH}common.h
	${CC} ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}common.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}parser.o: ${SRC_PATH}parser.c ${INC_PATH}parser.h
	${CC} ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}parser.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}comm.o: ${SRC_PATH}comm.c ${INC_PATH}comm.h
	${CC} ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}comm.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}test.o: ${SRC_PATH}test/test.c
	${CC} ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}test/test.c
	mv *.o ${OUTPUT_PATH}

.PHONY: clean
clean:
	rm -r ${OUTPUT_PATH}*.o ${OUTPUT_PATH}main ${OUTPUT_PATH}test