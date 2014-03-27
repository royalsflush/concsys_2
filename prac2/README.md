Cond vars
=========
Observations:
* MacOS scheduler is very fair, it gives equal opportunities for all threads :).
* Even though the program takes more than 50 seconds (not sure how much more, a
little lazy to count right now), CPU time is 0.01, unsurprisingly, since sleep
function suspends execution and so does waiting for condition variables.

Sample
------
Taken directly from CS3D4 website - this sample assumes producer is going to,
well, produce forever, so when the producer is done, one of the consumer threads
gets stuck on waiting more.

Cond Var:
------------------------
This is a modification of the sample program that handles the producer inactive
case. Also, there's a timer for production and a timer for consumption.
Underflow was already handled in the sample program.
Overflow is handled (upper bound is set with a define).
