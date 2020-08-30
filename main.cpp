#include "config.h"

#include "vgde.h"
#include "graphics/draw.h"

#include "input.h"

#include "util/vmath.h"
#include "util/clock.h"
#include "util/humanReadable.h"
#include "util/resourceManager.h"

int main() {
	VGDE *vgde = VGDE::instance();
	//vgde->init(vgde->nativeVideoMode());
	vgde->init();

	drawSetColor(Color::Red);
	//drawSetClearColor(Color::Blue);

	if (Clock::isPM()) {
	    db(Clock::hour(true) << ":" << Clock::minute() << ":" << Clock::second() << ":" << Clock::millisecond() << " pm");
	} else {
        db(Clock::hour(true) << ":" << Clock::minute() << ":" << Clock::second() << ":" << Clock::millisecond() << " am");
	}

    ResourceManager::instance()->setImgPath("../res");

	Sprite spr("tv.png");
	spr.setOrigin({spr.width() / 2, spr.height() / 2});
	spr.setPosition({spr.width() / 2, spr.height() / 2});
    Sprite fire("fire.png");
	fire.setPosition({100, 200});
	fire.setSize({32, 32});
	fire.setRotation(90);

	Sprite f("tst.png");
	//f.setScale(.1);
	f.setPosition({700, 400});
	f.setOrigin({32, 64});

	float a = 0;

	vgdewarn("warn yo");
	here
	dbln
	trace("ye");
	tracemsg << "haw\n";

	while (vgde->running()) {
	    vgde->preRender();

		if (isKeyDown(vk_escape) || isButtonDown(mb_middle) || isKeyReleased(vk_m)) {
			vgde->exit();
		}

		f.setRotation(a);
		a += 0.5f;

		if (a >= 360) {
		    a = 0;
		}

		spr.draw();
        fire.draw();
        f.draw();

		drawSetColor(Color::Red);
		drawLine(10, 10, 100, 100);

		drawSetColor(Color::Blue);
		drawLine(300, 0, 300, 23);

        drawSetColor(Color::White);
        drawLine(0, 0, f.position().x, f.position().y);

		drawRectangle(400, 500, 100, 35);
		drawSetColor(Color::Red + Color::Blue);
		drawRectangle(400, 500, 100, 35, true);

		//vgde->setWindowTitle("vgde " + std::to_string(vgde->fps()));

		std::string txt = "fps: " + std::to_string(vgde->fps()) + "\t[blue]" + std::to_string(vgde->frameTime()) + "ms\n"
		        + "[cyan]" + secondsToHHMMSS((int)vgde->totalInGameTime());
		drawText(txt, 0, 0, 1, Color::Green);

        drawText("The quick brown fox jumped over the lazy dog", 200, 50, 1, Color::Red);

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
