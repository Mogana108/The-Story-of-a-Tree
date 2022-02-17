#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Edge {
    int x;
    int y;
};

static int n;
static int nedges;
static int g;
static int k;
static struct Edge edge[200000];
static int first[100000];
static int last[100000];
static int parent[100000];
static int nwrong[100000];
static int nright[100000];
static int ngoodroots;
static int count;

static int compareedge(const void *a, const void *b)
{
    const struct Edge *e1 = a;
    const struct Edge *e2 = b;
    if (e1->x != e2->x)
        return e1->x - e2->x;
    return e1->y - e2->y;
}

static void dfs(int node, int from)
{
    int i;
    parent[node] = from;
    for (i = first[node]; i < last[node]; i++) {
        assert(edge[i].x == node);
        if (edge[i].y != from) {
            dfs(edge[i].y, node);
        }
    }
}

static void dfs2(int node, int from)
{
    int i;
    assert(parent[node] == from);
    //printf("visit %d (parent %d), count=%d\n", node, from, count);
    count += nwrong[node] - nright[node];
    /*printf("add %d: nwrong[%d]=%d, nright[%d]=%d\n",
           nwrong[node]-nright[node],
           node,nwrong[node],
           node,nright[node]);
           */
    if (count >= k) {
        ngoodroots ++;
    }
    for (i = first[node]; i < last[node]; i++) {
        assert(edge[i].x == node);
        if (edge[i].y != from) {
            dfs2(edge[i].y, node);
        }
    }
    count -= nwrong[node] - nright[node];
}

static void one(void)
{
    int i;
    scanf("%d", &n);
    memset(first, 0, n * sizeof *first);
    memset(last, 0, n * sizeof *last);
    memset(parent, 0, n * sizeof *parent);
    memset(nwrong, 0, n * sizeof *nwrong);
    memset(nright, 0, n * sizeof *nright);
    ngoodroots = 0;
    count = 0;
    for (i = 0; i < n-1; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        edge[2*i].x = x-1;
        edge[2*i].y = y-1;
        edge[2*i+1].x = y-1;
        edge[2*i+1].y = x-1;
    }
    nedges = 2*(n-1);
    qsort(edge, nedges, sizeof *edge, compareedge);
    for (i = 0; i < nedges; i++) {
        if (i == 0 || edge[i].x != edge[i-1].x)
            first[edge[i].x] = i;
        if (i == nedges-1 || edge[i].x != edge[i+1].x)
            last[edge[i].x] = i+1;
    }
    dfs(0, 0);
    scanf("%d %d", &g, &k);
    for (i = 0; i < g; i++) {
        int x, y;
        scanf("%d %d", &x, &y);
        if (parent[y-1] != x-1) {
            nwrong[x-1] ++;
        }
        else {
            nright[y-1] = 1;
            count ++;
        }
    }
    //printf("%d right\n", count);
    dfs2(0, 0);
    
    int a = ngoodroots;
    int b = n;
    for (i = n; i > 0; i--) {
        if ((a % i)==0 && (b % i)==0) {
            a /= i;
            b /= i;
        }
    }
    printf("%d/%d\n", a, b);
}

int main(void)
{
    int q;
    scanf("%d", &q);
    while (q--)
        one();
    return 0;
}
