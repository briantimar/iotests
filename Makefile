CFLAGS = -g -Wall
CC = gcc

readmonitor: readmonitor.c
	${CC} readmonitor.c ${CFLAGS} -o readmonitor


time_readmonitor: time_readmonitor.c
	${CC} time_readmonitor.c ${CFLAGS} -o time_readmonitor


all: readmonitor time_readmonitor
