// author: mathias, 2018, mathias.rothermel@geod.baug.ethz.ch
#pragma once
#include <vector>
#include <string>
#include "../Ori/Orientation.h"

class OriListIO
{
    public:

    	OriListIO();
    	~OriListIO();

    	// Do the saving
	static void saveList(	const std::vector<Orientation>& orivec,
				const std::vector<std::string>& namevec,
				const std::string& savename);

    	// Do the loading
    	static void loadList(std::vector<Orientation>& orivec, const std::string& savename);

    private:

	static void convertStringToOri(const std::string& s, Orientation&ori);

	static void convertOriToString(const Orientation& ori, std::string& s);
};
