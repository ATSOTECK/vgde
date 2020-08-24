#include "config.h"

#include "vgde.h"
#include "draw.h"

#include "input.h"

#include "util/vmath.h"
#include "util/clock.h"

int main() {
	VGDE *vgde = VGDE::instance();
	//vgde->init(2560, 1440, "", true);
	vgde->init();

	drawSetColor(Color::Red);
	//drawSetClearColor(Color::Blue);

	int c = clampi(12, 0, 10);

	if (Clock::isPM()) {
	    db(Clock::getHour(true) << ":" << Clock::getMinute() << ":" << Clock::getSecond() << ":" << Clock::getMillisecond() << " pm");
	} else {
        db(Clock::getHour(true) << ":" << Clock::getMinute() << ":" << Clock::getSecond() << ":" << Clock::getMillisecond() << " am");
	}

	Sprite spr("hot.png");
	spr.setScale(0.25f);
    Sprite fire("fire.png");
	fire.setPosition({100, 200});
	fire.setSize({32, 32});

	while (vgde->running()) {
		vgde->preRender();

		if (isKeyDown(vk_escape) || isButtonDown(mb_middle) || isKeyReleased(vk_m)) {
			vgde->exit();
		}

        spr.draw();
		fire.draw();

		drawSetColor(Color::Red);
		drawLine(10, 10, 100, 100);

		drawSetColor(Color::Blue);
		drawLine(300, 0, 300, 23);

        drawSetColor(Color::White);
        drawLine(100, 100, 500, 500);

		drawRectangle(400, 400, 100, 35);
		drawSetColor(Color::Red + Color::Blue);
		drawRectangle(400, 400, 100, 35, true);

		vgde->setWindowTitle("vgde " + std::to_string(vgde->fps()));

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
