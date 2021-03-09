test:main.c bmp.c
	arm-linux-gnueabihf-gcc -o test main.c bmp.c input_manager.c touch.c netinput.c -I . -lpthread -lts
clean:
	rm -f *.o