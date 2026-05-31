extends Node2D
# Shared movement + firing for all tanks. Subclasses override _decide() to set
# `want` (the requested direction) and to call fire().

const TILE := 64
const HALF := TILE / 2.0
const TANK_RADIUS := 24.0  # Half the hull; how close the front gets to a wall.
const TANK_SEP := 44.0  # Min center-to-center distance between two tanks.

# Turning window toward the lane AHEAD (px).
const SNAP_TOLERANCE := 20.0
# How far past a lane center you may still turn BACK onto it (px). Beyond this
# you're committed forward and can only turn into the next lane ahead.
# NOTE: kept equal to HALF - TANK_RADIUS so a tank stopped flush against a wall
# is still inside this grace and can turn along the wall.
const COMMIT_THRESHOLD := 8.0
# Snap feel on a 1-10 dial. 1 = lazy glide onto the lane, 10 = near-instant.
const SNAP_ASSIST := 5
const CORRECTION_SPEED := SNAP_ASSIST * 90.0

const TEAM_PLAYER := 0
const TEAM_ENEMY := 1

var facing := Vector2.DOWN
var want := Vector2.ZERO
var speed := 220.0
var fire_cooldown := 0.22
var fire_timer := 0.0
var team := TEAM_PLAYER
var hull_color := Color(0.45, 0.8, 0.45)
var barrel_color := Color(0.2, 0.5, 0.2)
var blocked := false  # True when the last move was stopped by a wall or tank.

func _process(delta: float) -> void:
	if not get_parent().is_playing():
		return
	fire_timer -= delta
	_decide(delta)
	if want != Vector2.ZERO:
		if want == facing or want == -facing:
			facing = want
		elif _can_turn(want):
			facing = want
		_move(delta)
	_glide_to_lane(delta)
	queue_redraw()

func _decide(_delta: float) -> void:
	pass  # Override in subclass.

func _move(delta: float) -> void:
	var target := position + facing * speed * delta

	# Wall: clamp the front to the wall face.
	var hit_wall := false
	var lead := target + facing * TANK_RADIUS
	if get_parent().is_solid(lead):
		hit_wall = true
		var cell := Vector2i(floori(lead.x / TILE), floori(lead.y / TILE))
		if facing == Vector2.RIGHT:
			target.x = cell.x * TILE - TANK_RADIUS
		elif facing == Vector2.LEFT:
			target.x = (cell.x + 1) * TILE + TANK_RADIUS
		elif facing == Vector2.DOWN:
			target.y = cell.y * TILE - TANK_RADIUS
		else:
			target.y = (cell.y + 1) * TILE + TANK_RADIUS

	# Tank: if moving there would overlap another tank ahead, hold position.
	if _blocked_by_tank(target):
		blocked = true
		return

	position = target
	blocked = hit_wall

func _blocked_by_tank(target: Vector2) -> bool:
	for o in _other_tanks():
		if facing.dot(o.position - position) > 0.0 and target.distance_to(o.position) < TANK_SEP:
			return true
	return false

func _other_tanks() -> Array:
	var arr := []
	var tree := get_tree()
	for n in tree.get_nodes_in_group("player"):
		if n != self:
			arr.append(n)
	for n in tree.get_nodes_in_group("enemies"):
		if n != self:
			arr.append(n)
	return arr

func _can_turn(_dir: Vector2) -> bool:
	var p: float
	var s: float
	if facing.y != 0.0:
		p = position.y
		s = facing.y
	else:
		p = position.x
		s = facing.x
	var c := _snap(p)
	var ahead := (p - c) * s  # How far past center c, positive in travel direction.
	if ahead <= 0.0:
		return -ahead <= SNAP_TOLERANCE
	if ahead <= COMMIT_THRESHOLD:
		return true
	return (TILE - ahead) <= SNAP_TOLERANCE

func _glide_to_lane(delta: float) -> void:
	var step := CORRECTION_SPEED * delta
	if facing.x != 0.0:
		position.y = move_toward(position.y, _snap(position.y), step)
	else:
		position.x = move_toward(position.x, _snap(position.x), step)

func _snap(v: float) -> float:
	return round((v - HALF) / TILE) * TILE + HALF

func _cell_center(cx: int, cy: int) -> Vector2:
	return Vector2(cx * TILE + HALF, cy * TILE + HALF)

func fire() -> void:
	if fire_timer > 0.0:
		return
	var b := Node2D.new()
	b.set_script(load("res://bullet.gd"))
	b.position = position + facing * HALF
	b.dir = facing
	b.team = team
	get_parent().add_child(b)
	fire_timer = fire_cooldown

func _draw() -> void:
	draw_rect(Rect2(Vector2(-20, -20), Vector2(40, 40)), hull_color)
	draw_line(Vector2.ZERO, facing * 28.0, barrel_color, 10.0)
