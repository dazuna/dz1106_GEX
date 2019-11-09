#include "cGLColourHelper.h"
#include <algorithm>



cGLColourHelper::cColour::cColour(std::string sName, cGLColourHelper::enumColours colourEnum, glm::vec3 initRGB)
{
	this->enumName = colourEnum;
	this->name = sName;
	this->rgb = initRGB;
	return;
}

cGLColourHelper::cColour::cColour(std::string sName, cGLColourHelper::enumColours colourEnum, float r, float g, float b)
{
	this->enumName = colourEnum;
	this->name = sName;
	this->rgb.r = r;
	this->rgb.g = g;
	this->rgb.b = b;
	return;
}

//static 
cGLColourHelper* cGLColourHelper::m_p_theInstance = 0;

//static 
cGLColourHelper* cGLColourHelper::getInstance(void)
{
	if (cGLColourHelper::m_p_theInstance == 0)
	{
		cGLColourHelper::m_p_theInstance = new cGLColourHelper();
	}
	return cGLColourHelper::m_p_theInstance;
}

cGLColourHelper::cGLColourHelper()
{
	this->m_mapColours[ALICE_BLUE] = cGLColourHelper::cColour("AliceBlue", ALICE_BLUE, 0.941f, 0.973f, 1.000f);
	this->m_mapColours[ANTIQUE_WHITE] = cGLColourHelper::cColour("AntiqueWhite", ANTIQUE_WHITE, 0.980f, 0.922f, 0.843f);
	this->m_mapColours[AQUA] = cGLColourHelper::cColour("Aqua", AQUA, 0.000f, 1.000f, 1.000f);
	this->m_mapColours[AQUAMARINE] = cGLColourHelper::cColour("Aquamarine", AQUAMARINE, 0.498f, 1.000f, 0.831f);
	this->m_mapColours[AZURE] = cGLColourHelper::cColour("Azure", AZURE, 0.941f, 1.000f, 1.000f);
	this->m_mapColours[BEIGE] = cGLColourHelper::cColour("Beige", BEIGE, 0.961f, 0.961f, 0.863f);
	this->m_mapColours[BISQUE] = cGLColourHelper::cColour("Bisque", BISQUE, 1.000f, 0.894f, 0.769f);
	this->m_mapColours[BLACK] = cGLColourHelper::cColour("Black", BLACK, 0.000f, 0.000f, 0.000f);
	this->m_mapColours[BLANCHED_ALMOND] = cGLColourHelper::cColour("BlanchedAlmond", BLANCHED_ALMOND, 1.000f, 0.922f, 0.804f);
	this->m_mapColours[BLUE] = cGLColourHelper::cColour("Blue", BLUE, 0.000f, 0.000f, 1.000f);
	this->m_mapColours[BLUE_VIOLET] = cGLColourHelper::cColour("BlueViolet", BLUE_VIOLET, 0.541f, 0.169f, 0.886f);
	this->m_mapColours[BROWN] = cGLColourHelper::cColour("Brown", BROWN, 0.647f, 0.165f, 0.165f);
	this->m_mapColours[BURLY_WOOD] = cGLColourHelper::cColour("BurlyWood", BURLY_WOOD, 0.871f, 0.722f, 0.529f);
	this->m_mapColours[CADET_BLUE] = cGLColourHelper::cColour("CadetBlue", CADET_BLUE, 0.373f, 0.620f, 0.627f);
	this->m_mapColours[CHARTREUSE] = cGLColourHelper::cColour("Chartreuse", CHARTREUSE, 0.498f, 1.000f, 0.000f);
	this->m_mapColours[CHOCOLATE] = cGLColourHelper::cColour("Chocolate", CHOCOLATE, 0.824f, 0.412f, 0.118f);
	this->m_mapColours[CORAL] = cGLColourHelper::cColour("Coral", CORAL, 1.000f, 0.498f, 0.314f);
	this->m_mapColours[CORNFLOWER_BLUE] = cGLColourHelper::cColour("CornflowerBlue", CORNFLOWER_BLUE, 0.392f, 0.584f, 0.929f);
	this->m_mapColours[CORNSILK] = cGLColourHelper::cColour("Cornsilk", CORNSILK, 1.000f, 0.973f, 0.863f);
	this->m_mapColours[CRIMSON] = cGLColourHelper::cColour("Crimson", CRIMSON, 0.863f, 0.078f, 0.235f);
	this->m_mapColours[CYAN] = cGLColourHelper::cColour("Cyan", CYAN, 0.000f, 1.000f, 1.000f);
	this->m_mapColours[DARK_BLUE] = cGLColourHelper::cColour("DarkBlue", DARK_BLUE, 0.000f, 0.000f, 0.545f);
	this->m_mapColours[DARK_CYAN] = cGLColourHelper::cColour("DarkCyan", DARK_CYAN, 0.000f, 0.545f, 0.545f);
	this->m_mapColours[DARK_GOLDEN_ROD] = cGLColourHelper::cColour("DarkGoldenRod", DARK_GOLDEN_ROD, 0.722f, 0.525f, 0.043f);
	this->m_mapColours[DARK_GRAY] = cGLColourHelper::cColour("DarkGray", DARK_GRAY, 0.663f, 0.663f, 0.663f);
	this->m_mapColours[DARK_GREEN] = cGLColourHelper::cColour("DarkGreen", DARK_GREEN, 0.000f, 0.392f, 0.000f);
	this->m_mapColours[DARK_KHAKI] = cGLColourHelper::cColour("DarkKhaki", DARK_KHAKI, 0.741f, 0.718f, 0.420f);
	this->m_mapColours[DARK_MAGENTA] = cGLColourHelper::cColour("DarkMagenta", DARK_MAGENTA, 0.545f, 0.000f, 0.545f);
	this->m_mapColours[DARK_OLIVE_GREEN] = cGLColourHelper::cColour("DarkOliveGreen", DARK_OLIVE_GREEN, 0.333f, 0.420f, 0.184f);
	this->m_mapColours[DARK_ORANGE] = cGLColourHelper::cColour("DarkOrange", DARK_ORANGE, 1.000f, 0.549f, 0.000f);
	this->m_mapColours[DARK_ORCHID] = cGLColourHelper::cColour("DarkOrchid", DARK_ORCHID, 0.600f, 0.196f, 0.800f);
	this->m_mapColours[DARK_RED] = cGLColourHelper::cColour("DarkRed", DARK_RED, 0.545f, 0.000f, 0.000f);
	this->m_mapColours[DARK_SALMON] = cGLColourHelper::cColour("DarkSalmon", DARK_SALMON, 0.914f, 0.588f, 0.478f);
	this->m_mapColours[DARK_SEA_GREEN] = cGLColourHelper::cColour("DarkSeaGreen", DARK_SEA_GREEN, 0.561f, 0.737f, 0.561f);
	this->m_mapColours[DARK_SLATE_BLUE] = cGLColourHelper::cColour("DarkSlateBlue", DARK_SLATE_BLUE, 0.282f, 0.239f, 0.545f);
	this->m_mapColours[DARK_SLATE_GRAY] = cGLColourHelper::cColour("DarkSlateGray", DARK_SLATE_GRAY, 0.184f, 0.310f, 0.310f);
	this->m_mapColours[DARK_TURQUOISE] = cGLColourHelper::cColour("DarkTurquoise", DARK_TURQUOISE, 0.000f, 0.808f, 0.820f);
	this->m_mapColours[DARK_VIOLET] = cGLColourHelper::cColour("DarkViolet", DARK_VIOLET, 0.580f, 0.000f, 0.827f);
	this->m_mapColours[DEEP_PINK] = cGLColourHelper::cColour("DeepPink", DEEP_PINK, 1.000f, 0.078f, 0.576f);
	this->m_mapColours[DEEP_SKY_BLUE] = cGLColourHelper::cColour("DeepSkyBlue", DEEP_SKY_BLUE, 0.000f, 0.749f, 1.000f);
	this->m_mapColours[DIM_GRAY] = cGLColourHelper::cColour("DimGray", DIM_GRAY, 0.412f, 0.412f, 0.412f);
	this->m_mapColours[DODGER_BLUE] = cGLColourHelper::cColour("DodgerBlue", DODGER_BLUE, 0.118f, 0.565f, 1.000f);
	this->m_mapColours[FIRE_BRICK] = cGLColourHelper::cColour("FireBrick", FIRE_BRICK, 0.698f, 0.133f, 0.133f);
	this->m_mapColours[FLORAL_WHITE] = cGLColourHelper::cColour("FloralWhite", FLORAL_WHITE, 1.000f, 0.980f, 0.941f);
	this->m_mapColours[FOREST_GREEN] = cGLColourHelper::cColour("ForestGreen", FOREST_GREEN, 0.133f, 0.545f, 0.133f);
	this->m_mapColours[FUCHSIA] = cGLColourHelper::cColour("Fuchsia", FUCHSIA, 1.000f, 0.000f, 1.000f);
	this->m_mapColours[GAINSBORO] = cGLColourHelper::cColour("Gainsboro", GAINSBORO, 0.863f, 0.863f, 0.863f);
	this->m_mapColours[GHOST_WHITE] = cGLColourHelper::cColour("GhostWhite", GHOST_WHITE, 0.973f, 0.973f, 1.000f);
	this->m_mapColours[GOLD] = cGLColourHelper::cColour("Gold", GOLD, 1.000f, 0.843f, 0.000f);
	this->m_mapColours[GOLDEN_ROD] = cGLColourHelper::cColour("GoldenRod", GOLDEN_ROD, 0.855f, 0.647f, 0.125f);
	this->m_mapColours[GRAY] = cGLColourHelper::cColour("Gray", GRAY, 0.502f, 0.502f, 0.502f);
	this->m_mapColours[GREEN] = cGLColourHelper::cColour("Green", GREEN, 0.000f, 0.502f, 0.000f);
	this->m_mapColours[GREEN_YELLOW] = cGLColourHelper::cColour("GreenYellow", GREEN_YELLOW, 0.678f, 1.000f, 0.184f);
	this->m_mapColours[HONEY_DEW] = cGLColourHelper::cColour("HoneyDew", HONEY_DEW, 0.941f, 1.000f, 0.941f);
	this->m_mapColours[HOT_PINK] = cGLColourHelper::cColour("HotPink", HOT_PINK, 1.000f, 0.412f, 0.706f);
	this->m_mapColours[INDIAN_RED] = cGLColourHelper::cColour("IndianRed", INDIAN_RED, 0.804f, 0.361f, 0.361f);
	this->m_mapColours[INDIGO] = cGLColourHelper::cColour("Indigo", INDIGO, 0.294f, 0.000f, 0.510f);
	this->m_mapColours[IVORY] = cGLColourHelper::cColour("Ivory", IVORY, 1.000f, 1.000f, 0.941f);
	this->m_mapColours[KHAKI] = cGLColourHelper::cColour("Khaki", KHAKI, 0.941f, 0.902f, 0.549f);
	this->m_mapColours[LAVENDER] = cGLColourHelper::cColour("Lavender", LAVENDER, 0.902f, 0.902f, 0.980f);
	this->m_mapColours[LAVENDER_BLUSH] = cGLColourHelper::cColour("LavenderBlush", LAVENDER_BLUSH, 1.000f, 0.941f, 0.961f);
	this->m_mapColours[LAWN_GREEN] = cGLColourHelper::cColour("LawnGreen", LAWN_GREEN, 0.486f, 0.988f, 0.000f);
	this->m_mapColours[LEMON_CHIFFON] = cGLColourHelper::cColour("LemonChiffon", LEMON_CHIFFON, 1.000f, 0.980f, 0.804f);
	this->m_mapColours[LIGHT_BLUE] = cGLColourHelper::cColour("LightBlue", LIGHT_BLUE, 0.678f, 0.847f, 0.902f);
	this->m_mapColours[LIGHT_CORAL] = cGLColourHelper::cColour("LightCoral", LIGHT_CORAL, 0.941f, 0.502f, 0.502f);
	this->m_mapColours[LIGHT_CYAN] = cGLColourHelper::cColour("LightCyan", LIGHT_CYAN, 0.878f, 1.000f, 1.000f);
	this->m_mapColours[LIGHT_GOLDEN_ROD_YELLOW] = cGLColourHelper::cColour("LightGoldenRodYellow", LIGHT_GOLDEN_ROD_YELLOW, 0.980f, 0.980f, 0.824f);
	this->m_mapColours[LIGHT_GRAY] = cGLColourHelper::cColour("LightGray", LIGHT_GRAY, 0.827f, 0.827f, 0.827f);
	this->m_mapColours[LIGHT_GREEN] = cGLColourHelper::cColour("LightGreen", LIGHT_GREEN, 0.565f, 0.933f, 0.565f);
	this->m_mapColours[LIGHT_PINK] = cGLColourHelper::cColour("LightPink", LIGHT_PINK, 1.000f, 0.714f, 0.757f);
	this->m_mapColours[LIGHT_SALMON] = cGLColourHelper::cColour("LightSalmon", LIGHT_SALMON, 1.000f, 0.627f, 0.478f);
	this->m_mapColours[LIGHT_SEA_GREEN] = cGLColourHelper::cColour("LightSeaGreen", LIGHT_SEA_GREEN, 0.125f, 0.698f, 0.667f);
	this->m_mapColours[LIGHT_SKY_BLUE] = cGLColourHelper::cColour("LightSkyBlue", LIGHT_SKY_BLUE, 0.529f, 0.808f, 0.980f);
	this->m_mapColours[LIGHT_SLATE_GRAY] = cGLColourHelper::cColour("LightSlateGray", LIGHT_SLATE_GRAY, 0.467f, 0.533f, 0.600f);
	this->m_mapColours[LIGHT_STEEL_BLUE] = cGLColourHelper::cColour("LightSteelBlue", LIGHT_STEEL_BLUE, 0.690f, 0.769f, 0.871f);
	this->m_mapColours[LIGHT_YELLOW] = cGLColourHelper::cColour("LightYellow", LIGHT_YELLOW, 1.000f, 1.000f, 0.878f);
	this->m_mapColours[LIME] = cGLColourHelper::cColour("Lime", LIME, 0.000f, 1.000f, 0.000f);
	this->m_mapColours[LIME_GREEN] = cGLColourHelper::cColour("LimeGreen", LIME_GREEN, 0.196f, 0.804f, 0.196f);
	this->m_mapColours[LINEN] = cGLColourHelper::cColour("Linen", LINEN, 0.980f, 0.941f, 0.902f);
	this->m_mapColours[MAGENTA] = cGLColourHelper::cColour("Magenta", MAGENTA, 1.000f, 0.000f, 1.000f);
	this->m_mapColours[MAROON] = cGLColourHelper::cColour("Maroon", MAROON, 0.502f, 0.000f, 0.000f);
	this->m_mapColours[MEDIUM_AQUA_MARINE] = cGLColourHelper::cColour("MediumAquaMarine", MEDIUM_AQUA_MARINE, 0.400f, 0.804f, 0.667f);
	this->m_mapColours[MEDIUM_BLUE] = cGLColourHelper::cColour("MediumBlue", MEDIUM_BLUE, 0.000f, 0.000f, 0.804f);
	this->m_mapColours[MEDIUM_ORCHID] = cGLColourHelper::cColour("MediumOrchid", MEDIUM_ORCHID, 0.729f, 0.333f, 0.827f);
	this->m_mapColours[MEDIUM_PURPLE] = cGLColourHelper::cColour("MediumPurple", MEDIUM_PURPLE, 0.576f, 0.439f, 0.859f);
	this->m_mapColours[MEDIUM_SEA_GREEN] = cGLColourHelper::cColour("MediumSeaGreen", MEDIUM_SEA_GREEN, 0.235f, 0.702f, 0.443f);
	this->m_mapColours[MEDIUM_SLATE_BLUE] = cGLColourHelper::cColour("MediumSlateBlue", MEDIUM_SLATE_BLUE, 0.482f, 0.408f, 0.933f);
	this->m_mapColours[MEDIUM_SPRING_GREEN] = cGLColourHelper::cColour("MediumSpringGreen", MEDIUM_SPRING_GREEN, 0.000f, 0.980f, 0.604f);
	this->m_mapColours[MEDIUM_TURQUOISE] = cGLColourHelper::cColour("MediumTurquoise", MEDIUM_TURQUOISE, 0.282f, 0.820f, 0.800f);
	this->m_mapColours[MEDIUM_VIOLET_RED] = cGLColourHelper::cColour("MediumVioletRed", MEDIUM_VIOLET_RED, 0.780f, 0.082f, 0.522f);
	this->m_mapColours[MIDNIGHT_BLUE] = cGLColourHelper::cColour("MidnightBlue", MIDNIGHT_BLUE, 0.098f, 0.098f, 0.439f);
	this->m_mapColours[MINT_CREAM] = cGLColourHelper::cColour("MintCream", MINT_CREAM, 0.961f, 1.000f, 0.980f);
	this->m_mapColours[MISTY_ROSE] = cGLColourHelper::cColour("MistyRose", MISTY_ROSE, 1.000f, 0.894f, 0.882f);
	this->m_mapColours[MOCCASIN] = cGLColourHelper::cColour("Moccasin", MOCCASIN, 1.000f, 0.894f, 0.710f);
	this->m_mapColours[NAVAJO_WHITE] = cGLColourHelper::cColour("NavajoWhite", NAVAJO_WHITE, 1.000f, 0.871f, 0.678f);
	this->m_mapColours[NAVY] = cGLColourHelper::cColour("Navy", NAVY, 0.000f, 0.000f, 0.502f);
	this->m_mapColours[OLD_LACE] = cGLColourHelper::cColour("OldLace", OLD_LACE, 0.992f, 0.961f, 0.902f);
	this->m_mapColours[OLIVE] = cGLColourHelper::cColour("Olive", OLIVE, 0.502f, 0.502f, 0.000f);
	this->m_mapColours[OLIVE_DRAB] = cGLColourHelper::cColour("OliveDrab", OLIVE_DRAB, 0.420f, 0.557f, 0.137f);
	this->m_mapColours[ORANGE] = cGLColourHelper::cColour("Orange", ORANGE, 1.000f, 0.647f, 0.000f);
	this->m_mapColours[ORANGE_RED] = cGLColourHelper::cColour("OrangeRed", ORANGE_RED, 1.000f, 0.271f, 0.000f);
	this->m_mapColours[ORCHID] = cGLColourHelper::cColour("Orchid", ORCHID, 0.855f, 0.439f, 0.839f);
	this->m_mapColours[PALE_GOLDEN_ROD] = cGLColourHelper::cColour("PaleGoldenRod", PALE_GOLDEN_ROD, 0.933f, 0.910f, 0.667f);
	this->m_mapColours[PALE_GREEN] = cGLColourHelper::cColour("PaleGreen", PALE_GREEN, 0.596f, 0.984f, 0.596f);
	this->m_mapColours[PALE_TURQUOISE] = cGLColourHelper::cColour("PaleTurquoise", PALE_TURQUOISE, 0.686f, 0.933f, 0.933f);
	this->m_mapColours[PALE_VIOLET_RED] = cGLColourHelper::cColour("PaleVioletRed", PALE_VIOLET_RED, 0.859f, 0.439f, 0.576f);
	this->m_mapColours[PAPAYA_WHIP] = cGLColourHelper::cColour("PapayaWhip", PAPAYA_WHIP, 1.000f, 0.937f, 0.835f);
	this->m_mapColours[PEACH_PUFF] = cGLColourHelper::cColour("PeachPuff", PEACH_PUFF, 1.000f, 0.855f, 0.725f);
	this->m_mapColours[PERU] = cGLColourHelper::cColour("Peru", PERU, 0.804f, 0.522f, 0.247f);
	this->m_mapColours[PINK] = cGLColourHelper::cColour("Pink", PINK, 1.000f, 0.753f, 0.796f);
	this->m_mapColours[PLUM] = cGLColourHelper::cColour("Plum", PLUM, 0.867f, 0.627f, 0.867f);
	this->m_mapColours[POWDER_BLUE] = cGLColourHelper::cColour("PowderBlue", POWDER_BLUE, 0.690f, 0.878f, 0.902f);
	this->m_mapColours[PURPLE] = cGLColourHelper::cColour("Purple", PURPLE, 0.502f, 0.000f, 0.502f);
	this->m_mapColours[REBECCA_PURPLE] = cGLColourHelper::cColour("RebeccaPurple", REBECCA_PURPLE, 0.400f, 0.200f, 0.600f);
	this->m_mapColours[RED] = cGLColourHelper::cColour("Red", RED, 1.000f, 0.000f, 0.000f);
	this->m_mapColours[ROSY_BROWN] = cGLColourHelper::cColour("RosyBrown", ROSY_BROWN, 0.737f, 0.561f, 0.561f);
	this->m_mapColours[ROYAL_BLUE] = cGLColourHelper::cColour("RoyalBlue", ROYAL_BLUE, 0.255f, 0.412f, 0.882f);
	this->m_mapColours[SADDLE_BROWN] = cGLColourHelper::cColour("SaddleBrown", SADDLE_BROWN, 0.545f, 0.271f, 0.075f);
	this->m_mapColours[SALMON] = cGLColourHelper::cColour("Salmon", SALMON, 0.980f, 0.502f, 0.447f);
	this->m_mapColours[SANDY_BROWN] = cGLColourHelper::cColour("SandyBrown", SANDY_BROWN, 0.957f, 0.643f, 0.376f);
	this->m_mapColours[SEA_GREEN] = cGLColourHelper::cColour("SeaGreen", SEA_GREEN, 0.180f, 0.545f, 0.341f);
	this->m_mapColours[SEA_SHELL] = cGLColourHelper::cColour("SeaShell", SEA_SHELL, 1.000f, 0.961f, 0.933f);
	this->m_mapColours[SIENNA] = cGLColourHelper::cColour("Sienna", SIENNA, 0.627f, 0.322f, 0.176f);
	this->m_mapColours[SILVER] = cGLColourHelper::cColour("Silver", SILVER, 0.753f, 0.753f, 0.753f);
	this->m_mapColours[SKY_BLUE] = cGLColourHelper::cColour("SkyBlue", SKY_BLUE, 0.529f, 0.808f, 0.922f);
	this->m_mapColours[SLATE_BLUE] = cGLColourHelper::cColour("SlateBlue", SLATE_BLUE, 0.416f, 0.353f, 0.804f);
	this->m_mapColours[SLATE_GRAY] = cGLColourHelper::cColour("SlateGray", SLATE_GRAY, 0.439f, 0.502f, 0.565f);
	this->m_mapColours[SNOW] = cGLColourHelper::cColour("Snow", SNOW, 1.000f, 0.980f, 0.980f);
	this->m_mapColours[SPRING_GREEN] = cGLColourHelper::cColour("SpringGreen", SPRING_GREEN, 0.000f, 1.000f, 0.498f);
	this->m_mapColours[STEEL_BLUE] = cGLColourHelper::cColour("SteelBlue", STEEL_BLUE, 0.275f, 0.510f, 0.706f);
	this->m_mapColours[TAN] = cGLColourHelper::cColour("Tan", TAN, 0.824f, 0.706f, 0.549f);
	this->m_mapColours[TEAL] = cGLColourHelper::cColour("Teal", TEAL, 0.000f, 0.502f, 0.502f);
	this->m_mapColours[THISTLE] = cGLColourHelper::cColour("Thistle", THISTLE, 0.847f, 0.749f, 0.847f);
	this->m_mapColours[TOMATO] = cGLColourHelper::cColour("Tomato", TOMATO, 1.000f, 0.388f, 0.278f);
	this->m_mapColours[TURQUOISE] = cGLColourHelper::cColour("Turquoise", TURQUOISE, 0.251f, 0.878f, 0.816f);
	this->m_mapColours[VIOLET] = cGLColourHelper::cColour("Violet", VIOLET, 0.933f, 0.510f, 0.933f);
	this->m_mapColours[WHEAT] = cGLColourHelper::cColour("Wheat", WHEAT, 0.961f, 0.871f, 0.702f);
	this->m_mapColours[WHITE] = cGLColourHelper::cColour("White", WHITE, 1.000f, 1.000f, 1.000f);
	this->m_mapColours[WHITE_SMOKE] = cGLColourHelper::cColour("WhiteSmoke", WHITE_SMOKE, 0.961f, 0.961f, 0.961f);
	this->m_mapColours[YELLOW] = cGLColourHelper::cColour("Yellow", YELLOW, 1.000f, 1.000f, 0.000f);
	this->m_mapColours[YELLOW_GREEN] = cGLColourHelper::cColour("YellowGreen", YELLOW_GREEN, 0.604f, 0.804f, 0.196f);

	this->m_mapColourNameToEnum["AliceBlue"] = ALICE_BLUE;
	this->m_mapColourNameToEnum["AntiqueWhite"] = ANTIQUE_WHITE;
	this->m_mapColourNameToEnum["Aqua"] = AQUA;
	this->m_mapColourNameToEnum["Aquamarine"] = AQUAMARINE;
	this->m_mapColourNameToEnum["Azure"] = AZURE;
	this->m_mapColourNameToEnum["Beige"] = BEIGE;
	this->m_mapColourNameToEnum["Bisque"] = BISQUE;
	this->m_mapColourNameToEnum["Black"] = BLACK;
	this->m_mapColourNameToEnum["BlanchedAlmond"] = BLANCHED_ALMOND;
	this->m_mapColourNameToEnum["Blue"] = BLUE;
	this->m_mapColourNameToEnum["BlueViolet"] = BLUE_VIOLET;
	this->m_mapColourNameToEnum["Brown"] = BROWN;
	this->m_mapColourNameToEnum["BurlyWood"] = BURLY_WOOD;
	this->m_mapColourNameToEnum["CadetBlue"] = CADET_BLUE;
	this->m_mapColourNameToEnum["Chartreuse"] = CHARTREUSE;
	this->m_mapColourNameToEnum["Chocolate"] = CHOCOLATE;
	this->m_mapColourNameToEnum["Coral"] = CORAL;
	this->m_mapColourNameToEnum["CornflowerBlue"] = CORNFLOWER_BLUE;
	this->m_mapColourNameToEnum["Cornsilk"] = CORNSILK;
	this->m_mapColourNameToEnum["Crimson"] = CRIMSON;
	this->m_mapColourNameToEnum["Cyan"] = CYAN;
	this->m_mapColourNameToEnum["DarkBlue"] = DARK_BLUE;
	this->m_mapColourNameToEnum["DarkCyan"] = DARK_CYAN;
	this->m_mapColourNameToEnum["DarkGoldenRod"] = DARK_GOLDEN_ROD;
	this->m_mapColourNameToEnum["DarkGray"] = DARK_GRAY;
	this->m_mapColourNameToEnum["DarkGreen"] = DARK_GREEN;
	this->m_mapColourNameToEnum["DarkKhaki"] = DARK_KHAKI;
	this->m_mapColourNameToEnum["DarkMagenta"] = DARK_MAGENTA;
	this->m_mapColourNameToEnum["DarkOliveGreen"] = DARK_OLIVE_GREEN;
	this->m_mapColourNameToEnum["DarkOrange"] = DARK_ORANGE;
	this->m_mapColourNameToEnum["DarkOrchid"] = DARK_ORCHID;
	this->m_mapColourNameToEnum["DarkRed"] = DARK_RED;
	this->m_mapColourNameToEnum["DarkSalmon"] = DARK_SALMON;
	this->m_mapColourNameToEnum["DarkSeaGreen"] = DARK_SEA_GREEN;
	this->m_mapColourNameToEnum["DarkSlateBlue"] = DARK_SLATE_BLUE;
	this->m_mapColourNameToEnum["DarkSlateGray"] = DARK_SLATE_GRAY;
	this->m_mapColourNameToEnum["DarkTurquoise"] = DARK_TURQUOISE;
	this->m_mapColourNameToEnum["DarkViolet"] = DARK_VIOLET;
	this->m_mapColourNameToEnum["DeepPink"] = DEEP_PINK;
	this->m_mapColourNameToEnum["DeepSkyBlue"] = DEEP_SKY_BLUE;
	this->m_mapColourNameToEnum["DimGray"] = DIM_GRAY;
	this->m_mapColourNameToEnum["DodgerBlue"] = DODGER_BLUE;
	this->m_mapColourNameToEnum["FireBrick"] = FIRE_BRICK;
	this->m_mapColourNameToEnum["FloralWhite"] = FLORAL_WHITE;
	this->m_mapColourNameToEnum["ForestGreen"] = FOREST_GREEN;
	this->m_mapColourNameToEnum["Fuchsia"] = FUCHSIA;
	this->m_mapColourNameToEnum["Gainsboro"] = GAINSBORO;
	this->m_mapColourNameToEnum["GhostWhite"] = GHOST_WHITE;
	this->m_mapColourNameToEnum["Gold"] = GOLD;
	this->m_mapColourNameToEnum["GoldenRod"] = GOLDEN_ROD;
	this->m_mapColourNameToEnum["Gray"] = GRAY;
	this->m_mapColourNameToEnum["Green"] = GREEN;
	this->m_mapColourNameToEnum["GreenYellow"] = GREEN_YELLOW;
	this->m_mapColourNameToEnum["HoneyDew"] = HONEY_DEW;
	this->m_mapColourNameToEnum["HotPink"] = HOT_PINK;
	this->m_mapColourNameToEnum["IndianRed"] = INDIAN_RED;
	this->m_mapColourNameToEnum["Indigo"] = INDIGO;
	this->m_mapColourNameToEnum["Ivory"] = IVORY;
	this->m_mapColourNameToEnum["Khaki"] = KHAKI;
	this->m_mapColourNameToEnum["Lavender"] = LAVENDER;
	this->m_mapColourNameToEnum["LavenderBlush"] = LAVENDER_BLUSH;
	this->m_mapColourNameToEnum["LawnGreen"] = LAWN_GREEN;
	this->m_mapColourNameToEnum["LemonChiffon"] = LEMON_CHIFFON;
	this->m_mapColourNameToEnum["LightBlue"] = LIGHT_BLUE;
	this->m_mapColourNameToEnum["LightCoral"] = LIGHT_CORAL;
	this->m_mapColourNameToEnum["LightCyan"] = LIGHT_CYAN;
	this->m_mapColourNameToEnum["LightGoldenRodYellow"] = LIGHT_GOLDEN_ROD_YELLOW;
	this->m_mapColourNameToEnum["LightGray"] = LIGHT_GRAY;
	this->m_mapColourNameToEnum["LightGreen"] = LIGHT_GREEN;
	this->m_mapColourNameToEnum["LightPink"] = LIGHT_PINK;
	this->m_mapColourNameToEnum["LightSalmon"] = LIGHT_SALMON;
	this->m_mapColourNameToEnum["LightSeaGreen"] = LIGHT_SEA_GREEN;
	this->m_mapColourNameToEnum["LightSkyBlue"] = LIGHT_SKY_BLUE;
	this->m_mapColourNameToEnum["LightSlateGray"] = LIGHT_SLATE_GRAY;
	this->m_mapColourNameToEnum["LightSteelBlue"] = LIGHT_STEEL_BLUE;
	this->m_mapColourNameToEnum["LightYellow"] = LIGHT_YELLOW;
	this->m_mapColourNameToEnum["Lime"] = LIME;
	this->m_mapColourNameToEnum["LimeGreen"] = LIME_GREEN;
	this->m_mapColourNameToEnum["Linen"] = LINEN;
	this->m_mapColourNameToEnum["Magenta"] = MAGENTA;
	this->m_mapColourNameToEnum["Maroon"] = MAROON;
	this->m_mapColourNameToEnum["MediumAquaMarine"] = MEDIUM_AQUA_MARINE;
	this->m_mapColourNameToEnum["MediumBlue"] = MEDIUM_BLUE;
	this->m_mapColourNameToEnum["MediumOrchid"] = MEDIUM_ORCHID;
	this->m_mapColourNameToEnum["MediumPurple"] = MEDIUM_PURPLE;
	this->m_mapColourNameToEnum["MediumSeaGreen"] = MEDIUM_SEA_GREEN;
	this->m_mapColourNameToEnum["MediumSlateBlue"] = MEDIUM_SLATE_BLUE;
	this->m_mapColourNameToEnum["MediumSpringGreen"] = MEDIUM_SPRING_GREEN;
	this->m_mapColourNameToEnum["MediumTurquoise"] = MEDIUM_TURQUOISE;
	this->m_mapColourNameToEnum["MediumVioletRed"] = MEDIUM_VIOLET_RED;
	this->m_mapColourNameToEnum["MidnightBlue"] = MIDNIGHT_BLUE;
	this->m_mapColourNameToEnum["MintCream"] = MINT_CREAM;
	this->m_mapColourNameToEnum["MistyRose"] = MISTY_ROSE;
	this->m_mapColourNameToEnum["Moccasin"] = MOCCASIN;
	this->m_mapColourNameToEnum["NavajoWhite"] = NAVAJO_WHITE;
	this->m_mapColourNameToEnum["Navy"] = NAVY;
	this->m_mapColourNameToEnum["OldLace"] = OLD_LACE;
	this->m_mapColourNameToEnum["Olive"] = OLIVE;
	this->m_mapColourNameToEnum["OliveDrab"] = OLIVE_DRAB;
	this->m_mapColourNameToEnum["Orange"] = ORANGE;
	this->m_mapColourNameToEnum["OrangeRed"] = ORANGE_RED;
	this->m_mapColourNameToEnum["Orchid"] = ORCHID;
	this->m_mapColourNameToEnum["PaleGoldenRod"] = PALE_GOLDEN_ROD;
	this->m_mapColourNameToEnum["PaleGreen"] = PALE_GREEN;
	this->m_mapColourNameToEnum["PaleTurquoise"] = PALE_TURQUOISE;
	this->m_mapColourNameToEnum["PaleVioletRed"] = PALE_VIOLET_RED;
	this->m_mapColourNameToEnum["PapayaWhip"] = PAPAYA_WHIP;
	this->m_mapColourNameToEnum["PeachPuff"] = PEACH_PUFF;
	this->m_mapColourNameToEnum["Peru"] = PERU;
	this->m_mapColourNameToEnum["Pink"] = PINK;
	this->m_mapColourNameToEnum["Plum"] = PLUM;
	this->m_mapColourNameToEnum["PowderBlue"] = POWDER_BLUE;
	this->m_mapColourNameToEnum["Purple"] = PURPLE;
	this->m_mapColourNameToEnum["RebeccaPurple"] = REBECCA_PURPLE;
	this->m_mapColourNameToEnum["Red"] = RED;
	this->m_mapColourNameToEnum["RosyBrown"] = ROSY_BROWN;
	this->m_mapColourNameToEnum["RoyalBlue"] = ROYAL_BLUE;
	this->m_mapColourNameToEnum["SaddleBrown"] = SADDLE_BROWN;
	this->m_mapColourNameToEnum["Salmon"] = SALMON;
	this->m_mapColourNameToEnum["SandyBrown"] = SANDY_BROWN;
	this->m_mapColourNameToEnum["SeaGreen"] = SEA_GREEN;
	this->m_mapColourNameToEnum["SeaShell"] = SEA_SHELL;
	this->m_mapColourNameToEnum["Sienna"] = SIENNA;
	this->m_mapColourNameToEnum["Silver"] = SILVER;
	this->m_mapColourNameToEnum["SkyBlue"] = SKY_BLUE;
	this->m_mapColourNameToEnum["SlateBlue"] = SLATE_BLUE;
	this->m_mapColourNameToEnum["SlateGray"] = SLATE_GRAY;
	this->m_mapColourNameToEnum["Snow"] = SNOW;
	this->m_mapColourNameToEnum["SpringGreen"] = SPRING_GREEN;
	this->m_mapColourNameToEnum["SteelBlue"] = STEEL_BLUE;
	this->m_mapColourNameToEnum["Tan"] = TAN;
	this->m_mapColourNameToEnum["Teal"] = TEAL;
	this->m_mapColourNameToEnum["Thistle"] = THISTLE;
	this->m_mapColourNameToEnum["Tomato"] = TOMATO;
	this->m_mapColourNameToEnum["Turquoise"] = TURQUOISE;
	this->m_mapColourNameToEnum["Violet"] = VIOLET;
	this->m_mapColourNameToEnum["Wheat"] = WHEAT;
	this->m_mapColourNameToEnum["White"] = WHITE;
	this->m_mapColourNameToEnum["WhiteSmoke"] = WHITE_SMOKE;
	this->m_mapColourNameToEnum["Yellow"] = YELLOW;
	this->m_mapColourNameToEnum["YellowGreen"] = YELLOW_GREEN;

	// Generate the random lookup
	for (std::map< cGLColourHelper::enumColours, cGLColourHelper::cColour >::iterator itColour = this->m_mapColours.begin();
		 itColour != this->m_mapColours.end(); itColour++)
	{
		this->m_vecRandomColourEnumLookup.push_back(itColour->first);
	}
	// Scramble them
	std::random_shuffle(this->m_vecRandomColourEnumLookup.begin(), this->m_vecRandomColourEnumLookup.begin());
	// Start the 'next' loop up at the start of the vector
	this->m_nextRandomIndex = 0;

	return;
}

