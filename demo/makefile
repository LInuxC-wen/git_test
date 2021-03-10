test:main.c bmp.c
	arm-linux-gnueabihf-gcc -o test main.c bmp.c input_manager.c touch.c netinput.c cJSON.c read_cJSON.c weather_client.c -I . -lpthread -lts -lm
clean:
	rm -f *.o