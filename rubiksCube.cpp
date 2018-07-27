#include <bits/stdc++.h>
using namespace std;
enum colors
{
  white = 0,
  blue = 1,
  red = 2,
  green = 3,
  orange = 4,
  yellow = 5
};

enum directions
{
  top = 0,
  right = 1,
  bottom = 2,
  left = 3,
  topright = 0,
  botright = 1,
  botleft = 2,
  topleft = 3
};

class Color
{
  public:
  vector<int> color;

  Color(vector<int> c)
  {
    for (int i = 0; i < c.size(); i ++)
      color.push_back(c[i]);
  }
};

class Corner
{
  public:
  Color color = Color({});

  Corner(Color c)
  {
    color = c;
  }

  int getColor(int pos)
  {
    return(color.color[pos]);
  }

  Corner()
  {

  }

  void rotate(Corner *&a, Corner *&b, Corner *&c, Corner *&d)
  {
    Corner *aux = b;
    b = a;
    a = d;
    d = c;
    c = aux;
  }
};

class Edge
{
  public:
  Color color = Color({});

  Edge(Color c)
  {
    color = c;
  }

  Edge()
  {

  }

  int getColor(int pos)
  {
    return(color.color[pos]);
  }

  void rotate(Edge *&a, Edge *&b, Edge *&c, Edge *&d)
  {
    Edge *aux = b;
    b = a;
    a = d;
    d = c;
    c = aux;
  }
};

class Center
{
  public:
  Color color = Color({});

  Center(Color c)
  {
    color = c;
  }

  int getColor(int pos)
  {
    return(color.color[pos]);
  }
};

class Face
{
  public:
  vector<Corner*> corners;
  vector<Edge*> edges[4];
  Center center = Center(Color({}));
  vector<int> connectedFaces;
  int flipped;

  Face(int size, int f, int t, int r, int b, int l)
  {
    connectedFaces = {t, r, b, l};

    corners.push_back(new Corner(Color({f, t, r})));
    corners.push_back(new Corner(Color({f, b, r})));
    corners.push_back(new Corner(Color({f, b, l})));
    corners.push_back(new Corner(Color({f, t, l})));

    for (int j = 0; j < connectedFaces.size(); j ++)
      for (int i = 0; i < size - 2; i ++)
        edges[j].push_back(new Edge(Color({f, connectedFaces[j]})));

    center = Center(Color({f}));
  }

  void setEdges(vector<vector<Edge*> > e)
  {
    for (int i = 0; i < 4; i ++)
      edges[i] = e[i];
  }

  void setCorners(vector<Corner*> c)
  {
    corners = c;
  }
};

class Cube
{
  public:
  vector<Face> faces;
  int size;
  Edge e; Corner c;

  Cube(int sz)
  {
    size = sz;
    faces.push_back(Face(sz, white, blue, red, green, orange));
    faces.push_back(Face(sz, blue, yellow, red, white, orange));
    faces.push_back(Face(sz, red, blue, yellow, green, white));
    faces.push_back(Face(sz, green, white, red, yellow, orange));
    faces.push_back(Face(sz, orange, blue, white, green, yellow));
    faces.push_back(Face(sz, yellow, blue, orange, green, red));

    faces[blue].setEdges({faces[white].edges[directions::top], faces[red].edges[directions::top], faces[yellow].edges[directions::top], faces[orange].edges[directions::top]});
    faces[blue].setCorners({faces[yellow].corners[directions::botleft], faces[yellow].corners[directions::botright], faces[white].corners[directions::topright], faces[white].corners[directions::topleft]});
  }

  void right()
  {
    for (int i = 0; i < size - 2; i ++)
    {
      e.rotate(faces[0].edges[directions::right][i], faces[1].edges[directions::right][i], faces[5].edges[directions::right][i], faces[3].edges[directions::right][i]);
    }
    c.rotate(faces[0].corners[directions::topright], faces[1].corners[directions::topright], faces[5].corners[directions::topright], faces[3].corners[directions::topright]);
    c.rotate(faces[0].corners[directions::botright], faces[1].corners[directions::botright], faces[5].corners[directions::botright], faces[3].corners[directions::botright]);
  }

  void left()
  {
    for (int i = 0; i < size - 2; i ++)
    {
      e.rotate(faces[3].edges[directions::left][i], faces[5].edges[directions::left][i], faces[1].edges[directions::left][i], faces[0].edges[directions::left][i]);
    }
    c.rotate(faces[3].corners[directions::topleft], faces[5].corners[directions::topleft], faces[1].corners[directions::topleft], faces[0].corners[directions::topleft]);
    c.rotate(faces[3].corners[directions::botleft], faces[5].corners[directions::botleft], faces[1].corners[directions::botleft], faces[0].corners[directions::botleft]);
  }

  void top()
  {
    for (int i = 0; i < size - 2; i ++)
    {
      e.rotate(faces[0].edges[directions::top][i], faces[4].edges[directions::top][i], faces[5].edges[directions::top][i], faces[2].edges[directions::top][i]);
    }
    c.rotate(faces[0].corners[directions::topleft], faces[4].corners[directions::topleft], faces[5].corners[directions::topleft], faces[2].corners[directions::topleft]);
    c.rotate(faces[0].corners[directions::topright], faces[4].corners[directions::topright], faces[5].corners[directions::topright], faces[2].corners[directions::topright]);
  }

  void printCube()
  {
    for (int i = 0; i < faces.size(); i ++)
    {
      printf("%d", faces[i].corners[directions::topleft]->getColor(0));
      for (int j = 0; j < size - 2; j ++)
        printf(" %d", faces[i].edges[directions::top][j]->getColor(0));
      printf(" %d | ", faces[i].corners[directions::topright]->getColor(0));
    }
    printf("\n");

    for (int j = 0; j < size - 2; j ++)
    {
      for (int i = 0; i < faces.size(); i ++)
      {
        printf("%d", faces[i].edges[directions::left][0]->getColor(0)); // Change this
        printf(" %d", faces[i].center.getColor(0));
        printf(" %d | ", faces[i].edges[directions::right][0]->getColor(0)); // Change this
      }
      printf("\n");
    }

    for (int i = 0; i < faces.size(); i ++)
    {
      printf("%d", faces[i].corners[directions::botleft]->getColor(0));
      for (int j = 0; j < size - 2; j ++)
        printf(" %d", faces[i].edges[directions::top][j]->getColor(0));
      printf(" %d | ", faces[i].corners[directions::botright]->getColor(0));
    }
    printf("\n\n");
  }
};

int main()
{
  int size = 3;

  Cube cube = Cube(size);
  cube.printCube();

  char movement;
  while (scanf("") != EOF)
  {
    while (scanf("%c", &movement) && movement != '\n')
    {
      if (movement == 'R') cube.right();
      if (movement == 'L') cube.left();
      if (movement == 'U') cube.top();
    }
    cube.printCube();
  }

  return(0);
}
