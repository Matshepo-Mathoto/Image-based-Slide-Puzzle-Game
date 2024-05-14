#include "ImagePuzzle.h"

MTHMAT043::Tile::Tile(int a_height, int a_width):height(a_height),width(a_width)
{
    array = new unsigned char*[a_height];
    //allocate space for tile array
    for(int c =0; c<height;c++)
    {
        array[c] = new unsigned char[width];
    }
}
MTHMAT043::Tile::Tile()
{}
MTHMAT043::Tile::~Tile()
{
    for(int i=0; i<height; i++)
    {
        delete [] array[i];
    }
    delete [] array;
}

MTHMAT043::TileManager::TileManager(int a_height, int a_width, int a_gridsize):height(a_height),width(a_width),gridsize(a_gridsize)
{
    //Allocate memory for the array to store the tiles
    array = new Tile*[a_width];
    for(int c =0; c<height;c++)
    {
        array[c] = new Tile[width];
    }
}
MTHMAT043::TileManager::~TileManager()
{
    //deallocate memory for the tiles
    for(int c =0; height;c++)
    {
        delete [] array[c];
    }
    delete [] array;
}
void MTHMAT043::TileManager::addTileM(Tile &array_2d, int x_index, int y_index){
    array[x_index][y_index] = array_2d;
}
void MTHMAT043::TileManager::outputImage(unsigned char **ptr, int grid_size){
    //convert the tile manager array with tiles into a normal 2d array
    for(int i=0; i<height; i++)
    {
        for(int j=0; j<width;j++)
        {
           int start = array[i][j].x_start;
           int end = array[i][j].y_end;
           for(int a=0; a<grid_size;a++)
           {
                for(int b=0; b< grid_size;b++)
                {
                    ptr[end+a][start+b] =array[i][j].array[a][b];
                }
           }
        }
    }
}
void MTHMAT043::TileManager::makePuzzle(Tile &blacktile)
{
    //update the position of the black tile and make the bottom end tile black
    black_x = height-1;
    black_y = width -1;
    for(int i=0; i<gridsize;i++)
    {
        for(int j=0;j<gridsize;j++)
        {
            blacktile.array[i][j] =0;
        }
    }
}
void MTHMAT043::TileManager::movePuzzle()
{
    //check if a random movement selected is valid or not
    bool check_rand = true; 
    while(check_rand)
    {
        //posible moves of each tile, up, left, right and down
        int check[4][2] = {{black_x-1,black_y},{black_x+1,black_y},{black_x,black_y-1},{black_x,black_y+1}};
        //random generator
        std::random_device rd;
        std::mt19937 rng(rd());
        //range of possible tile movements
        int min =0;
        int max =3;
        //uniform distributor for the range
        std::uniform_int_distribution<int> distribution(min,max);
        //generate random number
        int randomNum = distribution(rng);
        //check if the random choosen move is valid
        if((check[randomNum][0] < height && check[randomNum][1] < width) && (check[randomNum][0] >= 0 && check[randomNum][1] >= 0))
        {
            check_rand = false;
            /*
                -get the random tile and the black tile
                -update the coordinates of the black tiles
                -exchenge the image tile with the black tile
            */
            MTHMAT043::Tile *tile_change = &array[check[randomNum][0]][check[randomNum][1]];
            MTHMAT043::Tile *black_tile = &array[black_x][black_y];
            black_x = check[randomNum][0];
            black_y = check[randomNum][1]; 
            for(int i=0;i<tile_change->width;i++)
            {
                for(int j=0; j<tile_change->height;j++)
                {
                    black_tile->array[i][j] = tile_change->array[i][j];
                    tile_change->array[i][j] =0;
                }
            }
        }
    }
}
void MTHMAT043::TileManager::background(unsigned char **summary_array, int summary_height, int summary_width)
{
    //make this array to have a white image pixle
    for(int i=0; i< summary_height; i++)
    {
        for(int j=0; j < summary_width; j++)
        {
            summary_array[i][j] = 255;
        }
    }
}
void MTHMAT043::TileManager::addToSummary(unsigned char **summary_array, int start, unsigned char **array_2d, int height_tile, int y_height, int width_tile)
{
    //add an image to summary array
    for(int i=0; i<height_tile;i++)
    {
        for(int j=0; j<width_tile ; j++)
        {
            summary_array[i+y_height][j+start] = array_2d[i][j];
        }
    }
}