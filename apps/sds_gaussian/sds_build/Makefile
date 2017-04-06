APPSOURCES = ../test.cpp ../cpu.cpp ../top.cpp ../offload.cpp
EXECUTABLE = test.elf

PLATFORM = zed
SDSFLAGS = -sds-pf ${PLATFORM} \
	-sds-hw offload ../offload.cpp -sds-end \
	-poll-mode 1

CC = sds++ ${SDSFLAGS}

CFLAGS = -std=c++11 -Wall -O3 -I../../sds_support -c
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"
LFLAGS = -O3 -ldl -lpthread

OBJECTS := $(APPSOURCES:.cpp=.o)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all

all: ${EXECUTABLE}

${EXECUTABLE}: ${OBJECTS}
	${CC} ${LFLAGS} ${OBJECTS} ../../sds_support/halide_runtime_arm32.a -o $@

-include ${DEPS}

%.o: %.cpp
	${CC} ${CFLAGS} $< -o $@

clean:
	${RM} ${EXECUTABLE} ${OBJECTS} *.d

ultraclean: clean
	${RM} ${EXECUTABLE}.bit
	${RM} -rf _sds sd_card
