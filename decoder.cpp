#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG &tm, pair<int, int> s)
{
   start = s;
   mapImg = tm;
   vector<vector<bool>> visited(mapImg.height(), vector<bool>(mapImg.width(), false));
   vector<vector<int>> paths(mapImg.height(), vector<int>(mapImg.width(), 0));
   vector<vector<pair<int, int>>> parents(mapImg.height(), vector<pair<int, int>>(mapImg.width(), pair<int, int>()));
   Queue<pair<int, int>> q;
   Stack<pair<int, int>> revPath;
   visited[start.second][start.first] = true;
   paths[start.second][start.first] = 0;
   parents[start.second][start.first] = start;
   q.enqueue(start);
   pair<int, int> curr;
   while (!q.isEmpty())
   {
      curr = q.dequeue();
      vector<pair<int, int>> neighbors_vec = neighbors(curr);
      for (pair<int, int> &next : neighbors_vec)
      {
         if (good(visited, paths, curr, next))
         {
            visited[next.second][next.first] = true;
            paths[next.second][next.first] = paths[curr.second][curr.first] + 1;
            parents[next.second][next.first] = curr;
            q.enqueue(next);
         }
      }
   }

   pair<int, int> end = curr;
   //backtrace through parents to get solution path (but in reverse order);
   while (end != start)
   {
      revPath.push(end);
      end = parents[end.second][end.first];
   }
   revPath.push(start);
   //populate pathPts in the correct order
   while (!revPath.isEmpty())
      pathPts.push_back(revPath.pop());
}

PNG decoder::renderSolution()
{

   PNG res = mapImg;
   for (pair<int, int> &loc : pathPts)
   {
      RGBAPixel *pixel = res.getPixel(loc.first, loc.second);
      pixel->r = 255;
      pixel->g = 0;
      pixel->b = 0;
      pixel->a = 1;
   }
   return res;
}

PNG decoder::renderMaze()
{
   PNG res = mapImg;
   vector<vector<bool>> visited(res.height(), vector<bool>(res.width(), bool()));
   vector<vector<int>> paths(res.height(), vector<int>(res.width(), int()));
   Queue<pair<int, int>> q;
   visited[start.second][start.first] = true;
   paths[start.second][start.first] = 0;
   q.enqueue(start);

   while (!q.isEmpty())
   {

      pair<int, int> curr = q.dequeue();
      int curr_dist = paths[curr.second][curr.first];
      vector<pair<int, int>> neighbors_vec = neighbors(curr);
      for (pair<int, int> &next : neighbors_vec)
      {
         if (good(visited, paths, curr, next))
         {
            visited[next.second][next.first] = true;
            paths[next.second][next.first] = curr_dist + 1;
            setGrey(res, next);
            q.enqueue(next);
         }
      }
   }
   for (int i = start.second - 3; i <= start.second + 3; i++)
   {
      for (int j = start.first - 3; j <= start.first + 3; j++)
      {
         if (0 <= j && j < (signed)mapImg.width() && 0 <= i && i < (signed)mapImg.height())
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

void decoder::setGrey(PNG &im, pair<int, int> loc)
{

   RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
   pixel->r = 2 * ((pixel->r) / 4);
   pixel->g = 2 * ((pixel->g) / 4);
   pixel->b = 2 * ((pixel->b) / 4);
}

pair<int, int> decoder::findSpot()
{
   return pathPts[pathLength() - 1];
}

int decoder::pathLength()
{
   return pathPts.size();
}

bool decoder::good(vector<vector<bool>> &v, vector<vector<int>> &d, pair<int, int> curr, pair<int, int> next)
{
   //test if pixel is in image
   bool inImage = (0 <= next.first && next.first < (signed)mapImg.width() && 0 <= next.second && next.second < (signed)mapImg.height());
   if (inImage)
   {//tests if pixel is unvisited (by using v)
      bool visited = v[next.second][next.first];
      if (!visited)
      {
         RGBAPixel *pixel = mapImg.getPixel(next.first, next.second);
         return compare(*pixel, d[curr.second][curr.first]);
      }
      return false;
   }
   return false;
}

vector<pair<int, int>> decoder::neighbors(pair<int, int> curr)
{

   vector<pair<int, int>> result;
   result.push_back(make_pair(curr.first - 1, curr.second)); //left
   result.push_back(make_pair(curr.first, curr.second + 1)); //below
   result.push_back(make_pair(curr.first + 1, curr.second)); //right
   result.push_back(make_pair(curr.first, curr.second - 1)); //above
   return result;
}

bool decoder::compare(RGBAPixel p, int d)
{
   int MSB = (p.r % 4) * 16;
   int MIDSB = (p.g % 4) * 4;
   int LSB = p.b % 4;

   int decoded = MSB + MIDSB + LSB;
   return (decoded == ((d + 1) % 64));
}