import dom
import html5_canvas


dom.window.onload = proc(e: dom.Event) =
  let 
    canvas = dom.document.getElementById("3d-cube-canvas").Canvas
    ctx = canvas.getContext2D()

  ctx.fillText("ASDF", 0, 0)

