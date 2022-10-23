#pragma once
#include <gumbo.h>
#include <string>
#include <vector>
#include <optional>


class Navi
{
public:
	std::optional<std::string> back;
	std::optional<std::string> forward;
	std::optional<std::vector<std::string>> pages;

	constexpr static auto fwd_lit = ">>";
	constexpr static auto bac_lit = "<<";

	inline bool waysSet() {
		return back || forward;
	}

	void getDataFromNode(GumboNode* navi_node);
};

