# LogyCar

## Description

A fun game about a remote control car in which, through stunts, 
you will have to reach the goal as best you can. Be careful, don't fall!

## Key Features

- Physics:
  + Move car
  + Jump with force
  + Bearing friction
  + Assist Direction
  + Sides with torques
  + Parkour physics
  + Reverse direction
  + Turbo 
  + Apply forces by states
  + Orientation car in air

- State Machine
- Game Loop
- Infinite retries
- Music intro
- Music Win
- Jump feedback fx
- Lose feedback fx
- Brake feedback fx

- Platforms
  + Looping
  + Ice peralta
  + WallRide
  + Spiral cylinder
  + Dynamic platforms 
  + Dynamic change color
  + Platforms with different coefficient of friction
  + Wood friction
  + Ice friction
  + Grip friction
  + Ground friction
  + Dynamic button 

- Checkpoints
  + Respawn in last Checkpoint
  + Reset forces
  + Car orientation

- World
  + Color World blueSky
  + Reposition Lights
  + Two lights in world
  + Colors Platforms
  + Gradien Colors

- Sensor
  + Calculate sensor position with quaterions
  + Ghost Body 
  + Detect floor
  + States control

- Camera
  + Intro animation 
  + Win animation 
  + Lose animtion 
  + Position camera in center of Looping
  + Look at Player
  + Follow Player

## Controls
- W to accelerate
- S to reverse 
- W/S Break to change direction
- A/D to direction car
- Space to jump
- Shift to turbo
- W/A/S/D orientation car in air
- ESC to close window

## DEBUG keys
- Keys Respawn:
  + R respawn in start
  + 1 respawn post cylinder (checkpoint 1)
  + 2 respawn in ice curves (checkpoint 2)
  + 3 respawn post Wallride (checkpoint 3)
  + 4 respawn pre Looping  (checkpoint 4)
  + 5 respawn post Looping (checkpoint 5)
  + 6 respawn post Peralta (checkpoint 6)
  + 7 respawn post Ramps  (checkpoint 7)

- F1 to free camera
  + 'O' spawn car in position camera
  + W/A/S/D Move camera
  + Mouse Right-button drag orientaiton camera 
  + Space Up camera
  + LAlat Down camera
  + LShift speed up camera 

- I intro animation camera
- Space to skip animation camera Win/Intro 


# Annexed
- Investigation of remote control car physics
- We have tried to put rotary motors on the platforms
- Looping generation system
- Ramp generation system
- WallRide generation system
- File .rc
- Icon in the executable
- Icon in toolbar
- Randomize sound feedback Jump [1-6] 
- Randomize sound feedback Break [1-7]


## Developers

 - Ismael Tejada - Programmer / Game Designer
 - Mario Hernández - Lead Programmer
 - Alejandro Moreno - Programmer / Level Designer

## GitHub 4HD
- https://github.com/JandroK/LogyCar

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

