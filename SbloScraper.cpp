#include <cpr/cpr.h>
#include <gumbo.h>

#include "ParsedPage.h"
#include "Decoder.h"
#include "Navi.h"

#include <iostream>
#include <fstream>
#include <optional>


void search_for_links(GumboNode* node)
{
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    if (node->v.element.tag == GUMBO_TAG_A)
    {
        GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
        if (href)
            std::cout << href->value << "\n";
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; i++)
    {
        search_for_links(static_cast<GumboNode*>(children->data[i]));
    }
}

std::optional<GumboNode*> search_for_title(GumboNode* node)
{
    if (node->type != GUMBO_NODE_ELEMENT)
        return {};

    if (node->v.element.tag == GUMBO_TAG_TITLE)
    {
        GumboNode* title_text = static_cast<GumboNode*>(node->v.element.children.data[0]);
        std::cout << title_text->v.text.text << "\n";
        return title_text;
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; i++) {
        if (auto val = search_for_title(static_cast<GumboNode*>(children->data[i]))) {
            return val;
        }
    }
    return {};
}

std::string get_page(std::string const& url)
{
    cpr::Url curl = cpr::Url{ url };
    cpr::Response response = cpr::Get(curl);
    auto &page_content = response.text;
    auto encoded_page = sjisToUtf8(page_content);
    return encoded_page;
}

int main()
{
    std::string path;
    std::cout << "Enter the blog's path: ";
    std::cin >> path;
    auto page = ParsedPage::fromText(get_page(path));

    std::cout << page.nav.back.value_or("No back!") << page.nav.forward.value_or("No forward!");
    //std::ofstream writeCsv("links.csv");
    //writeCsv << "type,link" << "\n";
    //search_for_links(parsed_response->root);
    //writeCsv.close();
}