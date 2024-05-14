#include "ImagePuzzle.h"
int main(int argc, char *argv[])
{
    std::string filename;
    int gridsize;
    int n_moves;
    int count =0;
    std::string filename_summary;
    std::vector<std::string> filedata;
    //check argument count ;
    if(argc == 5)
    {
        gridsize = std::stoi(argv[1]);
        n_moves = std::stoi(argv[2]);
        filename = argv[3];
        filename_summary = argv[4];
        if(gridsize%2 ==0)
        {
            std::cout << "Program requires grid size to be an odd number" << std::endl;
            return 1;
        }
    }
    else
    {
        std::cout << "program requires programName -s(gridSize)  -n(moves No) -i(inputImage) -x (summaryImageName)" << std::endl;
        return 1;
    }
    //read the image 
    std::ifstream myfile(filename+".pgm", std::ios::binary);
    //check if file exist
    if(!myfile)
    {
        std::cout << "File not file." << std::endl;
        return 1;
    }
    std::string line;
    int width;
    int height;
    //check if we have reached the end of the file
    while(!myfile.eof())
    {
        std::getline(myfile,line);
        //get all the file data and store them to a vector
        if(isdigit(line[0]))
        {
            //read the width and heigh of the image
            std::istringstream iss(line);
            iss >> width;
            iss >> height;
            //resize the width and height of the output image
            int width_tile = (width/gridsize) *gridsize;
            int height_tile = (height/gridsize) *gridsize;
            std::string a = std::to_string(width_tile) + " "+ std::to_string(height_tile);
            filedata.push_back(a);
            myfile >> line >> std::ws;
            filedata.push_back(line);
            //allocate space for the array to store the input image
            unsigned char ** array_2d = new unsigned char*[height];
            for(int i=0; i < height; i++)
            {
                array_2d[i] = new unsigned char[width];
            }
            //read all the the pixles into the input array
            for(int i=0; i< height; i++)
            {
                myfile.read(reinterpret_cast<char*> (array_2d[i]), width);
            }
            myfile.close();
            /*  
                allocate space for the array with adjusted height and width that will store the output images
                of the manipulated input image
            */
            unsigned char ** array_2d_out= new unsigned char*[height_tile];
            for(int i=0; i < height_tile; i++)
            {
                array_2d_out[i] = new unsigned char[width_tile];
            }
             /*
                -rows and colombs of images in the summary image
                -added 10*width (extra space to the summary array) for the white lines to separate the images
             */
            int y_images = round(sqrt(n_moves+1));
            int x_images = ceil(sqrt(n_moves+1));
            int summary_width = width_tile*x_images+(x_images*10);
            int summary_height = height_tile*y_images+(y_images*10);
            //allocate space for summary array
            unsigned char ** summary_array= new unsigned char*[summary_height];
            for(int i=0; i < summary_height; i++)
            {
                summary_array[i] = new unsigned char[summary_width];
            }
            //tilemanager object to store the the tiles array
            MTHMAT043::TileManager * tileManager = new  MTHMAT043::TileManager((height_tile/gridsize), (width_tile/gridsize),gridsize);
            for(int i=0; i< height_tile; i+=gridsize)
            {
                for(int j=0; j< width_tile; j+=gridsize)
                {
                    /*
                        -store the indexes of the images to use them when transforming the tiles back into a uniform 
                        -store the images pixles into the tiles and add them to the tilemanagement array.
                        -pass the indexed of the array to tilemanagement to ensure that the tiles are added
                        at appropriate position
                    */
                    MTHMAT043::Tile *tile = new  MTHMAT043::Tile(gridsize, gridsize);
                    tile->x_start = j;
                    tile ->y_end = i;
                    //populate the tile with appropriate information
                    for(int a =0; a<gridsize;a++)
                    {
                        for(int b=0; b<gridsize;b++)
                        {
                            tile->array[a][b] = array_2d[a+i][b+j];
                        }
                    }
                    if(i==0 && j>0)
                    {
                        tileManager->addTileM(*tile,i,(j/gridsize));
                    }
                    else if(j==0 && i>0)
                    {
                        tileManager->addTileM(*tile,(i/gridsize),(j));
                    }
                    else{
                        tileManager->addTileM(*tile,(i/gridsize),(j/gridsize));
                    }
                }
            }
            //Use this method to make the bottom end tile black
            tileManager->makePuzzle(tileManager->array[tileManager->height-1][tileManager->width-1]);
            //deallocate memory of the input since we won't be using this array
            for(int i=0; i<height ;i++)
            {
                delete [] array_2d[i];
            }
            delete [] array_2d;
            //populate the summary array with white pixles
            tileManager->background(summary_array, summary_height, summary_width);
            //move the puzzle in random ways n_moves times
            int start = 0+10; //stores the width value of each image which is the index of where each image should start and end in the summary array
            int end = width_tile+10;
            int y_height=0+10; //stores the height value of each image which is the index of which row the image will be in the summary array
            for(int i=0;i<n_moves+1;i++)
            {
                if(count >0)
                {
                    //exchanges the blacktile with adjacent tile
                    tileManager->movePuzzle(); 
                }
                //converts the tile array into a 2d array
                tileManager->outputImage(array_2d_out, gridsize);
                //add the converted 2d array to the summary array
                tileManager->addToSummary(summary_array, start, array_2d_out, height_tile, y_height,width_tile);
                std::ofstream output(filename.substr(0,filename.find("."))+"-"+std::to_string(count)+".pgm", std::ios::binary);
                //the 2 for loops one writes the image's data and the other writes the image's pixles
                for(const std::string &str: filedata) 
                {
                    size_t strSize = str.size(); 
                    output.write(str.c_str(),strSize);
                    output.put('\n');
                }
                for(int i=0; i< height_tile; i++)
                {
                    output.write(reinterpret_cast<char*>(array_2d_out[i]),width_tile);
                }
                count++;
                output.close();
                if(end < (width_tile*x_images))
                {
                    //increament the start and end for the next image
                    start +=width_tile+10;
                    end +=width_tile+10;
                }
                else{
                    //reset the value of the start and end for a new row
                    start =0+10;
                    end =width_tile+10;
                }
                if((count)%x_images ==0 && count>0)
                {
                    //increament the row value of the summary image when the first row is full
                    y_height+=height_tile+10;
                }
            }
            std::ofstream summary_file(filename_summary+".pgm", std::ios::binary);
            //change the width and height values for the summary image
            a = std::to_string(summary_width) + " "+ std::to_string(summary_height);
            filedata.at(2) = a;
            //the 2 for loops one writes the image's data and the other writes the image's pixles
            for(const std::string &str: filedata) 
            {
                size_t strSize = str.size(); 
                summary_file.write(str.c_str(),strSize); 
                summary_file.put('\n'); 
            }
            for(int i=0; i< summary_height; i++)
            {
                summary_file.write(reinterpret_cast<char*>(summary_array[i]),summary_width);
            }
            summary_file.close();
            //deallocate memory of the output image
            for(int i=0; i<height_tile ;i++)
            {
                delete [] array_2d_out[i];
            }
            delete [] array_2d_out;
            //deallocate memory of the summary array image
            for(int i=0; i<summary_height ;i++)
            {
                delete [] summary_array[i];
            }
            delete [] summary_array;
        }
        filedata.push_back(line);
    }

}