CUR_PATH = ./

SRC_PATH = ${CUR_PATH}src/
INC_PATH = ${CUR_PATH}inc/
OUTPUT_PATH = ${CUR_PATH}output/

CFLAGS =
IFLAGS = -I${CUR_PATH}inc/

#main: ${OUTPUT_PATH}main.o ${OUTPUT_PATH}encrypt.o ${OUTPUT_PATH}checksum.o
#	gcc ${OUTPUT_PATH}main.o ${OUTPUT_PATH}encrypt.o ${OUTPUT_PATH}checksum.o -o main
main: ${OUTPUT_PATH}main.o ${OUTPUT_PATH}net_control.o ${OUTPUT_PATH}net_comm.o ${OUTPUT_PATH}common.o
	gcc ${OUTPUT_PATH}main.o ${OUTPUT_PATH}net_control.o ${OUTPUT_PATH}net_comm.o ${OUTPUT_PATH}common.o -o main
	mv main ${OUTPUT_PATH} 

${OUTPUT_PATH}main.o: ${SRC_PATH}main.c
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}main.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}net_control.o: ${SRC_PATH}net_control.c ${INC_PATH}net_control.h
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}net_control.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}net_comm.o: ${SRC_PATH}net_comm.c ${INC_PATH}net_comm.h
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}net_comm.c
	mv *.o ${OUTPUT_PATH}

${OUTPUT_PATH}common.o: ${SRC_PATH}common.c ${INC_PATH}common.h
	gcc ${CFLAGS} ${IFLAGS} -c ${SRC_PATH}common.c
	mv *.o ${OUTPUT_PATH}

.PHONY: clean
clean:
	cd ${OUTPUT_PATH} && rm -r *.o || rm main