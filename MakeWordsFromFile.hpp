/*
	Author: Chris Gibb
*/
#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
//parse file identified by string filename into individual words
//held in tokenlist
bool MakeWordsFromFile(std::string filename,std::vector<std::string>&tokenlist,bool lowercase = true)
{

    std::ifstream file(filename.c_str(),std::ios::in);
    char byte;
    std::string str;
    bool add = true;
    if(file.fail())
	{
		return false;
	}
    while(file.get(byte))
    {
        add = true;
        switch(byte)
        {
            case ' ':
                //cull common punctuation and formating characters
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'.'),str.end());
                str.erase(std::remove(str.begin(),str.end(),','),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                tokenlist.push_back(str);
                str = "";
                add = false;
            break;
            case '\n':
                //cull common punctuation and formating characters
                str.erase(std::remove(str.begin(),str.end(),'\n'),str.end());
                str.erase(std::remove(str.begin(),str.end(),'.'),str.end());
                str.erase(std::remove(str.begin(),str.end(),','),str.end());
                str.erase(std::remove(str.begin(),str.end(),'\t'),str.end());
                //make everything lowercase
                if(lowercase)
                    std::transform(str.begin(),str.end(),str.begin(),::tolower);
                if(str != "")
                tokenlist.push_back(str);
                str = "";
                add = false;
            break;
        }
        if(add)
            str += byte;
    }
	file.close();
    return true;
}
