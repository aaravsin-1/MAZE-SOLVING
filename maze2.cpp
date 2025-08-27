#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

int rows = 16;
int maze[16][16] = {0};   // 0 = empty, 1 = path, 9 = wall
int startX = 0, startY = 0;
int endX = 15, endY = 8;
int robotX = startX, robotY = startY;

// Display maze with robot position
void display_maze()
{
    system("cls"); // use "clear" for Linux/Mac
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<rows;j++)
        {
            if(i == robotX && j == robotY) cout << "R ";
            else if(maze[i][j] == 2) cout << "S ";
            else if(maze[i][j] == 3) cout << "E ";
            else if(maze[i][j] == 9) cout << "# ";
            else if(maze[i][j] == 1) cout << ". ";
            else cout << "0 ";
        }
        cout << endl;
    }
    this_thread::sleep_for(chrono::milliseconds(100));
}

// Generate random maze
void generate_maze()
{
    srand(time(0));
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<rows;j++)
        {
            int r = rand() % 6; // 20% chance wall
            if(r == 0) maze[i][j] = 9;
            else maze[i][j] = 0;
        }
    }
    maze[startX][startY] = 2;
    maze[endX][endY] = 3;
}

// Automatic exploration (DFS)
void explore_maze(int x, int y, vector<vector<bool>> &visited)
{
    robotX = x;
    robotY = y;
    visited[x][y] = true;
    if(maze[x][y] == 0) maze[x][y] = 1;
    display_maze();

    int dir[4][2] = {{-1,0},{1,0},{0,-1},{0,1}}; // up, down, left, right

    for(int i=0;i<4;i++)
    {
        int nx = x + dir[i][0];
        int ny = y + dir[i][1];

        if(nx>=0 && nx<rows && ny>=0 && ny<rows && !visited[nx][ny])
        {
            if(maze[nx][ny] == 9) continue; // wall
            explore_maze(nx, ny, visited);
            // backtrack
            robotX = x;
            robotY = y;
            display_maze();
        }
    }
}

// BFS to find fastest path
void find_fast_route()
{
    struct Node{
        int i,j;
        vector<pair<int,int>> path;
    };
    bool visited[16][16] = {false};
    queue<Node> q;
    q.push({robotX, robotY, {{robotX, robotY}}});
    visited[robotX][robotY] = true;
    int dir[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};

    while(!q.empty())
    {
        Node cur = q.front(); q.pop();
        int ci = cur.i, cj = cur.j;

        if(ci == endX && cj == endY)
        {
            for(auto p: cur.path)
            {
                if(maze[p.first][p.second]!=2 && maze[p.first][p.second]!=3)
                {
                    robotX = p.first;
                    robotY = p.second;
                    maze[p.first][p.second] = 1;
                    display_maze();
                }
            }
            cout << "Fastest path completed!\n";
            return;
        }

        for(auto d: dir)
        {
            int ni = ci + d[0];
            int nj = cj + d[1];
            if(ni>=0 && ni<rows && nj>=0 && nj<rows && !visited[ni][nj] && maze[ni][nj]!=9)
            {
                visited[ni][nj] = true;
                vector<pair<int,int>> newPath = cur.path;
                newPath.push_back({ni,nj});
                q.push({ni,nj,newPath});
            }
        }
    }
    cout << "No path found!\n";
}

int main()
{
    generate_maze();
    display_maze();
    cout << "Robot starts exploring the maze...\n";
    vector<vector<bool>> visited(rows, vector<bool>(rows,false));
    explore_maze(startX, startY, visited);
    cout << "Maze fully explored. Press Enter to find fastest path...\n";
    cin.get(); cin.get(); // wait for user input
    find_fast_route();
}
