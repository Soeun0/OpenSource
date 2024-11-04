# Makefile

CC = gcc
CFLAGS = -c -fPIC
LIB_DIR = ./lib
TARGET = calculate

# 소스 파일과 오브젝트 파일
SRCS = main.c \
       $(LIB_DIR)/add.c \
       $(LIB_DIR)/subtract.c \
       $(LIB_DIR)/multiply.c \
       $(LIB_DIR)/divide.c

OBJS = $(SRCS:.c=.o)

# 기본 목표
all: $(TARGET)

# 프로그램 빌드
$(TARGET): $(OBJS) $(LIB_DIR)/lib1.so
	$(CC) -o $@ $^ -L$(LIB_DIR) -l1

# 오브젝트 파일 생성 규칙
%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

# 클린업
clean:
	rm -f $(TARGET) $(OBJS) $(LIB_DIR)/lib1.so

