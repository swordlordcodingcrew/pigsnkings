# Semi-concurrent cascading state automaton (first iteration)
------------------------------------------------------------



SomaticState
- normal
   + MotionState
   + ActionState
- on speed (double speed)
   + MotionState
   + ActionState
- not vulnerable
   + MotionState
   + ActionState
- hit
- dead
- entering
- exiting
- pause


MotionState
- wait
- walk
- jump
- on_air

ActionState
- bubble
- no action
- other action?


