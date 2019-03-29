#include <stdio.h>
#include <stdlib.h>
//var names are in span_case
//function names are in camelCase 

const int EMPTY_CELL = 1;
const int OCCUPIED_CELL = 0;
const int VISITED_CELL = -1;

int maze_grid[1000][1000];
int height,width;

int mx_r=999,mx_c=999,mx_path;

int depth = 1;

void readSchemaFile(){
   char ch;
   FILE * file_pointer;
 
   file_pointer = fopen("input.in", "r"); 
 
   if (file_pointer == NULL){
      perror("Error while opening the file. Make Sure File (input.in) exists in the same directory\n");
      exit(EXIT_FAILURE);
   }

   int current_row = 0,current_column = 0;
   while((ch = fgetc(file_pointer)) != EOF){
      if(ch == '#'){
       maze_grid[current_row][current_column] = OCCUPIED_CELL;   
      }
      else{
        if(ch == '.'){
            maze_grid[current_row][current_column] = EMPTY_CELL;   
        }
        else{  
            width = current_column;
            current_column = -1;
            current_row++;   
        }
      }
      current_column++;
   }
   height = current_row;
    
   fclose(file_pointer);
}

bool checkLocationValidity(int r,int c){
    return r >= 0 && c >= 0 && r < height && c < width;
}

int dfsPath(int r,int c){
    maze_grid[r][c] = VISITED_CELL;


    int path_length = 0;
    int dx[4] = {0,0,1,-1};
    int dy[4] = {1,-1,0,0};
    int curr_r,curr_c;

    for(int i=0;i<4;i++){
        curr_r = r + dy[i];
        curr_c = c + dx[i];

        if(checkLocationValidity(curr_r,curr_c) && 
        maze_grid[curr_r][curr_c] != OCCUPIED_CELL 
        && maze_grid[curr_r][curr_c]  != VISITED_CELL  && maze_grid[curr_r][curr_c] > 0 ){

            path_length += dfsPath(curr_r,curr_c);


        }

    }
    return path_length + 1;
}

void setLongestPath(int r,int c){
    maze_grid[r][c] = depth++;


    int path_length = 0;
    int dx[4] = {0,0,1,-1};
    int dy[4] = {1,-1,0,0};
    int curr_r,curr_c;

    for(int i = 0;i < 4;i++){
        curr_r = r + dy[i];
        curr_c = c + dx[i];

        if(checkLocationValidity(curr_r,curr_c) && 
        maze_grid[curr_r][curr_c] != OCCUPIED_CELL 
        && maze_grid[curr_r][curr_c] < 1 ){

            setLongestPath(curr_r,curr_c);

        }

    }

}

void locateStartingPointInLongestPath(){
    
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            if(checkLocationValidity(r,c) && maze_grid[r][c] != OCCUPIED_CELL && maze_grid[r][c]  != VISITED_CELL){
                
                maze_grid[r][c] =  dfsPath(r,c);

                if(maze_grid[r][c]  > mx_path){
                    maze_grid[mx_r][mx_c] = VISITED_CELL;

                    mx_path = maze_grid[r][c];
                    mx_r = r;
                    mx_c = c;
                }
                else{
                    maze_grid[r][c] = VISITED_CELL;
                }
            }
        }
    }

}



int main(){ 
    readSchemaFile();//read file data into  2d array 

    locateStartingPointInLongestPath();//get longest path
    printf("%d \n",mx_path);

    setLongestPath(mx_r,mx_c);
    for(int r = 0; r < height; r++){
        for(int c = 0; c < width; c++){
            if(maze_grid[r][c] == OCCUPIED_CELL)
                printf("#");
            else 
                if(maze_grid[r][c] == VISITED_CELL )
                    printf(".");
                else
                    printf("%d",abs(maze_grid[r][c]));
        }
        printf("\n");
    }
    
   return 0;
}