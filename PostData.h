#pragma once
#include <string>
#include <vector>
#include <gumbo.h>

class PostDate {
public:
    unsigned char day;
    unsigned char month;
    unsigned long year;
    bool valid = false;

    void setFromString(std::string const& date_str);
};


class PostData
{
public:
    PostData(GumboNode* post_node);

	//Getters
	bool isValid() const { return m_valid; }
private:
    bool setTitleLink(GumboNode* title_node);

    PostDate m_date;
    std::string m_title;
    std::string m_url;

	bool m_valid = false;
};

