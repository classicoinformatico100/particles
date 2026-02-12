# Particle System Simulator in C

## What it does
This program creates a real-time particle physics simulation in your terminal with:
- **Physics engine**: Gravity, velocity, acceleration, damping
- **Collision detection**: Particles bounce off walls and floor
- **Multiple effects**: Explosions and fountain patterns
- **ASCII rendering**: Uses different characters for visual variety
- **Lifecycle management**: Particles fade and disappear

## Compilation
```bash
gcc -o particle_system particle_system.c -lm -O2
```

## Running
```bash
./particle_system
```

Press `Ctrl+C` to exit.

## Technical Highlights

### Low-level C features used:
- **Struct composition**: Complex data structures for particles and system
- **Memory management**: Fixed-size arrays with manual slot management
- **Bit manipulation**: Active/inactive flags
- **Math operations**: Trigonometry for angles, vector physics
- **Terminal control**: ANSI escape sequences for screen clearing
- **Time management**: Frame-rate control with `usleep()`

### Physics implementation:
1. **Euler integration**: Updates position based on velocity each frame
2. **Force accumulation**: Gravity constantly accelerates particles downward
3. **Damping coefficient**: Simulates air resistance (0.99 factor)
4. **Elastic collisions**: Walls and floor reflect particles with energy loss
5. **Kinetic decay**: Particles gradually lose energy

### Key algorithms:
- Particle pool management with active/inactive states
- Double buffering for screen rendering
- Polar coordinate explosion vectors (angle + speed)
- Lifetime-based particle fading

## Customization
Modify these constants to change behavior:
- `MAX_PARTICLES`: Maximum simultaneous particles
- `GRAVITY`: Downward acceleration strength
- `DAMPING`: Air resistance (0.0 to 1.0)
- `EXPLOSION_FORCE`: Initial particle speed
