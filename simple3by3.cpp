#include <bits/stdc++.h>
using namespace std;
map<int, string> colorCode = {{0, "\033[01;37m"}, {1, "\033[38;5;39m"}, {2, "\x1b[31m"}, {3, "\x1b[32m"}, {4, "\033[38;5;202m"}, {5, "\x1b[33m"}};
#define ANSI_COLOR_RESET   "\x1b[0m"
#define cubeOfVector vector<vector<vector<int> > >
enum directions
{
  right = 0, bottom = 1, left = 2, top = 3, upRight = 4, downRight = 5, downLeft = 6, upLeft = 7
};
int dy[8] = {1, 2, 1, 0, 0, 2, 2, 0}, dx[8] = {2, 1, 0, 1, 2, 2, 0, 0};
int white = 0, blue = 1, red = 2, green = 3, orange = 4, yellow = 5;
int moveMap[6][4] = {{1, 2, 3, 4}, {0, 4, 5, 2}, {0, 1, 5, 3}, {0, 2, 5, 4}, {1, 0, 3, 5}, {1, 4, 3, 2}};
int faceMap[6][4] = {{directions::bottom, directions::left, directions::top, directions::right}, {directions::top, directions::top, directions::bottom, directions::top}, {directions::right, directions::right, directions::right, directions::right}, {directions::bottom, directions::bottom, directions::top, directions::bottom}, {directions::left, directions::left, directions::left, directions::left}, {directions::top, directions::left, directions::bottom, directions::right}};
map<char, int> moveToFace = {{'R', 2}, {'U', 1}, {'F', 0}, {'L', 4}, {'B', 5}, {'D', 3}};
int edges[12][2][2] = {
  {{white, directions::top}, {blue, directions::bottom}},
  {{white, directions::right}, {red, directions::left}},
  {{white, directions::bottom}, {green, directions::top /*dubious*/}},
  {{white, directions::left}, {orange, directions::right}},
  {{blue, directions::right}, {red, directions::top}},
  {{blue, directions::top}, {yellow, directions::bottom}},
  {{blue, directions::left}, {orange, directions::top}},
  {{red, directions::right}, {yellow, directions::right}},
  {{red, directions::bottom}, {green, directions::right}},
  {{green, directions::left}, {orange, directions::bottom}},
  {{green, directions::bottom}, {yellow, directions::top}},
  {{orange, directions::left}, {yellow, directions::left}}
};
int corners[8][3][2] = {
  {{white, directions::upRight}, {blue, directions::downRight}, {red, directions::upLeft}},
  {{white, directions::downRight}, {red, directions::downLeft}, {green, directions::upRight}},
  {{white, directions::downLeft}, {green, directions::upLeft}, {orange, directions::downRight}},
  {{white, directions::upLeft}, {orange, directions::upRight}, {blue, directions::downLeft}},
  {{yellow, directions::downLeft}, {blue, directions::upLeft}, {orange, directions::upLeft}},
  {{yellow, directions::downRight}, {red, directions::upRight}, {blue, directions::upRight}},
  {{yellow, directions::upRight}, {green, directions::downRight}, {red, directions::downRight}},
  {{yellow, directions::upLeft}, {orange, directions::downLeft}, {green, directions::downLeft}}
};
int edgesDP[12][12][2]; // [edge_i][edge_j where edje_i is][orientation]
int cornersDP[8][8][3]; // [corner_i][corner_j where corner_j is][orientation]
int inf = 1<<20;

cubeOfVector cube; int steps;
set<cubeOfVector> visited;
class State
{
public:
  cubeOfVector cube;
  int steps;
  double distance;
  State(cubeOfVector c, int s, double d)
  {
    cube = c, steps = s, distance = d;
  }
  State(cubeOfVector c, int s)
  {
    cube = c, steps = s;
  }

  bool operator()(const State& a, const State& b)
  {
    return(a.distance < b.distance);
  }

  bool operator<(const State& a) const
  {
    return(distance > a.distance);
  }
};

