#include "stats.h"
//constructor for the stats object - populates sum and sumsq vectors for each colour channel 
stats::stats(PNG &im)
{
    RGBAPixel *topL = im.getPixel(0, 0);
    sumRed.resize(im.height(), vector<long>(im.width(), topL->r));
    sumGreen.resize(im.height(), vector<long>(im.width(), topL->g));
    sumBlue.resize(im.height(), vector<long>(im.width(), topL->b));
    sumsqRed.resize(im.height(), vector<long>(im.width(), (topL->r) * (topL->r)));
    sumsqGreen.resize(im.height(), vector<long>(im.width(), (topL->g) * (topL->g)));
    sumsqBlue.resize(im.height(), vector<long>(im.width(), (topL->b) * (topL->b)));
    for (unsigned y = 1; y < im.height(); y++)
    {
        RGBAPixel *p = im.getPixel(0, y);
        sumRed[y][0] = sumRed[y - 1][0] + p->r, sumGreen[y][0] = sumGreen[y - 1][0] + p->g;
        sumBlue[y][0] = sumBlue[y - 1][0] + p->b, sumsqRed[y][0] = sumsqRed[y - 1][0] + (p->r) * (p->r);
        sumsqGreen[y][0] = sumsqGreen[y - 1][0] + (p->g) * (p->g),sumsqBlue[y][0] = sumsqBlue[y - 1][0] + (p->b) * (p->b);
    }
    for (unsigned x = 1; x < im.width(); x++)
    {
        RGBAPixel *p = im.getPixel(x, 0);
        sumRed[0][x] = sumRed[0][x - 1] + p->r, sumGreen[0][x] = sumGreen[0][x - 1] + p->g;
        sumBlue[0][x] = sumBlue[0][x - 1] + p->b, sumsqRed[0][x] = sumsqRed[0][x - 1] + (p->r) * (p->r);
        sumsqGreen[0][x] = sumsqGreen[0][x - 1] + (p->g) * (p->g), sumsqBlue[0][x] = sumsqBlue[0][x - 1] + (p->b) * (p->b);
    }

    for (unsigned y = 1; y < im.height(); y++)
    {
        for (unsigned x = 1; x < im.width(); x++)
        {
            RGBAPixel *p = im.getPixel(x, y);
            sumRed[y][x] = sumRed[y][x - 1] + sumRed[y - 1][x] - sumRed[y - 1][x - 1] + p->r;
            sumGreen[y][x] = sumGreen[y][x - 1] + sumGreen[y - 1][x] - sumGreen[y - 1][x - 1] + p->g;
            sumBlue[y][x] = sumBlue[y][x - 1] + sumBlue[y - 1][x] - sumBlue[y - 1][x - 1] + p->b;
            sumsqRed[y][x] = sumsqRed[y][x - 1] + sumsqRed[y - 1][x] - sumsqRed[y - 1][x - 1] + (p->r) * (p->r);
            sumsqGreen[y][x] = sumsqGreen[y][x - 1] + sumsqGreen[y - 1][x] - sumsqGreen[y - 1][x - 1] + (p->g) * (p->g);
            sumsqBlue[y][x] = sumsqBlue[y][x - 1] + sumsqBlue[y - 1][x] - sumsqBlue[y - 1][x - 1] + (p->b) * (p->b);
        }
    }
}
long stats::getSumHelper(vector<vector<long>> &arr, pair<int,int> ul, int w, int h){
    long total = arr[h + ul.second - 1][w + ul.first - 1];
    if (ul.second > 0)
        total -= arr[ul.second - 1][w + ul.first - 1];
    if (ul.first > 0)
        total -= arr[h + ul.second - 1][ul.first - 1];
    if (ul.first > 0 && ul.second > 0)
        total += arr[ul.second - 1][ul.first - 1];
    return total;
}
long stats::getSum(char channel, pair<int, int> ul, int w, int h)
{
    if (channel == 'r') return getSumHelper(sumRed, ul, w, h);
    if (channel == 'g') return getSumHelper(sumGreen, ul, w, h);
    if (channel == 'b') return getSumHelper(sumBlue, ul, w, h);
}

long stats::getSumSq(char channel, pair<int, int> ul, int w, int h)
{
    if (channel == 'r') return getSumHelper(sumsqRed, ul, w, h);
    if (channel == 'g') return getSumHelper(sumsqGreen, ul, w, h);
    if (channel == 'b') return getSumHelper(sumsqBlue, ul, w, h);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int w, int h)
{
    if (w * h == 0)
        return 0;
    double varR = (double)getSumSq('r', ul, w, h) - (double)getSum('r', ul, w, h) * (double)getSum('r', ul, w, h) / (double)(w * h);
    double varG = (double)getSumSq('g', ul, w, h) - (double)getSum('g', ul, w, h) * (double)getSum('g', ul, w, h) / (double)(w * h);
    double varB = (double)getSumSq('b', ul, w, h) - (double)getSum('b', ul, w, h) * (double)getSum('b', ul, w, h) / (double)(w * h);
    return varR + varG + varB;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int w, int h)
{
    double avgR = getSum('r', ul, w, h) / (w * h);
    double avgG = getSum('g', ul, w, h) / (w * h);
    double avgB = getSum('b', ul, w, h) / (w * h);
    RGBAPixel avgPx((int)avgR, (int)avgG, (int)avgB);
    return avgPx;
}
