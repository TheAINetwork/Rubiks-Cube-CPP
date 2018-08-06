import java.util.HashMap;
import java.util.HashSet;
import java.util.Queue;
import java.util.ArrayDeque;
import java.util.PriorityQueue;
import java.util.ArrayList;
import java.util.Comparator;

float tileSize = 100, xAngle = 0, yAngle = 0, zAngle = 0;
boolean frameFlag = false, mousePressing = false, keyPressing;
int[] dx = {0, 0, 1, 1, 0, 0}, dy = {0, 0, 0, 0, 1, 1}, dz = {0, 0, 0, 0, 0, 0}, flip = {1, -1, 1, -1, 1, -1};

color[] faceColor = new color[6];
int right = 1, bottom = 2, left = 3, top = 4, white = 0, blue = 1, red = 2, green = 3, orange = 4, yellow = 5;
int[][] moveMap = {{1, 2, 3, 4}, {0, 4, 5, 2}, {0, 1, 5, 3}, {0, 2, 5, 4}, {1, 0, 3, 5}, {1, 4, 3, 2}};
int[][] faceMap = {{bottom, left, top, right}, {top, top, bottom, top}, {right, right, right, right}, {bottom, bottom, top, bottom}, {left, left, left, left}, {top, left, bottom, right}};
int[] posMap = {0, 2, 4, 3, 5, 1};
HashMap<Character, Integer> moveToFace = new HashMap<Character, Integer>();
HashSet<String> visited = new HashSet<String>();
int[][][] cube = new int[6][3][3];

class State {
  int[][][] cube;
  int steps;
  double distance;

  State(int[][][] c, int s, double d) {
    cube = new int[6][3][3];
    for (int i = 0; i < 6; i ++)
      for (int j = 0; j < 3; j ++)
        cube[i][j] = c[i][j].clone();
    steps = s;
    distance = d;
  }

  State(int[][][] c, int s) {
    cube = new int[6][3][3];
    for (int i = 0; i < 6; i ++)
      for (int j = 0; j < 3; j ++)
        cube[i][j] = c[i][j].clone();
    steps = s;
  }
}
class StateComparator implements Comparator<State> {
  @Override
  int compare(State a, State b) {
    if (a.distance > b.distance) return(1);
    else if (a.distance < b.distance) return(-1);
    return(0);
  }
}

boolean solved() {
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 3; j ++)
      for (int k = 0; k < 3; k ++)
        if (cube[i][0][0] != cube[i][j][k])
          return(false);
  return(true);
}

String stateHash() {
  String hash = "";
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 3; j ++)
      for (int k = 0; k < 3; k ++)
        hash += str(cube[i][j][k]);
  return(hash);
}

void getCube(int[][][] c) {
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 3; j ++)
      cube[i][j] = c[i][j].clone();
}

void swapOwnCorner(int face) {
  int aux = cube[face][0][2];
  cube[face][0][2] = cube[face][0][0];
  cube[face][0][0] = cube[face][2][0];
  cube[face][2][0] = cube[face][2][2];
  cube[face][2][2] = aux;
}

void swapOwnEdge(int face) {
  int aux = cube[face][1][2];
  cube[face][1][2] = cube[face][0][1];
  cube[face][0][1] = cube[face][1][0];
  cube[face][1][0] = cube[face][2][1];
  cube[face][2][1] = aux;
}

void ordinarySwap(int i1, int j1, int k1, int i2, int j2, int k2) {
  int aux = cube[i1][j1][k1];
  cube[i1][j1][k1] = cube[i2][j2][k2];
  cube[i2][j2][k2] = aux;
}

