#include "ParsedPage.h"
#include "Wumbo.h"
#include <iostream>

namespace {
    
}

ParsedPage ParsedPage::fromText(std::string const& text) {
    ParsedPage page;
    auto parsed_response = gumbo_parse(text.c_str());

    //plan:
    //1. Get contents node(done)
    //2. Parse navi(done?)
    //3. Load other pages(if present) and gather post links
    //4. Load post pages and scrape data

    //Gather navi data
    //Navi should contain previous/next links, as well as links to pages(for monthly search). Post parser should lookup page links.
    auto contents_node = Wumbo::node_recursive_lookup(parsed_response->root, { GUMBO_TAG_DIV, "id" , "content" });
    if (!contents_node) {
        std::cout << "Parsing error: unable to find the 'content' node.";
        return page;
    }

    //auto elements_node = Wumbo::make_node_vector(contents_node.value()->v.element.children);
    auto navi_nodes = Wumbo::node_linear_search(contents_node.value(), { GUMBO_TAG_DIV, "class" , "navi" });
    if (navi_nodes.empty()) {
        std::cout << "Parsing error: unable to find the 'navi' node.";
        return page;
    }
    for (auto nav_node : navi_nodes) {
        page.nav.getDataFromNode(nav_node);
    }

    auto post_nodes = Wumbo::node_linear_search(contents_node.value(), { GUMBO_TAG_DIV, "class" , "blog" });
    if (navi_nodes.empty()) {
        std::cout << "Parsing error: unable to find any posts for the date.";
        return page;
    }
    std::vector<PostData> post_entries;

    for (auto post_node : post_nodes) {
        post_entries.emplace_back(post_node);
    }

    //search_for_links(parsed_response->root);

    // free the allocated memory
    gumbo_destroy_output(&kGumboDefaultOptions, parsed_response);

    return page;
}