int solvedCube(/*cubeOfVector& cube*/)
{
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 3; j ++)
      for (int k = 0; k < 3; k ++)
        if (cube[i][0][0] != cube[i][j][k])
          return(0);
  return(1);
}

int e;
int solvedEdge()
{
  if (cube[edges[e][0][0]][dy[edges[e][0][1]]][dx[edges[e][0][1]]] == edges[e][0][0] && cube[edges[e][1][0]][dy[edges[e][1][1]]][dx[edges[e][1][1]]] == edges[e][1][0]) return(1);
  return(0);
}

int c;
int solvedCorner()
{
  for (int i = 0; i < 3; i ++)
    if (cube[corners[c][i][0]][dy[corners[c][i][1]]][dx[corners[c][i][1]]] != corners[c][i][0]) return(0);
  return(1);
}

void printCube(/*cubeOfVector& cube*/)
{
  for (int j = 0; j < 3; j ++)
  {
    for (int i = 0; i < 6; i ++)
    {
      for (int k = 0; k < 3; k ++)
        printf("%s@", colorCode[cube[i][j][k]].c_str()/*, 1+cube[i][j][k]*/);
      printf(" ");
    }
    printf("\n");
  }
  for (int i = 0; i < 12; i ++)
    for (int j = 0; j < 2; j ++)
    {
      printf("%s@%s", colorCode[cube[edges[i][j][0]][dy[edges[i][j][1]]][dx[edges[i][j][1]]]].c_str(), j ? (i == 11 ? "\n" : " ") : "\0");
    }
  printf("%s", ANSI_COLOR_RESET);
}

void swap(int *a, int *b, int *c, int *d)
{
  int aux = *b;
  *b = *a;
  *a = *d;
  *d = *c;
  *c = aux;
}

void ordinarySwap(int *a, int *b)
{
  int aux = *a;
  *a = *b;
  *b = aux;
}

void turn(/*cubeOfVector& cube, */int face)
{
  swap(&cube[face][0][0], &cube[face][0][2], &cube[face][2][2], &cube[face][2][0]); // Corner

  for (int k = 0; k < 2; k ++)
  {
    int *a[4];
    for (int i = 0; i < 4; i ++)
    {
      if (faceMap[face][i] == directions::right) a[i] = &cube[moveMap[face][i]][face != 0 ? 2*k : 2*(1-k)][2];
      if (faceMap[face][i] == directions::bottom) a[i] = &cube[moveMap[face][i]][2][2*k];
      if (faceMap[face][i] == directions::left) a[i] = &cube[moveMap[face][i]][face != 0 ? 2*k : 2*(1-k)][0];
      if (faceMap[face][i] == directions::top) a[i] = &cube[moveMap[face][i]][0][2*k];
    }
    swap(a[0], a[1], a[2], a[3]);
  } // Aux corner
  if (face == 0) ordinarySwap(&cube[4][0][2], &cube[4][2][2]);
  if (face == 0) ordinarySwap(&cube[2][0][0], &cube[2][2][0]);
  if (face == 1) ordinarySwap(&cube[2][0][2], &cube[2][0][0]);
  if (face == 1) ordinarySwap(&cube[5][2][2], &cube[5][2][0]);
  if (face == 5) ordinarySwap(&cube[2][0][2], &cube[2][2][2]);
  if (face == 5) ordinarySwap(&cube[4][0][0], &cube[4][2][0]);
  if (face == 3) ordinarySwap(&cube[4][2][0], &cube[4][2][2]);
  if (face == 3) ordinarySwap(&cube[5][0][0], &cube[5][0][2]);

  swap(&cube[face][0][1], &cube[face][1][2], &cube[face][2][1], &cube[face][1][0]);
  int *a[4];
  for (int i = 0; i < 4; i ++)
  {
    if (faceMap[face][i] == directions::right) a[i] = &cube[moveMap[face][i]][1][2];
    if (faceMap[face][i] == directions::bottom) a[i] = &cube[moveMap[face][i]][2][1];
    if (faceMap[face][i] == directions::left) a[i] = &cube[moveMap[face][i]][1][0];
    if (faceMap[face][i] == directions::top) a[i] = &cube[moveMap[face][i]][0][1];
  }
  swap(a[0], a[1], a[2], a[3]);
}

