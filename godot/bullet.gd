extends Node2D

const SPEED := 520.0
var dir := Vector2.DOWN
var team := 0

func _process(delta: float) -> void:
	if not get_parent().is_playing():
		return
	position += dir * SPEED * delta
	if not get_viewport_rect().has_point(position):
		queue_free()
		return
	if get_parent().bullet_hits(self):
		queue_free()
		return
	queue_redraw()

func _draw() -> void:
	draw_circle(Vector2.ZERO, 5.0, Color(1.0, 0.9, 0.35))
