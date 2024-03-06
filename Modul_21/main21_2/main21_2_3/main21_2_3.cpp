#include <iostream>
#include <vector>

enum building_tipe
{
	house, garage, barn, bathhouse
};
enum room_tipe
{
	living, children, kitchen, bedroom, bathroom
};

struct rooms {
	room_tipe tipe = room_tipe::kitchen;
	float room_area = 0.0f;
};

struct floors {
	float ceiling_height = 0.0f;
	std::vector<rooms> vec_rooms;
};

struct buildings {
	building_tipe tipe = building_tipe::house;
	float building_area = 0.0f;
	bool furnace_with_pipe = false;
	std::vector<floors> vec_floors;
};

struct land_plots {
	int number = 0;
	float area_land_plot = 0.0f;
	std::vector<buildings> vec_buildings;
};

bool invalid_data_int(int intcount) {
	if (intcount <= 0) {
		std::cout << "Invalid data! Try again.\n\n";
		return false;
	}
	return true;
}

bool invalid_data_flt(float fltcount) {
	if (fltcount <= 0) {
		std::cout << "Invalid data! Try again.\n\n";
		return false;
	}
	return true;
}

bool invalid_data_per(int number, int begin, int end) {
	if (number < begin || number > end) {
		std::cout << "Invalid data! Try again.\n\n";
		return false;
	}
	return true;
}

int main()
{

	std::cout << "***************** The data model for the village *****************\n\n";
	int plots_number;
	bool invalid_data;

	std::cout << "Specify the number of plots in the village: ";
	std::cin >> plots_number;
	
	invalid_data = invalid_data_int(plots_number);
	std::vector<land_plots> vec_plots;
	float land_plots_area = 0.0f;		//Переменная для подсчёта площади посёлка
	float all_buildings_area = 0.0f;	//Переменная для подсчёта площади всех зданий в посёлке
	

	for (int i = 0; i < plots_number; i++) {		//Создадим и опишем каждый участок
		land_plots plot;

		std::cout << "Specify the plot number: ";
		std::cin >> plot.number;
		invalid_data = invalid_data_int(plot.number);
		if (!invalid_data)
			return 0;

		std::cout << "Specify the area of the plot: ";
		std::cin >> plot.area_land_plot;
		invalid_data = invalid_data_flt(plot.area_land_plot);
		if (!invalid_data)
			return 0;

		land_plots_area += plot.area_land_plot;

		int buildings_number;
		std::cout << "Specify the number of buildings on the plot: ";
		std::cin >> buildings_number;
		invalid_data = invalid_data_int(buildings_number);
		if (!invalid_data)
			return 0;

		//plot.vec_buildings.resize(buildings_number);

		for (int b = 0; b < buildings_number; b++) {		//Создадим и опишем каждое здание на участке
			buildings building;

			std::cout << "\nSpecify the tipe of building on the plot number " << plot.number << " (0 - house, 1 - garage, 2 - barn, 3 - bathhouse) : ";
			int intbuilding;
			std::cin >> intbuilding;
			invalid_data = invalid_data_per(intbuilding, 0, 3);
			if (!invalid_data)
				return 0;

			switch (intbuilding)
			{
			case 0:
				building.tipe = building_tipe::house;
				break;
			case 1:
				building.tipe = building_tipe::garage;
				break;
			case 2:
				building.tipe = building_tipe::barn;
				break;
			case 3:
				building.tipe = building_tipe::bathhouse;
				break;
			}

			std::cout << "Specify the building area: ";
			std::cin >> building.building_area;
			invalid_data = invalid_data_flt(building.building_area);
			if (!invalid_data)
				return 0;

			all_buildings_area += building.building_area;

			if (intbuilding == 0 || intbuilding == 3) {
				std::cout << "Indicate the presence of a furnace with a pipe in the building (y/n) : ";
				char presence_furnace;
				std::cin >> presence_furnace;
				if (presence_furnace == 'y')				//По умолчанию - ложь
					building.furnace_with_pipe = true;
			}

			int floors_number;
			if (intbuilding == 0) {
				std::cout << "Specify the number of floors in the house (1-3): ";
				std::cin >> floors_number;
				invalid_data = invalid_data_per(floors_number, 1, 3);
				if (!invalid_data)
					return 0;

				//building.vec_floors.resize(floors_number);
			}

			if (intbuilding == 0) {
				for (int f = 0; f < floors_number; f++) {		//Создадим и опишем каждый этаж в жилом доме
					floors floor;

					std::cout << "\nSpecify the ceiling height on the " << f + 1 << " floor: ";
					std::cin >> floor.ceiling_height;
					invalid_data = invalid_data_flt(floor.ceiling_height);
					if (!invalid_data)
						return 0;

					int rooms_number;
					std::cout << "Specify the number of rooms on the " << f + 1 << " floor (2-4): ";
					std::cin >> rooms_number;
					invalid_data = invalid_data_per(rooms_number, 2, 4);
					if (!invalid_data)
						return 0;

					//floor.vec_rooms.resize(rooms_number);
					
					for (int r = 0; r < rooms_number; r++) {		//Создадим и опишем каждую комнату на этаже в жилом доме
						rooms room;

						std::cout << "\nSpecify the tipe of room (0 - living, 1 - children, 2 - kitchen, 3 - bedroom, 4 - bathroom) on the "
							<< f + 1 << " floor: ";
						int introom;
						std::cin >> introom;
						invalid_data = invalid_data_per(introom, 0, 4);
						if (!invalid_data)
							return 0;

						switch (introom)
						{
						case 0:
							room.tipe = room_tipe::living;
							break;
						case 1:
							room.tipe = room_tipe::children;
							break;
						case 2:
							room.tipe = room_tipe::kitchen;
							break;
						case 3:
							room.tipe = room_tipe::bedroom;
							break;
						case 4:
							room.tipe = room_tipe::bathroom;
							break;
						}

						std::cout << "Specify the " << room_tipe(room.tipe) << " room area : ";
						std::cin >> room.room_area;
						invalid_data = invalid_data_flt(room.room_area);
						if (!invalid_data)
							return -1;

						floor.vec_rooms.push_back(room);
					}	//Конец описания комнаты жилого дома

					std::cout << "End of the description of the house floor " << f + 1 << ".\n\n";
					building.vec_floors.push_back(floor);
				}	//Конец описания этажа жилого дома

			}	//Если здание - жилой дом

			std::cout << "End of the description of the building.\n\n";
			plot.vec_buildings.push_back(building);
		}	//Конец описания здания на участке

		std::cout << "End of the description of the plot number " << plot.number << ".\n\n";
		vec_plots.push_back(plot);
	}	//Конец описания участка

	//Рассчитаем, например, процент земли, занимаемой постройками, от общей площади посёлка
	std::cout << "Buildings occupy " << (all_buildings_area / land_plots_area) * 100 << " % of the village area\n\n";
}