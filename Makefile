#c for compile

cPong:
	g++ pong/pong.cpp -Idependencies/include -Ldependencies/lib -o pong.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf

cTetris:
	g++ tetris/tetris.cpp -Idependencies/include -Ldependencies/lib -o tetris.exe -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf