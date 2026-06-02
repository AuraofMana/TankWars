extends "res://tank.gd"

# Enemies have fog of war: they wander until they SEE the player, then hunt the
# last spot they saw you, then give up and wander again. main.enemy_can_see() does
# the line-of-sight test; main.alert_near() lets a spotter wake its neighbors.

enum Kind { PAWN, SKIRMISHER, BRUTE, ARTILLERY, TURRET }
const DIRS := [Vector2.UP, Vector2.DOWN, Vector2.LEFT, Vector2.RIGHT]
const ALERT_MEMORY := 3.0  # Seconds to keep hunting your last-seen spot after losing sight.
const BRUTE_MEMORY := 7.0  # Brutes give up much slower; they keep closing on you.
						   # RETUNE once map size is locked — feels long on the small test map.

# Artillery siege behavior (Starcraft siege-tank style): it must STOP and DEPLOY to
# fire, and can't hit you point-blank. Counterplay: rush inside min range, or punish
# the deploy/pack-up windows when it can't shoot. Ranges in tiles (TILE = 64).
const ART_MAX_RANGE_TILES := 8.0   # Long sniper reach.
const ART_MIN_RANGE_TILES := 2.0   # Real dead zone: can't hit you inside it, flees to re-open range.
const ART_MAX_RANGE := ART_MAX_RANGE_TILES * TILE
const ART_MIN_RANGE := ART_MIN_RANGE_TILES * TILE
const ART_SIEGE_WINDUP := 1.0      # Deploy time — vulnerable, can't move or fire.
const ART_UNSIEGE_WINDUP := 1.0    # Pack-up time before it can move again — vulnerable.
const ART_CADENCE := 3.0           # Seconds between salvos once deployed.
enum Siege { MOBILE, SIEGING, SIEGED, UNSIEGING }
const BRUTE_HP := 4        # 4 head-on hits (1 dmg) or 2 flank/rear hits (2 dmg) to kill.
const TURRET_CADENCE := 1.1  # Stationary gun: steady fire down whatever lane it's facing.
const BRUTE_TURN := 0.75   # Seconds between turns: slow enough to be flankable, not a sitting duck.
# Hunters stop pressing once this close to the player and just hold the gap + shoot, instead of
# driving into contact. Keeps a swarm from surrounding you to point-blank and pinning you in place.
const STANDOFF_TILES := 1.2
const STANDOFF := STANDOFF_TILES * TILE

# Three-state senses (Commandos-style). CALM: wander, no idea you exist. INVESTIGATING: heard a
# gunshot or felt a bullet impact — moves toward that spot but DOESN'T see you, so it can't fire.
# SPOTTED: has line of sight right now — hunts your exact position and shoots.
enum Alert { CALM, INVESTIGATING, SPOTTED }

var kind := Kind.PAWN
var alert := Alert.CALM
var last_seen := Vector2.ZERO
var memory := 0.0
var health := 1
var flash := 0.0  # Brief white hit-flash so landed shots read.
var alert_memory := ALERT_MEMORY
var siege_state := Siege.MOBILE  # Artillery only.
var siege_timer := 0.0
# Per-kind sight range in px; 0 = use main's global default. Set in _ready when a kind
# should see farther/less (e.g. the future Scout). Artillery stays default so its sight
# deliberately falls short of its firing range.
var vision_range := 0.0
var fleeing := false  # True while actively running away — shows a distinct icon. Artillery/scout.

var ai_timer := 0.0
var fire_ai := 0.0

