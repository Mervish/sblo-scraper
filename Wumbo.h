#pragma once
#include <gumbo.h>
#include <span>
#include <vector>
#include <string>
#include <optional>

namespace Wumbo
{
	inline std::span<void*> make_children_span(GumboVector const& vec) {
		return std::span<void*>(vec.data, vec.length);
	}

	std::optional<std::pair<std::string, std::string> > getLinkData(GumboNode* node);

	struct WumboQuery {
		GumboTag tag;
		std::string attr_name;
		std::string attr_val;
	};

	std::vector<GumboNode*> node_make_children_vector(GumboNode*);
	std::vector<GumboNode*> make_node_vector_type(GumboNode* cvec, GumboNodeType filter);
	std::vector<GumboNode*> make_node_vector_tag(GumboNode* cvec, GumboTag filter);

	std::string extract_text(GumboNode* node);

	bool node_is_atrribute(GumboNode* node, WumboQuery const& query);

	std::vector<GumboNode*> node_linear_search(GumboNode* node, WumboQuery const& query);
	std::optional<GumboNode*> node_recursive_lookup(GumboNode* node, WumboQuery const& query);
}

