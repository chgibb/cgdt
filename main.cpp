/*
	Author: Chris Gibb

    Simple command line utility to transfer data and check for transfer integrity.

    -copy src to dest
        cgdt src dest
    -compare size of src to size of dest (without any copying)
        cgdt src dest -check
*/
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "MakeWordsFromFile.hpp"
using namespace std;
//parse directory listing generated from dir
int GetTotalBytesFromDirListing(std::vector<std::string>&dump)
{
    auto end = dump.end();
    for(auto iter = dump.begin(); iter != end; ++iter)
    {
        //at the end of the output there will be something like
        //"Total Files Listed:\n
        //xxx File(s)  x,xxx,xxx bytes"
        //Search for this pattern and extract the total bytes in the listing
        if(*iter == "Total")
        {
            ++iter;
            if(*iter == "Files")
            {
                ++iter;
                if(*iter == "Listed:")
                {
                    ++iter;
                    ++iter;
                    ++iter;
                    return std::atoi((*iter).c_str());
                }
            }
        }
    }
    return 0;
}
int main(int argc,char* argv[])
{
    //only check size of source vs destination, do not copy anything
    bool checkonly = false;
    std::vector<std::string> args;
    for(int i = 0; i != argc + 1; i++)
    {
        if(argv[i])
            args.push_back(std::string(argv[i]));
    }
    for(auto iter = args.begin(); iter != args.end(); ++iter)
    {
        if(*iter == "-check")
            checkonly = true;
    }
    system("rmdir /S /Q tmp");
    system("mkdir tmp");
    if(!checkonly)
        system(std::string("robocopy " + args[1] + " " + args[2]+ " /E /R:1 /W:1").c_str());

    //get directory listing of source and destination
    system(std::string("dir /S "+args[1]+" > tmp/src.txt").c_str());
    system(std::string("dir /S "+args[2]+" > tmp/dest.txt").c_str());

    std::vector<std::string> src;
    std::vector<std::string> dest;
    ::MakeWordsFromFile("tmp/src.txt",src,false);
    ::MakeWordsFromFile("tmp/dest.txt",dest,false);

    std::cout<<"\n\n\n";
    int srcsize = ::GetTotalBytesFromDirListing(src);
    int destsize = ::GetTotalBytesFromDirListing(dest);

    if(srcsize != destsize)
        std::cout<<"Error in copy. Source and destination sizes do not match.\n";
    if(srcsize == destsize)
        std::cout<<"Success. Total Size In Bytes: "<<srcsize<<"\n";
    if(srcsize > destsize)
        std::cout<<"Error. Source is Larger By "<<srcsize-destsize<<" bytes.\n";
    if(srcsize < destsize)
        std::cout<<"Error. Source is Smaller By "<<destsize-srcsize<<" bytes.\n";

    system("rmdir /S /Q tmp");
    return 0;
}