func _ready() -> void:
	team = TEAM_ENEMY
	add_to_group("enemies")
	separates = true  # Pack members push out of overlap so they don't stack into a blob.
	facing = Vector2.DOWN
	want = Vector2.DOWN
	fire_cooldown = 0.0  # Cadence is handled by fire_ai below.
	ai_timer = randf_range(0.4, 1.2)
	fire_ai = randf_range(0.6, 1.6)
	match kind:
		Kind.SKIRMISHER:
			hull_color = Color(0.9, 0.55, 0.2)
			barrel_color = Color(0.6, 0.3, 0.12)
			speed = 235.0  # Fast flanker.
		Kind.BRUTE:
			hull_color = Color(0.42, 0.42, 0.5)
			barrel_color = Color(0.2, 0.2, 0.26)
			speed = 110.0              # Slow but relentless.
			health = BRUTE_HP          # Armored: front plate soaks hits.
			alert_memory = BRUTE_MEMORY
			turn_cooldown = BRUTE_TURN  # Oversteers past you — that's the flank window.
		Kind.ARTILLERY:
			hull_color = Color(0.55, 0.35, 0.6)  # Violet: the back-line lobber, rush it.
			barrel_color = Color(0.3, 0.18, 0.34)
			speed = 150.0  # Same as a pawn — the siege windups are what slow it, not foot speed.
		Kind.TURRET:
			hull_color = Color(0.3, 0.5, 0.55)   # Teal: bolted-down lane guardian.
			barrel_color = Color(0.16, 0.28, 0.32)
			speed = 0.0  # Never moves — it tracks you with the barrel and holds the lane.
			separates = false  # Bolted down: a passing tank must not shove it off its lane.
		_:
			hull_color = Color(0.85, 0.35, 0.3)
			barrel_color = Color(0.5, 0.18, 0.15)
			speed = 150.0

func _decide(delta: float) -> void:
	if flash > 0.0:
		flash -= delta
	var p := get_tree().get_first_node_in_group("player")
	if p and get_parent().enemy_can_see(position, p.position, vision_range):
		if alert != Alert.SPOTTED:
			get_parent().alert_near(position, p.position)  # Wake the neighbors.
		alert = Alert.SPOTTED
		last_seen = p.position
		memory = alert_memory
	elif alert != Alert.CALM:
		# Lost sight (or only ever heard you): drop to INVESTIGATING and run down the clock.
		if alert == Alert.SPOTTED:
			alert = Alert.INVESTIGATING
		memory -= delta
		if memory <= 0.0:
			alert = Alert.CALM

	if kind == Kind.ARTILLERY:
		_artillery_decide(delta)
		return

	if kind == Kind.TURRET:
		_turret_decide(delta)
		return

	if alert == Alert.CALM:
		_wander(delta)
	else:
		_hunt(delta, p)  # Both INVESTIGATING and SPOTTED close on last_seen.

	fire_ai -= delta
	if alert == Alert.SPOTTED and fire_ai <= 0.0:  # Only shoot when you can actually see them.
		fire()
		fire_ai = randf_range(0.9, 2.2)

# Siege-tank loop: reposition into the firing band, deploy, bombard, pack up to flee.
func _artillery_decide(delta: float) -> void:
	siege_timer -= delta
	fleeing = false
	var d := position.distance_to(last_seen) if _is_alert() else 1.0e9
	match siege_state:
		Siege.MOBILE:
			if not _is_alert():
				_wander(delta)
			elif d < ART_MIN_RANGE:
				_step_away(delta)              # Too close: back off to re-establish range.
			elif d > ART_MAX_RANGE:
				_step_toward(delta, last_seen)  # Too far: close in.
			else:
				want = Vector2.ZERO
				siege_state = Siege.SIEGING     # In the band: dig in.
				siege_timer = ART_SIEGE_WINDUP
		Siege.SIEGING:
			want = Vector2.ZERO
			if siege_timer <= 0.0:
				siege_state = Siege.SIEGED
				fire_ai = 0.4                   # First salvo shortly after deploying.
		Siege.SIEGED:
			want = Vector2.ZERO
			if not _is_alert() or d < ART_MIN_RANGE or d > ART_MAX_RANGE:
				siege_state = Siege.UNSIEGING   # Lost the band: pack up (the vulnerable window).
				siege_timer = ART_UNSIEGE_WINDUP
			else:
				fire_ai -= delta
				if fire_ai <= 0.0:
					get_parent().spawn_artillery_strike(last_seen)
					fire_ai = ART_CADENCE
		Siege.UNSIEGING:
			want = Vector2.ZERO
			if siege_timer <= 0.0:
				siege_state = Siege.MOBILE

