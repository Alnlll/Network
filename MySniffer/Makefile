CUR_PATH = ./

SRC_PATH = ${CUR_PATH}src/
INC_PATH = ${CUR_PATH}inc/
OUTPUT_PATH = ${CUR_PATH}output/

CFLAGS =
IFLAGS = -I${CUR_PATH}inc/

#main: ${OUTPUT_PATH}main.o ${OUTPUT_PATH}encrypt.o ${OUTPUT_PATH}checksum.o
#	gcc ${OUTPUT_PATH}main.o ${OUTPUT_PATH}encrypt.o ${OUTPUT_PATH}checksum.o -o main
main: ${OUTPUT_PATH}main.o ${OUTPUT_PATH}interface.o ${OUTPUT_PATH}capture.o ${OUTPUT_PATH}common.o ${OUTPUT_PATH}parser.o
	gcc ${OUTPUT_PATH}main.o ${OUTPUT_PATH}interface.o ${OUTPUT_PATH}capture.o ${OUTPUT_PATH}common.o ${OUTPUT_PATH}parser.o -o main
	mv main ${OUTPUT_PATH} 

${OUTPUT_PATH}main.o: ${SRC_PATH}main.c
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}main.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}interface.o: ${SRC_PATH}interface.c ${INC_PATH}interface.h
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}interface.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}capture.o: ${SRC_PATH}capture.c ${INC_PATH}capture.h
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}capture.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}common.o: ${SRC_PATH}common.c ${INC_PATH}common.h
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}common.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}parser.o: ${SRC_PATH}parser.c ${INC_PATH}parser.h
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}parser.c
	mv *.o ${OUTPUT_PATH}

.PHONY: clean
clean:
	rm -r ${OUTPUT_PATH}*.o ${OUTPUT_PATH}main