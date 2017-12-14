#include <iostream>
#include <cmath>
#include <math.h>

using namespace std;

#define INF 2000000000.0

struct point{
    int x, y;
};

inline void swap(point pts[], int x, int y){
    int t = pts[x].x;
    pts[x].x = pts[y].x;
    pts[y].x = t;
    t = pts[x].y;
    pts[x].y = pts[y].y;
    pts[y].y = t;
}

inline bool comp(point pts[], int ax, int x, int y){
    if (ax == 0)
        return pts[x].x <= pts[y].x;
    return pts[x].y <= pts[y].y;
}


void sort_pts(point pts[], int ax, int i, int j){
    // quick sort
    if (i>=j){
        return;
    }
    if (i+1==j){
        if (!comp(pts, ax, i, j))
            swap(pts, i, j);
        return;
    }


    int pvt = i;
    int r = j, ii = pvt+1;
    while (ii<r){
        if (comp(pts, ax, pvt, ii)){
            swap(pts, ii, r--);
        }else{
            ii++;
        }
    }

    swap(pts, pvt, comp(pts, ax, pvt, ii)?(--ii):ii);

    sort_pts(pts, ax, i, ii);
    sort_pts(pts, ax, ii+1, j);

}

inline void copy_p(point pts1[], point pts2[], int i, int j){
    pts1[i].x = pts2[j].x;
    pts1[i].y = pts2[j].y;
}

inline float dist(int x, int y, int i, int j){
    return pow((pow((x-i), 2) + pow((y-j), 2)), 0.5);
}

float closest_dist(point ptsx[], point ptsy[],  int i, int j){
    if(i+1>=j){
        if(i<j){
            return dist(ptsx[i].x, ptsx[i].y, ptsx[j].x, ptsx[j].y);
        }
        return INF;
    }
    int mid = (j+i)/2;
    point ptsyl[mid-i+1], ptsyr[j-mid];
    for(int ii=i, li=0, ri=0; ii<=j; ++ii){
        if (ptsy[ii].x<=ptsx[mid].x)
            copy_p(ptsyl, ptsy, li++, ii);
        else
            copy_p(ptsyr, ptsy, ri++, ii);
    }

    float dl = closest_dist(ptsx, ptsyl, i, mid);
    float dr = closest_dist(ptsx, ptsyr, mid+1, j);

    float d = min(dl, dr);

    point strip[j-i+1];
    int jj = 0;
    for(int ii=i; ii<=j; ++ii){
        if (abs(ptsy[ii].x - ptsx[mid].x) < d)
            copy_p(strip, ptsy, jj++, ii);
    }

    float cur = d;
    float res = d;
    // The inner loop is guaranteed to end in not more than 6 iterations which can be proved using geometry
    for (int ii=0, lim=0; ii<jj-1; ++ii){
        lim = min(jj, ii+8);
        for (int in=ii+1; in<lim; ++in){
            res = dist(strip[ii].x, strip[ii].y, strip[in].x, strip[in].y);
            if (res<cur)
                cur = res;
        }
    }

    return res;
}

int main(){
    int n = 6;
    point pts[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};

    sort_pts(pts, 0, 0, n-1);

    point* ptsy = new point[n];

    for(int i=0; i<n; i++){
        ptsy[i] = pts[i];
    }

    sort_pts(ptsy, 1, 0, n-1);

    cout << closest_dist(pts, ptsy, 0, n-1) << endl;

    return 0;
}