func _step_toward(delta: float, target: Vector2) -> void:
	ai_timer -= delta
	if blocked or ai_timer <= 0.0 or want == Vector2.ZERO:
		want = _dir_toward(target)
		ai_timer = randf_range(0.3, 0.7)

func _step_away(delta: float) -> void:
	fleeing = true
	ai_timer -= delta
	if blocked or ai_timer <= 0.0 or want == Vector2.ZERO:
		want = _dir_toward(position * 2.0 - last_seen)  # Aim at the point opposite the player.
		ai_timer = randf_range(0.3, 0.7)

# Stationary lane guardian: never moves, just swings its barrel to the nearest cardinal toward
# your last-known spot and fires only when it can actually SEE you. Counterplay = stay off its
# lane or break line of sight, then rush in (it's 1 HP). Set facing directly: with no movement
# there are no lane-turn constraints to respect.
func _turret_decide(delta: float) -> void:
	want = Vector2.ZERO  # Bolted down.
	if alert == Alert.CALM:
		return
	facing = _toward(last_seen)  # Track you — barrel telegraphs the danger lane.
	fire_ai -= delta
	if alert == Alert.SPOTTED and fire_ai <= 0.0:
		fire()
		fire_ai = TURRET_CADENCE

func is_sieged() -> bool:
	return siege_state != Siege.MOBILE

func is_turret() -> bool:
	return kind == Kind.TURRET

# Units that should spread into a ring around the player when hunting (so a pack doesn't clump on
# one cell). Skirmisher already flanks to your back, artillery sieges, turret is bolted down.
func is_surrounder() -> bool:
	return alert != Alert.CALM and (kind == Kind.PAWN or kind == Kind.BRUTE)

# Artillery is indirect fire: it bombards a KNOWN spot, so any non-calm alert (even one a
# spotter handed it) is enough to deploy. It never needs eyes on you itself.
func _is_alert() -> bool:
	return alert != Alert.CALM

func _wander(delta: float) -> void:
	ai_timer -= delta
	if blocked or ai_timer <= 0.0:
		want = _pick_open_dir()
		ai_timer = randf_range(0.6, 1.6)

func _hunt(delta: float, p) -> void:
	var target := last_seen
	if kind == Kind.SKIRMISHER and alert == Alert.SPOTTED and p != null:
		target = p.position - p.facing * (TILE * 1.5)  # Seen you: aim for your back, not your front.
	elif is_surrounder():
		target = get_parent().hunt_target(self, last_seen)  # Take a distinct ring slot, don't clump.
	# Standoff: when we actually SEE you and we're already close, stop advancing. Holding the gap
	# (and firing) instead of ramming into you is what keeps a pack from boxing you against a wall.
	if alert == Alert.SPOTTED and p != null and position.distance_to(p.position) < STANDOFF:
		want = Vector2.ZERO
		return
	ai_timer -= delta
	if blocked or ai_timer <= 0.0 or want == Vector2.ZERO:
		want = _dir_toward(target)
		ai_timer = randf_range(0.3, 0.7)
	# Reached the last-seen spot with nobody in sight: start giving up.
	if position.distance_to(last_seen) < TILE * 0.6:
		memory = min(memory, 0.4)

func _dir_toward(target: Vector2) -> Vector2:
	# A* routes around walls and other tanks (no more ping-ponging at a wall).
	var step: Vector2 = get_parent().path_step(self, target)
	if step != Vector2.ZERO:
		return step
	# A* found no path (target unreachable / off-grid): fall back to greedy steering.
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
		return -facing  # Boxed in; turn around.sw
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

