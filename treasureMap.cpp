#include "treasureMap.h"
#include "queue.h"
#include <bitset>
using namespace std;

treasureMap::treasureMap(const PNG &baseim, const PNG &mazeim, pair<int, int> s)
{
    start = s;
    base = baseim;
    maze = mazeim;
}

void treasureMap::setGrey(PNG &im, pair<int, int> loc)
{

    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2 * ((pixel->r) / 4);
    pixel->g = 2 * ((pixel->g) / 4);
    pixel->b = 2 * ((pixel->b) / 4);
}

void treasureMap::setLOB(PNG &im, pair<int, int> loc, int d)
{
    RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
    int mazeValue = d % 64;
    pixel->r = ((mazeValue & 0b110000) >> 4) | (pixel->r & 0b11111100);
    pixel->g = ((mazeValue & 0b001100) >> 2) | (pixel->g & 0b11111100);
    pixel->b = (mazeValue & 0b000011) | (pixel->b & 0b11111100);
}

PNG treasureMap::renderMap()
{
    PNG res = base;

    vector<vector<bool>> visited(res.height(), vector<bool>(res.width(), bool()));
    vector<vector<int>> paths(res.height(), vector<int>(res.width(), int()));
    Queue<pair<int, int>> q;
    visited[start.second][start.first] = true;
    paths[start.second][start.first] = 0;
    setLOB(res, start, 0);
    q.enqueue(start);

    while (!q.isEmpty())
    {

        pair<int, int> curr = q.peek();
        int curr_dist = paths[curr.second][curr.first];
        q.dequeue();
        vector<pair<int, int>> neighbors_vec = neighbors(curr);
        for (pair<int, int> &next : neighbors_vec)
        {
            if (good(visited, curr, next))
            {
                visited[next.second][next.first] = true;
                paths[next.second][next.first] = curr_dist + 1;
                setLOB(res, next, curr_dist + 1);
                q.enqueue(next);
            }
        }
    }

    return res;
}

PNG treasureMap::renderMaze()
{
    PNG res = base;

    vector<vector<bool>> visited(res.height(), vector<bool>(res.width(), bool()));
    vector<vector<int>> paths(res.height(), vector<int>(res.width(), int()));
    Queue<pair<int, int>> q;
    visited[start.second][start.first] = true;
    paths[start.second][start.first] = 0;
    q.enqueue(start);

    while (!q.isEmpty())
    {
        int curr_dist = paths[start.second][start.first];
        pair<int, int> curr = q.peek();
        q.dequeue();
        setGrey(res, curr);
        vector<pair<int, int>> neighbors_vec = neighbors(curr);
        for (pair<int, int> &next : neighbors_vec)
        {
            if (good(visited, curr, next))
            {
                visited[next.second][next.first] = true;
                paths[next.second][next.first] = curr_dist + 1;
                q.enqueue(next);
            }
        }
    }
    for (int i = start.second - 3; i <= start.second + 3; i++)
    {
        for (int j = start.first - 3; j <= start.first + 3; j++)
        {
            if (0 <= j && j < (signed)base.width() && 0 <= i && i < (signed)base.height())
            {

                RGBAPixel *currPixel = res.getPixel(j, i);
                currPixel->r = 255;
                currPixel->g = 0;
                currPixel->b = 0;
            }
        }
    }

    return res;
}
bool treasureMap::good(vector<vector<bool>> &v, pair<int, int> curr, pair<int, int> next)
{
    //test if pixel is in image
    bool inImage = (0 <= next.first && next.first < (signed)base.width() && 0 <= next.second && next.second < (signed)base.height());
    if (inImage)
    {
        //tests if pixel is unvisited (by using v)
        bool visited = v[next.second][next.first];
        if (!visited)
        {
            RGBAPixel *currPixel = maze.getPixel(curr.first, curr.second);
            RGBAPixel *nextPixel = maze.getPixel(next.first, next.second);
            return (*currPixel == *nextPixel);
        }
        return false;
    }
    return false;
}

vector<pair<int, int>> treasureMap::neighbors(pair<int, int> curr)
{
    vector<pair<int, int>> result;
    result.push_back(make_pair(curr.first - 1, curr.second)); //left
    result.push_back(make_pair(curr.first, curr.second + 1)); //below
    result.push_back(make_pair(curr.first + 1, curr.second)); //right
    result.push_back(make_pair(curr.first, curr.second - 1)); //above

    return result;
}
