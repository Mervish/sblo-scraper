#include "PostData.h"
#include "Wumbo.h"
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <optional>

namespace {
    struct ImageLinkData {
        std::string image;
        std::string thumb;
    };

	std::optional<ImageLinkData> getImageData(GumboNode* image_link_node) {
		//Getting image links from node
		if (auto image = gumbo_get_attribute(&image_link_node->v.element.attributes, "href")) {
			if (auto children = Wumbo::node_make_children_vector(image_link_node); 1 == children.size()) {
				if (auto thumb = gumbo_get_attribute(&children.front()->v.element.attributes, "src")) {
					return ImageLinkData{ image->value, thumb->value };
				}
			}
		}
		return {};
	}
}

class PostGatherer
{


};

void PostDate::setFromString(std::string const& date_str) {
    std::vector<std::string> result;
    boost::split(result, date_str, boost::is_any_of("年月日"), boost::algorithm::token_compress_on);
    if (3 <= result.size()) {
        auto iter = result.begin();
        try {
            year  = std::stoul(*iter++);
            month = std::stoul(*iter++);
            day   = std::stoul(*iter++);
            valid = true;
            return;
        }
        catch (...) {
            std::cout << "Failure to convert string'" << date_str << "' to date." << '\n';
        }
    }
    valid = false;
    return;
}

PostData::PostData(GumboNode* post_node) {
    std::optional<GumboNode*> header;
    std::optional<GumboNode*> body;
    auto post_elements = Wumbo::make_node_vector_type(post_node, GUMBO_NODE_ELEMENT);
    auto iter = post_elements.begin();
    auto const end = post_elements.end();
    while (iter != end) {
        if (Wumbo::node_is_atrribute(*iter, { GUMBO_TAG_H2 , "class","date" })) {
            header = *iter;
            ++iter;
            break;
        }
        ++iter;
    }
    while (iter != end) {
        if (Wumbo::node_is_atrribute(*iter, { GUMBO_TAG_DIV , "class","blogbody" })) {
            body = *iter;
            break;
        }
        ++iter;
    }
    if (header && body) {
        auto date_str = Wumbo::extract_text(header.value());
        m_date.setFromString(date_str);
        auto body_children = Wumbo::make_node_vector_type(body.value(), GUMBO_NODE_ELEMENT);
        if (body_children.size() == 3) {
            //TO DO:: Add attribute checkers
            
            std::vector<bool> checks{
                Wumbo::node_is_atrribute(body_children[0], { GUMBO_TAG_H3, "class", "title" }),
                Wumbo::node_is_atrribute(body_children[1], { GUMBO_TAG_DIV, "class", "text" }),
                Wumbo::node_is_atrribute(body_children[2], { GUMBO_TAG_DIV, "class", "posted" }) 
            };

            auto title_node = Wumbo::node_make_children_vector(body_children[0]);
            if (1 == title_node.size()) {
                if(!setTitleLink(title_node.front())) {
                    std::cout << "Unable to parse title data";
                    return;
                }
            }
            auto text_data_nodes = Wumbo::node_make_children_vector(body_children[1]);
            std::cout << "Writing post's content:" << '\n';
            for (auto text_data_node: text_data_nodes) {
                switch (text_data_node->type)
                {
                case GUMBO_NODE_TEXT:
                {
                    std::cout << "Text:" << text_data_node->v.text.text << '\n';
                }
                    break;
                case GUMBO_NODE_ELEMENT:
                {
                    switch (text_data_node->v.element.tag)
                    {
                    case GUMBO_TAG_BR:
                        std::cout << "Newline\n";
                        break;
                    case GUMBO_TAG_A:
                        if (auto image_text_data = getImageData(text_data_node)) {
                            auto&& [thumb, image] = std::move(image_text_data.value());
                            std::cout << "Image: thumb = '" << thumb << "' image='" << image << "'\n";
                        }
                        break;
                    }
                }
                    break;
                case GUMBO_NODE_WHITESPACE:
                    break;
                default:
                    std::cout << "Unknown data in post!" << '\n';
                    break;
                }
            }

            auto finisher_node = Wumbo::node_make_children_vector(body_children[2]);


            //auto 
            //auto 
            int s = 5;
        }
        else {
            std::cout << "failed to parse post data";
        }
    }
    else {
        return;
    }

    m_valid = true;
}

bool PostData::setTitleLink(GumboNode* title_node)
{
    if (auto o_title = Wumbo::getLinkData(title_node)) {
        auto& [title, link] = o_title.value();
        m_title = title;
        m_url = link;
        return true;
    }
    return false;
}
