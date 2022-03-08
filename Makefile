#c for compile

cPong:
	g++ pong/pong.cpp -Idependencies/include -Ldependencies/lib -o pong.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
