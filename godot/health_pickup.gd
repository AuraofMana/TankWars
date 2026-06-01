extends Node2D
# Dropped by a killed enemy (chance-based). Drive over it to restore 1 health.

const PICK_RADIUS := 26.0

func _process(_delta: float) -> void:
	if not get_parent().is_playing():
		return
	var p := get_tree().get_first_node_in_group("player")
	if p and p.position.distance_to(position) < PICK_RADIUS:
		get_parent().heal()
		queue_free()

func _draw() -> void:
	draw_rect(Rect2(-13, -13, 26, 26), Color(0.1, 0.35, 0.15))
	draw_rect(Rect2(-4, -10, 8, 20), Color(0.3, 0.95, 0.45))
	draw_rect(Rect2(-10, -4, 20, 8), Color(0.3, 0.95, 0.45))
