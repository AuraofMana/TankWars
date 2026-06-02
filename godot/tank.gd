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
# Perpendicular turns are gated by this cooldown; 0 = instant (player + most enemies).
# The Brute sets it high so it oversteers past you, opening a flank/rear window.
var turn_cooldown := 0.0
var turn_timer := 0.0
# When true, this tank actively pushes out of overlap with other tanks each frame (boids
# separation). Enemies turn it on so a pack can't stack into a single blob; the player leaves
# it off so the locked movement feel is never perturbed by a shove.
var separates := false
const SEP_SPEED := 130.0  # Push-apart speed (px/s). Kept below `speed` so it nudges, not shoves.

func _process(delta: float) -> void:
	if not get_parent().is_playing():
		return
	fire_timer -= delta
	turn_timer -= delta
	_decide(delta)
	if want != Vector2.ZERO:
		if want == facing or want == -facing:
			facing = want  # Reversing stays on the same lane — never gated.
		elif turn_timer <= 0.0 and _can_turn(want):
			facing = want
			turn_timer = turn_cooldown  # Slow turners commit forward before they can turn again.
		_move(delta)
	_glide_to_lane(delta)
	_separate(delta)
	queue_redraw()

# Boids-style separation: push directly away from any tank we're overlapping (closer than
# TANK_SEP). This actively un-stacks a clump — the lane-glide alone keeps yanking crowded tanks
# back onto the same lane center, so without this they re-merge into a blob. Wall-guarded so we
# never get shoved into a solid tile. Off for the player (separates == false).
func _separate(delta: float) -> void:
	if not separates:
		return
	var push := Vector2.ZERO
	for o in _other_tanks():
		var d: Vector2 = position - o.position
		var dist := d.length()
		if dist > 0.01 and dist < TANK_SEP:
			push += (d / dist) * (TANK_SEP - dist)  # Weighted: deeper overlap pushes harder.
	if push == Vector2.ZERO:
		return
	var dir := push.normalized()
	var move := dir * SEP_SPEED * delta
	if not get_parent().is_solid(position + dir * TANK_RADIUS + move):
		position += move

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

# Block a move ONLY if it brings us CLOSER to a tank that's already within separation. Moving
# away or sideways is always allowed, so two tanks that end up overlapping (the lane-glide can
# nudge them together — it ignores other tanks) can always slide apart instead of deadlocking.
# This is what keeps the player from getting permanently pinned when enemies crowd in.
func _blocked_by_tank(target: Vector2) -> bool:
	for o in _other_tanks():
		var after: float = target.distance_to(o.position)
		if after < TANK_SEP and after < position.distance_to(o.position):
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
	b.add_to_group("bullets")
	get_parent()._play_sfx("res://sound/TankFire1.wav")
	if team == TEAM_PLAYER:
		get_parent().on_player_fired(position)  # Gunshots are heard: nearby enemies wake.
	fire_timer = fire_cooldown

func _draw() -> void:
	draw_rect(Rect2(Vector2(-20, -20), Vector2(40, 40)), hull_color)
	draw_line(Vector2.ZERO, facing * 28.0, barrel_color, 10.0)
