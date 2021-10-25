EXE = reversi
SOURCE = main.cpp reversi.cpp MCTS.cpp game.cpp
INCLUDE = reversi.hpp MCTS.hpp game.hpp render.hpp
OBJECT = $(addsuffix .o, $(basename $(SOURCE)))
FLAG =  -std=c++11 -I render/ -I render/deps 
RENDER_SOURCE  = ResourceManager.cpp Shader.cpp Texture.cpp render.cpp
RENDER_OBJECT = $(addsuffix .o, $(basename $(RENDER_SOURCE)))
GL_LIBS = -lglfw3 -lassimp -framework OpenGL -framework IOKit -framework CoreVideo -framework Cocoa

reversi: $(EXE) depend

#==========================================
depend:
	clang++ -E -M $(SOURCE) $(INCLUDE) $(FLAG) > Makefile.depend

#==========================================
#compile rules
%.o: %.cpp
	clang++ -O2 -g -c -o $@ $< $(FLAG)
glad.o : render/deps/glad/glad.c
	clang -c -o $@ $< 
%.o : render/%.cpp
	clang++ -c -o $@ $< 

$(EXE): $(OBJECT) render.o glad.o $(RENDER_OBJECT)
	clang++ -O2 -g -o $@ $(OBJECT) $(RENDER_OBJECT) glad.o $(GL_LIBS)
#==========================================
.PHONY: clean depend reversi
clean:
	-rm *.o $(EXE) Makefile.depend result render/*.o

#==========================================
-include Makefile.depend