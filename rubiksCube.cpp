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
};

class Edge
{
  public:
  Color color = Color({});

  Edge(Color c)
  {
    color = c;
  }

  void swapEdge(Edge &a, Edge &b)
  {
    Edge *aux = *a;
    *a = *b;
    *b = aux;
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
};

class Face
{
public:
  vector<Corner> corners;
  vector<Edge> edges;
  Center center = Center(Color({}));
  vector<int> connectedFaces;

  Face(int size, int f, int t, int r, int b, int l)
  {
    size -= 2;
    connectedFaces = {t, r, b, l};

    corners.push_back(Corner(Color({f, t, r})));
    corners.push_back(Corner(Color({f, b, r})));
    corners.push_back(Corner(Color({f, b, l})));
    corners.push_back(Corner(Color({f, t, l})));

    for (int j = 0; j < connectedFaces.size(); j ++)
      for (int i = 0; i < size; i ++)
      {
        edges.push_back(Edge(Color({f, connectedFaces[j]})));
      }

    Center center = Center(Color({f}));
  }
};

class Cube
{
  public:
  vector<Face> faces;

  Cube(int size)
  {
    faces.push_back(Face(size, white, blue, red, green, orange));
    faces.push_back(Face(size, blue, yellow, red, white, orange));
    faces.push_back(Face(size, red, blue, yellow, green, white));
    faces.push_back(Face(size, green, white, red, yellow, orange));
    faces.push_back(Face(size, orange, blue, white, green, yellow));
    faces.push_back(Face(size, yellow, blue, orange, green, red));
  }

  void right()
  {
    for (int i = 0; i < 4; i ++)
  }
};

int main()
{
  int size = 3;


  return(0);
}
