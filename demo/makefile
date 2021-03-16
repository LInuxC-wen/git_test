#定义变量
CC=arm-linux-gnueabihf-gcc
DEPS = bmp.h chat.h cJSON.h input_manager.h read_cJSON.h weather_client.h common.h disp_manager.h font_manager.h font_test.h
CFLAGS=-lpthread -lts -lm -lfreetype

#目标文件
test: main.o bmp.o input_manager.o touch.o netinput.o cJSON.o read_cJSON.o weather_client.o disp_manager.o font_manager.o font_test.o framebuffer.o freetype.o
	$(CC) -o test main.o bmp.o input_manager.o touch.o netinput.o cJSON.o read_cJSON.o weather_client.o disp_manager.o font_manager.o font_test.o framebuffer.o freetype.o $(CFLAGS)

#*.o文件的生成规则
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

#伪目标
.PHONY: clean
clean:
	rm -f *.o test