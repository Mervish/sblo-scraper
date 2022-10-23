#pragma once
#include "Navi.h"
#include "PostData.h"

class ParsedPage
{
public:
	Navi nav;

	static ParsedPage fromText(std::string const& text);
};

