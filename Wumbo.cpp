#include "Wumbo.h"

namespace Wumbo
{
	std::optional<std::pair<std::string, std::string>> getLinkData(GumboNode* node) {
		GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
		if (href) {
			auto subnodes = Wumbo::node_make_children_vector(node);
			if (1 == subnodes.size()) {
				auto text_n = subnodes.front();
				if (text_n->type == GUMBO_NODE_TEXT) {
					return std::make_pair(text_n->v.text.text, href->value);
				}
			}
		}
		return {};
	}


	std::vector<GumboNode*> node_make_children_vector(GumboNode* cvec) {
		std::vector<GumboNode*> vec;
		for (auto child : make_children_span(cvec->v.element.children)) {
			vec.push_back(static_cast<GumboNode*>(child));
		}
		return vec;
	}


	std::vector<GumboNode*> make_node_vector_type(GumboNode* cvec, GumboNodeType filter) {
		std::vector<GumboNode*> vec;
		for (auto child : make_children_span(cvec->v.element.children)) {
			auto node = static_cast<GumboNode*>(child);
			if (node->type == filter) {
				vec.push_back(node);
			}
		}
		return vec;
	}


	std::vector<GumboNode*> make_node_vector_tag(GumboNode* cvec, GumboTag filter) {
		std::vector<GumboNode*> vec;
		for (auto child : make_children_span(cvec->v.element.children)) {
			auto node = static_cast<GumboNode*>(child);
			if (node->v.element.tag == filter) {
				vec.push_back(node);
			}
		}
		return vec;
	}

	std::string extract_text(GumboNode* node)
	{
		auto c_vec = node->v.document.children;
		if (1 == c_vec.length) {
			auto child = static_cast<GumboNode*>(c_vec.data[0]);
			if (child->type == GUMBO_NODE_TEXT) {
				return child->v.text.text;
			}
		}
		return std::string();
	}


	bool node_is_atrribute(GumboNode* node, WumboQuery const& query)
	{
		if (node->v.element.tag == query.tag)
		{
			if (auto g_attr = gumbo_get_attribute(&node->v.element.attributes, query.attr_name.c_str())) {
				return query.attr_val == g_attr->value;
			}
		}
		return false;
	}


	std::vector<GumboNode*> node_linear_search(GumboNode* node, WumboQuery const& query)
	{
		std::vector<GumboNode*> vec;

		for (auto child : make_node_vector_type(node, GUMBO_NODE_ELEMENT)) {
			if (node_is_atrribute(child, query))
			{
				vec.push_back(child);
			}
		}
		return vec;
	}


	std::optional<GumboNode*> node_recursive_lookup(GumboNode* node, WumboQuery const& query)
	{
		if (node_is_atrribute(node, query)) {
			return node;
		}

		for (auto child : make_node_vector_type(node, GUMBO_NODE_ELEMENT)) {
			if (auto res = node_recursive_lookup(child, query)) {
				return res;
			}
		}
		return {};
	}
}