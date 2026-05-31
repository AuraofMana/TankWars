extends "res://tank.gd"

const DIRS := [Vector2.UP, Vector2.DOWN, Vector2.LEFT, Vector2.RIGHT]

var ai_timer := 0.0
var fire_ai := 0.0

func _ready() -> void:
	team = TEAM_ENEMY
	hull_color = Color(0.85, 0.35, 0.3)
	barrel_color = Color(0.5, 0.18, 0.15)
	speed = 150.0
	fire_cooldown = 0.0  # Cadence is controlled by fire_ai below.
	facing = Vector2.DOWN
	want = Vector2.DOWN
	add_to_group("enemies")
	ai_timer = randf_range(0.4, 1.2)
	fire_ai = randf_range(0.6, 1.6)

func _decide(delta: float) -> void:
	# Repath when we hit something, or periodically to keep things lively.
	ai_timer -= delta
	if blocked or ai_timer <= 0.0:
		want = _pick_new_dir()
		ai_timer = randf_range(0.6, 1.6)

	fire_ai -= delta
	if fire_ai <= 0.0:
		fire()
		fire_ai = randf_range(0.9, 2.2)

func _pick_new_dir() -> Vector2:
	var open := _open_dirs()
	if open.is_empty():
		return -facing  # Boxed in; turn around.
	# About half the time, head toward the player if that way is open.
	if randf() < 0.5:
		var p := get_tree().get_first_node_in_group("player")
		if p:
			var toward := _toward(p.position)
			if open.has(toward):
				return toward
	return open[randi() % open.size()]

func _open_dirs() -> Array:
	var open := []
	for d in DIRS:
		if _can_go(d):
			open.append(d)
	return open

func _can_go(d: Vector2) -> bool:
	# Wall just ahead in this direction?
	if get_parent().is_solid(position + d * (TANK_RADIUS + 8.0)):
		return false
	# Another tank close ahead in this direction?
	for o in _other_tanks():
		if d.dot(o.position - position) > 0.0 and position.distance_to(o.position) < TANK_SEP + 18.0:
			return false
	return true

func _toward(target: Vector2) -> Vector2:
	var d := target - position
	if abs(d.x) > abs(d.y):
		return Vector2.RIGHT if d.x > 0.0 else Vector2.LEFT
	return Vector2.DOWN if d.y > 0.0 else Vector2.UP

func kill() -> void:
	queue_free()
