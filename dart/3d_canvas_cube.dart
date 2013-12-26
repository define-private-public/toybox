
library toybox;

import 'dart:html';
import 'dart:math';
import 'dart:async';

/*== Global Variables ==*/
int scale = 1;
int size = 200;
int zMin = (scale * (-size / 2)).toInt();
int zMax = (scale * (size / 2)).toInt();
int xOffset = (scale * (size / 2)).toInt();
int yOffset = (scale * (size / 2)).toInt();
int maxDist = 50;

DivElement content = querySelector('#content');
CanvasElement c = new CanvasElement(width: scale * size, height: scale * size);

List<Vec3> points = new List(8);  // could technically do this with 3
List<Line> lines = new List(12);

// Used for which type of projection we are using
const int ORTHOGRAPHIC = 0;
const int OBLIQUE = 1;
const int ISOMETRIC = 2;          // Also kind of orthographic
const int PERSPECTIVE = 3;        // Not sure if is really a true perspective matrix
int projectionType = PERSPECTIVE;

boolean renderStero = false;      // To do steroscopic rendering?
                                 // If doing so, try setting the scale to 2


// 3D Vector class
class Vec3 {
  num x, y, z;

  Vec3(this.x, this.y, this.z);

  // Returns an instance of the vector, but with everything as an integer
  Vec3 toInt() => new Vec3(this.x.round(), this.y.round(), this.z.round());

  String toString() => '<$x, $y, $z>';
}

// 3D Line class
class Line {
  Vec3 a, b;

  Line(this.a, this.b);

  String toString() => '$a ---- $b';
}



// For making RGB values
String rgb(int r, int g, int b) => 'rgb($r, $g, $b)';

// Remaps value, in the range from a to b, to the range of x to y
num remap(num value, num a, num b, num x, num y) => (value - a) / (b - a) * (y - x) + x;

// Performs an X axis rotation
Vec3 rotateX(Vec3 vec, double theta) {
  Vec3 vecPrime = new Vec3(vec.x, 0, 0);
  
  // X compoentn doens't change
  vecPrime.y = (vec.y * cos(theta)) - (vec.z * sin(theta));
  vecPrime.z = (vec.y * sin(theta)) + (vec.z * cos(theta));

  return vecPrime;
}

// Performs a Y axis rotation
Vec3 rotateY(Vec3 vec, double theta) {
  Vec3 vecPrime = new Vec3(0, vec.y, 0);
  
  // X compoentn doens't change
  vecPrime.x = (vec.x * cos(theta)) + (vec.z * sin(theta));
  vecPrime.z = -(vec.x * sin(theta)) + (vec.z * cos(theta));

  return vecPrime;
}

// Performs a Z axis rotation
Vec3 rotateZ(Vec3 vec, double theta) {
  Vec3 vecPrime = new Vec3(0, 0, vec.z);
  
  // X compoentn doens't change
  vecPrime.x = (vec.x * cos(theta)) - (vec.y * sin(theta));
  vecPrime.y = (vec.x * sin(theta)) + (vec.y * cos(theta));

  return vecPrime;
}

// Moves the vector over somewhere
Vec3 translate(Vec3 vec, Vec3 delta) => new Vec3(vec.x + delta.x, vec.y + delta.y, vec.z + delta.z);



