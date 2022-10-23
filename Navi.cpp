#include "Navi.h"
#include "Wumbo.h"

#include <iostream>
#include <unordered_map>

void Navi::getDataFromNode(GumboNode* navi_node) {
	if (waysSet() && pages) {
		return;
	}

	//navi_node should be correct
	std::unordered_map<std::string, std::string> links;
	auto nodes = Wumbo::make_node_vector_tag(navi_node, GUMBO_TAG_A);
	for (auto node : nodes)
	{
		if (auto data = Wumbo::getLinkData(node)) {
			auto&& [text, link] = data.value();
			links[text] = link;
		}
	}

	if (!waysSet() && links.contains("TOP")) {
		if (links.contains(fwd_lit)) {
			forward = links.at(fwd_lit);
		}
		if (links.contains(bac_lit)) {
			back = links.at(bac_lit);
		}
	}
	else {
		std::vector<std::string> p_vec;
		unsigned int page_counter = 2;
		while (links.contains(std::to_string(page_counter))) {
			p_vec.emplace_back(links.at(std::to_string(page_counter)));
			++page_counter;
		}
		if (!p_vec.empty()) {
			pages = p_vec;
		}
	}
}