void cornerSwap(int face) {
  swapOwnCorner(posMap[face]);
  int[] a = new int[4];
  for (int k = 0; k < 2; k ++) {
    for (int i = 0; i < 4; i ++) {
      if (faceMap[face][i] == right) a[i] = cube[moveMap[face][i]][face != 0 ? 2*k : 2*(1-k)][2];
      if (faceMap[face][i] == bottom) a[i] = cube[moveMap[face][i]][2][2*k];
      if (faceMap[face][i] == left) a[i] = cube[moveMap[face][i]][face != 0 ? 2*k : 2*(1-k)][0];
      if (faceMap[face][i] == top) a[i] = cube[moveMap[face][i]][0][2*k];
    }
    for (int i = 0; i < 4; i ++) {
      if (faceMap[face][i] == right) cube[moveMap[face][i]][face != 0 ? 2*k : 2*(1-k)][2] = a[(i+3)%4];
      if (faceMap[face][i] == bottom) cube[moveMap[face][i]][2][2*k] = a[(i+3)%4];
      if (faceMap[face][i] == left) cube[moveMap[face][i]][face != 0 ? 2*k : 2*(1-k)][0] = a[(i+3)%4];
      if (faceMap[face][i] == top) cube[moveMap[face][i]][0][2*k] = a[(i+3)%4];
    }
  }
}

void edgeSwap(int face) {
  swapOwnEdge(posMap[face]);
  int[] a = new int[4];
  for (int i = 0; i < 4; i ++)
  {
    if (faceMap[face][i] == right) a[i] = cube[moveMap[face][i]][1][2];
    if (faceMap[face][i] == bottom) a[i] = cube[moveMap[face][i]][2][1];
    if (faceMap[face][i] == left) a[i] = cube[moveMap[face][i]][1][0];
    if (faceMap[face][i] == top) a[i] = cube[moveMap[face][i]][0][1];
  }
  for (int i = 0; i < 4; i ++)
  {
    if (faceMap[face][i] == right) cube[moveMap[face][i]][1][2] = a[(i + 3) % 4];
    if (faceMap[face][i] == bottom) cube[moveMap[face][i]][2][1] = a[(i + 3) % 4];
    if (faceMap[face][i] == left) cube[moveMap[face][i]][1][0] = a[(i + 3) % 4];
    if (faceMap[face][i] == top) cube[moveMap[face][i]][0][1] = a[(i + 3) % 4];
  }
}

void turn(int face) {
  cornerSwap(face);
  if (face == 0) ordinarySwap(posMap[4], 0, 2, posMap[4], 2, 2);
  if (face == 0) ordinarySwap(posMap[2], 0, 0, posMap[2], 2, 0);
  if (face == 1) ordinarySwap(posMap[2], 0, 2, posMap[2], 0, 0);
  if (face == 1) ordinarySwap(posMap[5], 2, 2, posMap[5], 2, 0);
  if (face == 5) ordinarySwap(posMap[2], 0, 2, posMap[2], 2, 2);
  if (face == 5) ordinarySwap(posMap[4], 0, 0, posMap[4], 2, 0);
  if (face == 3) ordinarySwap(posMap[4], 2, 0, posMap[4], 2, 2);
  if (face == 3) ordinarySwap(posMap[5], 0, 0, posMap[5], 0, 2);
  edgeSwap(face);
}

void aStar() {
  PriorityQueue<State> pq = new PriorityQueue<State>(9999999, new StateComparator());
}

void bfs() {
  Queue<State> queue = new ArrayDeque<State>();
  queue.add(new State(cube, 0));

  while (queue.size() > 0) {
    getCube(queue.peek().cube); queue.remove();
    if (solved()) break;
    for (int k = 0; k < 6; k ++) {
      turn(k);
      String nowStateHash = stateHash();
      if (!visited.contains(nowStateHash)) {
        visited.add(nowStateHash);
        queue.add(new State(cube, 0));
      }
      turn(k); turn(k);
      nowStateHash = stateHash();
      if (!visited.contains(nowStateHash)) {
        visited.add(nowStateHash);
        queue.add(new State(cube, 0));
      }
      turn(k);
    }
  }
  queue.clear();
}

void solve() {
  visited.clear();
  bfs();
}