/*== Drawing Functions ==*/
void plotLine(CanvasRenderingContext2D ctx, Line line) {
  // NOTE: Failed persepctive matrix
  // Working on a perspective projection; 
  //                          
  //                          Left side is original mathmatical representation:
  //        +Y    -Z          -------------------------------------------------
  //         |  /             +X -> +Z                                          
  //         | /              +Y -> +X                                          
  //         |/               +Z -> +Y                                                     
  //-X ------------- +X                                   
  //        /|                           
  //       / |                           
  //      /  |          
  //   +Z   -Y          
//  Vec3 a, b;
//  List<Vec3> point = new List(2);
//  points[0] = line.a;
//  points[1] = line.b;
//
//  Vec3 cam = new Vec3(0, 0, 100);            // Camera
//  Vec3 eye = new Vec3(0, 0, 200);
//  Vec3 ori = new Vec3(PI / 4, PI / 4, PI / 4);   // Orientation of camera
//
//  // Make the projection pragmatically
//  for (int i = 0; i < points.length; i++) {
//    // Some variables to make our life easier
//    double x, y, z;
//    double alpha, beta;
//    x = points[i].x - cam.x;
//    y = points[i].y - cam.y;
//    z = points[i].z - cam.z;
//    alpha = cos(ori.x) * sin(ori.x) * (sin(ori.y) * x + cos(ori.y) * z);
//    beta = cos(ori.y) * x - sin(ori.y) * z;
//
//    // Figure out the "d,"
//    Vec3 d = new Vec3(0, 0, 0);
//    d.x = sin(ori.z) * alpha + cos(ori.z) * beta;
//    d.y = cos(ori.z) * alpha - sin(ori.z) * beta;
//    d.z = cos(ori.x) * (sin(ori.y) * x + cos(ori.y) * z) - sin(ori.x) * y;
//
//    // Finalize the projeciton
//    double f = eye.y / d.y;
//    points[i] = new Vec3(
//      f * d.z - eye.z,
//      f * d.x - eye.x,
//      points[i].z
//    );
//  }
//  a = points[0].toInt();
//  b = points[1].toInt();
//  print(a);
//  print(b);

  // Chose which pojection type we should use
  Vec3 a, b;
  switch (projectionType) {
    case OBLIQUE:
      // For an oblique projection, apply it's "projection matrix";
      // mathmatically, we shouldn't give it the z component, but we need to
      double theta = PI / 8;
      a = new Vec3(line.a.x + (0.5 * -line.a.z * cos(theta)), line.a.y + (0.5 * -line.a.z * sin(theta)), line.a.z);
      b = new Vec3(line.b.x + (0.5 * -line.b.z * cos(theta)), line.b.y + (0.5 * -line.b.z * sin(theta)), line.b.z);
      break;

    case ISOMETRIC:
      // An ISO metric view, just rotate the line
      a = rotateX(rotateY(line.a, PI / 4), 35.264 * PI / 180);
      b = rotateX(rotateY(line.b, PI / 4), 35.264 * PI / 180);
      break;

    case PERSPECTIVE:
      // Not sure if is a true, or a good perspective projection, but it works.
      double near = scale * 100;
      double far = -near;
      double right = scale * 125;
      double left = -right;
      double top = scale * 125;
      double bot = -top;
      a = new Vec3(near / (right - line.a.z) * line.a.x, near / (top - line.a.z) * line.a.y, line.a.z);
      b = new Vec3(near / (right - line.b.z) * line.b.x, near / (top - line.b.z) * line.b.y, line.b.z);
      break;

    case ORTHOGRAPHIC:
    default:
      // Not specified, do an orthographical
      a = line.a;
      b = line.b;
      break;
  }

  if (renderStero) {
    // Some sort of 3D
    double theta_blue = PI / 180;
    double theta_red = PI;
    CanvasGradient grad;
    double skew = 0.05 / scale;
    Vec3 refA, refB;
    refA = a;
    refB = b;

    // Blue
    a = new Vec3(a.x + (skew * -a.z * cos(theta_blue)), a.y + (skew * -a.z * sin(theta_blue)), a.z);
    b = new Vec3(b.x + (skew * -b.z * cos(theta_blue)), b.y + (skew * -b.z * sin(theta_blue)), b.z);
    grad = ctx.createLinearGradient(xOffset + a.x, yOffset - a.y, xOffset + b.x, yOffset - b.y);
    grad..addColorStop(0, rgb(0x00, remap(a.z, zMin, zMax, 0x00, 0xFF).floor(), remap(a.z, zMin, zMax, 0x00, 0xFF).floor()))
        ..addColorStop(1, rgb(0x00, remap(a.z, zMin, zMax, 0x00, 0xFF).floor(), remap(a.z, zMin, zMax, 0x00, 0xFF).floor()));
    ctx..beginPath()
       ..lineWidth = scale
       ..strokeStyle = grad
       ..moveTo(xOffset + a.x, yOffset - a.y)
       ..lineTo(xOffset + b.x, yOffset - b.y)
       ..stroke();

    // Red
    a = refA;   // Reset them
    b = refB;
    a = new Vec3(a.x + (skew * -a.z * cos(theta_red)), a.y + (skew * -a.z * sin(theta_red)), a.z);
    b = new Vec3(b.x + (skew * -b.z * cos(theta_red)), b.y + (skew * -b.z * sin(theta_red)), b.z);
    grad = ctx.createLinearGradient(xOffset + a.x, yOffset - a.y, xOffset + b.x, yOffset - b.y);
    grad..addColorStop(0, rgb(remap(a.z, zMin, zMax, 0x00, 0xFF).floor(), 0x00, 0x00))
        ..addColorStop(1, rgb(remap(b.z, zMin, zMax, 0x00, 0xFF).floor(), 0x00, 0x00));
  
    ctx..beginPath()
       ..lineWidth = scale
       ..strokeStyle = grad
       ..moveTo(xOffset + a.x, yOffset - a.y)
       ..lineTo(xOffset + b.x, yOffset - b.y)
       ..stroke();
    

  } else {
    // Normal rendering
    // Setup the gradient
    CanvasGradient grad = ctx.createLinearGradient(xOffset + a.x, yOffset - a.y, xOffset + b.x, yOffset - b.y);
    grad..addColorStop(0, rgb(0x00, remap(a.z, zMin, zMax, 0x00, 0xFF).floor(), 0xFF))
        ..addColorStop(1, rgb(0x00, remap(b.z, zMin, zMax, 0x00, 0xFF).floor(), 0xFF));
  
    ctx..beginPath()
       ..lineWidth = scale
       ..strokeStyle = grad
       ..moveTo(xOffset + a.x, yOffset - a.y)
       ..lineTo(xOffset + b.x, yOffset - b.y)
       ..stroke();
  }
}