# A heard gunshot / felt bullet impact / neighbor's shout. Never sight, so it can only
# raise INVESTIGATING — it won't override an enemy that already has eyes on you.
func receive_alert(seen: Vector2) -> void:
	if alert != Alert.SPOTTED:
		alert = Alert.INVESTIGATING
	last_seen = seen
	memory = alert_memory

# Returns true if this hit killed the enemy. bdir is the bullet's travel direction.
func take_hit(bdir: Vector2) -> bool:
	var dmg := 1
	if kind == Kind.BRUTE:
		# Head-on hits glance off the front plate (1 dmg); flank/rear hits bite (2).
		dmg = 1 if bdir.dot(facing) < -0.5 else 2
	health -= dmg
	if health <= 0:
		kill()
		return true
	flash = 0.12
	queue_redraw()
	return false

func kill() -> void:
	queue_free()

func _draw() -> void:
	super._draw()
	if kind == Kind.TURRET:  # Bolted base + central pivot, so it reads as a fixed gun, not a tank.
		for c in [Vector2(-15, -15), Vector2(15, -15), Vector2(-15, 15), Vector2(15, 15)]:
			draw_circle(c, 3.0, Color(0.13, 0.16, 0.18))
		draw_circle(Vector2.ZERO, 9.0, Color(0.2, 0.34, 0.38))
	if kind == Kind.BRUTE:  # Heavy plate on the front face: where shots bounce off.
		var perp := Vector2(facing.y, -facing.x)
		var c := facing * 17.0
		draw_line(c - perp * 19.0, c + perp * 19.0, Color(0.16, 0.16, 0.2), 9.0)
	if kind == Kind.ARTILLERY:  # Deployment outriggers extend as it sieges, retract as it packs up.
		var deploy := 0.0
		match siege_state:
			Siege.SIEGING: deploy = 1.0 - siege_timer / ART_SIEGE_WINDUP
			Siege.SIEGED: deploy = 1.0
			Siege.UNSIEGING: deploy = siege_timer / ART_UNSIEGE_WINDUP
		if deploy > 0.0:
			var arm := lerpf(8.0, 22.0, deploy)
			for diag in [Vector2(1, 1), Vector2(1, -1), Vector2(-1, 1), Vector2(-1, -1)]:
				draw_line(diag * 14.0, diag * (14.0 + arm), Color(0.85, 0.7, 0.3, 0.9), 3.0)
	if flash > 0.0:
		draw_rect(Rect2(Vector2(-20, -20), Vector2(40, 40)), Color(1, 1, 1, 0.7))
	if fleeing:  # Cyan retreat chevrons — it's running, can't hit you right now.
		var fc := Color(0.4, 0.85, 1.0)
		draw_line(Vector2(-9, -40), Vector2(0, -34), fc, 3.0)
		draw_line(Vector2(0, -34), Vector2(9, -40), fc, 3.0)
		draw_line(Vector2(-9, -34), Vector2(0, -28), fc, 3.0)
		draw_line(Vector2(0, -28), Vector2(9, -34), fc, 3.0)
	elif alert == Alert.SPOTTED:  # Red "!" — it sees you and is shooting.
		var pts := PackedVector2Array([Vector2(-7, -30), Vector2(7, -30), Vector2(0, -44)])
		draw_colored_polygon(pts, Color(1.0, 0.85, 0.2))
	elif alert == Alert.INVESTIGATING:  # Amber "?" — heard something, closing in but blind.
		var qc := Color(1.0, 0.72, 0.2)
		draw_arc(Vector2(0, -38), 5.0, deg_to_rad(-210), deg_to_rad(30), 12, qc, 2.5)
		draw_line(Vector2(0, -34), Vector2(0, -30), qc, 2.5)
		draw_circle(Vector2(0, -26), 1.8, qc)
