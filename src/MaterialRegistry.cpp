#include "MaterialRegistry.h"
#include <nlohmann/json.hpp>
#include <filesystem>
#include <fstream>
#include <unordered_map>
#include <string>
#include <iostream>
#include <stdexcept>
	
using json = nlohmann::json;

static std::unordered_map<std::string, CellType> cellTypeMap = {
	{ "Air", CellType::Air },
	{ "Sand", CellType::Sand },
	{ "Water", CellType::Water }
};

static std::unordered_map<CellType, std::string> reverseCellTypeMap = {
	{ CellType::Air, "Air" },
	{ CellType::Sand, "Sand" },
	{ CellType::Water, "Water" }
};

bool MaterialRegistry::load() {
	loaded.fill(false);

	std::ifstream f("assets/Materials.json");

	if (!f.is_open()) {
		std::cerr << "Failed to open Materials.json, working dir: " << std::filesystem::current_path() << std::endl;
		return false;
	}

	std::string curKey;

	try {
		json data = json::parse(f);

		for (auto& [key, val] : data.items()) {
			curKey = key;

			auto it = cellTypeMap.find(key);

			if (it == cellTypeMap.end()) {
				std::cerr << "Unknown cell type: " << key << std::endl;
				return false;
			}

			const json& color = val.at("color");
			const int r = color.at("r").get<int>();
			const int g = color.at("g").get<int>();
			const int b = color.at("b").get<int>();
			const int a = color.at("a").get<int>();
			sf::Color colorObj;

			if (0 <= r && r <= 255 && 0 <= g && g <= 255 && 0 <= b && b <= 255 && 0 <= a && a <= 255) {
				colorObj = sf::Color(static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b), static_cast<uint8_t>(a));
			}
			else {
				std::cerr << "Invalid color values for cell type: " << key << std::endl;
				return false;
			}

			const float velocityTransfer = val.at("velocityTransfer").get<float>();
			const float bounce = val.at("bounce").get<float>();
			const float  friction = val.at("friction").get<float>();

			if (velocityTransfer < 0.0f || velocityTransfer > 1.0f) {
				std::cerr << "Invalid velocityTransfer value for cell type: " << key << std::endl;
				return false;
			}

			if (bounce < 0.0f || bounce > 1.0f) {
				std::cerr << "Invalid bounce value for cell type: " << key << std::endl;
				return false;
			}

			if (friction < 0.0f || friction > 1.0f) {
				std::cerr << "Invalid friction value for cell type: " << key << std::endl;
				return false;
			}

			properties[static_cast<size_t>(it->second)] = {
				colorObj,
				velocityTransfer,
				bounce,
				friction
			};

			loaded[static_cast<size_t>(it->second)] = true;
		}

		for (size_t i = 0; i < static_cast<size_t>(CellType::Count); i++)
		{
			if (loaded[i] == false) {
				auto it = reverseCellTypeMap.find(static_cast<CellType>(i));

				if (it == reverseCellTypeMap.end()) {
					std::cerr << "Unknown cell type: " << i << std::endl;
					return false;
				}

				std::cerr << "Missing properties for cell type: " << it->second << std::endl;
				return false;
			}
		}
	}
	catch (const json::exception& e) {
		std::cerr << "Error parsing Materials.json at key: " << curKey << ". Exception: " << e.what() << std::endl;	
		return false;
	}

	return true;
};