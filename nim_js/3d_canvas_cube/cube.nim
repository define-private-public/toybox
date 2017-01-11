# This was made using Nim v0.16.0 and the `html5_canvas` package

import dom
import html5_canvas
import basic3d
from basic3d import
  vector3d, rotateX, rotateY
from math import
  Pi, sin, cos, floor
from tables import
  TableRef, newTable, hasKey, keys, add, `[]`, `[]=`
from sequtils import toSeq
from algorithm import sort
from colors import colBlueViolet


# Custom types
type
  Line = object
    a, b: Vector3d

  ProjectionType = enum
    Orthogrpahic
    Oblique
    Isometric
    Perspective


## intializes a Line object
proc line(a, b: Vector3d): Line =
  return Line(a:a, b:b)


# Global variables
var
  scale = 3.0
  size = 200.0
  zMin = (scale * (-size / 2)).int
  zMax = (scale * (size / 2)).int
  xOffset = (scale * (size / 2)).int
  yOffset = (scale * (size / 2)).int
  maxDist = 50.0
  canvas: Canvas

  points = newSeq[Vector3d](8) 
  edges = newSeq[Line](12)

  projectionType = Perspective


## Maps a value from the range of [a, b] to [x, y]
proc remap(value, a, b, x, y: float): float {.inline.} =
  return (value - a) / (b - a) * (y - x) + x


## plots a line on the canvas
proc plotLine(ctx: CanvasRenderingContext2D; line: Line; drawGlow: bool = false) =
  var a, b: Vector3d

  case projectionType:
    of Oblique:
      # For an oblique projection, apply it's "projection matrix"
      # mathmatically, we shouldn't give it the z component, but we need to
      let theta = Pi / 8.0
      a = vector3d(line.a.x + (0.5 * -line.a.z * cos(theta)), line.a.y + (0.5 * -line.a.z * sin(theta)), line.a.z)
      b = vector3d(line.b.x + (0.5 * -line.b.z * cos(theta)), line.b.y + (0.5 * -line.b.z * sin(theta)), line.b.z)

    of Isometric:
      # An Isometric view, just rotate the line
      a = line.a
      a &= rotateY(Pi / 4.0)
      a &= rotateX(35.0 * Pi / 180.0)
      b = line.b
      b &= rotateY(Pi / 4.0)
      b &= rotateX(35.0 * Pi / 180.0)

    of Perspective:
      let
        near = scale * 100.0
        right = scale * 125.0
        top = scale * 125.0
      a = vector3d(near / (right - line.a.z) * line.a.x, near / (top - line.a.z) * line.a.y, line.a.z)
      b = vector3d(near / (right - line.b.z) * line.b.x, near / (top - line.b.z) * line.b.y, line.b.z)

    of Orthogrpahic:
      a = line.a
      b = line.b
    else:
      # Todo add something else
      discard

  if drawGlow:
    ctx.strokeStyle = "black"
    ctx.shadowColor = rgb(colBlueViolet)
    ctx.shadowBlur = 15 * scale
  else:
    # Create a gradient for the lines based on their Z positions
    var grad = ctx.createLinearGradient(xOffset.float + a.x,
                                        yOffset.float - a.y,
                                        xOffset.float + b.x,
                                        yOffset.float - b.y)
    grad.addColorStop(0, rgb(0x00, remap(a.z, zMin.float, zMax.float, 0x00.float, 0xFF.float).floor.int, 0xFF))
    grad.addColorStop(1, rgb(0x00, remap(b.z, zMin.float, zMax.float, 0x00.float, 0xFF.float).floor.int, 0xFF))

    ctx.strokeStyle = grad
    ctx.shadowBlur = 0
  
  ctx.beginPath()
  ctx.lineJoin = RoundJoin
  ctx.lineCap = RoundCap
  ctx.lineWidth = 5 * scale
  ctx.moveTo(xOffset.float + a.x, yOffset.float - a.y)
  ctx.lineTo(xOffset.float + b.x, yOffset.float - b.y)
  ctx.stroke()


## Draws the cube with the supplied points
proc drawCube(ctx: CanvasRenderingContext2D; edges: seq[Line]) =
  # We need to draw the lines in a different order, so we have to sort them based
  # upon their position in the Z axis
  var sortedLines = newSeq[Line](edges.len)

  # Sort the lines into a map, based upon location in the Z axis
  var zLineMap:TableRef[float, seq[Line]] = newTable[float, seq[Line]]()
  for line in edges:
    # Which Z coordinate is lesser?
    let z = min(line.a.z, line.b.z)

    # Either put it in a list, or add it to another one
    if zLineMap.hasKey(z):
      var s: seq[Line] = zLineMap[z]
      s.add(line)
      zLineMap[z] = s
    else:
      zLineMap[z] = @[line]

  # Put it in the sorted list now
  var zKeys: seq[float] = toSeq(zLineMap.keys)
  zKeys.sort(cmp[float])

  # Sort them
  var i = 0
  for key in zKeys:
    for line in zLineMap[key]:
      sortedLines[i] = line
      i += 1

  # Draw the lines (glow first then the actual lines)
  for i in 0..high(sortedLines):
    plotLine(ctx, sortedLines[i], true)
  for i in 0..high(sortedLines):
    plotLine(ctx, sortedLines[i])


## A handy proch to use JS's SetTimout function
proc setTimeout(function: proc(); ms: float) =
  {.emit: ["setTimeout(", function, ",", ms, ");"].}


# The callback that will loop and draw
proc drawRoutine() =
  # Draw and stuff
  var ctx = canvas.getContext2D()
  ctx.fillStyle = rgb(0, 0, 0)
  ctx.fillRect(0, 0, canvas.width.float, canvas.height.float)
  drawCube(ctx, edges)

  # Update the lines
  for j in 0..high(edges):
    edges[j].a &= rotateX(0.005)
    edges[j].b &= rotateX(0.005)
    edges[j].a &= rotateY(0.004)
    edges[j].b &= rotateY(0.004)
    edges[j].a &= rotateZ(0.003)
    edges[j].b &= rotateZ(0.003)

  # Recall the function eventually
  setTimeout(drawRoutine, 25)


dom.window.onload = proc(e: dom.Event) =
  canvas = dom.document.getElementById("3d-cube-canvas").Canvas

  points[0] = vector3d(scale * -maxDist, scale * maxDist, scale * maxDist);
  points[1] = vector3d(scale * maxDist, scale * maxDist, scale * maxDist);
  points[2] = vector3d(scale * maxDist, scale * -maxDist, scale * maxDist);
  points[3] = vector3d(scale * -maxDist, scale * -maxDist, scale * maxDist);
  points[4] = vector3d(scale * -maxDist, scale * maxDist, scale * -maxDist);
  points[5] = vector3d(scale * maxDist, scale * maxDist, scale * -maxDist);
  points[6] = vector3d(scale * maxDist, scale * -maxDist, scale * -maxDist);
  points[7] = vector3d(scale * -maxDist, scale * -maxDist, scale * -maxDist);

  for i in 0..<4:
    edges[i] = line(points[i], points[(i + 1) mod 4])                # Front
    edges[i + 4] = line(points[i + 4], points[((i + 1) mod 4) + 4])  # Back
    edges[i + 8] = line(points[i], points[i + 4])                    # Sides

  drawRoutine()