void fillEdgesDP()
{
  visited.clear();
  for (int i = 0; i < 12; i ++) for (int j = 0; j < 12; j ++) for (int k = 0; k < 2; k ++) edgesDP[i][j][k] = inf;
  int nowSteps = 0, toDo = 0;
  deque<State> queue; queue.push_back(State(cube, 0));
  while (!queue.empty())
  {
    cube = queue.front().cube, nowSteps = queue.front().steps;
    queue.pop_front();
    toDo = 0;
    for (int i = 0; i < 12; i ++)
      for (int j = 0; j < 12; j ++)
        for (int k = 0; k < 2; k ++)
        {
          if (edges[i][0][0] == cube[edges[j][k][0]][dy[edges[j][k][1]]][dx[edges[j][k][1]]] && edges[i][1][0] == cube[edges[j][1-k][0]][dy[edges[j][1-k][1]]][dx[edges[j][1-k][1]]])
            edgesDP[i][j][k] = min(edgesDP[i][j][k], nowSteps);
          if (edgesDP[i][j][k] == inf) toDo = 1;
        }
    if (!toDo) break;

    for (int i = 0; i < 6; i ++)
    {
      turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        queue.push_back(State(cube, nowSteps + 1));
      }
      for (int j = 0; j < 2; j ++) turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        queue.push_back(State(cube, nowSteps + 1));
      }
      turn(i);
    }
  }
}

void fillCornersDP()
{
  visited.clear();
  for (int i = 0; i < 8; i ++) for (int j = 0; j < 8; j ++) for (int k = 0; k < 3; k ++) cornersDP[i][j][k] = inf;
  int nowSteps = 0, toDo = 0;
  deque<State> queue; queue.push_back(State(cube, 0));
  while (!queue.empty())
  {
    cube = queue.front().cube, nowSteps = queue.front().steps;
    queue.pop_front();
    toDo = 0;
    for (int i = 0; i < 8; i ++)
      for (int j = 0; j < 8; j ++)
        for (int k = 0; k < 3; k ++)
        {
          int allEqual = 1;
          for (int m = 0; m < 3; m ++)
            if (corners[i][m][0] != cube[corners[j][(k+m)%3][0]][dy[corners[j][(k+m)%3][1]]][dx[corners[j][(k+m)%3][1]]])
              allEqual = 0;
          if (allEqual) cornersDP[i][j][k] = min(cornersDP[i][j][k], nowSteps);
          if (cornersDP[i][j][k] == inf) toDo ++;
        }
    // printf("steps: %d, toDo: %d\n", nowSteps, toDo);
    if (!toDo) break;

    for (int i = 0; i < 6; i ++)
    {
      turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        queue.push_back(State(cube, nowSteps + 1));
      }
      for (int j = 0; j < 2; j ++) turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        queue.push_back(State(cube, nowSteps + 1));
      }
      turn(i);
    }
  }
}

int bfs(int (*solved)())
{
  //visited.clear();
  int nowSteps = 0;
  deque<State> queue; queue.push_back(State(cube, 0));
  while (!queue.empty())
  {
    cube = queue.front().cube, nowSteps = queue.front().steps; queue.pop_front();
    if (solved()) break;

    // if (nowSteps < 6)
    for (int i = 0; i < 6; i ++)
    {
      turn(i);
      if (1/*!visited.count(cube)*/)
      {
        // visited.insert(cube);
        queue.push_back(State(cube, nowSteps + 1));
      }
      for (int j = 0; j < 2; j ++) turn(i);
      if (1/*!visited.count(cube)*/)
      {
        // visited.insert(cube);
        queue.push_back(State(cube, nowSteps + 1));
      }
      turn(i);
    }
  }
  steps = nowSteps;
  return(steps);
}