cGLColourHelper::enumColours cGLColourHelper::getRandomColourEnum(void)
{
	cGLColourHelper::enumColours randColourEnum = this->m_vecRandomColourEnumLookup[this->m_nextRandomIndex];
	this->m_nextRandomIndex++;
	if (this->m_nextRandomIndex >= this->m_vecRandomColourEnumLookup.size())
	{
		this->m_nextRandomIndex = 0;
	}
	return randColourEnum;
}

cGLColourHelper::cColour cGLColourHelper::getRandomColour(void)
{
	cColour randColour;
	randColour.rgb.r = this->getRand(0.0f, 1.0f);
	randColour.rgb.g = this->getRand(0.0f, 1.0f);
	randColour.rgb.b = this->getRand(0.0f, 1.0f);
	randColour.enumName = CUSTOM;
	return randColour;
}

glm::vec3 cGLColourHelper::getRandomColourRGB(void)
{
	return this->getRandomColour().rgb;
}

void cGLColourHelper::ShuffleRandomColours(void)
{
	std::random_shuffle(this->m_vecRandomColourEnumLookup.begin(), this->m_vecRandomColourEnumLookup.begin());
	return;
}


glm::vec3 cGLColourHelper::getColourRGB(cGLColourHelper::enumColours colourEnum)
{
	cColour theColour = this->getColour(colourEnum);
	return theColour.rgb;
}

