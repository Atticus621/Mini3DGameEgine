
#include "game.h"
#include "core.h"
int main(int argc, char** argv) {

	Game* game = new Game();

	auto& engine=engine::Engine::GetInstance();
	engine.SetApplication(game);

	if (engine.Init(1280,720)) {
		engine.Run();
	}
	
	engine.Destory();
	return 0;
}