#include <bits/stdc++.h>
using namespace std;
map<int, string> colorCode = {{0, "\033[01;37m"}, {1, "\033[38;5;39m"}, {2, "\x1b[31m"}, {3, "\x1b[32m"}, {4, "\033[38;5;202m"}, {5, "\x1b[33m"}};
#define ANSI_COLOR_RESET   "\x1b[0m"
#define cubeOfVector vector<vector<vector<int> > >
enum directions
{
  right = 1, bottom = 2, left = 3, top = 4
};
int moveMap[6][4] = {{1, 2, 3, 4}, {0, 4, 5, 2}, {0, 1, 5, 3}, {0, 2, 5, 4}, {1, 0, 3, 5}, {1, 4, 3, 2}};
int faceMap[6][4] = {{directions::bottom, directions::left, directions::top, directions::right}, {directions::top, directions::top, directions::bottom, directions::top}, {directions::right, directions::right, directions::right, directions::right}, {directions::bottom, directions::bottom, directions::top, directions::bottom}, {directions::left, directions::left, directions::left, directions::left}, {directions::top, directions::left, directions::bottom, directions::right}};
map<char, int> moveToFace = {{'R', 2}, {'U', 1}, {'F', 0}, {'L', 4}, {'B', 5}, {'D', 3}};

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

int solved(/*cubeOfVector& cube*/)
{
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 3; j ++)
      for (int k = 0; k < 3; k ++)
        if (cube[i][0][0] != cube[i][j][k])
          return(0);
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

void bfs()
{
  visited.clear();
  deque<State> queue; queue.push_back(State(cube, 0));
  while (!queue.empty())
  {
    cube = queue.front().cube, steps = queue.front().steps; queue.pop_front();
    if (solved()) break;

    for (int i = 0; i < 6; i ++)
    {
      turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        queue.push_back(State(cube, steps + 1));
      }
      for (int j = 0; j < 2; j ++) turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        queue.push_back(State(cube, steps + 1));
      }
      turn(i);
    }
  }
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

void aStar(double (*heuristic)())
{
  visited.clear();
  priority_queue<State> pq; pq.push(State(cube, 0, heuristic()));
  while (!pq.empty())
  {
    cube = pq.top().cube, steps = pq.top().steps; pq.pop();
    if (solved()) break;

    for (int i = 0; i < 6; i ++)
    {
      turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        pq.push(State(cube, steps + 1, heuristic()));
      }
      for (int j = 0; j < 2; j ++) turn(i);
      if (!visited.count(cube))
      {
        visited.insert(cube);
        pq.push(State(cube, steps + 1, heuristic()));
      }
      turn(i);
    }
  }
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

  printCube();

  char movement[100]; scanf("%s", movement);
  for (int i = 0; movement[i]; i ++)
    for (int j = 0; j < 1 + (2*(movement[i]>'a')); j ++)
      turn(moveToFace[toupper(movement[i])]);

  cubeOfVector aux = cube;
  printf("BFS:\n");
  bfs();
  printResult();

  cube = aux;
  printf("A*:\n");
  aStar(euclideanDistance);
  printResult();

  printCube();

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