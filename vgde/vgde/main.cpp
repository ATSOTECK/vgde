#include "config.h"

#include "vgde.h"
#include "draw.h"

#include "input.h"

#include "vmath.h"

int main() {
	VGDE *vgde = VGDE::instance();
	//vgde->init(2560, 1440, "", true);
	vgde->init();

	drawSetColor(Color::Red);
	//drawSetClearColor(Color::Blue);

	int c = clampi(12, 0, 10);

	while (vgde->running()) {
		vgde->preRender();

		if (isKeyDown(vk_escape) || isButtonDown(mb_middle) || isKeyReleased(vk_m)) {
			vgde->exit();
		}

		drawSetColor(Color::Red);
		drawLine(10, 10, 100, 100);

		drawSetColor(Color::Blue);
		drawLine(300, 0, 300, 23);

		drawRectangle(400, 400, 100, 35);
		drawSetColor(Color::Red + Color::Blue);
		drawRectangle(400, 400, 100, 35, true);

		//setMousePositionGlobal(vec2i(100, 100));
		if (isKeyDown(vk_space)) {
			setMousePosition(vec2i(100, 100));
		} else if (isKeyDown(vk_z)) {
			setMousePositionGlobal(vec2i(100, 100));
		}

		vgde->postRender();
	}

	vgde->cleanUp();
	return 0;
}
