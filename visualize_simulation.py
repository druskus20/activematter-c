import matplotlib.pyplot as plt
import re
import numpy as np
from matplotlib.animation import FuncAnimation


def read_file(file_path):
    pattern = r"(\d+)\s+(\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)\s+([-+]?\d*\.?\d+)"
    step = 0
    with open(file_path) as f:
        lines = f.readlines()
        positions = []
        velocities = []
        for line in lines:
            match = re.match(pattern, line)
            if match:
                step = int(match.group(1))
                bird = int(match.group(2))
                x_position = float(match.group(3))
                y_position = float(match.group(4))
                x_velocity = float(match.group(5))
                y_velocity = float(match.group(6))

                # Extend positions array as necessary
                while len(positions) <= step:
                    positions.append([])
        
                # Extend positions for bird as necessary
                while len(positions[step]) <= bird:
                    positions[step].append([])


                # Extend positions array as necessary
                while len(velocities) <= step:
                    velocities.append([])
        
                # Extend positions for bird as necessary
                while len(velocities[step]) <= bird:
                    velocities[step].append([])

                # Add position to positions array
                positions[step][bird] = [x_position, y_position]
                velocities[step][bird] = [x_velocity, y_velocity]
    steps = step + 1
    return np.asarray(positions), np.asarray(velocities), steps
    


def animate(name: str, positions: np.ndarray, velocities: np.ndarray, n_steps: int) -> None:
    plt.clf()
    plt.close()
    fig = plt.figure(figsize=(4,4), dpi=600)
    ax = plt.gca()
    ax.clear()

    x: np.ndarray = positions[0,:,0]
    y: np.ndarray = positions[0,:,1]
    vx: np.ndarray = velocities[0,:,0]
    vy: np.ndarray = velocities[0,:,1]

    quiver = ax.quiver(x,y,vx,vy)

    def step_function(frame: int, positions: np.ndarray, velocities: np.ndarray) -> None:
        x: np.ndarray = positions[frame,:,0]
        y: np.ndarray = positions[frame,:,1]
        vx: np.ndarray = velocities[frame,:,0]
        vy: np.ndarray = velocities[frame,:,1]
        quiver.set_offsets(np.column_stack([x, y]))
        quiver.set_UVC(vx, vy)

    ani = FuncAnimation(
        fig, 
        step_function,
        fargs=(positions, velocities),
        frames=n_steps,
        interval=1, 
        blit=False, 
        repeat=False, 
    )  

    ani.save(f"{name}.mp4", fps=60, writer='ffmpeg')


positions, velocities, steps = read_file("simulation.txt")
print(positions)
print(velocities)
animate("simulation", positions, velocities, steps)
