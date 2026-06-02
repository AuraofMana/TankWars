extends Node2D
# A telegraphed artillery shell already in the air. Draws a 3x3 warning box on the
# ground, counts down, then tells main to detonate. Lives independently of the
# artillery that fired it, so the shell still lands even if you kill the shooter.
# Drawn ABOVE the field + tanks via z_index (the "above-field" layer Ben wanted;
# the same layer will carry bush concealment later).

const TILE := 64
const HALF := TILE / 2.0
const FUSE := 2.0  # Seconds of warning before it detonates. Dodge by leaving the box.

var center_cell := Vector2i.ZERO
var fuse := FUSE

func _ready() -> void:
	z_index = 10

func _process(delta: float) -> void:
	if not get_parent().is_playing():
		return
	fuse -= delta
	if fuse <= 0.0:
		get_parent().artillery_detonate(center_cell)
		queue_free()
		return
	queue_redraw()

func _draw() -> void:
	var origin := Vector2((center_cell.x - 1) * TILE, (center_cell.y - 1) * TILE)
	var rect := Rect2(origin, Vector2(TILE * 3, TILE * 3))
	var t: float = clamp(1.0 - fuse / FUSE, 0.0, 1.0)  # 0 -> 1 as it nears impact.
	draw_rect(rect, Color(0.95, 0.15, 0.1, 0.10 + 0.30 * t))  # Fill darkens as the fuse burns.
	var pulse: float = 0.5 + 0.5 * sin(fuse * 12.0)  # Flashing border reads "danger, soon."
	draw_rect(rect, Color(1.0, 0.4, 0.25, 0.45 + 0.45 * pulse), false, 3.0)
	var ctr := Vector2(center_cell.x * TILE + HALF, center_cell.y * TILE + HALF)
	draw_line(ctr - Vector2(14, 0), ctr + Vector2(14, 0), Color(1, 0.5, 0.3, 0.8), 2.0)
	draw_line(ctr - Vector2(0, 14), ctr + Vector2(0, 14), Color(1, 0.5, 0.3, 0.8), 2.0)
