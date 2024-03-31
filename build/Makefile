windows:
	g++ -I ../src/include -L ../src/lib -o main_windows ../src/main.cpp ../src/Game.cpp ../src/TextureManager.cpp ../src/Map.cpp ../src/Vector2D.cpp ../src/Collision.cpp ../src/ECS/ECS.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

linux:
	g++ main.cpp -g -pedantic -o main_linux -lSDL2main -lSDL2
