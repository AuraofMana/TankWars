extends Node2D

const TILE := 64
const SPEED := 520.0
const CANCEL_RADIUS := 0.3 * TILE  # Opposing bullets this close annihilate each other.
var dir := Vector2.DOWN
var team := 0

func _process(delta: float) -> void:
	if not get_parent().is_playing():
		return
	position += dir * SPEED * delta
	# No viewport check: the field is bigger than the window now (camera zoomed out), so
	# screen-space bounds would despawn bullets that are still in play. Out-of-bounds cells
	# read as STEEL in bullet_hits, so the map edge despawns the bullet AND bangs (_impact_noise)
	# just like an invisible boundary wall — you can't plink off into the distance for free.
	# Opposing bullets shoot each other down — you can defend, not just dodge.
	for o in get_tree().get_nodes_in_group("bullets"):
		if o != self and o.team != team and position.distance_to(o.position) < CANCEL_RADIUS:
			o.queue_free()
			queue_free()
			return
	if get_parent().bullet_hits(self):
		queue_free()
		return
	queue_redraw()

func _draw() -> void:
	var col := Color(0.4, 0.85, 1.0) if team == 0 else Color(1.0, 0.5, 0.2)
	draw_circle(Vector2.ZERO, 5.0, col)
