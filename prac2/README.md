Cond vars
=========

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
Overflow is not currently handled (and buffer doesn't have an upper bound).
