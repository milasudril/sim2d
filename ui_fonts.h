#ifdef __WAND__
target[name[ui_fonts.h] type[include]]
#endif

#ifndef SIM2D_UIFONTS_H
#define SIM2D_UIFONTS_H

namespace Gui
	{
	class Font;
	}

namespace Sim2d
	{
	struct UiFonts
		{
		const Gui::Font* header;
		const Gui::Font* label;
		const Gui::Font* content;
		const Gui::Font* content_large;
		};
	}

#endif
