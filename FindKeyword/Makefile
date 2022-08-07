CXX = gcc
CXXFLAGS = -std=c99
# INC=/usr/local/ssl/include/ or the actual location in your system
# INC = "c:\Program Files\OpenSSL-Win64\incldue"
# LIB=/usr/local/ssl/lib/ or the actual location in your system
# LIB = "c:\Program Files\OpenSSL-Win64\lib\"

# ${CXX} ${CXXFLAGS} -o enc.exe main.c -lcrypto -ldl
# ${CXX} ${CXXFLAGS} -I$(INC) -L$(LIB) -o enc.exe main.c -lcrypto -ldl
all:
	${CXX} ${CXXFLAGS} -o enc.exe main.c -lcrypto -ldl

clean:
	rm -f *.o enc.c