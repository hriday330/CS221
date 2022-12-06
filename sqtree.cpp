/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int, int> ul, int w, int h, RGBAPixel a, double v)
    : upLeft(ul), width(w), height(h), avg(a), var(v), NW(NULL), NE(NULL), SE(NULL), SW(NULL)
{
}

// Second Node constructor, given
SQtree::Node::Node(stats &s, pair<int, int> ul, int w, int h)
    : upLeft(ul), width(w), height(h), NW(NULL), NE(NULL), SE(NULL), SW(NULL)
{
  avg = s.getAvg(ul, w, h);
  var = s.getVar(ul, w, h);
}

// SQtree destructor, given.
SQtree::~SQtree()
{
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree &other)
{
  copy(other);
}

// SQtree assignment operator, given.
SQtree &SQtree::operator=(const SQtree &rhs)
{
  if (this != &rhs)
  {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG &imIn, double tol)
{
  im = imIn;
  treeSize = 0;
  stats s(imIn);
  pair<int, int> ulRoot;
  ulRoot.first = 0;
  ulRoot.second = 0;
  root = buildTree(s, ulRoot, (int)imIn.width(), (int)imIn.height(), tol);
}
/**
 * Helper for the SQtree constructor.
 */
SQtree::Node *SQtree::buildTree(stats &s, pair<int, int> &ul, int w, int h, double tol)
{
  //null node - added as a design choice to absorb the cases of two rectangles (up-down/ left-right) into one. 
  if (w == 0 || h == 0) return NULL;
  treeSize++;
  //rectangle within permitted variability - save node and initialise avg + var. This includes the single pixel case (var =0)
  if (s.getVar(ul, w, h) <= tol)
  {
    return new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));
  }
  //else find the optimal partition and recur on the (smaller) rectangles created
  pair<int, int> bestPart = findBestPartition(s, ul, w, h); //The partition is optimal and doesnt return the original rectangle
  int x = bestPart.first, y = bestPart.second;
  pair<int, int> ne(ul.first + x, ul.second), sw(ul.first, ul.second + y), se(ul.first + x, ul.second + y);
  Node *newNode = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));
  newNode->NW = buildTree(s, ul, x, y, tol), newNode->NE = buildTree(s, ne, w - x, y, tol);
  newNode->SW = buildTree(s, sw, x, h - y, tol), newNode->SE = buildTree(s, se, w - x, h - y, tol);
  return newNode;
}

/**
 * Find the best partition in rectangle given, splitting the rectangle in 2 or 4 
 * The partition is given as a as a location (x,y) between (0,0) and (w,h);
 */

pair<int, int> SQtree::findBestPartition(stats &s, pair<int, int> &ul, int w, int h)
{
  double minVar = -1;
  pair<int, int> res = make_pair(0, 0);
  for (int y = 0; y <= h; y++) //iterate through rectangle represented by node.
  {
    for (int x = 0; x <= w; x++)
    {
      //this avoids the partitioning into the original rectangle
      if ((x == 0 && y == 0) || (x == w && y == 0) || (x == 0 && y == h) || (x == w && y == h)) continue; 
      pair<int,int> p2 = {ul.first + x, ul.second}, p3 = {ul.first, ul.second + y}, p4 = {ul.first + x, ul.second + y};
      double NW = s.getVar(ul, x, y), NE = s.getVar(p2, w - x, y), SW = s.getVar(p3, x, h - y), SE = s.getVar(p4, w - x, h - y), var = max(NE, max(NW, max(SW, SE)));
      if (var <= minVar || minVar == -1)
        minVar = var, res.first = x, res.second = y;
    }
  }
  return res;
}
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render()
{
  renderHelper(root);
  return im;
}
void SQtree::renderHelper(Node *subtree)
{
  //null node case - also added to clean up cases of two-rectangle splits. 
  if (!subtree)
    return;
  //leaf node case
  if (!subtree->SW && !subtree->SE && !subtree->NW && !subtree->NE)
  {
    for (int y = 0; y < subtree->height; y++)
    {
      for (int x = 0; x < subtree->width; x++)
      {
        RGBAPixel *pix = im.getPixel(subtree->upLeft.first + x, subtree->upLeft.second + y);
        *pix = subtree->avg;
      }
    }
  }
  else
  {
    renderHelper(subtree->NE);
    renderHelper(subtree->NW);
    renderHelper(subtree->SW);
    renderHelper(subtree->SE);
  }
}

/**
 * Delete allocated memory.
 */
void SQtree::clear()
{
  clearHelper(root);
}

void SQtree::clearHelper(Node *&subRoot)
{
  if (!subRoot)
    return;
  clearHelper(subRoot->NW);
  clearHelper(subRoot->NE);
  clearHelper(subRoot->SW);
  clearHelper(subRoot->SE);
  delete subRoot;
  subRoot = NULL;
}

void SQtree::copy(const SQtree &other)
{
  treeSize = other.treeSize;
  im = other.im;
  root = copyHelper(other.root);
}

SQtree::Node *SQtree::copyHelper(Node *other)
{
  if (!other)
    return NULL;
  Node *copyNode = new Node(make_pair(other->upLeft.first, other->upLeft.second), other->width, other->height, other->avg, other->var);
  copyNode->NW = copyHelper(other->NW);
  copyNode->NE = copyHelper(other->NE);
  copyNode->SW = copyHelper(other->SW);
  copyNode->SE = copyHelper(other->SE);
  return copyNode;
}

int SQtree::size()
{
  return treeSize;
}
