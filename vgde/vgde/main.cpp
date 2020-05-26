#include "config.h"

#include "vgde.h"
#include "draw.h"

#include "vec.h"
#include "rect.h"

int main() {
	VGDE *vgde = VGDE::instance();
	//vgde->init(2560, 1440, "", true);
	vgde->init();

	drawSetColor(Color::Yellow);
	drawSetClearColor(Color::Blue);

	while (vgde->running()) {
		vgde->preRender();
		drawLine(10, 10, 100, 100);
		vgde->postRender();
	}

	vgde->cleanUp();

	//return vgde->run();
	return 0;
}