glm::vec3 cGLColourHelper::getColourRGB(std::string colourName)
{
	cGLColourHelper::cColour returnColour = this->getColour(colourName);
	return returnColour.rgb;
}

cGLColourHelper::cColour cGLColourHelper::getColour(std::string colourName)
{
	cGLColourHelper::enumColours colourEnum = this->getColourEnumFromName(colourName);
	if (colourEnum == cGLColourHelper::UNKNOWN)
	{	// Didn't find it
		return this->m_getUnknownColour();
	}
	// Look up the colour
	cGLColourHelper::cColour returnColour = this->getColour(colourEnum);
	return returnColour;
}

cGLColourHelper::cColour cGLColourHelper::getColour(cGLColourHelper::enumColours colourEnum)
{
	// 
	std::map< cGLColourHelper::enumColours, cGLColourHelper::cColour >::iterator itColour = m_mapColours.find(colourEnum);
	if (itColour == this->m_mapColours.end())
	{	// Didn't find a match (should "never" happen as it's an enum... but you could pass an int, I suppose
		return this->m_getUnknownColour();
	}
	// Found it
	return itColour->second;
}

cGLColourHelper::cColour cGLColourHelper::m_getUnknownColour(void)
{
	cGLColourHelper::cColour unknownColour = cGLColourHelper::cColour();
	unknownColour.enumName = UNKNOWN;
	unknownColour.name = "Unknown";
	unknownColour.rgb = glm::vec3(0.0f, 0.0f, 0.0f);
	return unknownColour;
}


std::string cGLColourHelper::getColourNameFromEnum(cGLColourHelper::enumColours colourEnum)
{
	cGLColourHelper::cColour returnColour = this->getColour(colourEnum);
	return returnColour.name;
}

cGLColourHelper::enumColours cGLColourHelper::getColourEnumFromName(std::string sColour)
{
	std::map< std::string, cGLColourHelper::enumColours >::iterator itColour = this->m_mapColourNameToEnum.find(sColour);
	if (itColour == this->m_mapColourNameToEnum.end())
	{	// Didn't find it
		return cGLColourHelper::UNKNOWN;
	}
	return itColour->second;
}