double euclideanDistance()
{
  double dist = 0;
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 3; j ++)
      for (int k = 0; k < 3; k ++)
        dist += abs(cube[i][0][0] != cube[i][j][k]);
  return(dist);
}

double edgeDistance()
{
  double sum = 0;
  for (int i = 0; i < 12; i ++)
    for (int j = 0; j < 12; j ++)
      for (int k = 0; k < 2; k ++)
        if (edges[i][0][0] == cube[edges[j][k][0]][dy[edges[j][k][1]]][dx[edges[j][k][1]]] && edges[i][1][0] == cube[edges[j][1-k][0]][dy[edges[j][1-k][1]]][dx[edges[j][1-k][1]]])
          sum += edgesDP[i][j][k];
  // printf("%lf\n", sum);
  return(sum);
}

double cornerDistance()
{
  double sum = 0;
  for (int i = 0; i < 8; i ++)
    for (int j = 0; j < 8; j ++)
      for (int k = 0; k < 3; k ++)
      {
        int allEqual = 1;
        for (int m = 0; m < 3; m ++)
          if (corners[i][m][0] != cube[corners[j][(k+m)%3][0]][dy[corners[j][(k+m)%3][1]]][dx[corners[j][(k+m)%3][1]]])
            allEqual = 0;
        sum += cornersDP[i][j][k] * allEqual;
      }
  return(sum);
}

double bothDistance()
{
  double sum = max(edgeDistance() / 4.0, cornerDistance() / 4.0);
  // double sum = edgeDistance() / 4.0 + cornerDistance() / 4.0;
  return(sum);
}

double minimize()
{
  return(euclideanDistance() / 10);
}

void aStar(double (*heuristic)())
{
  int nowSteps; visited.clear();
  priority_queue<State> pq; pq.push(State(cube, 0, heuristic()));
  double prevDist = 0;
  while (!pq.empty())
  {
    cube = pq.top().cube, nowSteps = pq.top().steps; double dist = pq.top().distance;
    if (prevDist < dist)
    {
      printf("%d %lf\n", nowSteps, dist);
      prevDist = dist;
    }
    pq.pop();
    if (solvedCube()) break;

    for (int i = 0; i < 6; i ++)
    {
      turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        pq.push(State(cube, nowSteps + 1, dist/1.15 + heuristic()));
      }
      for (int j = 0; j < 2; j ++) turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        pq.push(State(cube, nowSteps + 1, dist/1.15 + heuristic()));
      }
      turn(i);
    }
  }
  steps = nowSteps;
}

void printResult()
{
  printf("\tReached  : %d states\n", (int) visited.size());
  printf("\tSolved in: %d steps\n", steps);
}

int main()
{
  for (int i = 0; i < 6; i ++)
  {
    cube.push_back(vector<vector<int> >(3));
    for (int j = 0; j < 3; j ++)
      for (int k = 0; k < 3; k ++)
        cube[i][j].push_back(i);
  }
  cubeOfVector aux = cube;
  fillEdgesDP(); printf("Filled EdgesDP\n");
  cube = aux;
  fillCornersDP(); printf("Filled CornersDP\n");
  cube = aux;

  printCube();

  char movement[100];
  while (scanf("\n%s", movement) != EOF)
  {
    for (int i = 0; movement[i]; i ++)
    for (int j = 0; j < 1 + (2*(movement[i]>'a')); j ++)
    turn(moveToFace[toupper(movement[i])]);

    // printf("BFS:\n");
    // bfs();
    // printResult();

    // cube = aux;
    printf("A*:\n");
    aStar(bothDistance);
    printResult();

    printCube();

    // printf("edges: %lf, corners: %lf\n", edgeDistance(), cornerDistance());
  }

  return(0);
}
/*
char movement[100];
while (scanf("%s", movement) != EOF)
{
  getchar();

  printf("\n");
  printCube(cube);
}
*/
