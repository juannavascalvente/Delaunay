/*
 * Menu.cpp
 *
 *  Created on: Jul 4, 2016
 *      Author: juan
 */

#include <GL/glut.h>
using namespace std;

#include "Menu.h"
#include "MenuOption.h"

/**************************************************************************
* 							Defines section
**************************************************************************/
// Input points set menu entries text.
#define	INPUTSET_MENU_TEXT				"Input"
#define	GENERATE_MENU_TEXT				"Generate"
#define	INPUTSET_RANDOM_TEXT			"randomly"
#define	INPUTSET_CLUSTER_TEXT			"Clusters"
#define	READ_MENU_TEXT					"Read"
#define	INPUTSET_READ_FLAT_TEXT			"Flat file"
#define	INPUTSET_READ_DCEL_POINTS_TEXT 	"Points DCEL file"
#define	INPUTSET_READ_TRIANG_TEXT		"StarTriangulation file"
#define	INPUTSET_READ_INCREMENTAL_TEXT	"Incremental files"
#define	INPUTSET_READ_VORONOI_TEXT		"Voronoi file"
#define INPUTSET_READ_GABRIEL_TEXT		"Gabriel file"
#define	INPUTSET_SHAKE_DCEL_TEXT		"Shake points"

// Parameters menu entries text.
#define PARAMETERS_MENU_TEXT			"Read parameters"

// StarTriangulation menu entries text.
#define	TRIANGULATIONS_MENU_TEXT		"Triangulations"
#define	TRIANGULATIONS_DELAUNAY_TEXT	"Delaunay"
#define	TRIANGULATIONS_STAR_TEXT		"Star triangulation"

// Figures menu entries.
#define FIGURES_MENU_TEXT				"Figures"
#define FIGURES_CONVEX_TEXT				"Convex hull"
#define FIGURES_CIRCUMCENTRES_TEXT		"Circumcircles"
#define FIGURES_EDGES_CRICLES_TEXT		"Edges circles"
#define FIGURES_VORONOI_TEXT			"Voronoi"
#define FIGURES_GABRIEL_TEXT			"Gabriel"
#define FIGURES_FILTER_EDGES			"Filter edges"
#define FIGURES_VORONOI_PATH_TEXT		"Voronoi path"
#define	FIGURES_TRIANGULATION_PATH_TEXT	"StarTriangulation path"

// Location menu entries.
#define LOCATION_MENU_TEXT				"Location"
#define	LOCATION_CLOSEST_POINTS_TEXT	"Closest to a given"
#define	LOCATION_2CLOSEST_POINTS_TEXT	"Two closest"
#define	LOCATION_FIND_FACE_TEXT			"Locate face"

// Export menu entries text.
#define	EXPORT_MENU_TEXT				"Write"
#define EXPORT_FLAT_FILE				"Write flat file"
#define EXPORT_DCEL_FILE				"Write DCEL file"
#define EXPORT_DELAUNAY_FILE			"Write Delaunay files"
#define EXPORT_VORONOI_FILE				"Write Voronoi file"
#define EXPORT_GABRIEL_FILE				"Write Gabriel file"

// Export menu entries text.
#define	INFORMATION_MENU_TEXT			"Information"
#define INFORMATION_DCEL_TEXT			"Dcel info"
#define INFORMATION_VORONOI_TEXT		"Voronoi info"

// Other menu entries text.
#define	ZOOM_MENU_TEXT					"Zoom"
#define	CLEAR_MENU_TEXT					"Clear"
#define	QUIT_MENU_TEXT					"Quit"


//------------------------------------------------------------------------
// Functions.
//------------------------------------------------------------------------
void menu_level_1(int menu_Id);

MenuOption enOption;


/***********************************************************************************************************************
* Class declaration
***********************************************************************************************************************/
Menu::Menu()
{
	// Initialize class attributes.
	this->menu_Id = 0;
	this->sub_Menu_Input = 0;
	this->sub_Menu_Id2 = 0;
	this->sub_Menu_Id3 = 0;
	this->sub_Menu_Id4 = 0;
	this->sub_Menu_Id5 = 0;
	this->subMenuRead = 0;
	this->subMenuGenerate = 0;
    enOption = MenuOption::defaultOption;
};


