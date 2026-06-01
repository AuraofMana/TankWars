extends "res://tank.gd"

# Enemies have fog of war: they wander until they SEE the player, then hunt the
# last spot they saw you, then give up and wander again. main.enemy_can_see() does
# the line-of-sight test; main.alert_near() lets a spotter wake its neighbors.

enum Kind { PAWN, SKIRMISHER }
const DIRS := [Vector2.UP, Vector2.DOWN, Vector2.LEFT, Vector2.RIGHT]
const ALERT_MEMORY := 3.0  # Seconds to keep hunting your last-seen spot after losing sight.

var kind := Kind.PAWN
var alerted := false
var last_seen := Vector2.ZERO
var memory := 0.0

var ai_timer := 0.0
var fire_ai := 0.0

func _ready() -> void:
	team = TEAM_ENEMY
	add_to_group("enemies")
	facing = Vector2.DOWN
	want = Vector2.DOWN
	fire_cooldown = 0.0  # Cadence is handled by fire_ai below.
	ai_timer = randf_range(0.4, 1.2)
	fire_ai = randf_range(0.6, 1.6)
	if kind == Kind.SKIRMISHER:
		hull_color = Color(0.9, 0.55, 0.2)
		barrel_color = Color(0.6, 0.3, 0.12)
		speed = 235.0  # Fast flanker.
	else:
		hull_color = Color(0.85, 0.35, 0.3)
		barrel_color = Color(0.5, 0.18, 0.15)
		speed = 150.0

func _decide(delta: float) -> void:
	var p := get_tree().get_first_node_in_group("player")
	if p and get_parent().enemy_can_see(position, p.position):
		if not alerted:
			get_parent().alert_near(position, p.position)  # Wake the neighbors.
		alerted = true
		last_seen = p.position
		memory = ALERT_MEMORY
	elif alerted:
		memory -= delta
		if memory <= 0.0:
			alerted = false

	if alerted:
		_hunt(delta, p)
	else:
		_wander(delta)

	fire_ai -= delta
	if alerted and fire_ai <= 0.0:  # Only shoot when you actually have a target.
		fire()
		fire_ai = randf_range(0.9, 2.2)

func _wander(delta: float) -> void:
	ai_timer -= delta
	if blocked or ai_timer <= 0.0:
		want = _pick_open_dir()
		ai_timer = randf_range(0.6, 1.6)

func _hunt(delta: float, p) -> void:
	var target := last_seen
	if kind == Kind.SKIRMISHER and p != null:
		target = p.position - p.facing * (TILE * 1.5)  # Aim for your back, not your front.
	ai_timer -= delta
	if blocked or ai_timer <= 0.0 or want == Vector2.ZERO:
		want = _dir_toward(target)
		ai_timer = randf_range(0.3, 0.7)
	# Reached the last-seen spot with nobody in sight: start giving up.
	if position.distance_to(last_seen) < TILE * 0.6:
		memory = min(memory, 0.4)

func _dir_toward(target: Vector2) -> Vector2:
	var primary := _toward(target)
	if _can_go(primary):
		return primary
	# Blocked: pick the open direction that still heads closest to the target.
	var open := _open_dirs()
	if open.is_empty():
		return -facing
	var best: Vector2 = open[0]
	var best_dot := -INF
	var goal := (target - position).normalized()
	for d in open:
		var dot: float = d.dot(goal)
		if dot > best_dot:
			best_dot = dot
			best = d
	return best

func _pick_open_dir() -> Vector2:
	var open := _open_dirs()
	if open.is_empty():
		return -facing  # Boxed in; turn around.
	return open[randi() % open.size()]

func _open_dirs() -> Array:
	var open := []
	for d in DIRS:
		if _can_go(d):
			open.append(d)
	return open

func _can_go(d: Vector2) -> bool:
	if get_parent().is_solid(position + d * (TANK_RADIUS + 8.0)):
		return false
	for o in _other_tanks():
		if d.dot(o.position - position) > 0.0 and position.distance_to(o.position) < TANK_SEP + 18.0:
			return false
	return true

func _toward(target: Vector2) -> Vector2:
	var d := target - position
	if abs(d.x) > abs(d.y):
		return Vector2.RIGHT if d.x > 0.0 else Vector2.LEFT
	return Vector2.DOWN if d.y > 0.0 else Vector2.UP

func receive_alert(seen: Vector2) -> void:
	alerted = true
	last_seen = seen
	memory = ALERT_MEMORY

func kill() -> void:
	queue_free()

func _draw() -> void:
	super._draw()
	if alerted:  # Little "!" so you can read when an enemy is hunting you.
		var pts := PackedVector2Array([Vector2(-7, -30), Vector2(7, -30), Vector2(0, -44)])
		draw_colored_polygon(pts, Color(1.0, 0.9, 0.2))
