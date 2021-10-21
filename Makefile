CC=g++

stegreg : obj/main.o obj/extract.o obj/embed.o obj/blowfish.o obj/encrypt.o obj/check.o obj/general.o obj/decrypt.o obj/md5.o obj/png.o
	$(CC) obj/* -o stegreg -lm

obj/main.o : src/main.cpp
	$(CC) -Wall -g -c src/main.cpp -o obj/main.o

obj/extract.o : src/extract.cpp
	$(CC) -Wall -g -c src/extract.cpp -o obj/extract.o

obj/embed.o : src/embed.cpp
	$(CC) -Wall -g -c src/embed.cpp -o obj/embed.o

obj/encrypt.o : src/encrypt.cpp
	$(CC) -Wall -g -c src/encrypt.cpp -o obj/encrypt.o

obj/check.o : src/check.cpp
	$(CC) -Wall -g -c src/check.cpp -o obj/check.o

obj/general.o : src/general.cpp
	$(CC) -Wall -g -c src/general.cpp -o obj/general.o

obj/decrypt.o : src/decrypt.cpp
	$(CC) -Wall -g -c src/decrypt.cpp -o obj/decrypt.o

obj/png.o : src/png.cpp
	$(CC) -Wall -g -c src/png.cpp -o obj/png.o

# blowfish library
obj/blowfish.o : lib/Blowfish/blowfish.cpp
	$(CC) -Wall -g -c lib/Blowfish/blowfish.cpp -o obj/blowfish.o
# md5 lib
obj/md5.o : lib/Md5/MD5.cpp
	$(CC) -Wall -g -c lib/Md5/MD5.cpp -o obj/md5.o

clean : 
	rm obj/*.o 