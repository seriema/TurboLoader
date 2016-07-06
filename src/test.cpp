#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
namespace pt = boost::property_tree;

struct RomData
{
	std::string path_rom;     // "./Super Mario All-Stars (U) [!].smc"
	std::string name;         // "Super Mario All-Stars + Super Mario World"
	std::string description;  // "Revisit the magic and fun of the classic Super Mario Bros. series on your Super NES!  Play through Super Mario World, plus all the great Super Mario Bros. games for the NES have been powered up with 16-bit graphics and sound and collected on one super game pak.  As an added bonus, the previously unreleased “Lost Levels” are included.  These super challenging courses have never been available in this country until now!"
	std::string path_boxart;  // "~/.emulationstation/downloaded_images/snes/Super Mario All-Stars (U) [!]-image.jpg"
	float       rating;       // 0.8
	std::string release_date; // "19931216T000000"
	std::string developer;    // "Nintendo"
	std::string publisher;    // "Nintendo"
	std::string genre;        // "Adventure"
	int         players;      // 2
};

std::vector<RomData> load(const std::string &filename)
{
	// Create empty property tree object
	pt::ptree tree;

	// Parse the XML into the property tree.
	pt::read_xml(filename, tree);

	std::vector<RomData> roms;

	for (auto& child : tree.get_child("gameList"))
	{
		if (child.first != "game") {
			continue;
		}

		RomData rom;

		rom.path_rom     = child.second.get<std::string>("path");
		rom.name         = child.second.get<std::string>("name");
		rom.description  = child.second.get<std::string>("desc");
		rom.path_boxart  = child.second.get<std::string>("image");
		rom.rating       = child.second.get("rating", -1);
		rom.release_date = child.second.get<std::string>("releasedate");
		rom.developer    = child.second.get<std::string>("developer");
		rom.publisher    = child.second.get<std::string>("publisher");
		rom.genre        = child.second.get<std::string>("genre");
		rom.players      = child.second.get("players", -1);

		roms.push_back(rom);
	}

	return roms;
}


int main()
{
    try
    {
	auto roms = load("res/gamelist.xml");
	std::cout << "Success" << std::endl;
		std::cout << roms.size() << " games parsed" << std::endl;
    }
    catch (std::exception &e)
    {
	std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