void Menu::updateMenu(Status *status)
{
	// Check if first time menu created.
	if (status->isFirstTime())
	{
		// Clear current menu.
		if (menu_Id != 0)
		{
			glutDestroyMenu(menu_Id);
		}

		// Add generate sub menu entries.
		this->subMenuRead = glutCreateMenu(menu_level_1);
		glutAddMenuEntry(INPUTSET_RANDOM_TEXT, randomly);
		glutAddMenuEntry(INPUTSET_CLUSTER_TEXT, cluster);

		// Add read sub menu entries.
		this->subMenuGenerate = glutCreateMenu(menu_level_1);
		glutAddMenuEntry(INPUTSET_READ_FLAT_TEXT, read_points_flat_file);
		glutAddMenuEntry(INPUTSET_READ_DCEL_POINTS_TEXT, read_points_dcel_file);
		glutAddMenuEntry(INPUTSET_READ_TRIANG_TEXT, read_dcel);
		glutAddMenuEntry(INPUTSET_READ_INCREMENTAL_TEXT, read_delaunay);
		glutAddMenuEntry(INPUTSET_READ_VORONOI_TEXT, read_voronoi);
		glutAddMenuEntry(INPUTSET_READ_GABRIEL_TEXT, read_gabriel);

		// Input points sub-menu.
		sub_Menu_Input = glutCreateMenu(menu_level_1);
		glutAddSubMenu(GENERATE_MENU_TEXT, this->subMenuRead);
		glutAddSubMenu(READ_MENU_TEXT, this->subMenuGenerate);

		// Main menu.
		menu_Id = glutCreateMenu(menu_level_1);
		glutAddSubMenu(INPUTSET_MENU_TEXT, sub_Menu_Input);
		glutAddMenuEntry(PARAMETERS_MENU_TEXT, parameters);
		glutAddMenuEntry(QUIT_MENU_TEXT, quit);
		glutAttachMenu(GLUT_RIGHT_BUTTON);

		status->setFirstTime(false);
	}
	else
	{
		// Clear current menu.
		glutDestroyMenu(menu_Id);

		// Add generate sub menu entries.
		this->subMenuRead = glutCreateMenu(menu_level_1);
		glutAddMenuEntry(INPUTSET_RANDOM_TEXT, randomly);
		glutAddMenuEntry(INPUTSET_CLUSTER_TEXT, cluster);

		// Add read sub menu entries.
		this->subMenuGenerate = glutCreateMenu(menu_level_1);
		glutAddMenuEntry(INPUTSET_READ_FLAT_TEXT, read_points_flat_file);
		glutAddMenuEntry(INPUTSET_READ_DCEL_POINTS_TEXT, read_points_dcel_file);
		glutAddMenuEntry(INPUTSET_READ_TRIANG_TEXT, read_dcel);
		glutAddMenuEntry(INPUTSET_READ_INCREMENTAL_TEXT, read_delaunay);
		glutAddMenuEntry(INPUTSET_READ_VORONOI_TEXT, read_voronoi);
		glutAddMenuEntry(INPUTSET_READ_GABRIEL_TEXT, read_gabriel);

		// Input points sub-menu.
		sub_Menu_Input = glutCreateMenu(menu_level_1);
		glutAddSubMenu(GENERATE_MENU_TEXT, this->subMenuRead);
		glutAddSubMenu(READ_MENU_TEXT, this->subMenuGenerate);

		// Triangulations submenu.
		if (status->isSet())
		{
			sub_Menu_Id2 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(TRIANGULATIONS_STAR_TEXT, star_triangulation);
			glutAddMenuEntry(TRIANGULATIONS_DELAUNAY_TEXT, delaunay_triangulation);
		}

		// Main figures submenu.
		if (status->isTriangulation() || status->isDelaunay())
		{
			sub_Menu_Id3 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(FIGURES_CONVEX_TEXT, convex_hull);
			glutAddMenuEntry(FIGURES_CIRCUMCENTRES_TEXT, circumcentres);
			glutAddMenuEntry(FIGURES_EDGES_CRICLES_TEXT, edge_circles);
			glutAddMenuEntry(FIGURES_VORONOI_TEXT, voronoi);
			glutAddMenuEntry(LOCATION_CLOSEST_POINTS_TEXT, closest_point);
			glutAddMenuEntry(LOCATION_2CLOSEST_POINTS_TEXT, two_closest);
			glutAddMenuEntry(LOCATION_FIND_FACE_TEXT, find_face);
			glutAddMenuEntry(FIGURES_TRIANGULATION_PATH_TEXT, triangulation_path);
			glutAddMenuEntry(FIGURES_FILTER_EDGES, filter_edges);

			if (status->isVoronoi())
			{
				glutAddMenuEntry(FIGURES_GABRIEL_TEXT, gabriel);
				glutAddMenuEntry(FIGURES_VORONOI_PATH_TEXT, voronoi_path);
			}
		}

		// Output data submenu.
		if (status->isSet())
		{
			sub_Menu_Id4 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(EXPORT_FLAT_FILE, write_points);
			if (status->isTriangulation())
			{
				glutAddMenuEntry(EXPORT_DCEL_FILE, write_dcel);
				if (status->isDelaunay())
				{
					glutAddMenuEntry(EXPORT_DELAUNAY_FILE, write_delaunay);
				}
				if (status->isVoronoi())
				{
					glutAddMenuEntry(EXPORT_VORONOI_FILE, write_voronoi);
				}
				if (status->isGabriel())
				{
					glutAddMenuEntry(EXPORT_GABRIEL_FILE, write_gabriel);
				}
			}
		}

		// Information submenu.
		if (status->isSet())
		{
			sub_Menu_Id5 = glutCreateMenu(menu_level_1);
			glutAddMenuEntry(INFORMATION_DCEL_TEXT, dcel_info);
			if (status->isVoronoi())
			{
				glutAddMenuEntry(INFORMATION_VORONOI_TEXT, voronoi_info);
			}
		}

		// Main menu.
		menu_Id = glutCreateMenu(menu_level_1);
		glutAddSubMenu(INPUTSET_MENU_TEXT, sub_Menu_Input);
		glutAddMenuEntry(PARAMETERS_MENU_TEXT, parameters);
		if (status->isSet())
		{
			glutAddSubMenu(TRIANGULATIONS_MENU_TEXT, sub_Menu_Id2);
		}
		if (status->isTriangulation() || status->isDelaunay())
		{
			glutAddSubMenu(FIGURES_MENU_TEXT, sub_Menu_Id3);
		}
		if (status->isSet())
		{
			glutAddSubMenu(EXPORT_MENU_TEXT, sub_Menu_Id4);
		}
		if (status->isSet())
		{
			glutAddSubMenu(INFORMATION_MENU_TEXT, sub_Menu_Id5);
		}
		glutAddMenuEntry(CLEAR_MENU_TEXT, clear);
		glutAddMenuEntry(QUIT_MENU_TEXT, quit);
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
}

MenuOption Menu::getMenuOption()
{
	return(enOption);
}


void Menu::resetMenuOption()
{
    enOption = defaultOption;
}

void menu_level_1(int menu_Id)
{
    enOption = (MenuOption) menu_Id;
}
