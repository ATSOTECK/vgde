#include "config.h"

#include "vgde.h"
#include "draw.h"

#include "vec.h"
#include "rect.h"

#include "input.h"

int main() {
	VGDE *vgde = VGDE::instance();
	//vgde->init(2560, 1440, "", true);
	vgde->init();

	drawSetColor(Color::Yellow);
	drawSetClearColor(Color::Blue);

	while (vgde->running()) {
		vgde->preRender();

		if (isKeyDown(vk_escape) || isButtonDown(mb_middle) || isKeyReleased(vk_m)) {
			vgde->exit();
		}

		drawLine(10, 10, 100, 100);

		drawLine(300, 0, 300, 23);

		//setMousePositionGlobal(vec2i(100, 100));
		if (isKeyDown(vk_space)) {
			setMousePosition(vec2i(100, 100));
		} else if (isKeyDown(vk_z)) {
			setMousePositionGlobal(vec2i(100, 100));
		}

		vgde->postRender();
	}

	vgde->cleanUp();

	//return vgde->run();
	return 0;
}