// Draws the cube with the supplied points
void drawCube(CanvasRenderingContext2D ctx, List<Line> lines) {
  // We need to draw the lines in a different order, so we have to sort them based
  // upon their position in the Z axis
  List<Line> sortedLines = new List(lines.length);

  // Sort the lines into a map, based upon location in the Z axis
  Map<double, List<Line>> zLineMap = new Map();
  for (Line line in lines) {
    // Which Z coordinate is lesser?
    double z = min(line.a.z, line.b.z);

    // Either put it in a list, or add it to another one
    if (zLineMap.containsKey(z))
      zLineMap[z].add(line);
    else
      zLineMap[z] = [line];
  }

  // Put it in the sorted list now
  int i = 0;
  List<double> zKeys = zLineMap.keys.toList();
  zKeys.sort();
  for (double key in zKeys) {
    for (Line line in zLineMap[key])
      sortedLines[i++] = line;
  }

  // Draw the lines
  for (int i = 0; i < sortedLines.length; i++)
    plotLine(ctx, sortedLines[i]);

}

// The callback that will loop and draw
void drawRoutine() {
  // Draw and stuff
  CanvasRenderingContext2D ctx = c.context2D;
  ctx..fillStyle = rgb(0, 0, 0)
     ..fillRect(0, 0, c.width, c.height);
  drawCube(ctx, lines);

  // Update the lines
  for (int j = 0; j < lines.length; j++) {
    lines[j].a = rotateX(lines[j].a, 0.005);
    lines[j].b = rotateX(lines[j].b, 0.005);
    lines[j].a = rotateY(lines[j].a, 0.004);
    lines[j].b = rotateY(lines[j].b, 0.004);
    lines[j].a = rotateZ(lines[j].a, 0.003);
    lines[j].b = rotateZ(lines[j].b, 0.003);
  }

  // Recall the function eventually
  new Timer(new Duration(milliseconds: 25), drawRoutine);
}



// Main funciton
void main() {
  // The Mesh
  points[0] = new Vec3(scale * -maxDist, scale * maxDist, scale * maxDist);
  points[1] = new Vec3(scale * maxDist, scale * maxDist, scale * maxDist);
  points[2] = new Vec3(scale * maxDist, scale * -maxDist, scale * maxDist);
  points[3] = new Vec3(scale * -maxDist, scale * -maxDist, scale * maxDist);
  points[4] = new Vec3(scale * -maxDist, scale * maxDist, scale * -maxDist);
  points[5] = new Vec3(scale * maxDist, scale * maxDist, scale * -maxDist);
  points[6] = new Vec3(scale * maxDist, scale * -maxDist, scale * -maxDist);
  points[7] = new Vec3(scale * -maxDist, scale * -maxDist, scale * -maxDist);

  // Set the faces pragmatically
  for (int i = 0; i < 4; i++) {
    lines[i] = new Line(points[i], points[(i + 1) % 4]);                // Front
    lines[i + 4] = new Line(points[i + 4], points[((i + 1) % 4) + 4]);  // Back
    lines[i + 8] = new Line(points[i], points[i + 4]);                  // Sides
  }

  // Add to the document
  content.append(c);
  drawRoutine();          // Start drawing
}