void setup() {
  size(900, 900, P3D);
  colorMode(HSB, 360, 100, 100);
  rectMode(CENTER);
  strokeWeight(10);
  textAlign(CENTER, CENTER);
  textSize(tileSize / 2);
  smooth();

  faceColor[0] = color(0, 0, 100); faceColor[1] = color(52, 100, 100);
  faceColor[2] = color(226, 100, 100); faceColor[3] = color(115, 100, 100);
  faceColor[4] = color(0, 100, 100); faceColor[5] = color(27, 100, 100);
  moveToFace.put('R', 2); moveToFace.put('U', 1); moveToFace.put('F', 0);
  moveToFace.put('L', 4); moveToFace.put('B', 5); moveToFace.put('D', 3);
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 3; j ++)
      for (int k = 0; k < 3; k ++)
        cube[i][j][k] = i;
  for (int i = 0; i < 6; i ++)
    for (int j = 0; j < 4; j ++)
      moveMap[i][j] = posMap[moveMap[i][j]];
}

void mouseReleased() {
  pushMatrix();
  rotateX(radians(xAngle));
  rotateY(radians(yAngle));
  popMatrix();
  mousePressing = false;
}

void mouseDragged() {
  mousePressing = true;
}

void keyReleased() {
  if (Character.toUpperCase(key) == 'S')
    thread("solve");
  keyPressing = false;
}

void keyPressed() {
  keyPressing = true;
}

void drawTile(float x, float y, color col, int cc) {
  translate(x, y);
  fill(col);
  rect(0, 0, tileSize, tileSize);
   //box(tileSize);
  // fill(0, 100, 0);
  // text(str(cc), 0, 0);
  translate(-x, -y);
}

void drawCube() {
  for (int k = 0; k < 6; k ++) {
    rotateY(radians(yAngle) + radians(90*dy[k]));
    if (k < 4) rotateZ(radians(zAngle) + radians(90*dz[k]));
    if (k < 4) rotateX(radians(xAngle) + radians(90*dx[k]));
    if (k >= 4) rotateX(-radians(zAngle) + radians(90*dz[k]));
    if (k >= 4) rotateZ(radians(xAngle) + radians(90*dx[k]));

    translate(0, 0, flip[k]*(tileSize * 3) / 2.0);

    if (k == 3 || k == 1) {
      for (int i = 0; i < 3; i ++) for (int j = 0; j < 3; j ++)
          drawTile(tileSize * (j - 1), tileSize * (1 - i), faceColor[cube[k][i][j]], cube[k][i][j]);
    }
    else if (k == 5) {
      for (int i = 0; i < 3; i ++) for (int j = 0; j < 3; j ++)
          drawTile(tileSize * (1 - j), tileSize * (i - 1), faceColor[cube[k][i][j]], cube[k][i][j]);
    }
    else {
      for (int i = 0; i < 3; i ++) for (int j = 0; j < 3; j ++)
          drawTile(tileSize * (j - 1), tileSize * (i - 1), faceColor[cube[k][i][j]], cube[k][i][j]);
    }

    translate(0, 0, flip[k]*-(tileSize * 3) / 2.0);

    if (k >= 4) rotateZ(-radians(xAngle) - radians(90*dx[k]));
    if (k >= 4) rotateX(radians(zAngle) - radians(90*dz[k]));
    if (k < 4) rotateX(-radians(xAngle) - radians(90*dx[k]));
    if (k < 4) rotateZ(-radians(zAngle) - radians(90*dz[k]));
    rotateY(-radians(yAngle) - radians(90*dy[k]));
  }
}

void draw() {
  background(0, 0, 100);

  if (mousePressing) {
    yAngle = (yAngle + mouseX - pmouseX) % 360;
    xAngle = (xAngle - (mouseY - pmouseY)) % 360;
    //zAngle += pow(mouseX - pmouseX, 2) + pow(mouseY - pmouseY, 2);
  }
  if (keyPressing) {
    if (key == 'z' || key == 'x') zAngle = (zAngle + (key == 'z' ? 1 : -1)) % 360;
    else {
      if (moveToFace.containsKey(Character.toUpperCase(key))) {
        turn(moveToFace.get(Character.toUpperCase(key)));
        if (key > 'a') for (int i = 0; i < 2; i ++) turn(moveToFace.get(Character.toUpperCase(key)));
      }
      keyPressing = false;
    }
  }

  translate(width / 3.0, height / 4.0, -(tileSize * 3) / 2.0);

  drawCube();

  translate(width / 3.0, height / 2.0, 0);
  rotateY(radians(180));
  rotateZ(radians(180));

  drawCube();
}
