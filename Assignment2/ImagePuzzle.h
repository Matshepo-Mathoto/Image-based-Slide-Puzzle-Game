#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <math.h>
namespace MTHMAT043{
    
    //have a Tile class to encapsulate individual tile state and image data.
    class Tile{
        public:
        int width; 
        int height;
        int x_start;//store the points of where each index was taken from
        int y_end;
        unsigned char ** array;
        Tile(int height, int width);
        Tile();
        ~Tile();
    };
    //encapsulte informattion for all information for all tile boards
    class TileManager{
        public:
        int width;
        int height;
        int gridsize;
        int black_x;//keeps track of where the black tile is
        int black_y;
        Tile ** array;
        TileManager(int width, int height, int gridsize);
        ~TileManager();
        void addTileM(Tile &array_2d, int x_index, int y_index);
        void outputImage(unsigned char **ptr, int grid_size);
        void makePuzzle(Tile &blacktile);
        void movePuzzle();
        void storeTiles(int height_tile, int width_tile, TileManager TileManager, unsigned char **array_2d);
        void background(unsigned char **summary_array, int summary_height, int summary_width);
        void addToSummary(unsigned char **summary_array, int start, unsigned char **array_2d,int y_stop, int y_height, int x_stop);
       };
}